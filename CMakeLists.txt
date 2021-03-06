project(dbow2)
cmake_minimum_required(VERSION 2.8.3)

find_package(catkin_simple REQUIRED)
catkin_simple(ALL_DEPS_REQUIRED)

PROTOBUF_CATKIN_GENERATE_CPP(PROTO_SRCS PROTO_HDRS proto/node.proto
                                                   proto/vocabulary.proto
                                                   proto/word.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(SRCS 
  src/BowVector.cpp     src/FBrief.cpp        src/FSurf64.cpp       src/FORB.cpp
  src/FeatureVector.cpp src/QueryResults.cpp  src/ScoringObject.cpp
  src/TemplatedVocabulary.cpp)

add_definitions("-std=c++11")

find_package(OpenCV 2 REQUIRED core imgproc features2d ml nonfree)
include_directories(include/DUtils include/DUtilsCV include/DVision ${OpenCV_INCLUDE_DIRS})
list(APPEND catkin_LIBRARIES ${OpenCV_LIBS})
message(STATUS "catkin_LIBRARIES: ${catkin_LIBRARIES}")

cs_add_library(${PROJECT_NAME} ${SRCS} ${PROTO_SRCS})

cs_add_executable(voc_to_proto src/voc_to_proto.cpp)
target_link_libraries(voc_to_proto ${PROJECT_NAME})

cs_add_executable(demo demo/demo.cpp)
target_link_libraries(demo ${PROJECT_NAME})
file(COPY demo/images DESTINATION ${CMAKE_BINARY_DIR}/)

cs_install()
# we need to add the current binary dir to include dirs due to protobuf
cs_export(INCLUDE_DIRS include ${CMAKE_CURRENT_BINARY_DIR})
