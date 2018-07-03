#ifndef SIC_MODEL_ABSTRACTMODELPORTFOLIO_H_
#define SIC_MODEL_ABSTRACTMODELPORTFOLIO_H_

#include "sic/Base/Iterators.hh"
#include "sic/Base/Types.hh"
#include "sic/External.hh"
#include "sic/Portfolio/AbstractAsset.hh"

namespace sic {

/**
 * A simple weighted Asset list portfolio model.
 *
 * This is an abstract representation of a ModelPortfolio to allow for mock
 * testing.
 */
class AbstractModelPortfolio : public sic::External {

protected:
	/**
	 * Construct an AbstractModelPortfolio.
	 *
	 * This is protected to prevent instantiation of an abstract class.
	 *
	 * @param externalID ID of the MPF in the external system.
	 */
	AbstractModelPortfolio(sic::External::ID externalID = 0)
		: sic::External(externalID) {}

public:
	/**
	 * The begin and end iterators of the ModelPortfolio's Asset/Weight-Range
	 * pairs.
	 *
	 * @returns Current and end iterators.
	 */
	virtual sic::Iterators<
		std::pair<const sic::AbstractAsset *const, sic::WeightRange>>
	getAssetWeightIterators() const = 0;

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
