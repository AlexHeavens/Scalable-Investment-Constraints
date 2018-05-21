#ifndef SIC_TOLERANCES_H_
#define SIC_TOLERANCES_H_

#include "sic/Base/Types.hh"

namespace sic::Tolerance {

/// Maximum tolerance in weights to be considered the same.
constexpr sic::Weight Weight = 0.00001;
} // namespace sic::Tolerance

#endif // SIC_TOLERANCES_H_
