#ifndef SIC_PORTFOLIO_ASSET_H
#define SIC_PORTFOLIO_ASSET_H

#include "sic/Base/Types.hh"

namespace sic {

class Asset {

private:
  sic::price referencePrice;

public:
  Asset(sic::value referencePrice) : referencePrice(referencePrice) {}

  sic::price getReferencePrice() const { return referencePrice; }
};

} // namespace sic

#endif // SIC_PORTFOLIO_ASSET_H
