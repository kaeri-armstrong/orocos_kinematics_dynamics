// Copyright (C) 2013 Stephen Roderick <kiwi dot net at mac dot com>

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

#ifndef	__SOLVERI_HPP
#define	__SOLVERI_HPP

namespace ARMstrongKDL {

/**
 * Solver interface supporting storage and description of the latest error.
 *
 * Error codes: Zero (0) indicates no error, positive error codes indicate more
 * of a warning (e.g. a degraded solution, but motion can continue), and
 * negative error codes indicate failure (e.g. a singularity, and motion
 * can not continue).
 *
 * Error codes between -99 and +99 (inclusive) are reserved for system-wide
 * error codes. Derived classes should use values > +100, and < -100.
 *
 * Example use
 *
 * \code
 * class MySolver : public SolverI
 * {
 * public:
 *    static const int E_CHILDFAILD = xxx;
 *
 *    MySolver(SomeOtherSolver& other);
 *    virtual ~MySolver();
 * 	  int CartToJnt(...);
 *    virtual const char* strError(const int error) const;
 * protected:
 *    SomeOtherSolver& child;
 * };
 *
 * ...
 *
 * int MySolver::CartToJnt(...)
 * {
 *     error = child->SomeCall();
 *     if (E_NOERROR != error) {
 *         error = E_CHILDFAILED;
 *     } else {
 *         ...
 *     }
 *     return error;
 * }
 *
 * const char* MySolver::strError(const int error) const
 * {
 *     if (E_CHILDFAILED == error) return "Child solver failed";
 *     else return SolverI::strError(error);
 * }
 *
 * void someFunc()
 * {
 *     SomeOtherSolver child = new SomeOtherSolver(...);
 *     MySolver parent = new MySolver(child);
 *     ...
 *     int rc = parent->CartToJnt(...);
 *     if (E_NOERROR != rc) {
 *         if (MySolver::E_CHILDFAILED == rc) {
 *             rc = child->getError();
 *             // cope with child failure 'rc'
 *         }
 *     }
 *     ...
 * }
 * \endcode
 */
class SolverI
{
public:
    enum {
	/// Converged but degraded solution (e.g. WDLS with psuedo-inverse singular)
        E_DEGRADED         = +1,
    //! No error
        E_NOERROR          =  0,
    //! Failed to converge
        E_NO_CONVERGE      = -1,
    //! Undefined value (e.g. computed a NAN, or tan(90 degrees) )
        E_UNDEFINED        = -2,
    //! Chain size changed
        E_NOT_UP_TO_DATE = -3,
    //! Input size does not match internal state
        E_SIZE_MISMATCH = -4,
    //! Maximum number of iterations exceeded
        E_MAX_ITERATIONS_EXCEEDED = -5,
    //! Requested index out of range
        E_OUT_OF_RANGE = -6,
    //! Not yet implemented
        E_NOT_IMPLEMENTED = -7,
    //! Internal svd calculation failed
        E_SVD_FAILED = -8
    };

	/// Initialize latest error to E_NOERROR
	SolverI() :
			error(E_NOERROR)
	{}

	virtual ~SolverI()
	{}

	/// Return the latest error
	virtual int getError() const { return error; }

	/** Return a description of the latest error
		\return if \a error is known then a description of \a error, otherwise
		"UNKNOWN ERROR"
	*/
	virtual const char* strError(const int error) const
	{
		if (E_NOERROR == error) return "No error";
		else if (E_NO_CONVERGE == error) return "Failed to converge";
		else if (E_UNDEFINED == error) return "Undefined value";
		else if (E_DEGRADED == error) return "Converged but degraded solution";
		else if (E_NOT_UP_TO_DATE == error) return "Internal data structures not up to date with Chain";
		else if (E_SIZE_MISMATCH == error) return "The size of the input does not match the internal state";
		else if (E_MAX_ITERATIONS_EXCEEDED == error) return "The maximum number of iterations is exceeded";
		else if (E_OUT_OF_RANGE == error) return "The requested index is out of range";
		else if (E_NOT_IMPLEMENTED == error) return "The requested function is not yet implemented";
		else  if (E_SVD_FAILED == error) return "SVD failed";
		else return "UNKNOWN ERROR";
	}

	/**
	 * Update the internal data structures. This is required if the number
	 * of segments or number of joints of a chain/tree have changed.
	 * This provides a single point of contact for solver memory allocations.
	 */
	virtual void updateInternalDataStructures() = 0;

protected:
	/// Latest error, initialized to E_NOERROR in constructor
	int		error;
};

}	//	namespaces

#endif
