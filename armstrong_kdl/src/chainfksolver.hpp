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

#ifndef KDL_CHAIN_FKSOLVER_HPP
#define KDL_CHAIN_FKSOLVER_HPP

#include "chain.hpp"
#include "framevel.hpp"
#include "frameacc.hpp"
#include "jntarray.hpp"
#include "jntarrayvel.hpp"
#include "jntarrayacc.hpp"
#include "solveri.hpp"

namespace ARMstrongKDL {
    /**
     * \brief This <strong>abstract</strong> class encapsulates a
     * solver for the forward position kinematics for a ARMstrongKDL::Chain.
     *
     * @ingroup KinematicFamily
     */
    //Forward definition
    class ChainFkSolverPos : public ARMstrongKDL::SolverI {
    public:
        /**
         * Calculate forward position kinematics for a ARMstrongKDL::Chain,
         * from joint coordinates to cartesian pose.
         *
         * @param q_in input joint coordinates
         * @param p_out reference to output cartesian pose
         *
         * @return if < 0 something went wrong
         */
        virtual int JntToCart(const JntArray& q_in, Frame& p_out,int segmentNr=-1)=0;
        /**
         * Calculate forward position kinematics for a ARMstrongKDL::Chain,
         * from joint coordinates to cartesian pose.
         *
         * @param q_in input joint coordinates
         * @param p_out reference to a vector of output cartesian poses for all segments
         *
         * @return if < 0 something went wrong
         */
        virtual int JntToCart(const JntArray& q_in, std::vector<ARMstrongKDL::Frame>& p_out,int segmentNr=-1)=0;

        virtual void updateInternalDataStructures()=0;
        virtual ~ChainFkSolverPos(){};
    };

    /**
     * \brief This <strong>abstract</strong> class encapsulates a solver
     * for the forward velocity kinematics for a ARMstrongKDL::Chain.
     *
     * @ingroup KinematicFamily
     */
    class ChainFkSolverVel : public ARMstrongKDL::SolverI {
    public:
        /**
         * Calculate forward position and velocity kinematics, from
         * joint coordinates to cartesian coordinates.
         *
         * @param q_in input joint coordinates (position and velocity)
         * @param out output cartesian coordinates (position and velocity)
         *
         * @return if < 0 something went wrong
         */
        virtual int JntToCart(const JntArrayVel& q_in, FrameVel& out,int segmentNr=-1)=0;
        /**
         * Calculate forward position and velocity kinematics, from
         * joint coordinates to cartesian coordinates.
         *
         * @param q_in input joint coordinates (position and velocity)
         * @param out output cartesian coordinates for all segments (position and velocity)
         *
         * @return if < 0 something went wrong
         */
        virtual int JntToCart(const JntArrayVel& q_in, std::vector<ARMstrongKDL::FrameVel>& out,int segmentNr=-1)=0;

        virtual void updateInternalDataStructures()=0;
        virtual ~ChainFkSolverVel(){};
    };

    /**
     * \brief This <strong>abstract</strong> class encapsulates a solver
     * for the forward acceleration kinematics for a ARMstrongKDL::Chain.
     *
     * @ingroup KinematicFamily
     */
    class ChainFkSolverAcc : public ARMstrongKDL::SolverI {
    public:
        /**
         * Calculate forward position, velocity and acceleration
         * kinematics, from joint coordinates to cartesian coordinates
         *
         * @param q_in input joint coordinates (position, velocity and
         * acceleration
         @param out output cartesian coordinates (position, velocity
         * and acceleration
         *
         * @return if < 0 something went wrong
         */
    virtual int JntToCart(const JntArrayAcc& q_in, FrameAcc& out,int segmentNr=-1)=0;
        /**
         * Calculate forward position, velocity and acceleration
         * kinematics, from joint coordinates to cartesian coordinates
         *
         * @param q_in input joint coordinates (position, velocity and
         * acceleration
         * @param out output cartesian coordinates (position, velocity
         * and acceleration for all segments
         *
         * @return if < 0 something went wrong
         */
    virtual int JntToCart(const JntArrayAcc& q_in, std::vector<FrameAcc>& out,int segmentNr=-1)=0;
    
        virtual void updateInternalDataStructures()=0;
        virtual ~ChainFkSolverAcc()=0;
    };


}//end of namespace ARMstrongKDL

#endif
