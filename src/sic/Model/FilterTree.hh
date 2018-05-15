#ifndef SIC_MODEL_FILTERTREE_H_
#define SIC_MODEL_FILTERTREE_H_

#include "sic/Model/FilterNode.hh"

namespace sic::Model {

/**
 * A Tree of Asset filters.
 */
class FilterTree {

private:
	sic::Model::FilterNode rootNode;

public:
	/**
	 * Get the root filter node of the tree.
	 */
	sic::Model::FilterNode &getRootNode() { return rootNode; }
};

} // namespace sic::Model

#endif // SIC_MODEL_FILTERTREE_H_
