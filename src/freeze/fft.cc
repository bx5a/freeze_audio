#include <iostream>
#include "freeze/fft.h"
// TODO: quick and dirty. find a cleaner way to fix that
extern "C" {
#include "libavcodec/avfft.h"
}

namespace freeze {

class FFT::Impl {
 public:
  FFTContext* forward_ctx;
  FFTContext* inverse_ctx;

  float nfft;
};

FFT::FFT() : impl_(std::make_shared<FFT::Impl>()) {}

void FFT::Init(size_t nfft) {
  impl_->nfft = nfft;
  impl_->forward_ctx = av_fft_init(log2(nfft), 0);
  impl_->inverse_ctx = av_fft_init(log2(nfft), 1);
}
void FFT::Forward(std::complex<float>* data) {
  std::vector<FFTComplex> cplx(impl_->nfft);
  for (size_t index = 0; index < cplx.size(); index++) {
    cplx[index].re = data[index].real();
    cplx[index].im = data[index].imag();
  }

  av_fft_permute(impl_->forward_ctx, cplx.data());
  av_fft_calc(impl_->forward_ctx, cplx.data());

  for (size_t index = 0; index < cplx.size(); index++) {
    data[index] = std::complex<float>(cplx[index].re, cplx[index].im);
  }
}
void FFT::Inverse(std::complex<float>* data) {
  std::vector<FFTComplex> cplx(impl_->nfft);
  for (size_t index = 0; index < cplx.size(); index++) {
    cplx[index].re = data[index].real();
    cplx[index].im = data[index].imag();
  }

  av_fft_permute(impl_->inverse_ctx, cplx.data());
  av_fft_calc(impl_->inverse_ctx, cplx.data());

  for (size_t index = 0; index < cplx.size(); index++) {
    data[index] = std::complex<float>(cplx[index].re / impl_->nfft);
  }
}

}  // namespace freeze