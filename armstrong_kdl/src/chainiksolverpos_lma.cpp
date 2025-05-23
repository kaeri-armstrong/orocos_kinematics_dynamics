/**
 \file   chainiksolverpos_lma.cpp
 \brief  computing inverse position kinematics using Levenberg-Marquardt.
*/

/**************************************************************************
    begin                : May 2012
    copyright            : (C) 2012 Erwin Aertbelien
    email                : firstname.lastname@mech.kuleuven.ac.be

 History (only major changes)( AUTHOR-Description ) :

 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/

#include "chainiksolverpos_lma.hpp"
#include <iostream>

namespace ARMstrongKDL {




template <typename Derived>
inline void Twist_to_Eigen(const ARMstrongKDL::Twist& t,Eigen::MatrixBase<Derived>& e) {
	e(0)=t.vel.data[0];
	e(1)=t.vel.data[1];
	e(2)=t.vel.data[2];
	e(3)=t.rot.data[0];
	e(4)=t.rot.data[1];
	e(5)=t.rot.data[2];
}


ChainIkSolverPos_LMA::ChainIkSolverPos_LMA(
		const ARMstrongKDL::Chain& _chain,
		const Eigen::Matrix<double,6,1>& _l,
		double _eps,
		int _maxiter,
		double _eps_joints
) :
    chain(_chain),
	nj(chain.getNrOfJoints()),
	ns(chain.getNrOfSegments()),
	lastNrOfIter(0),
	lastDifference(0),
	lastTransDiff(0),
	lastRotDiff(0),
	lastSV(nj),
	jac(6, nj),
	grad(nj),
	display_information(false),
	maxiter(_maxiter),
	eps(_eps),
	eps_joints(_eps_joints),
	L(_l.cast<ScalarType>()),
	T_base_jointroot(nj),
	T_base_jointtip(nj),
	q(nj),
	A(nj, nj),
	tmp(nj),
	ldlt(nj),
	svd(6, nj,Eigen::ComputeThinU | Eigen::ComputeThinV),
	diffq(nj),
	q_new(nj),
	original_Aii(nj)
{}

ChainIkSolverPos_LMA::ChainIkSolverPos_LMA(
		const ARMstrongKDL::Chain& _chain,
		double _eps,
		int _maxiter,
		double _eps_joints
) :
    chain(_chain),
    nj(chain.getNrOfJoints()),
    ns(chain.getNrOfSegments()),
	lastNrOfIter(0),
	lastDifference(0),
    lastTransDiff(0),
	lastRotDiff(0),
	lastSV(nj>6?6:nj),
	jac(6, nj),
	grad(nj),
	display_information(false),
	maxiter(_maxiter),
	eps(_eps),
	eps_joints(_eps_joints),
	T_base_jointroot(nj),
	T_base_jointtip(nj),
	q(nj),
	A(nj, nj),
	ldlt(nj),
	svd(6, nj,Eigen::ComputeThinU | Eigen::ComputeThinV),
	diffq(nj),
	q_new(nj),
	original_Aii(nj)
{
	L(0)=1;
	L(1)=1;
	L(2)=1;
	L(3)=0.01;
	L(4)=0.01;
	L(5)=0.01;
}

void ChainIkSolverPos_LMA::updateInternalDataStructures() {
    nj = chain.getNrOfJoints();
    ns = chain.getNrOfSegments();
    lastSV.conservativeResize(nj>6?6:nj);
    jac.conservativeResize(Eigen::NoChange, nj);
    grad.conservativeResize(nj);
    T_base_jointroot.resize(nj);
    T_base_jointtip.resize(nj);
    q.conservativeResize(nj);
    A.conservativeResize(nj, nj);
    ldlt = Eigen::LDLT<MatrixXq>(nj);
    svd = Eigen::JacobiSVD<MatrixXq>(6, nj,Eigen::ComputeThinU | Eigen::ComputeThinV);
    diffq.conservativeResize(nj);
    q_new.conservativeResize(nj);
    original_Aii.conservativeResize(nj);
}

ChainIkSolverPos_LMA::~ChainIkSolverPos_LMA() {}

void ChainIkSolverPos_LMA::compute_fwdpos(const VectorXq& q) {
	using namespace ARMstrongKDL;
	unsigned int jointndx=0;
	T_base_head = Frame::Identity(); // frame w.r.t. base of head
	for (unsigned int i=0;i<chain.getNrOfSegments();i++) {
		const Segment& segment = chain.getSegment(i);
        if (segment.getJoint().getType()!=Joint::Fixed) {
			T_base_jointroot[jointndx] = T_base_head;
			T_base_head = T_base_head * segment.pose(q(jointndx));
			T_base_jointtip[jointndx] = T_base_head;
			jointndx++;
		} else {
			T_base_head = T_base_head * segment.pose(0.0);
		}
	}
}

void ChainIkSolverPos_LMA::compute_jacobian(const VectorXq& q) {
	using namespace ARMstrongKDL;
	unsigned int jointndx=0;
	for (unsigned int i=0;i<chain.getNrOfSegments();i++) {
		const Segment& segment = chain.getSegment(i);
        if (segment.getJoint().getType()!=Joint::Fixed) {
			// compute twist of the end effector motion caused by joint [jointndx]; expressed in base frame, with vel. ref. point equal to the end effector
			ARMstrongKDL::Twist t = ( T_base_jointroot[jointndx].M * segment.twist(q(jointndx),1.0) ).RefPoint( T_base_head.p - T_base_jointtip[jointndx].p);
			jac(0,jointndx)=t[0];
			jac(1,jointndx)=t[1];
			jac(2,jointndx)=t[2];
			jac(3,jointndx)=t[3];
			jac(4,jointndx)=t[4];
			jac(5,jointndx)=t[5];
			jointndx++;
		}
	}
}

void ChainIkSolverPos_LMA::display_jac(const ARMstrongKDL::JntArray& jval) {
	VectorXq q;
	q = jval.data.cast<ScalarType>();
	compute_fwdpos(q);
	compute_jacobian(q);
	svd.compute(jac);
	std::cout << "Singular values : " << svd.singularValues().transpose()<<"\n";
}


int ChainIkSolverPos_LMA::CartToJnt(const ARMstrongKDL::JntArray& q_init, const ARMstrongKDL::Frame& T_base_goal, ARMstrongKDL::JntArray& q_out) {
  if (nj != chain.getNrOfJoints())
    return (error = E_NOT_UP_TO_DATE);

  if (nj != q_init.rows() || nj != q_out.rows())
    return (error = E_SIZE_MISMATCH);

	using namespace ARMstrongKDL;
	double v      = 2;
	double tau    = 10;
	double rho;
	double lambda;
	Twist t;
	double delta_pos_norm;
	Eigen::Matrix<ScalarType,6,1> delta_pos;
	Eigen::Matrix<ScalarType,6,1> delta_pos_new;


	q=q_init.data.cast<ScalarType>();
	compute_fwdpos(q);
	Twist_to_Eigen( diff( T_base_head, T_base_goal), delta_pos );
	delta_pos=L.asDiagonal()*delta_pos;
	delta_pos_norm = delta_pos.norm();
	if (delta_pos_norm<eps) {
		lastNrOfIter    =0 ;
		Twist_to_Eigen( diff( T_base_head, T_base_goal), delta_pos );
		lastDifference  = delta_pos.norm();
		lastTransDiff   = delta_pos.topRows(3).norm();
		lastRotDiff     = delta_pos.bottomRows(3).norm();
		svd.compute(jac);
		original_Aii    = svd.singularValues();
		lastSV          = svd.singularValues();
		q_out.data      = q.cast<double>();
		return (error = E_NOERROR);
	}
	compute_jacobian(q);
	jac = L.asDiagonal()*jac;

	lambda = tau;
	double dnorm = 1;
	for (unsigned int i=0;i<maxiter;++i) {

		svd.compute(jac);
		original_Aii = svd.singularValues();
		for (unsigned int j=0;j<original_Aii.rows();++j) {
			original_Aii(j) = original_Aii(j)/( original_Aii(j)*original_Aii(j)+lambda);

		}
		tmp = svd.matrixU().transpose()*delta_pos;
		tmp = original_Aii.cwiseProduct(tmp);
		diffq = svd.matrixV()*tmp;
		grad = jac.transpose()*delta_pos;
		if (display_information) {
			std::cout << "------- iteration " << i << " ----------------\n"
					  << "  q              = " << q.transpose() << "\n"
					  << "  weighted jac   = \n" << jac << "\n"
					  << "  lambda         = " << lambda << "\n"
					  << "  eigenvalues    = " << svd.singularValues().transpose() << "\n"
					  << "  difference     = "   << delta_pos.transpose() << "\n"
					  << "  difference norm= "   << delta_pos_norm << "\n"
					  << "  proj. on grad. = "   << grad << "\n";
			std::cout << std::endl;
		}
		dnorm = diffq.lpNorm<Eigen::Infinity>();
		if (dnorm < eps_joints) {
				lastDifference = delta_pos_norm;
				lastNrOfIter   = i;
				lastSV         = svd.singularValues();
				q_out.data     = q.cast<double>();
				compute_fwdpos(q);
				Twist_to_Eigen( diff( T_base_head, T_base_goal), delta_pos );
				lastTransDiff  = delta_pos.topRows(3).norm();
				lastRotDiff    = delta_pos.bottomRows(3).norm();
				return (error = E_INCREMENT_JOINTS_TOO_SMALL);
		}


		if (grad.transpose()*grad < eps_joints*eps_joints ) {
			compute_fwdpos(q);
			Twist_to_Eigen( diff( T_base_head, T_base_goal), delta_pos );
			lastDifference = delta_pos_norm;
			lastTransDiff = delta_pos.topRows(3).norm();
			lastRotDiff   = delta_pos.bottomRows(3).norm();
			lastSV        = svd.singularValues();
			lastNrOfIter  = i;
			q_out.data    = q.cast<double>();
			return (error = E_GRADIENT_JOINTS_TOO_SMALL);
		}

		q_new = q+diffq;
		compute_fwdpos(q_new);
		Twist_to_Eigen( diff( T_base_head, T_base_goal), delta_pos_new );
		delta_pos_new             = L.asDiagonal()*delta_pos_new;
		double delta_pos_new_norm = delta_pos_new.norm();
		rho                       = delta_pos_norm*delta_pos_norm - delta_pos_new_norm*delta_pos_new_norm;
		rho                      /= diffq.transpose()*(lambda*diffq + grad);
		if (rho > 0) {
			q               = q_new;
			delta_pos       = delta_pos_new;
			delta_pos_norm  = delta_pos_new_norm;
			if (delta_pos_norm<eps) {
				Twist_to_Eigen( diff( T_base_head, T_base_goal), delta_pos );
				lastDifference = delta_pos_norm;
				lastTransDiff  = delta_pos.topRows(3).norm();
				lastRotDiff    = delta_pos.bottomRows(3).norm();
				lastSV         = svd.singularValues();
				lastNrOfIter   = i;
				q_out.data     = q.cast<double>();
				return (error = E_NOERROR);
			}
			compute_jacobian(q_new);
			jac = L.asDiagonal()*jac;
			double tmp=2*rho-1;
			lambda = lambda*max(1/3.0, 1-tmp*tmp*tmp);
			v = 2;
		} else {
			lambda = lambda*v;
			v      = 2*v;
		}
	}
	lastDifference = delta_pos_norm;
	lastTransDiff  = delta_pos.topRows(3).norm();
	lastRotDiff    = delta_pos.bottomRows(3).norm();
	lastSV         = svd.singularValues();
	lastNrOfIter   = maxiter;
	q_out.data     = q.cast<double>();
	return (error = E_MAX_ITERATIONS_EXCEEDED);

}

const char* ChainIkSolverPos_LMA::strError(const int error) const
    {
        if (E_GRADIENT_JOINTS_TOO_SMALL == error) return "The gradient of E towards the joints is to small";
        else if (E_INCREMENT_JOINTS_TOO_SMALL == error) return "The joint position increments are to small";
        else return SolverI::strError(error);
    }

}//namespace ARMstrongKDL
