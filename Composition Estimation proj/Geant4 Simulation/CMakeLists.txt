cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(ErTry1)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)

file(GLOB MACRO_FILES "macros/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${CMAKE_BINARY_DIR}/)

add_executable(simex sim.cc ${sources})
target_link_libraries(simex ${Geant4_LIBRARIES})

add_custom_target(ErTry1 DEPENDS simex)