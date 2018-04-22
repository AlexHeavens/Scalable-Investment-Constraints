#ifndef SIC_PORTFOLIO_ASSET_H_
#define SIC_PORTFOLIO_ASSET_H_

#include "sic/Base/Types.hh"

namespace sic {

/**
 * A financial instrument that can be held by a client.
 *
 * Assets define the general properties of the Positions a client holds in their
 * portfolio.
 */
class Asset {

private:
  sic::Price referencePrice;

public:
  /**
   * Create an Asset.
   *
   * @param referencePrice the price of the Asset in the (unspecified) reference
   * currency.
   */
  Asset(sic::Price referencePrice) : referencePrice(referencePrice) {}

  /**
   * The price of the Asset in the (unspecified) reference currency.
   */
  sic::Price getReferencePrice() const { return referencePrice; }
};

} // namespace sic

#endif // SIC_PORTFOLIO_ASSET_H_
