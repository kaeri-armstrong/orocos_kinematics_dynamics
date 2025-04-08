#ifndef KDLPV_TRAITS_H 
#define KDLPV_TRAITS_H 

#include "utility.h"

 
// forwards declarations :
namespace ARMstrongKDL {
   class Frame;
    class Rotation;
    class Vector;
    class Twist;
    class Wrench;
	class FrameVel;
	class RotationVel;
	class VectorVel;
	class TwistVel;
}


/**
 * @brief Traits are traits classes to determine the type of a derivative of another type.
 *
 * For geometric objects the "geometric" derivative is chosen.  For example the derivative of a Rotation
 * matrix is NOT a 3x3 matrix containing the derivative of the elements of a rotation matrix.  The derivative
 * of the rotation matrix is a Vector corresponding the rotational velocity.  Mostly used in template classes
 * and routines to derive a correct type when needed.
 * 
 * You can see this as a compile-time lookuptable to find the type of the derivative.
 *
 * Example
 * \verbatim
	Rotation R;
    Traits<Rotation> dR;
   \endverbatim
 */
template <typename T>
struct Traits {
	typedef T valueType;
	typedef T derivType;
};

template <>
struct Traits<ARMstrongKDL::Frame> {
	typedef ARMstrongKDL::Frame valueType;
	typedef ARMstrongKDL::Twist derivType;
};
template <>
struct Traits<ARMstrongKDL::Twist> {
	typedef ARMstrongKDL::Twist valueType;
	typedef ARMstrongKDL::Twist derivType;
};
template <>
struct Traits<ARMstrongKDL::Wrench> {
	typedef ARMstrongKDL::Wrench valueType;
	typedef ARMstrongKDL::Wrench derivType;
};

template <>
struct Traits<ARMstrongKDL::Rotation> {
	typedef ARMstrongKDL::Rotation valueType;
	typedef ARMstrongKDL::Vector derivType;
};

template <>
struct Traits<ARMstrongKDL::Vector> {
	typedef ARMstrongKDL::Vector valueType;
	typedef ARMstrongKDL::Vector derivType;
};

template <>
struct Traits<double> {
	typedef double valueType;
	typedef double derivType;
};

template <>
struct Traits<float> {
	typedef float valueType;
	typedef float derivType;
};

template <>
struct Traits<ARMstrongKDL::FrameVel> {
	typedef ARMstrongKDL::Frame valueType;
	typedef ARMstrongKDL::TwistVel derivType;
};
template <>
struct Traits<ARMstrongKDL::TwistVel> {
	typedef ARMstrongKDL::Twist valueType;
	typedef ARMstrongKDL::TwistVel derivType;
};

template <>
struct Traits<ARMstrongKDL::RotationVel> {
	typedef ARMstrongKDL::Rotation valueType;
	typedef ARMstrongKDL::VectorVel derivType;
};

template <>
struct Traits<ARMstrongKDL::VectorVel> {
	typedef ARMstrongKDL::Vector valueType;
	typedef ARMstrongKDL::VectorVel derivType;
};



#endif



