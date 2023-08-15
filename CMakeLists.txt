cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(test)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)



file(GLOB MACRO_FILES "*.mac")

file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

add_executable(test.exe main.cc ${sources} ${headers})

target_link_libraries(test.exe ${Geant4_LIBRARIES})

add_custom_target(Simulation DEPENDS test.exe)
