#include "sic/Portfolio/Asset.hh"

sic::Asset::Asset(
	sic::Price referencePrice, sic::External::ID externalID,
	std::experimental::optional<std::unique_ptr<ClassSet>> classes)
	: sic::External(externalID), referencePrice(referencePrice) {

	if (classes) {
		this->classes = std::move(*classes);
	}
}

std::pair<sic::Asset::ClassIterator, sic::Asset::ClassIterator>
sic::Asset::getClassIterators() {

	return std::make_pair(std::begin(*classes), std::end(*classes));
}
