#include <gtest/gtest.h>

#include "sic/Model.hh"

namespace {

class ModelTest : public testing::Test {};
TEST_F(ModelTest, CreateValidModel) {

	sic::External::ID expExternalID = 434223l;
	sic::Model validModel(expExternalID);

	ASSERT_EQ(expExternalID, validModel.getExternalID());
}

} // namespace
