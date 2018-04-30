#include "sic/Portfolio/Asset.hh"

sic::Asset::Asset(
	sic::Price referencePrice,
	std::experimental::optional<std::unique_ptr<ClassVector>> classes)
	: referencePrice(referencePrice) {

	if (classes) {
		this->classes = std::move(*classes);
	}
}

std::pair<sic::Asset::ClassIterator, sic::Asset::ClassIterator>
sic::Asset::getClassIterators() {

	return std::make_pair(std::begin(*classes), std::end(*classes));
}
