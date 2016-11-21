cmake_minimum_required(VERSION 3.2)

project(freeze CXX)
include(ExternalProject)

set(SRC ${CMAKE_SOURCE_DIR}/src)
include_directories(${SRC})

option(ENABLE_TESTS "Build Unit Tests" ON)

# thirdparty installed by configure script
set(eigen_install_dir ${CMAKE_BINARY_DIR}/eigen-install)
ExternalProject_Add(external_eigen
  HG_REPOSITORY "https://bitbucket.org/eigen/eigen/"
  HG_TAG "3.2.9"
  UPDATE_COMMAND ""
  CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX=${eigen_install_dir}"
)
set(eigen_include_dir ${eigen_install_dir}/include/eigen3)

# enable testing
if (${ENABLE_TESTS})
  enable_testing()

  set(gtest_install_dir ${CMAKE_BINARY_DIR}/gtest-install)
  ExternalProject_Add(external_gtest
    GIT_REPOSITORY "https://github.com/google/googletest.git"
    GIT_TAG "release-1.8.0"
    UPDATE_COMMAND ""
    CMAKE_ARGS
      "-DCMAKE_INSTALL_PREFIX=${gtest_install_dir}"
  )
  set(gtest_include_dir ${gtest_install_dir}/include)
  link_directories(${gtest_install_dir}/lib)
endif()

add_subdirectory(./src)