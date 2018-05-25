#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "sic/Base/Tolerances.hh"
#include "sic/Model/ModelPortfolio.hh"

namespace {

class ModelPortfolioTest : public testing::Test {
public:
	class MockAsset : public sic::AbstractAsset {
	public:
		explicit MockAsset(sic::External::ID externalID)
			: sic::AbstractAsset(externalID) {}

		MOCK_CONST_METHOD1(hasClass, bool(sic::AbstractAsset::Class));
	};
};

TEST_F(ModelPortfolioTest, CreateValid) {

	auto assetList = new sic::Model::ModelPortfolio::AssetWeightMap();
	const auto &assetListRef = *assetList;
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap>
		assetWeightsMapPtr(assetList);

	constexpr int expAssetCount = 19;
	assetWeightsMapPtr->reserve(expAssetCount);
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

	std::vector<std::unique_ptr<const MockAsset>> assets;
	assets.reserve(expAssetCount);
	for (int i = 0; i < expAssetCount; i++) {
		const auto assetID = static_cast<sic::External::ID>(i);

		assets.emplace_back(std::make_unique<const MockAsset>(assetID));
		assetWeightsMapPtr->insert({assets.at(i).get(), assetWeights.at(i)});
	}

	constexpr sic::External::ID expExternalID = 43534l;

	sic::Model::ModelPortfolio validMPF(std::move(assetWeightsMapPtr),
										expExternalID);

	ASSERT_EQ(expExternalID, validMPF.getExternalID());
	ASSERT_EQ(expAssetCount, validMPF.getAssetCount());

	// Check we can iterate through the items correctly.
	auto assetWeightIterators = validMPF.getAssetWeightIterators();
	ASSERT_EQ(assetListRef.begin(), assetWeightIterators.current());
	ASSERT_EQ(assetListRef.end(), assetWeightIterators.end());
}

TEST_F(ModelPortfolioTest, CreateInvalidEmptyAssetsList) {

	auto assetList = new sic::Model::ModelPortfolio::AssetWeightMap();
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap>
		assetWeightsMapPtr(assetList);

	constexpr sic::External::ID expExternalID = 43534l;
	const std::string expError =
		"assetWeightMap must contain at least one entry.";

	try {
		const sic::Model::ModelPortfolio invalidMPF(
			std::move(assetWeightsMapPtr), expExternalID);
		FAIL() << "Able to create MPF with empty assets map.";
	} catch (std::invalid_argument &e) {
		ASSERT_EQ(expError, e.what());
	} catch (...) {
		FAIL() << "Unexpected exception.";
	}
}

TEST_F(ModelPortfolioTest, CreateInvalidDuplicateAssets) {

	auto assetList = new sic::Model::ModelPortfolio::AssetWeightMap();
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap>
		assetWeightsMapPtr(assetList);

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

	std::vector<std::unique_ptr<const MockAsset>> assets;
	assets.reserve(expAssetCount);
	for (int i = 0; i < expAssetCount; i++) {
		const sic::External::ID assetID =
			(i < expDuplicateAssetCount) ? duplicateID
										 : static_cast<sic::External::ID>(i);

		assets.emplace_back(std::make_unique<const MockAsset>(assetID));
		assetWeightsMapPtr->insert({assets.at(i).get(), perAssetWeightRange});
	}

	const std::string expError =
		"assetWeightMap must not contain duplicate Assets.";

	try {
		const sic::Model::ModelPortfolio invalidMPF(
			std::move(assetWeightsMapPtr), expExternalID);
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
	auto validOverAssetList = new sic::Model::ModelPortfolio::AssetWeightMap();
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap>
		validOverAssetWeightsMapPtr(validOverAssetList);

	std::vector<std::unique_ptr<const MockAsset>> overAssets;
	overAssets.reserve(expAssetCount);
	for (int i = 0; i < expAssetCount; i++) {
		const auto assetID = static_cast<sic::External::ID>(i);

		// Offset one Asset weight by the maximum tolerance allowed.
		const sic::Weight assetWeight =
			(i == 0) ? perAssetTargetWeight + sic::Tolerance<sic::Weight>()
					 : perAssetTargetWeight;
		const sic::WeightRange adjustedWeightRange(
			perAssetMinWeight, assetWeight, perAssetMaxWeight);

		overAssets.emplace_back(std::make_unique<const MockAsset>(assetID));
		validOverAssetWeightsMapPtr->insert(
			{overAssets.at(i).get(), adjustedWeightRange});
	}

	const sic::Model::ModelPortfolio validOverMPF(
		std::move(validOverAssetWeightsMapPtr), expExternalID);

	// Overweight, still valid.
	auto validUnderAssetList = new sic::Model::ModelPortfolio::AssetWeightMap();
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap>
		validUnderAssetWeightsMapPtr(validUnderAssetList);

	std::vector<std::unique_ptr<const MockAsset>> underAssets;
	underAssets.reserve(expAssetCount);
	for (int i = 0; i < expAssetCount; i++) {
		const auto assetID = static_cast<sic::External::ID>(i);

		// Offset one Asset weight by the maximum tolerance allowed.
		const sic::Weight assetWeight =
			(i == 0) ? perAssetTargetWeight - sic::Tolerance<sic::Weight>()
					 : perAssetTargetWeight;
		const sic::WeightRange adjustedWeightRange(
			perAssetMinWeight, assetWeight, perAssetMaxWeight);

		underAssets.emplace_back(std::make_unique<const MockAsset>(assetID));
		validUnderAssetWeightsMapPtr->insert(
			{underAssets.at(i).get(), adjustedWeightRange});
	}

	const sic::Model::ModelPortfolio validUnderMPF(
		std::move(validUnderAssetWeightsMapPtr), expExternalID);
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
		new sic::Model::ModelPortfolio::AssetWeightMap();
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap>
		invalidOverAssetWeightsMapPtr(invalidOverAssetList);

	std::vector<std::unique_ptr<const MockAsset>> overAssets;
	overAssets.reserve(expAssetCount);
	for (int i = 0; i < expAssetCount; i++) {
		const auto assetID = static_cast<sic::External::ID>(i);

		// Offset one Asset weight by just over the maximum tolerance.
		const sic::Weight assetWeight =
			(i == 0) ? perAssetTargetWeight + sic::Tolerance<sic::Weight>() +
						   std::numeric_limits<sic::Weight>::epsilon()
					 : perAssetTargetWeight;
		const sic::WeightRange adjustedWeightRange(
			perAssetMinWeight, assetWeight, perAssetMaxWeight);

		overAssets.emplace_back(std::make_unique<const MockAsset>(assetID));
		invalidOverAssetWeightsMapPtr->insert(
			{overAssets.at(i).get(), adjustedWeightRange});
	}

	try {
		const sic::Model::ModelPortfolio invalidOverMPF(
			std::move(invalidOverAssetWeightsMapPtr), expExternalID);
		FAIL() << "Able to create a ModelPortfolio with weight sum greater "
				  "than tolerance.";
	} catch (std::invalid_argument &e) {
		ASSERT_EQ(expError, e.what());
	}

	// Overweight, still valid.
	auto invalidUnderAssetList =
		new sic::Model::ModelPortfolio::AssetWeightMap();
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap>
		invalidUnderAssetWeightsMapPtr(invalidUnderAssetList);

	std::vector<std::unique_ptr<const MockAsset>> underAssets;
	underAssets.reserve(expAssetCount);
	for (int i = 0; i < expAssetCount; i++) {
		const auto assetID = static_cast<sic::External::ID>(i);

		// Offset one Asset weight by just over the maximum tolerance.
		const sic::Weight assetWeight =
			(i == 0) ? perAssetTargetWeight - sic::Tolerance<sic::Weight>() -
						   std::numeric_limits<sic::Weight>::epsilon()
					 : perAssetTargetWeight;
		const sic::WeightRange adjustedWeightRange(
			perAssetMinWeight, assetWeight, perAssetMaxWeight);

		underAssets.emplace_back(std::make_unique<const MockAsset>(assetID));
		invalidUnderAssetWeightsMapPtr->insert(
			{underAssets.at(i).get(), adjustedWeightRange});
	}
	try {
		const sic::Model::ModelPortfolio invalidUnderMPF(
			std::move(invalidUnderAssetWeightsMapPtr), expExternalID);
		FAIL() << "Able to create a ModelPortfolio with weight sum less "
				  "than tolerance.";
	} catch (std::invalid_argument &e) {
		ASSERT_EQ(expError, e.what());
	}
}

} // namespace
