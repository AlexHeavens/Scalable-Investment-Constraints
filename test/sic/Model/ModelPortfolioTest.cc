#include <gtest/gtest.h>

#include <memory>

#include "sic/Base/Tolerances.hh"
#include "sic/Model/ModelPortfolio.hh"
#include "sic/Portfolio/MockAsset.hh"

namespace {

class ModelPortfolioTest : public testing::Test {};

TEST_F(ModelPortfolioTest, CreateValid) {

	auto assetWeightsMap =
		std::make_unique<sic::AbstractAsset::AssetWeightMap>();
	auto assetWeightsMapRawPtr = assetWeightsMap.get();

	constexpr int expAssetCount = 19;
	assetWeightsMap->reserve(expAssetCount);
	std::vector<sic::WeightRange> assetWeights;
	assetWeights.reserve(expAssetCount);

	// First set of Asset get 10% weighting, min/max -1/+1.
	constexpr int largePercentAssetCount = 9;
	constexpr sic::Weight largePercentTargetWeight = 0.1;
	constexpr sic::Weight largePercentMinWeight =
		largePercentTargetWeight - 0.01;
	constexpr sic::Weight largePercentMaxWeight =
		largePercentTargetWeight + 0.01;
	const sic::WeightRange largeWeightRange(
		largePercentMinWeight, largePercentTargetWeight, largePercentMaxWeight);

	for (int i = 0; i < largePercentAssetCount; i++) {
		assetWeights.push_back(largeWeightRange);
	}

	// Remaining Assets split the remainder of the weight.
	constexpr int remainingAssetCount = expAssetCount - largePercentAssetCount;
	constexpr sic::Weight remainingWeight =
		1.0 - static_cast<sic::Weight>(largePercentAssetCount) *
				  largePercentTargetWeight;
	constexpr sic::Weight remainingPercentTargetWeight =
		remainingWeight / static_cast<sic::Weight>(remainingAssetCount);

	constexpr sic::Weight remainingPercentMinWeight =
		remainingPercentTargetWeight - 0.01;
	constexpr sic::Weight remainingPercentMaxWeight =
		remainingPercentTargetWeight + 0.01;

	const sic::WeightRange remainingWeightRange(remainingPercentMinWeight,
												remainingPercentTargetWeight,
												remainingPercentMaxWeight);

	for (int i = 0; i < remainingAssetCount; i++) {
		assetWeights.push_back(remainingWeightRange);
	}

	std::vector<std::unique_ptr<const sic::MockAsset>> assets;
	assets.reserve(expAssetCount);
	for (int i = 0; i < expAssetCount; i++) {
		const auto assetID = static_cast<sic::External::ID>(i);

		assets.emplace_back(std::make_unique<const sic::MockAsset>(assetID));
		assetWeightsMap->insert({assets.at(i).get(), assetWeights.at(i)});
	}

	constexpr sic::External::ID expExternalID = 43534l;

	sic::ModelPortfolio validMPF(std::move(assetWeightsMap), expExternalID);

	ASSERT_EQ(expExternalID, validMPF.getExternalID());
	ASSERT_EQ(expAssetCount, validMPF.getAssetCount());

	// Check we can iterate through the items correctly.
	auto assetWeightIterators = validMPF.getAssetWeightIterators();
	ASSERT_EQ(assetWeightsMapRawPtr->begin()->first,
			  assetWeightIterators.current()->first);
	ASSERT_TRUE(assetWeightIterators.remaining());
}

TEST_F(ModelPortfolioTest, CreateInvalidEmptyAssetsList) {

	auto assetWeightsMap =
		std::make_unique<sic::AbstractAsset::AssetWeightMap>();

	constexpr sic::External::ID expExternalID = 43534l;
	const std::string expError =
		"assetWeightMap must contain at least one entry.";

	try {
		const sic::ModelPortfolio invalidMPF(std::move(assetWeightsMap),
											 expExternalID);
		FAIL() << "Able to create MPF with empty assets map.";
	} catch (std::invalid_argument &e) {
		ASSERT_EQ(expError, e.what());
	} catch (...) {
		FAIL() << "Unexpected exception.";
	}
}

TEST_F(ModelPortfolioTest, CreateInvalidDuplicateAssets) {

	auto assetWeightsMap =
		std::make_unique<sic::AbstractAsset::AssetWeightMap>();

	constexpr int expAssetCount = 20;
	constexpr int expDuplicateAssetCount = 2;
	constexpr sic::External::ID expExternalID = 43534;

	constexpr sic::Weight perAssetTargetWeight =
		1.0 / static_cast<sic::Weight>(expAssetCount);
	constexpr sic::Weight perAssetMinWeight = perAssetTargetWeight - 0.01;
	constexpr sic::Weight perAssetMaxWeight = perAssetTargetWeight + 0.01;
	const sic::WeightRange perAssetWeightRange(
		perAssetMinWeight, perAssetTargetWeight, perAssetMaxWeight);

	constexpr sic::External::ID duplicateID = 432344;

	std::vector<std::unique_ptr<const sic::MockAsset>> assets;
	assets.reserve(expAssetCount);
	for (int i = 0; i < expAssetCount; i++) {
		const sic::External::ID assetID =
			(i < expDuplicateAssetCount) ? duplicateID
										 : static_cast<sic::External::ID>(i);

		assets.emplace_back(std::make_unique<const sic::MockAsset>(assetID));
		assetWeightsMap->insert({assets.at(i).get(), perAssetWeightRange});
	}

	const std::string expError =
		"assetWeightMap must not contain duplicate Assets.";

	try {
		const sic::ModelPortfolio invalidMPF(std::move(assetWeightsMap),
											 expExternalID);
		FAIL() << "Able to create MPF with duplicate Assets.";
	} catch (std::invalid_argument &e) {
		ASSERT_EQ(expError, e.what());
	} catch (...) {
		FAIL() << "Unexpected exception.";
	}
}

TEST_F(ModelPortfolioTest, CreateValidAssetsSum100Percent) {

	constexpr int expAssetCount = 5;
	constexpr sic::Weight perAssetTargetWeight =
		1.0 / static_cast<sic::Weight>(expAssetCount);
	constexpr sic::Weight perAssetMinWeight = perAssetTargetWeight - 0.01;
	constexpr sic::Weight perAssetMaxWeight = perAssetTargetWeight + 0.01;

	constexpr sic::External::ID expExternalID = 435354;

	// Overweight, still valid.
	auto validOverAssetList =
		std::make_unique<sic::AbstractAsset::AssetWeightMap>();

	std::vector<std::unique_ptr<const sic::MockAsset>> overAssets;
	overAssets.reserve(expAssetCount);
	for (int i = 0; i < expAssetCount; i++) {
		const auto assetID = static_cast<sic::External::ID>(i);

		// Offset one Asset weight by the maximum tolerance allowed.
		const sic::Weight assetWeight =
			(i == 0)
				? perAssetTargetWeight + 0.99 * sic::Tolerance<sic::Weight>()
				: perAssetTargetWeight;
		const sic::WeightRange adjustedWeightRange(
			perAssetMinWeight, assetWeight, perAssetMaxWeight);

		overAssets.emplace_back(
			std::make_unique<const sic::MockAsset>(assetID));
		validOverAssetList->insert(
			{overAssets.at(i).get(), adjustedWeightRange});
	}

	const sic::ModelPortfolio validOverMPF(std::move(validOverAssetList),
										   expExternalID);

	// Overweight, still valid.
	auto validUnderAssetList =
		std::make_unique<sic::AbstractAsset::AssetWeightMap>();

	std::vector<std::unique_ptr<const sic::MockAsset>> underAssets;
	underAssets.reserve(expAssetCount);
	for (int i = 0; i < expAssetCount; i++) {
		const auto assetID = static_cast<sic::External::ID>(i);

		// Offset one Asset weight by the maximum tolerance allowed.
		const sic::Weight assetWeight =
			(i == 0)
				? perAssetTargetWeight - 0.99 * sic::Tolerance<sic::Weight>()
				: perAssetTargetWeight;
		const sic::WeightRange adjustedWeightRange(
			perAssetMinWeight, assetWeight, perAssetMaxWeight);

		underAssets.emplace_back(
			std::make_unique<const sic::MockAsset>(assetID));
		validUnderAssetList->insert(
			{underAssets.at(i).get(), adjustedWeightRange});
	}

	const sic::ModelPortfolio validUnderMPF(std::move(validUnderAssetList),
											expExternalID);
}

TEST_F(ModelPortfolioTest, CreateInvalidAssetsSum100Percent) {

	constexpr int expAssetCount = 5;
	constexpr sic::Weight perAssetTargetWeight =
		1.0 / static_cast<sic::Weight>(expAssetCount);
	constexpr sic::Weight perAssetMinWeight = perAssetTargetWeight - 0.01;
	constexpr sic::Weight perAssetMaxWeight = perAssetTargetWeight + 0.01;
	constexpr sic::External::ID expExternalID = 435354;
	const std::string expError =
		"ModelPortfolio Asset weights must sum to 1.0.";

	// Overweight, invalid.
	auto invalidOverAssetList =
		std::make_unique<sic::AbstractAsset::AssetWeightMap>();

	std::vector<std::unique_ptr<const sic::MockAsset>> overAssets;
	overAssets.reserve(expAssetCount);
	for (int i = 0; i < expAssetCount; i++) {
		const auto assetID = static_cast<sic::External::ID>(i);

		// Offset one Asset weight by just over the maximum tolerance.
		const sic::Weight assetWeight =
			(i == 0)
				? perAssetTargetWeight + 1.01 * sic::Tolerance<sic::Weight>()
				: perAssetTargetWeight;
		const sic::WeightRange adjustedWeightRange(
			perAssetMinWeight, assetWeight, perAssetMaxWeight);

		overAssets.emplace_back(
			std::make_unique<const sic::MockAsset>(assetID));
		invalidOverAssetList->insert(
			{overAssets.at(i).get(), adjustedWeightRange});
	}

	try {
		const sic::ModelPortfolio invalidOverMPF(
			std::move(invalidOverAssetList), expExternalID);
		FAIL() << "Able to create a ModelPortfolio with weight sum greater "
				  "than tolerance.";
	} catch (std::invalid_argument &e) {
		ASSERT_EQ(expError, e.what());
	}

	// Underweight, invalid.
	auto invalidUnderAssetList =
		std::make_unique<sic::AbstractAsset::AssetWeightMap>();

	std::vector<std::unique_ptr<const sic::MockAsset>> underAssets;
	underAssets.reserve(expAssetCount);

	sic::Weight sum = 0.0;

	for (int i = 0; i < expAssetCount; i++) {
		const auto assetID = static_cast<sic::External::ID>(i);

		// Offset one Asset weight by just over the maximum tolerance.
		const sic::Weight assetWeight =
			(i == 0)
				? perAssetTargetWeight - 1.01 * sic::Tolerance<sic::Weight>()
				: perAssetTargetWeight;
		const sic::WeightRange adjustedWeightRange(
			perAssetMinWeight, assetWeight, perAssetMaxWeight);

		underAssets.emplace_back(
			std::make_unique<const sic::MockAsset>(assetID));
		invalidUnderAssetList->insert(
			{underAssets.at(i).get(), adjustedWeightRange});

		sum += assetWeight;
	}

	try {
		const sic::ModelPortfolio invalidUnderMPF(
			std::move(invalidUnderAssetList), expExternalID);
		FAIL() << "Able to create a ModelPortfolio with weight sum less "
				  "than tolerance.";
	} catch (std::invalid_argument &e) {
		ASSERT_EQ(expError, e.what());
	}
}

} // namespace
