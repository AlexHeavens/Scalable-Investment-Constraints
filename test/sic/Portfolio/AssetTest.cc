#include <gtest/gtest.h>

#include "sic/Portfolio/Asset.hh"

namespace {

class AssetTest : public testing::Test {
public:
  void SetUp() override {}
};

TEST_F(AssetTest, CreateValid) {

  const sic::price price = 100.00;
  sic::Asset validAsset(price);

  ASSERT_EQ(price, validAsset.getReferencePrice());
}

} // namespace
