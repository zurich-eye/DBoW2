project(dbow2)
cmake_minimum_required(VERSION 2.8.3)

find_package(catkin_simple REQUIRED)
catkin_simple(ALL_DEPS_REQUIRED)

set(SRCS 
  src/BowVector.cpp     src/FBrief.cpp        src/FSurf64.cpp       src/FORB.cpp
  src/FeatureVector.cpp src/QueryResults.cpp  src/ScoringObject.cpp)

cs_add_library(${PROJECT_NAME} ${SRCS})

cs_add_executable(demo demo/demo.cpp)
target_link_libraries(demo ${PROJECT_NAME})
file(COPY demo/images DESTINATION ${CMAKE_BINARY_DIR}/)

cs_install()
cs_export()
