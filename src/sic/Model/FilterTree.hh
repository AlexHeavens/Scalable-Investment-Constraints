#ifndef SIC_MODEL_FILTERTREE_H_
#define SIC_MODEL_FILTERTREE_H_

#include "sic/Model/AbstractFilterNode.hh"
#include "sic/Model/AbstractFilterTree.hh"
#include "sic/Model/FilterNode.hh"

namespace sic::Model {

/**
 * A Tree of Asset filters.
 */
class FilterTree : public sic::Model::AbstractFilterTree {

private:
	sic::Model::FilterNode rootNode;

public:
	/**
	 * Get the root filter node of the tree.
	 */
	sic::Model::AbstractFilterNode &getRootNode() { return rootNode; }
};

} // namespace sic::Model

#endif // SIC_MODEL_FILTERTREE_H_
