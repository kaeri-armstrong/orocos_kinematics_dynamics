# Copyright  (C)  2007  Ruben Smits <ruben dot smits at mech dot kuleuven dot be>

# Version: 1.0
# Author: Ruben Smits <ruben dot smits at mech dot kuleuven dot be>
# Maintainer: Ruben Smits <ruben dot smits at mech dot kuleuven dot be>
# URL: http://www.orocos.org/kdl

# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


import unittest
from PyKDL import *
from math import *


class FramesTestFunctions(unittest.TestCase):
    def testVector(self):
        v = Vector(3, 4, 5)
        self.testVectorImpl(v)
        v = Vector()
        self.testVectorImpl(v)

        v = Vector(3, 4, 5)
        self.assertFalse(v == -v)  # Doesn't work for zero vector
        self.assertFalse(Equal(v, -v))  # Doesn't work for zero vector
        self.assertTrue(v != -v)  # Doesn't work for zero vector
        self.assertTrue(not Equal(v, -v))  # Doesn't work for zero vector
        # Test member get and set functions
        self.assertEqual(v.x(), 3)
        v.x(1)
        self.assertEqual(v, Vector(1, 4, 5))
        self.assertEqual(v.y(), 4)
        v.y(1)
        self.assertEqual(v, Vector(1, 1, 5))
        self.assertEqual(v.z(), 5)
        v.z(1)
        self.assertEqual(v, Vector(1, 1, 1))
        # Test __getitem__
        self.assertEqual(v[0], 1)
        self.assertEqual(v[1], 1)
        self.assertEqual(v[2], 1)
        with self.assertRaises(IndexError):
            _ = v[-1]
        with self.assertRaises(IndexError):
            _ = v[3]
        # Test __setitem__
        v[0] = 3
        self.assertEqual(v[0], 3)
        v[1] = 4
        self.assertEqual(v[1], 4)
        v[2] = 5
        self.assertEqual(v[2], 5)
        with self.assertRaises(IndexError):
            v[-1] = 1
        with self.assertRaises(IndexError):
            v[3] = 1

        SetToZero(v)
        self.assertEqual(v, Vector(0, 0, 0))
        self.assertEqual(Vector.Zero(), Vector(0, 0, 0))

    def testVectorImpl(self, v):
        self.assertTrue(v == v)
        self.assertTrue(Equal(v, v))
        self.assertFalse(v != v)
        self.assertFalse(not Equal(v, v))
        self.assertEqual(2*v-v, v)
        self.assertEqual(v*2-v, v)
        self.assertEqual(v+v+v-2*v, v)
        v2 = Vector(v)
        self.assertEqual(v, v2)
        v2 += v
        self.assertEqual(2*v, v2)
        v2 -= v
        self.assertEqual(v, v2)
        v2.ReverseSign()
        self.assertEqual(v, -v2)
        for v2 in [Vector(v), -Vector(v)]:
            self.assertAlmostEqual(v2.Norm()**2, sum(map(lambda i: i * i, v2)), delta=1e-10)
            self.assertEqual(v2.Norm(), v2.Normalize())  # Norm before Normalize, so taking norm of un-normalized vector

        self.assertEqual(dot(v, v), sum(map(lambda i: i * i, v)))

    def testTwist(self):
        t = Twist(Vector(6, 3, 5), Vector(4, -2, 7))
        self.testTwistImpl(t)
        t = Twist()
        self.testTwistImpl(t)
        t = Twist(Vector(0, -9, -3), Vector(1, -2, -4))
        self.testTwistImpl(t)

        t = Twist(Vector(1, 2, 3), Vector(1, 2, 3))
        self.assertFalse(t == -t)  # Doesn't work for zero twist
        self.assertFalse(Equal(t, -t))  # Doesn't work for zero twist
        self.assertTrue(t != -t)  # Doesn't work for zero twist
        self.assertTrue(not Equal(t, -t))  # Doesn't work for zero twist

        v1 = Vector(1, 2, 3)
        v2 = Vector(4, 5, 6)
        t = Twist(v1, v2)
        self.assertEqual(t.vel, v1)
        self.assertEqual(t.rot, v2)
        # Test __getitem__
        for i in range(6):
            self.assertEqual(t[i], i+1)
        with self.assertRaises(IndexError):
            _ = t[-1]
        with self.assertRaises(IndexError):
            _ = t[6]
        # Test __setitem__
        for i in range(6):
            t[i] = i
        for i in range(6):
            self.assertEqual(t[i], i)
        with self.assertRaises(IndexError):
            t[-1] = 1
        with self.assertRaises(IndexError):
            t[6] = 1

        SetToZero(t)
        self.assertEqual(t, Twist())
        self.assertEqual(Twist.Zero(), Twist())

    def testTwistImpl(self, t):
        self.assertTrue(t == t)
        self.assertTrue(Equal(t, t))
        self.assertFalse(t != t)
        self.assertFalse(not Equal(t, t))
        self.assertEqual(2*t-t, t)
        self.assertEqual(t*2-t, t)
        self.assertEqual(t+t+t-2*t, t)
        t2 = Twist(t)
        self.assertEqual(t, t2)
        t2 += t
        self.assertEqual(2*t, t2)
        t2 -= t
        self.assertEqual(t, t2)
        t2.ReverseSign()
        self.assertEqual(t, -t2)
        v = Vector(1, 2, 3)
        t2 = t.RefPoint(v)
        self.assertEqual(t2.vel, t.vel + t.rot*v)
        self.assertEqual(t2.rot, t.rot)

        # No need to test the dot functions again for Wrench
        w = Wrench(v, v)
        dot_result = dot(t.vel, w.force) + dot(t.rot, w.torque)
        self.assertEqual(dot(t, w), dot_result)
        self.assertEqual(dot(w, t), dot_result)

    def testWrench(self):
        w = Wrench(Vector(7, -1, 3), Vector(2, -3, 3))
        self.testWrenchImpl(w)
        w = Wrench()
        self.testWrenchImpl(w)
        w = Wrench(Vector(2, 1, 4), Vector(5, 3, 1))
        self.testWrenchImpl(w)

        w = Wrench(Vector(1, 2, 3), Vector(1, 2, 3))
        self.assertFalse(w == -w)  # Doesn't work for zero wrench
        self.assertFalse(Equal(w, -w))  # Doesn't work for zero wrench
        self.assertTrue(w != -w)  # Doesn't work for zero wrench
        self.assertTrue(not Equal(w, -w))  # Doesn't work for zero wrench

        v1 = Vector(1, 2, 3)
        v2 = Vector(4, 5, 6)
        w = Wrench(v1, v2)
        self.assertEqual(w.force, v1)
        self.assertEqual(w.torque, v2)
        # Test __getitem__
        for i in range(6):
            self.assertEqual(w[i], i+1)
        with self.assertRaises(IndexError):
            _ = w[-1]
        with self.assertRaises(IndexError):
            _ = w[6]
        # Test __setitem__
        for i in range(6):
            w[i] = i
        for i in range(6):
            self.assertEqual(w[i], i)
        with self.assertRaises(IndexError):
            w[-1] = 1
        with self.assertRaises(IndexError):
            w[6] = 1

        SetToZero(w)
        self.assertEqual(w, Wrench())
        self.assertEqual(Wrench.Zero(), Wrench())

    def testWrenchImpl(self, w):
        self.assertEqual(2*w-w, w)
        self.assertEqual(w*2-w, w)
        self.assertEqual(w+w+w-2*w, w)
        w2 = Wrench(w)
        self.assertEqual(w, w2)
        w2 += w
        self.assertEqual(2*w, w2)
        w2 -= w
        self.assertEqual(w, w2)
        w2.ReverseSign()
        self.assertEqual(w, -w2)

        v = Vector(1, 2, 3)
        w2 = w.RefPoint(v)
        self.assertEqual(w2.force, w.force)
        self.assertEqual(w2.torque, w.torque + w.force*v)

    def testRotation(self):
        self.testRotationImpl(Vector(3, 4, 5), radians(10), radians(20), radians(30))
        self.testRotationImpl(Vector(), radians(10), radians(20), radians(30))
        self.testRotationImpl(Vector(3, 4, 5), radians(0), radians(0), radians(0))
        self.testRotationImpl(Vector(), radians(0), radians(0), radians(0))

        r = Rotation(*range(1, 10))
        # Test __getitem__
        for i in range(3):
            for j in range(3):
                self.assertEqual(r[i, j], 3*i+j+1)
        with self.assertRaises(IndexError):
            _ = r[-1, 0]
        with self.assertRaises(IndexError):
            _ = r[0, -1]
        with self.assertRaises(IndexError):
            _ = r[3, 2]
        with self.assertRaises(IndexError):
            _ = r[2, 3]
        # Test __setitem__
        for i in range(3):
            for j in range(3):
                r[i, j] = 3*i+j
        for i in range(3):
            for j in range(3):
                self.assertEqual(r[i, j], 3*i+j)
        with self.assertRaises(IndexError):
            r[-1, 0] = 1
        with self.assertRaises(IndexError):
            r[0, -1] = 1
        with self.assertRaises(IndexError):
            r[3, 2] = 1
        with self.assertRaises(IndexError):
            r[2, 3] = 1

    def testRotationImpl(self, v, a, b, c):
        w = Wrench(Vector(7, -1, 3), Vector(2, -3, 3))
        t = Twist(Vector(6, 3, 5), Vector(4, -2, 7))
        r = Rotation.RPY(a, b, c)

        self.assertAlmostEqual(dot(r.UnitX(), r.UnitX()), 1.0, 15)
        self.assertEqual(dot(r.UnitY(), r.UnitY()), 1.0)
        self.assertEqual(dot(r.UnitZ(), r.UnitZ()), 1.0)
        self.assertAlmostEqual(dot(r.UnitX(), r.UnitY()), 0.0, 15)
        self.assertAlmostEqual(dot(r.UnitX(), r.UnitZ()), 0.0, 15)
        self.assertEqual(dot(r.UnitY(), r.UnitZ()), 0.0)
        r2 = Rotation(r)
        self.assertEqual(r, r2)
        self.assertAlmostEqual((r*v).Norm(), v.Norm(), 14)
        self.assertEqual(r.Inverse(r*v), v)
        self.assertEqual(r.Inverse(r*t), t)
        self.assertEqual(r.Inverse(r*w), w)
        self.assertEqual(r*r.Inverse(v), v)
        self.assertEqual(r*Rotation.Identity(), r)
        self.assertEqual(Rotation.Identity()*r, r)
        self.assertEqual(r*(r*(r*v)), (r*r*r)*v)
        self.assertEqual(r*(r*(r*t)), (r*r*r)*t)
        self.assertEqual(r*(r*(r*w)), (r*r*r)*w)
        self.assertEqual(r*r.Inverse(), Rotation.Identity())
        self.assertEqual(r.Inverse()*r, Rotation.Identity())
        self.assertEqual(r.Inverse()*v, r.Inverse(v))
        (ra, rb, rc) = r.GetRPY()
        self.assertEqual(ra, a)
        self.assertEqual(rb, b)
        self.assertEqual(rc, c)
        r = Rotation.EulerZYX(a, b, c)
        (ra, rb, rc) = r.GetEulerZYX()
        self.assertEqual(ra, a)
        self.assertEqual(rb, b)
        self.assertEqual(rc, c)
        r = Rotation.EulerZYZ(a, b, c)
        (ra, rb, rc) = r.GetEulerZYZ()
        self.assertEqual(ra, a)
        self.assertEqual(rb, b)
        self.assertAlmostEqual(rc, c, 15)
        (angle, v2) = r.GetRotAngle()
        r2 = Rotation.Rot(v2, angle)
        self.assertEqual(r2, r)
        r2 = Rotation.Rot(v2*1E20, angle)
        self.assertEqual(r, r2)
        v2 = Vector(6, 2, 4)
        self.assertAlmostEqual(v2.Norm(), sqrt(dot(v2, v2)), 14)

    def testFrame(self):
        v = Vector(3, 4, 5)
        w = Wrench(Vector(7, -1, 3), Vector(2, -3, 3))
        t = Twist(Vector(6, 3, 5), Vector(4, -2, 7))
        F = Frame(Rotation.EulerZYX(radians(10), radians(20), radians(-10)), Vector(4, -2, 1))
        F2 = Frame(F)
        self.assertEqual(F, F2)
        self.assertEqual(F.Inverse(F*v), v)
        self.assertEqual(F.Inverse(F*t), t)
        self.assertEqual(F.Inverse(F*w), w)
        self.assertEqual(F*F.Inverse(v), v)
        self.assertEqual(F*F.Inverse(t), t)
        self.assertEqual(F*F.Inverse(w), w)
        self.assertEqual(F*Frame.Identity(), F)
        self.assertEqual(Frame.Identity()*F, F)
        self.assertEqual(F*(F*(F*v)), (F*F*F)*v)
        self.assertEqual(F*(F*(F*t)), (F*F*F)*t)
        self.assertEqual(F*(F*(F*w)), (F*F*F)*w)
        self.assertEqual(F*F.Inverse(), Frame.Identity())
        self.assertEqual(F.Inverse()*F, Frame.Identity())
        self.assertEqual(F.Inverse()*v, F.Inverse(v))

    def testPickle(self):
        import cPickle as pickle
        data = dict()
        data['v'] = Vector(1, 2, 3)
        data['rot'] = Rotation().EulerZYZ(1, 2, 3)
        data['fr'] = Frame(data['rot'], data['v'])
        data['tw'] = Twist(data['v'], Vector(4, 5, 6))
        data['wr'] = Wrench(Vector(0.1, 0.2, 0.3), data['v'])

        f = open('/tmp/pickle_test', 'w')
        pickle.dump(data, f, 2)
        f.close()

        f = open('/tmp/pickle_test', 'r')
        data1 = pickle.load(f)
        f.close()

        self.assertEqual(data, data1)


def suite():
    suite = unittest.TestSuite()
    suite.addTest(FramesTestFunctions('testVector'))
    suite.addTest(FramesTestFunctions('testTwist'))
    suite.addTest(FramesTestFunctions('testWrench'))
    suite.addTest(FramesTestFunctions('testRotation'))
    suite.addTest(FramesTestFunctions('testFrame'))
    suite.addTest(FramesTestFunctions('testPickle'))
    return suite


if __name__ == '__main__':
    suite = suite()
    unittest.TextTestRunner(verbosity=3).run(suite)
