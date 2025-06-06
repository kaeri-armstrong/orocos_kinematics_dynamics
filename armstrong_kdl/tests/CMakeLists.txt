IF(ENABLE_TESTS)
  ENABLE_TESTING()

  INCLUDE_DIRECTORIES(${PROJ_SOURCE_DIR}/src ${CPPUNIT_HEADERS} ${PROJECT_BINARY_DIR}/src)

  ADD_EXECUTABLE(framestest framestest.cpp test-runner.cpp)
  TARGET_LINK_LIBRARIES(framestest armstrong-kdl ${CPPUNIT})
  SET(TESTNAME "framestest")
  SET_TARGET_PROPERTIES( framestest PROPERTIES
   COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${KDL_CFLAGS} -DTESTNAME=\"\\\"${TESTNAME}\\\"\" ")
  ADD_TEST(NAME framestest COMMAND framestest)

 ADD_EXECUTABLE(kinfamtest kinfamtest.cpp test-runner.cpp)
 TARGET_LINK_LIBRARIES(kinfamtest armstrong-kdl ${CPPUNIT})
 SET(TESTNAME "kinfamtest")
 SET_TARGET_PROPERTIES( kinfamtest PROPERTIES
  COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${KDL_CFLAGS}")
 ADD_TEST(NAME kinfamtest COMMAND kinfamtest)

 ADD_EXECUTABLE(solvertest solvertest.cpp test-runner.cpp)
 TARGET_LINK_LIBRARIES(solvertest armstrong-kdl ${CPPUNIT})
 SET(TESTNAME "solvertest")
 SET_TARGET_PROPERTIES( solvertest PROPERTIES
  COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${KDL_CFLAGS} -DTESTNAME=\"\\\"${TESTNAME}\\\"\" ")
 ADD_TEST(NAME solvertest COMMAND solvertest)

 ADD_EXECUTABLE(inertiatest inertiatest.cpp test-runner.cpp)
 TARGET_LINK_LIBRARIES(inertiatest armstrong-kdl ${CPPUNIT})
 SET(TESTNAME "inertiatest")
 SET_TARGET_PROPERTIES( inertiatest PROPERTIES
  COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${KDL_CFLAGS} -DTESTNAME=\"\\\"${TESTNAME}\\\"\" ")
 ADD_TEST(NAME inertiatest COMMAND inertiatest)

 ADD_EXECUTABLE(jacobiantest jacobiantest.cpp test-runner.cpp)
 SET(TESTNAME "jacobiantest")
 TARGET_LINK_LIBRARIES(jacobiantest armstrong-kdl ${CPPUNIT})
 SET_TARGET_PROPERTIES( jacobiantest PROPERTIES
  COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${KDL_CFLAGS} -DTESTNAME=\"\\\"${TESTNAME}\\\"\" ")
 ADD_TEST(NAME jacobiantest COMMAND jacobiantest)

 ADD_EXECUTABLE(jacobiandottest jacobiandottest.cpp test-runner.cpp)
 SET(TESTNAME "jacobiandottest")
 TARGET_LINK_LIBRARIES(jacobiandottest armstrong-kdl ${CPPUNIT})
 SET_TARGET_PROPERTIES( jacobiandottest PROPERTIES
  COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${KDL_CFLAGS} -DTESTNAME=\"\\\"${TESTNAME}\\\"\" ")
 ADD_TEST(NAME jacobiandottest COMMAND jacobiandottest)

 ADD_EXECUTABLE(velocityprofiletest velocityprofiletest.cpp test-runner.cpp)
 SET(TESTNAME "velocityprofiletest")
 TARGET_LINK_LIBRARIES(velocityprofiletest armstrong-kdl ${CPPUNIT})
 SET_TARGET_PROPERTIES( velocityprofiletest PROPERTIES
   COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${KDL_CFLAGS} -DTESTNAME=\"\\\"${TESTNAME}\\\"\" ")
 ADD_TEST(NAME velocityprofiletest COMMAND velocityprofiletest)

 ADD_EXECUTABLE(treeinvdyntest treeinvdyntest.cpp test-runner.cpp)
 SET(TESTNAME "treeinvdyntest")
 TARGET_LINK_LIBRARIES(treeinvdyntest armstrong-kdl ${CPPUNIT})
 SET_TARGET_PROPERTIES( treeinvdyntest PROPERTIES
   COMPILE_FLAGS "${CMAKE_CXX_FLAGS_ADD} ${KDL_CFLAGS} -DTESTNAME=\"\\\"${TESTNAME}\\\"\" ")
 ADD_TEST(NAME treeinvdyntest COMMAND treeinvdyntest)

#  ADD_EXECUTABLE(rframestest  rframestest.cpp)
#  TARGET_LINK_LIBRARIES(rframestest armstrong-kdl)
#  ADD_TEST(NAME rframestest COMMAND rframestest)
#
#  ADD_EXECUTABLE(rallnumbertest  rallnumbertest.cpp)
#  TARGET_LINK_LIBRARIES(rallnumbertest armstrong-kdl)
#  ADD_TEST(NAME rallnumbertest COMMAND rallnumbertest)

  ADD_CUSTOM_TARGET(check ctest -V WORKING_DIRECTORY ${PROJ_BINARY_DIR}/tests)
ENDIF(ENABLE_TESTS)
