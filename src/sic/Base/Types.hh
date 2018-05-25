#ifndef SIC_BASE_TYPES_H_
#define SIC_BASE_TYPES_H_

#include "sic/Base/Range.hh"

namespace sic {

/// Monetary value, without currency specified.
typedef float Value;

/// Monetary price, without currency specified.
typedef sic::Value Price;

/// Model weight (i.e. percentage).
typedef float Weight;

/// Ranges of model weights.
typedef sic::Range<sic::Weight> WeightRange;

} // namespace sic

#endif // SIC_BASE_TYPES_H_
