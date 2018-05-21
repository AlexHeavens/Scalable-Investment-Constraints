#include <gtest/gtest.h>

#include "sic/Model/ModelPortfolio.hh"

namespace {

class ModelPortfolioTest : public testing::Test {
public:
	class MockAsset : public sic::Asset {
	public:
		explicit MockAsset(sic::External::ID externalID)
			: sic::Asset(1.00, externalID) {}

		virtual ~MockAsset() = default;
	};
};

TEST_F(ModelPortfolioTest, CreateValid) {

	auto assetList = new sic::Model::ModelPortfolio::AssetWeightMap();
	std::unique_ptr<sic::Model::ModelPortfolio::AssetWeightMap>
		assetWeightsMapPtr(assetList);

	const int expAssetCount = 19;
	assetWeightsMapPtr->reserve(expAssetCount);
	std::vector<sic::Weight> assetWeights;
	assetWeights.reserve(expAssetCount);

	// First set of Asset get 10% weighting.
	const int tenPercentAssetCount = 9;
	const sic::Weight tenPercentWeight = 0.1;
	for (int i = 0; i < tenPercentAssetCount; i++) {
		assetWeights.push_back(tenPercentWeight);
	}

	// Remaining Assets split the remainder of the weight.
	const int remainingAssetCount = expAssetCount - tenPercentAssetCount;
	const sic::Weight remainingWeight =
		1.0 - static_cast<sic::Weight>(tenPercentAssetCount) * tenPercentWeight;
	const sic::Weight remainingPerAssetWeight =
		remainingWeight / static_cast<sic::Weight>(remainingAssetCount);
	for (int i = 0; i < remainingAssetCount; i++) {
		assetWeights.push_back(remainingPerAssetWeight);
	}

	for (int i = 0; i < expAssetCount; i++) {
		const auto assetID = static_cast<sic::External::ID>(i);

		std::shared_ptr<sic::Asset> assetPtr(new MockAsset(assetID));
		assetWeightsMapPtr->insert({std::move(assetPtr), assetWeights.at(i)});
	}

	const sic::External::ID expExternalID = 43534l;

	const sic::Model::ModelPortfolio validMPF(std::move(assetWeightsMapPtr),
											  expExternalID);

	ASSERT_EQ(expExternalID, validMPF.getExternalID());
	ASSERT_EQ(expAssetCount, validMPF.getAssetCount());
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
	const sic::Weight perAssetWeight =
		1.0 / static_cast<sic::Weight>(expAssetCount);
	const sic::External::ID duplicateID = 432344;

	for (int i = 0; i < expAssetCount; i++) {
		const sic::External::ID assetID =
			(i < expDuplicateAssetCount) ? duplicateID
										 : static_cast<sic::External::ID>(i);

		std::shared_ptr<sic::Asset> newAsset(new MockAsset(assetID));
		assetWeightsMapPtr->insert({newAsset, perAssetWeight});
	}

	constexpr sic::External::ID expExternalID = 43534l;
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

} // namespace
