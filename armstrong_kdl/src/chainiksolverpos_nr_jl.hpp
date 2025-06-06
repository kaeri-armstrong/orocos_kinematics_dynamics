// Copyright  (C)  2007-2008  Ruben Smits <ruben dot smits at mech dot kuleuven dot be>
// Copyright  (C)  2008  Mikael Mayer
// Copyright  (C)  2008  Julia Jesse

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

#ifndef KDLCHAINIKSOLVERPOS_NR_JL_HPP
#define KDLCHAINIKSOLVERPOS_NR_JL_HPP

#include "chainiksolver.hpp"
#include "chainfksolver.hpp"

namespace ARMstrongKDL {

    /**
     * Implementation of a general inverse position kinematics
     * algorithm based on Newton-Raphson iterations to calculate the
     * position transformation from Cartesian to joint space of a general
     * ARMstrongKDL::Chain. Takes joint limits into account.
     *
     * @ingroup KinematicFamily
     */
    class ChainIkSolverPos_NR_JL : public ChainIkSolverPos
    {
    public:

        static const int E_IKSOLVERVEL_FAILED = -100; //! Child IK solver vel failed
        static const int E_FKSOLVERPOS_FAILED = -101; //! Child FK solver failed

        /**
         * Constructor of the solver, it needs the chain, a forward
         * position kinematics solver and an inverse velocity
         * kinematics solver for that chain.
         *
         * @param chain the chain to calculate the inverse position for
         * @param q_min the minimum joint positions
         * @param q_max the maximum joint positions
         * @param fksolver a forward position kinematics solver
         * @param iksolver an inverse velocity kinematics solver
         * @param maxiter the maximum Newton-Raphson iterations,
         * default: 100
         * @param eps the precision for the position, used to end the
         * iterations, default: epsilon (defined in kdl.hpp)
         *
         * @return
         */
        ChainIkSolverPos_NR_JL(const Chain& chain,const JntArray& q_min, const JntArray& q_max, ChainFkSolverPos& fksolver,ChainIkSolverVel& iksolver,unsigned int maxiter=100,double eps=1e-6);

        /**
         * Constructor of the solver, it needs the chain, a forward
         * position kinematics solver and an inverse velocity
         * kinematics solver for that chain.
         *
         * @param chain the chain to calculate the inverse position for
         * @param fksolver a forward position kinematics solver
         * @param iksolver an inverse velocity kinematics solver
         * @param maxiter the maximum Newton-Raphson iterations,
         * default: 100
         * @param eps the precision for the position, used to end the
         * iterations, default: epsilon (defined in kdl.hpp)
         *
         * @return
         */
        ChainIkSolverPos_NR_JL(const Chain& chain, ChainFkSolverPos& fksolver,ChainIkSolverVel& iksolver,unsigned int maxiter=100,double eps=1e-6);

        ~ChainIkSolverPos_NR_JL();


        /**
         * Calculates the joint values that correspond to the input pose given an initial guess.
         * @param q_init Initial guess for the joint values.
         * @param p_in The input pose of the chain tip.
         * @param q_out The resulting output joint values
         * @return E_MAX_ITERATIONS_EXCEEDED if the maximum number of iterations was exceeded before a result was found
         *         E_NOT_UP_TO_DATE if the internal data is not up to date with the chain
         *         E_SIZE_MISMATCH if the size of the input/output data does not match the chain.
         */
        virtual int CartToJnt(const JntArray& q_init, const Frame& p_in, JntArray& q_out);

        /**
         * Function to set the joint limits.
         * @param q_min minimum values for the joints
         * @param q_max maximum values for the joints
         * @return E_SIZE_MISMATCH if input sizes do not match the chain
         */
        int setJointLimits(const JntArray& q_min, const JntArray& q_max);

        /// @copydoc ARMstrongKDL::SolverI::updateInternalDataStructures
        virtual void updateInternalDataStructures();

        /// @copydoc ARMstrongKDL::SolverI::strError()
        const char* strError(const int error) const;

    private:
        const Chain& chain;
        unsigned int nj;
        JntArray q_min;
        JntArray q_max;
        ChainIkSolverVel& iksolver;
        ChainFkSolverPos& fksolver;
        JntArray delta_q;
        unsigned int maxiter;
        double eps;

        Frame f;
        Twist delta_twist;

    };

}

#endif
