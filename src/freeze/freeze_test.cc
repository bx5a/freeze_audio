#include "freeze/freeze.h"
#include "gtest/gtest.h"

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
