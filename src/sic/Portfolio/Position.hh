#ifndef SIC_PORTFOLIO_POSITION_H_
#define SIC_PORTFOLIO_POSITION_H_

#include "sic/Base/Types.hh"
#include "sic/Portfolio/Asset.hh"

namespace sic {

class Position {

private:
  sic::Asset &asset;

public:
  Position(sic::Asset &asset) : asset(asset) {}

  sic::Asset &getAsset() const { return asset; }
};

} // namespace sic

#endif // SIC_PORTFOLIO_POSITION_H_
