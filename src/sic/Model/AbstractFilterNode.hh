#ifndef SIC_MODEL_ABSTRACTFILTERNODE_H_
#define SIC_MODEL_ABSTRACTFILTERNODE_H_

namespace sic::Model {

/**
 * A FilterNode in a tree of filters that defines the model (the Asset
 * Allocation).
 *
 * @see sic::Model
 */
class AbstractFilterNode {

public:
	/**
	 * Add a child FilterNode.
	 *
	 * The created FilterNode will be owned (and hence, freed) with its parent.
	 *
	 * @return a reference to the created child FilterNode.
	 */
	virtual sic::Model::AbstractFilterNode &addChild() = 0;

	/**
	 * The parent FilterNode, if non-root, otherwise nullptr.
	 */
	virtual const sic::Model::AbstractFilterNode *getParentNode() const = 0;

	virtual ~AbstractFilterNode(){};
};

} // namespace sic::Model

#endif // SIC_MODEL_ABSTRACTFILTERNODE_H_
