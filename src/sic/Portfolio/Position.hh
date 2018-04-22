#ifndef SIC_PORTFOLIO_POSITION_H_
#define SIC_PORTFOLIO_POSITION_H_

#include "sic/Base/Types.hh"
#include "sic/Portfolio/Asset.hh"

namespace sic {

/**
 * An instance of a financial asset within a client's portfolio.
 *
 * @see sic::Asset
 */
class Position {

private:
  sic::Asset &asset;
  sic::Value referenceValue;

public:
  /**
   * Create a position of a specific asset.
   */
  Position(sic::Asset &asset, sic::Value referenceValue)
      : asset(asset), referenceValue(referenceValue) {}

  /**
   * The financial asset the position represents a quantity of.
   */
  sic::Asset &getAsset() const { return asset; }

  /**
   * The value in the (unspecified) reference currency.
   */
  sic::Value getReferenceValue() const { return referenceValue; }
};

} // namespace sic

#endif // SIC_PORTFOLIO_POSITION_H_
