#ifndef SIC_PORTFOLIO_POSITION_H_
#define SIC_PORTFOLIO_POSITION_H_

#include <vector>

#include "sic/Portfolio/AbstractPosition.hh"

namespace sic {

/**
 * An instance of a financial asset within a client's portfolio.
 *
 * @see sic::Asset
 */
class Position : public sic::AbstractPosition {

private:
	sic::AbstractAsset &asset;
	sic::Value referenceValue;

public:
	/**
	 * Create a position of a specific asset.
	 */
	Position(sic::AbstractAsset &asset, sic::Value referenceValue,
			 sic::External::ID externalID)
		: sic::AbstractPosition(externalID), asset(asset),
		  referenceValue(referenceValue) {}

	sic::AbstractAsset &getAsset() const override { return asset; }

	sic::Value getReferenceValue() const override { return referenceValue; }
};

} // namespace sic

#endif // SIC_PORTFOLIO_POSITION_H_
