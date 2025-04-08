IF(ENABLE_EXAMPLES)
  INCLUDE_DIRECTORIES(${PROJ_SOURCE_DIR}/src ${PROJ_SOURCE_DIR}/models ${PROJ_BINARY_DIR}/src)

  add_executable(geometry geometry.cpp )
  TARGET_LINK_LIBRARIES(geometry armstrong-kdl)
  
  add_executable(trajectory_example trajectory_example.cpp )
  TARGET_LINK_LIBRARIES(trajectory_example armstrong-kdl)
  
  add_executable(chainiksolverpos_lma_demo chainiksolverpos_lma_demo.cpp )
  find_package(Boost REQUIRED)
  IF(${Boost_VERSION_MACRO} LESS 108300)
    TARGET_LINK_LIBRARIES(chainiksolverpos_lma_demo armstrong-kdl armstrong-kdl-models)
  ELSE()
    TARGET_LINK_LIBRARIES(chainiksolverpos_lma_demo boost_timer armstrong-kdl armstrong-kdl-models)
  ENDIF()

ENDIF(ENABLE_EXAMPLES)  

