// Copyright  (C)  2007  Ruben Smits <ruben dot smits at mech dot kuleuven dot be>

// Version: 1.0
// Author: Ruben Smits <ruben dot smits at mech dot kuleuven dot be>
// Maintainer: Ruben Smits <ruben dot smits at mech dot kuleuven dot be>
// URL: http://www.orocos.org/kdl

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include "chainiksolvervel_pinv.hpp"

namespace ARMstrongKDL
{
    ChainIkSolverVel_pinv::ChainIkSolverVel_pinv(const Chain& _chain,double _eps,int _maxiter):
        chain(_chain),
        jnt2jac(chain),
        nj(chain.getNrOfJoints()),
        jac(nj),
        svd(jac),
        U(6,JntArray(nj)),
        S(nj),
        V(nj,JntArray(nj)),
        tmp(nj),
        eps(_eps),
        maxiter(_maxiter),
        nrZeroSigmas(0),
        svdResult(0)
    {
    }

    void ChainIkSolverVel_pinv::updateInternalDataStructures() {
        jnt2jac.updateInternalDataStructures();
        nj = chain.getNrOfJoints();
        jac.resize(nj);
        svd = SVD_HH(jac);
        for(unsigned int i = 0 ; i < U.size(); i++)
            U[i].resize(nj);
        S.resize(nj);
        V.resize(nj);
        for(unsigned int i = 0 ; i < V.size(); i++)
            V[i].resize(nj);
        tmp.resize(nj);
    }

    ChainIkSolverVel_pinv::~ChainIkSolverVel_pinv()
    {
    }


    int ChainIkSolverVel_pinv::CartToJnt(const JntArray& q_in, const Twist& v_in, JntArray& qdot_out)
    {
        if (nj != chain.getNrOfJoints())
            return (error = E_NOT_UP_TO_DATE);

        if (nj != q_in.rows() || nj != qdot_out.rows())
            return (error = E_SIZE_MISMATCH);

        //Let the ChainJntToJacSolver calculate the jacobian "jac" for
        //the current joint positions "q_in" 
        error = jnt2jac.JntToJac(q_in,jac);
        if (error < E_NOERROR) return error;

        double sum;
        unsigned int i,j;

        // Initialize near zero singular value counter
        nrZeroSigmas = 0 ;

        //Do a singular value decomposition of "jac" with maximum
        //iterations "maxiter", put the results in "U", "S" and "V"
        //jac = U*S*Vt
        svdResult = svd.calculate(jac,U,S,V,maxiter);
        if (0 != svdResult)
        {
            qdot_out.data.setZero();
            return (error = E_SVD_FAILED);
        }

        // We have to calculate qdot_out = jac_pinv*v_in
        // Using the svd decomposition this becomes(jac_pinv=V*S_pinv*Ut):
        // qdot_out = V*S_pinv*Ut*v_in

        //first we calculate Ut*v_in
        for (i=0;i<jac.columns();i++) {
            sum = 0.0;
            for (j=0;j<jac.rows();j++) {
                sum+= U[j](i)*v_in(j);
            }
            //If the singular value is too small (<eps), don't invert it but
            //set the inverted singular value to zero (truncated svd)
            if ( fabs(S(i))<eps ) {
                tmp(i) = 0.0 ;
                //  Count number of singular values near zero
                ++nrZeroSigmas ;
            }
            else {
                tmp(i) = sum/S(i) ;
            }
        }
        //tmp is now: tmp=S_pinv*Ut*v_in, we still have to premultiply
        //it with V to get qdot_out
        for (i=0;i<jac.columns();i++) {
            sum = 0.0;
            for (j=0;j<jac.columns();j++) {
                sum+=V[i](j)*tmp(j);
            }
            //Put the result in qdot_out
            qdot_out(i)=sum;
        }

        // Note if the solution is singular, i.e. if number of near zero
        // singular values is greater than the full rank of jac
        if ( nrZeroSigmas > (jac.columns()-jac.rows()) ) {
            return (error = E_CONVERGE_PINV_SINGULAR);   // converged but pinv singular
        } else {
            return (error = E_NOERROR);                 // have converged
        }
    }

    const char* ChainIkSolverVel_pinv::strError(const int error) const
    {
        if (E_CONVERGE_PINV_SINGULAR == error) return "Converged put pseudo inverse of jacobian is singular.";
        else return SolverI::strError(error);
    }
}
