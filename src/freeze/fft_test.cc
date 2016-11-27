#include <gtest/gtest.h>

#include <iostream>
#include <vector>
#include "freeze/fft.h"

TEST(FFT, Symetric) {
  using namespace freeze;

  std::vector<float> input(2048);
  std::vector<std::complex<float> > output(2048 / 2 + 1);
  for (size_t index = 0; index < input.size(); index++) {
    input[index] = sin(2 * M_PI * 440 * (static_cast<float>(index) / 44100));
  }
  std::vector<float> copy = input;

  FFT fft;
  fft.Init(2048);
  fft.Forward(input.data(), output.data());
  fft.Inverse(output.data(), input.data());

  for (size_t index = 0; index < output.size(); index++) {
    ASSERT_LT(fabs(copy[index] - input[index]), pow(10, -6));
  }
}