#include <gtest/gtest.h>

#include "sic/Model/MPFAllocationNode.hh"
#include "sic/Model/MockModelPortfolio.hh"
#include "sic/Portfolio/MockAsset.hh"

namespace {

class MPFAllocationNodeTest : public testing::Test {};

TEST_F(MPFAllocationNodeTest, CreateValid) {

	const sic::WeightRange expRange(0.1, 0.1, 0.1);
	const sic::MockModelPortfolio expMPF;
	const sic::External::ID expID = 123;

	sic::MPFAllocationNode validNode(expMPF, expRange, expID);

	ASSERT_EQ(validNode.getWeightRange(), expRange);
	ASSERT_EQ(&validNode.getModelPortfolio(), &expMPF);
	ASSERT_EQ(validNode.getExternalID(), expID);
}

TEST_F(MPFAllocationNodeTest, attachToTopAssetWeights) {

	sic::MockModelPortfolio mpf;

	constexpr int assetCount = 5;
	std::vector<sic::MockAsset> assets;
	assets.resize(assetCount);

	sic::AbstractAsset::AssetWeightMap mpfWeights;
	mpfWeights.insert({&assets.at(0), sic::WeightRange(0.3, 0.4, 0.5)});
	mpfWeights.insert({&assets.at(1), sic::WeightRange(0.1, 0.2, 0.3)});
	mpfWeights.insert({&assets.at(2), sic::WeightRange(0.15, 0.2, 0.25)});
	mpfWeights.insert({&assets.at(3), sic::WeightRange(0.0, 0.1, 0.2)});
	mpfWeights.insert({&assets.at(4), sic::WeightRange(0.0, 0.1, 0.2)});

	sic::Iterators<std::pair<const sic::AbstractAsset *const, sic::WeightRange>>
		assetWeightIts(mpfWeights);

	const sic::WeightRange nodeWeight(0.4, 0.5, 0.6);
	const sic::External::ID expID = 123;

	sic::MPFAllocationNode mpfNode(mpf, nodeWeight, expID);

	EXPECT_CALL(mpf, getAssetWeightIterators())
		.WillOnce(testing::Return(assetWeightIts));

	sic::AbstractAsset::AssetWeightMap toTopWeights;
	mpfNode.attachToTopAssetWeights(&toTopWeights);

	ASSERT_EQ(toTopWeights.size(), assetCount);

	std::vector<sic::WeightRange> toTopRanges;
	toTopRanges.reserve(assetCount);
	for (int i = 0; i < assetCount; i++) {
		toTopRanges.push_back(toTopWeights.find(&assets.at(i))->second);
	}

	ASSERT_EQ(toTopRanges.at(0), sic::WeightRange(0.15, 0.2, 0.25));
	ASSERT_EQ(toTopRanges.at(1), sic::WeightRange(0.05, 0.1, 0.15));
	ASSERT_EQ(toTopRanges.at(2), sic::WeightRange(0.075, 0.1, 0.125));
	ASSERT_EQ(toTopRanges.at(3), sic::WeightRange(0.0, 0.05, 0.1));
	ASSERT_EQ(toTopRanges.at(4), sic::WeightRange(0.0, 0.05, 0.1));
}

} // namespace
