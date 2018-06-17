#ifndef SIC_ABSTRACTPOSITION_H_
#define SIC_ABSTRACTPOSITION_H_

#include "sic/Base/Types.hh"
#include "sic/External.hh"
#include "sic/Portfolio/AbstractAsset.hh"

namespace sic {

/**
 * Interface for an instance of a financial asset within a client's portfolio.
 *
 * @see sic::Asset
 */
class AbstractPosition : public sic::External {

protected:
	/**
	 * Root constructor for any Position.
	 *
	 * @param externalID ID of the Position in the external system.  Optional
	 * for easy mock testing.
	 */
	AbstractPosition(sic::External::ID externalID = 0)
		: sic::External(externalID) {}

	/**
	 *Destroy the Position.
	 */
	virtual ~AbstractPosition() {}

public:
	/**
	 * The financial asset the position represents a quantity of.
	 */
	virtual sic::AbstractAsset &getAsset() const = 0;

	/**
	 * The value in the (unspecified) reference currency.
	 */
	virtual sic::Value getReferenceValue() const = 0;
};

} // namespace sic

#endif // SIC_ABSTRACTPOSITION_H_
