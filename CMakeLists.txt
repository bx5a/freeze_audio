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

set(fftw_source_dir ${CMAKE_BINARY_DIR}/fftw-source)
set(fftw_install_dir ${CMAKE_BINARY_DIR}/fftw-install)
ExternalProject_Add(external_fftw
  URL "http://www.fftw.org/fftw-3.3.5.tar.gz"
  URL_MD5 6cc08a3b9c7ee06fdd5b9eb02e06f569
  SOURCE_DIR ${fftw_source_dir}
  BINARY_DIR ${fftw_source_dir}
  INSTALL_DIR ${fftw_install_dir}
  UPDATE_COMMAND ""
  CONFIGURE_COMMAND ./configure --prefix=${fftw_install_dir} --enable-static --enable-float
)
link_directories(${fftw_install_dir}/lib)
set(fftw_include_dir ${fftw_install_dir}/include)

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

  # test resources
  set(test_resource_dir ${CMAKE_BINARY_DIR}/test-resources)
  ExternalProject_Add(external_test_resource
    GIT_REPOSITORY "https://github.com/romi1502/freeze_audio.git"
    UPDATE_COMMAND ""
    SOURCE_DIR "${test_resource_dir}"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND cd ${test_resource_dir} && python test_script.py
    INSTALL_COMMAND ""
  )
  set(original_test_file "${test_resource_dir}/test_sample.wav")
  set(processed_test_file "${test_resource_dir}/test_sample_processed.wav")

  # wave library
  set(wave_install_dir ${CMAKE_BINARY_DIR}/wave-install)
  ExternalProject_Add(external_wave
    GIT_REPOSITORY "https://github.com/audionamix/wave.git"
    GIT_TAG "v0.4a"
    UPDATE_COMMAND ""
    CMAKE_ARGS
      "-DCMAKE_INSTALL_PREFIX=${wave_install_dir}"
  )
  set(wave_include_dir ${wave_install_dir}/include)
  link_directories(${wave_install_dir}/lib)
endif()

add_subdirectory(./src)
