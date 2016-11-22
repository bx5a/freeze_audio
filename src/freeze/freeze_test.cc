#include <gtest/gtest.h>
#include "wave/file.h"

#include "freeze/freeze.h"

TEST(freezer, Disabled) {
  const size_t kChannelNumber = 2;
  const size_t kSampleNumber = 2048;

  std::error_code err;
  std::vector<float> input(kSampleNumber * kChannelNumber);

  freeze::Freezer freezer;
  freezer.Init(kChannelNumber);

  freezer.Write(input, err);
  ASSERT_FALSE(err);

  auto output = freezer.Read(err);
  ASSERT_FALSE(err);

  ASSERT_EQ(input.size(), output.size());

  freezer.Enable();

  freezer.Write(input, err);
  ASSERT_FALSE(err);
  output = freezer.Read(err);
  ASSERT_FALSE(err);

  ASSERT_EQ(input.size(), output.size());
}

TEST(freezer, Numerical) {
  std::error_code err;

  wave::File original;
  original.Open(ORIGINAL_WAVE);
  auto original_content = original.Read();
  auto frame_number = original_content.size() / original.channel_number();

  // buffer_len = 256
  const size_t kBufferLen = 256;
  // processor = FreezeProcessor(Nfft=2048, overlap_factor=0.5, n_channel=2)
  freeze::Freezer freezer;
  freezer.Init(original.channel_number(), 2048, 0.5);

  //# time at which effect is switched on
  //  start_sample = 0.9*fs
  auto start_sample = 0.9 * original.sample_rate();
  //# time at which effect is switched off
  //  end_sample = 3.0*fs
  auto end_sample = 3.0 * original.sample_rate();

  //#  start gain of dry signal
  // dry_gain = 1
  float dry_gain = 1;

  // output_signal = np.zeros(input_signal.shape)
  std::vector<float> output_signal(original_content.size());
  // for k in range(0, int(input_signal.shape[0]), buffer_len):
  for (size_t k = 0; k < frame_number; k = k + kBufferLen) {
    std::cout << "processing frame " << k << "/" << frame_number << std::endl;
    //
    //# effect is switched on
    // if k <= start_sample < k+buffer_len:
    if (k <= start_sample && start_sample < k + kBufferLen) {
      // processor.is_on = True
      freezer.Enable();
    }
    //
    //# effect is switched off
    // if k <= end_sample < k+buffer_len:
    if (k <= end_sample && end_sample < k + kBufferLen) {
      // processor.is_on = False
      freezer.Disable();
    }
    // if processor.is_on:
    if (freezer.IsEnabled()) {
      // dry_gain*=0.8
      dry_gain *= 0.8;
    } else {
      // dry_gain = 1 - (1-dry_gain)*0.8;
      dry_gain = 1 - (1 - dry_gain) * 0.8;
    }

    // output_signal[k:k+buffer_len]+=
    // processor.process_buffer(input_signal[k:k+buffer_len,:])
    auto sample_start = (k * original.channel_number());
    auto sample_end =
        std::min(k * original.channel_number(), original_content.size());
    
    std::vector<float> frame(kBufferLen);
    memcpy(frame.data(), original_content.data() + sample_start, sample_end - sample_start);
    freezer.Write(frame, err);
    ASSERT_FALSE(err);
    auto result_frame = freezer.Read(err);
    ASSERT_FALSE(err);
    
    // output_signal[k:k+buffer_len]+= dry_gain*input_signal[k:k+buffer_len,:]
    for (size_t index = k; index < k+kBufferLen && index < output_signal.size(); index++) {
      output_signal[k] = frame[index - k] + dry_gain * original_content[index - k];
    }
  }
  
  // TODO: check content
}
