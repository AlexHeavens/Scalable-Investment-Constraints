#ifndef SIC_MODEL_ABSTRACTMODELPORTFOLIO_H_
#define SIC_MODEL_ABSTRACTMODELPORTFOLIO_H_

#include "sic/Base/Iterators.hh"
#include "sic/Base/Types.hh"
#include "sic/Portfolio/AbstractAsset.hh"

namespace sic {

/**
 * A simple weighted Asset list portfolio model.
 *
 * This is an abstract representation of a ModelPortfolio to allow for mock
 * testing.
 */
class AbstractModelPortfolio {

public:
	/**
	 * The begin and end iterators of the ModelPortfolio's Asset/Weight-Range
	 * pairs.
	 *
	 * @returns Current and end iterators.
	 */
	virtual sic::Iterators<
		std::pair<const sic::AbstractAsset *const, sic::WeightRange>>
	getAssetWeightIterators() = 0;

	/**
	 * Get the number of Assets stored in the MPF.
	 */
	virtual std::size_t getAssetCount() const = 0;

	/**
	 * Destroy the MPF.
	 */
	virtual ~AbstractModelPortfolio(){};
};

} // namespace sic

#endif // SIC_MODEL_ABSTRACTMODELPORTFOLIO_H_
