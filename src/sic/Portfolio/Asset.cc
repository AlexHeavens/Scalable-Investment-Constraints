#include "sic/Portfolio/Asset.hh"

namespace sic {

Asset::Asset(const sic::Asset &rhs) : sic::AbstractAsset(rhs) {

	// Deep copy classes to avoid damaging input.
	classes = std::make_unique<sic::AbstractAsset::ClassSet>(*rhs.classes);
}

Asset::Asset(sic::External::ID externalID,
			 std::experimental::optional<std::unique_ptr<ClassSet>> classes)
	: sic::AbstractAsset(externalID) {

	if (classes) {
		this->classes = std::move(*classes);
	} else {
		this->classes = std::make_unique<sic::AbstractAsset::ClassSet>();
	}
}

sic::Iterators<sic::AbstractAsset::Class> Asset::getClassIterators() {
	return sic::Iterators<sic::AbstractAsset::Class>(classes);
}

bool Asset::hasClass(sic::AbstractAsset::Class assetClass) const {
	const auto search = classes->find(assetClass);
	return search != classes->end();
}

} // namespace sic
