#ifndef SIC_TOLERANCES_H_
#define SIC_TOLERANCES_H_

#include "sic/Base/Types.hh"

namespace sic {

/// Maximum tolerance in a measurement to be considered the same.
template <typename Type> constexpr Type Tolerance();

template <> constexpr sic::Weight Tolerance() { return 0.00001; }

} // namespace sic

#endif // SIC_TOLERANCES_H_
