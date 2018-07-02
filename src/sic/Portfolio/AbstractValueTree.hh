#ifndef SIC_ABSTRACTVALUETREE_H_
#define SIC_ABSTRACTVALUETREE_H_

namespace sic {

/**
 * An overlay of a client's portfolio value onto nodes of a FilterTree.
 *
 * @see sic::AbstractFilterTree
 * @see sic::Portfolio
 */
class AbstractValueTree {

protected:
	/**
	 * Construct an AbstractValueTree.
	 *
	 * This is protected to avoid instantiation of a purely abstract class.
	 */
	AbstractValueTree() = default;

public:
	/// Storage mapping of FilterTree nodes to their weight in the Portfolio.
	typedef std::unordered_map<const sic::AbstractFilterNode *, sic::Weight>
		NodeWeightMap;

	/**
	 * Retrieve the to-top weight of the Positions that fall under a specific
	 * FilterNode in the client's Portfolio.
	 */
	virtual sic::Weight
	getNodeWeight(const sic::AbstractFilterNode &filterNode) const = 0;

	/**
	 * Destory AbstractValueTree.
	 */
	virtual ~AbstractValueTree() {}

	/**
	 * Compare equivalence to another ValueTree.
	 */
	virtual bool operator==(const sic::AbstractValueTree &rhs) = 0;

	/**
	 * Iterators over the nodes / weights of the ValueTree.
	 */
	virtual sic::Iterators<NodeWeightMap::value_type>
	getNodeWeightIterators() const = 0;
};

} // namespace sic

#endif // SIC_ABSTRACTVALUETREE_H_
