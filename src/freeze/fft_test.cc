#include <gtest/gtest.h>

#include "freeze/fft.h"
#include <iostream>

TEST(FFT, Symetric) {
  
  using namespace freeze;
  
  std::vector<std::complex<float> > input(2048);
  for (size_t index = 0; index < input.size(); index++) {
    input[index] = sin(2 * M_PI * 440 * (static_cast<float>(index)/44100));
  }
  auto copy = input;
  
  FFT fft;
  fft.Init(2048);
  fft.Forward(input.data());
  fft.Inverse(input.data());
  
//  ASSERT_EQ(copy, input);
  for (size_t index = 0; index < input.size(); index++) {
    std::cout << copy[index] << " " << input[index] << std::endl;
  }
}