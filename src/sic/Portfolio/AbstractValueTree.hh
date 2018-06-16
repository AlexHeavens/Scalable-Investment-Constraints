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

public:
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
};

} // namespace sic

#endif // SIC_ABSTRACTVALUETREE_H_
