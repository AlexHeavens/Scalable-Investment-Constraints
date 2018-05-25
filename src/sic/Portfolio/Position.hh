#ifndef SIC_PORTFOLIO_POSITION_H_
#define SIC_PORTFOLIO_POSITION_H_

#include <vector>

#include "sic/Base/Types.hh"
#include "sic/External.hh"
#include "sic/Portfolio/AbstractAsset.hh"

namespace sic {

/**
 * An instance of a financial asset within a client's portfolio.
 *
 * @see sic::Asset
 */
class Position : public sic::External {

public:
	/// Vector of Positions
	typedef std::vector<sic::Position> Vector;
	/// Iterator over a vector of Positions.
	typedef sic::Position::Vector::iterator VectorIterator;

private:
	sic::AbstractAsset &asset;
	sic::Value referenceValue;

public:
	/**
	 * Create a position of a specific asset.
	 */
	Position(sic::AbstractAsset &asset, sic::Value referenceValue,
			 sic::External::ID externalID)
		: sic::External(externalID), asset(asset),
		  referenceValue(referenceValue) {}

	/**
	 * The financial asset the position represents a quantity of.
	 */
	sic::AbstractAsset &getAsset() const { return asset; }

	/**
	 * The value in the (unspecified) reference currency.
	 */
	sic::Value getReferenceValue() const { return referenceValue; }
};

} // namespace sic

#endif // SIC_PORTFOLIO_POSITION_H_
