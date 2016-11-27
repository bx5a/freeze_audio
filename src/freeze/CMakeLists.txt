set(sources
  ${SRC}/freeze/freeze.h
  ${SRC}/freeze/freeze.cc
  ${SRC}/freeze/fft.h
  ${SRC}/freeze/fft.cc
)

include_directories(${eigen_src})

add_library(
  freeze
  ${sources}
)
target_link_libraries(freeze
avcodec avdevice avfilter avformat avutil swresample swscale)
add_dependencies(freeze external_eigen)# external_ffmpeg)
target_include_directories(freeze PRIVATE
  ${eigen_include_dir}
  ${ffmpeg_include_dir}
)

if (${ENABLE_TESTS})
  add_executable(
    freeze_test
    ${SRC}/freeze/fft_test.cc
    ${SRC}/freeze/freeze_test.cc
  )
  add_dependencies(freeze_test
    external_gtest
    external_test_resource
    external_wave
  )
  target_link_libraries(freeze_test
    gtest
    gtest_main
    freeze
    wave
  )
  target_compile_definitions(freeze_test
    PRIVATE
      -DORIGINAL_WAVE="${original_test_file}"
      -DEXPECTED_WAVE="${processed_test_file}"
  )
  target_include_directories(freeze_test
    PRIVATE ${gtest_include_dir} ${eigen_include_dir} ${wave_include_dir}
  )
endif()
