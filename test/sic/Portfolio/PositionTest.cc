#include <gtest/gtest.h>

#include "sic/Portfolio/Asset.hh"
#include "sic/Portfolio/Position.hh"

namespace {

class PositionTest : public testing::Test {
public:
  void SetUp() override {}
};

TEST_F(PositionTest, CreateValid) {

  static const sic::price price = 100.00;

  class MockAsset : public sic::Asset {

  public:
    explicit MockAsset(sic::price price) : sic::Asset(price) {}

    sic::price getReferencePrice() const { return price; }
  };

  MockAsset asset(price);

  sic::Position validPosition(asset);

  ASSERT_EQ(&asset, &validPosition.getAsset());
}

} // namespace
