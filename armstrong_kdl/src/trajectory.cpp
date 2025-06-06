/***************************************************************************
  tag: Erwin Aertbelien  Mon May 10 19:10:36 CEST 2004  trajectory.cxx

                        trajectory.cxx -  description
                           -------------------
    begin                : Mon May 10 2004
    copyright            : (C) 2004 Erwin Aertbelien
    email                : erwin.aertbelien@mech.kuleuven.ac.be

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
/*****************************************************************************
 *  \author
 *  	Erwin Aertbelien, Div. PMA, Dep. of Mech. Eng., K.U.Leuven
 *
 *  \version
 *		ORO_Geometry V0.2
 *
 *	\par History
 *		- $log$
 *
 *	\par Release
 *		$Id: trajectory.cpp,v 1.1.1.1.2.4 2003/07/18 14:58:36 psoetens Exp $
 *		$Name:  $
 ****************************************************************************/


#include "utilities/error.h"
#include "utilities/error_stack.h"
#include "utilities/scoped_ptr.hpp"
#include "trajectory.hpp"
#include "path.hpp"
#include "trajectory_segment.hpp"

#include <memory>
#include <cstring>


namespace ARMstrongKDL {

Trajectory* Trajectory::Read(std::istream& is) {
	IOTrace("Trajectory::Read");
	char storage[64];
	EatWord(is,"[",storage,sizeof(storage));
	Eat(is,'[');
	if (strcmp(storage,"SEGMENT")==0) {
		IOTrace("SEGMENT");
		scoped_ptr<Path>      geom(    Path::Read(is)       );
		scoped_ptr<VelocityProfile> motprof( VelocityProfile::Read(is)  );
		EatEnd(is,']');
		IOTracePop();
		IOTracePop();
		return new  Trajectory_Segment(geom.release(),motprof.release());
	} else {
		throw Error_MotionIO_Unexpected_Traj();
	}
	return NULL; // just to avoid the warning;
}

}
