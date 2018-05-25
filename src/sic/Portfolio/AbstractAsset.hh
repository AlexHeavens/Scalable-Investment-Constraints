#ifndef SIC_ABSTRACTASSET_H_
#define SIC_ABSTRACTASSET_H_

#include <memory>
#include <unordered_set>

#include "sic/External.hh"

namespace sic {

/**
 * A financial instrument that can be held by a client.
 *
 * Assets define the general properties of the Positions a client holds in their
 * portfolio.
 */
class AbstractAsset : public sic::External {

public:
	typedef unsigned Class;
	typedef std::unordered_set<sic::AbstractAsset::Class> ClassSet;
	typedef sic::AbstractAsset::ClassSet::iterator ClassIterator;

	/**
	 * Construct an AbstractAsset.
	 *
	 * Without a clean way to inherity from an "AbstractExternal", it is
	 * necessary to pass the externalID to the real External class.
	 *
	 * @param externalID ID of the Asset as represented by the external system
	 * using the SIC Engine.  In the results the engine provides, this will be
	 * the identifier used to ensure the external system can match their Asset
	 * to these results.
	 */
	AbstractAsset(sic::External::ID externalID) : sic::External(externalID){};

	/**
	 * If the Asset has a given Asset Class.
	 */
	virtual bool hasClass(sic::AbstractAsset::Class assetClass) const = 0;
};

} // namespace sic

#endif // SIC_ABSTRACTASSET_H_
