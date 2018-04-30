#ifndef SIC_PORTFOLIO_ASSET_H_
#define SIC_PORTFOLIO_ASSET_H_

#include <experimental/optional>
#include <memory>
#include <utility>
#include <vector>

#include "sic/Base/Types.hh"

namespace sic {

/**
 * A financial instrument that can be held by a client.
 *
 * Assets define the general properties of the Positions a client holds in their
 * portfolio.
 */
class Asset {

public:
	typedef unsigned Class;
	typedef std::vector<sic::Asset::Class> ClassVector;
	typedef sic::Asset::ClassVector::iterator ClassIterator;

private:
	sic::Price referencePrice;
	std::unique_ptr<sic::Asset::ClassVector> classes;

public:
	/**
	 * Create an Asset.
	 *
	 * @param referencePrice the price of the Asset in the (unspecified)
	 * reference currency.
	 * @param classes vector of bank-specific classes the Asset matches.  For
	 * example, market category.  The Asset takes ownership of this vector.
	 */
	Asset(
		sic::Price referencePrice,
		std::experimental::optional<std::unique_ptr<ClassVector>> classes = {})
		: referencePrice(referencePrice) {

		if (classes) {
			this->classes = std::move(*classes);
		}
	}

	/**
	 * The price of the Asset in the (unspecified) reference currency.
	 */
	sic::Price getReferencePrice() const { return referencePrice; }

	/**
	 * The begin and end iterators of the Asset's class vector.
	 *
	 * @returns Begin and end as the first and second sides of a pair.
	 */
	std::pair<sic::Asset::ClassIterator, sic::Asset::ClassIterator>
	getClassIterators();
};

} // namespace sic

#endif // SIC_PORTFOLIO_ASSET_H_
