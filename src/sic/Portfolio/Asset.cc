#include "sic/Portfolio/Asset.hh"

sic::Asset::Asset(const sic::Asset &rhs)
	: sic::AbstractAsset(rhs.getExternalID()) {
	referencePrice = rhs.referencePrice;

	// Deep copy classes to avoid damaging input.
	classes = std::make_unique<sic::AbstractAsset::ClassSet>(*rhs.classes);
}

sic::Asset::Asset(
	sic::Price referencePrice, sic::External::ID externalID,
	std::experimental::optional<std::unique_ptr<ClassSet>> classes)
	: sic::AbstractAsset(externalID), referencePrice(referencePrice) {

	if (classes) {
		this->classes = std::move(*classes);
	} else {
		this->classes = std::make_unique<sic::AbstractAsset::ClassSet>();
	}
}

sic::Iterators<sic::AbstractAsset::ClassIterator>
sic::Asset::getClassIterators() {

	return sic::Iterators<sic::AbstractAsset::ClassIterator>(
		std::begin(*classes), std::end(*classes));
}

bool sic::Asset::hasClass(sic::AbstractAsset::Class assetClass) const {
	const auto search = classes->find(assetClass);
	return search != classes->end();
}
