#include <gtest/gtest.h>

#include "sic/EvaluationContext.hh"

namespace {

class EvaluationContextTest : public testing::Test {};

TEST_F(EvaluationContextTest, CreateValid) {

	sic::EvaluationContext validContext;

	// Does nothing other than hold the object caches right now.
}

} // namespace
