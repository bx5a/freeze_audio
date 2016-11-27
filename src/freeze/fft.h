#ifndef FREEZE_FREEZE_FFT_H_
#define FREEZE_FREEZE_FFT_H_

#include <complex>
#include <vector>
#include <memory>

#include <Eigen/Core>

namespace freeze {

using Vector = Eigen::VectorXf;
using CplxVector = Eigen::VectorXcf;

class FFT {
 public:
  FFT();
  void Init(size_t nfft);
  void Forward(std::complex<float>* data);
  void Inverse(std::complex<float>* data);

 private:
  class Impl;
  std::shared_ptr<Impl> impl_;
};
}  // namespace freeze

#endif  // FREEZE_FREEZE_FFT_H_
