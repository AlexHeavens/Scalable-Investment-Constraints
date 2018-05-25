#ifndef SIC_PORTFOLIO_ASSET_H_
#define SIC_PORTFOLIO_ASSET_H_

#include <experimental/optional>

#include "sic/Base/Iterators.hh"
#include "sic/Base/Types.hh"
#include "sic/Portfolio/AbstractAsset.hh"

namespace sic {

/**
 * A financial instrument that can be held by a client.
 *
 * Assets define the general properties of the Positions a client holds in their
 * portfolio.
 */
class Asset : public sic::AbstractAsset {

private:
	sic::Price referencePrice;
	std::unique_ptr<sic::AbstractAsset::ClassSet> classes;

public:
	/**
	 * Copy constructor for Asset.
	 *
	 * Creates a deep copy of the Asset fields, including the list of Asset
	 * Classes.
	 */
	Asset(const sic::Asset &rhs);

	/**
	 * Create an Asset.
	 *
	 * @param referencePrice the price of the Asset in the (unspecified)
	 * reference currency.
	 * @param externalID ID of the Asset as represented by the external system
	 * using the SIC Engine.  In the results the engine provides, this will be
	 * the identifier used to ensure the external system can match their Asset
	 * to these results.
	 * @param classes set of bank-specific classes the Asset matches.  For
	 * example, market category.  The Asset takes ownership of this set.
	 */
	Asset(sic::Price referencePrice, sic::External::ID externalID,
		  std::experimental::optional<std::unique_ptr<ClassSet>> classes = {});

	/**
	 * The price of the Asset in the (unspecified) reference currency.
	 */
	sic::Price getReferencePrice() const { return referencePrice; }

	/**
	 * The current and end iterators of the Asset's class set.
	 *
	 * @returns Current and end Asset classes iterators.
	 */
	sic::Iterators<sic::AbstractAsset::ClassIterator> getClassIterators();

	/**
	 * If the Asset has a given Asset Class.
	 */
	bool hasClass(sic::AbstractAsset::Class assetClass) const override;
};

} // namespace sic

#endif // SIC_PORTFOLIO_ASSET_H_
