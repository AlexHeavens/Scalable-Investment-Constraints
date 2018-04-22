#include <gtest/gtest.h>

#include "sic/Portfolio/Asset.hh"
#include "sic/Portfolio/Position.hh"

namespace {

class PositionTest : public testing::Test {
public:
  void SetUp() override {}
};

TEST_F(PositionTest, CreateValid) {

  static const sic::Price price = 100.00;

  class MockAsset : public sic::Asset {

  public:
    explicit MockAsset(sic::Price price) : sic::Asset(price) {}

    sic::Price getReferencePrice() const { return price; }
  };

  MockAsset asset(price);

  const sic::Value value = 333.33;
  sic::Position validPosition(asset, value);

  ASSERT_EQ(&asset, &validPosition.getAsset());
  ASSERT_EQ(value, validPosition.getReferenceValue());
}

} // namespace
