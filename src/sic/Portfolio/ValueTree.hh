#ifndef SIC_VALUETREE_H_
#define SIC_VALUETREE_H_

#include <memory>
#include <unordered_map>

#include "sic/Base/Types.hh"
#include "sic/Portfolio/AbstractValueTree.hh"

namespace sic {

/**
 * Concrete implementation of an overlay of a client's portfolio value onto
 * nodes of a FilterTree.
 *
 * @see sic::AbstractFilterTree
 * @see sic::Portfolio
 */
class ValueTree : public sic::AbstractValueTree {

public:
	/// Storage mapping of FilterTree nodes to their weight in the Portfolio.
	typedef std::unordered_map<const sic::AbstractFilterNode *, sic::Weight>
		NodeWeightMap;

private:
	const std::unique_ptr<NodeWeightMap> nodeWeightMap;

public:
	/**
	 * Construct a ValueTree.
	 */
	ValueTree(std::unique_ptr<NodeWeightMap> nodeWeightMap)
		: nodeWeightMap(std::move(nodeWeightMap)) {}

	sic::Weight
	getNodeWeight(const sic::AbstractFilterNode &filterNode) const override {
		return nodeWeightMap->find(&filterNode)->second;
	}

	/**
	 * Compare equivalence to another ValueTree.
	 */
	bool operator==(const ValueTree &rhs) {
		return *this->nodeWeightMap == *rhs.nodeWeightMap;
	}
};

} // namespace sic

#endif // SIC_VALUETREE_H_
