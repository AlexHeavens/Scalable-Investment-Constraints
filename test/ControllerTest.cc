#include <gtest/gtest.h>

#include "Controller.h"

namespace {

class ControllerTest : public testing::Test {
public:

  project::Controller c;

  ControllerTest() = default;

  void SetUp() override {}

};

TEST_F(ControllerTest, ValidRun) {

  ASSERT_FALSE(c.getHasExecuted());
  c.run();
  ASSERT_TRUE(c.getHasExecuted());

}

} // namespace
