#include "sic/Portfolio/Asset.hh"

sic::Asset::Asset(const sic::Asset &rhs)
	: sic::AbstractAsset(rhs.getExternalID()) {

	// Deep copy classes to avoid damaging input.
	classes = std::make_unique<sic::AbstractAsset::ClassSet>(*rhs.classes);
}

sic::Asset::Asset(
	sic::External::ID externalID,
	std::experimental::optional<std::unique_ptr<ClassSet>> classes)
	: sic::AbstractAsset(externalID) {

	if (classes) {
		this->classes = std::move(*classes);
	} else {
		this->classes = std::make_unique<sic::AbstractAsset::ClassSet>();
	}
}

sic::Iterators<sic::AbstractAsset::Class> sic::Asset::getClassIterators() {

	sic::Iterators<sic::AbstractAsset::Class>::It begin(classes->begin());
	const sic::Iterators<sic::AbstractAsset::Class>::It end(classes->end());

	return sic::Iterators<sic::AbstractAsset::Class>(begin, end);
}

bool sic::Asset::hasClass(sic::AbstractAsset::Class assetClass) const {
	const auto search = classes->find(assetClass);
	return search != classes->end();
}
