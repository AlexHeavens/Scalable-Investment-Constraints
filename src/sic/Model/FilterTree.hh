#ifndef SIC_MODEL_FILTERTREE_H_
#define SIC_MODEL_FILTERTREE_H_

#include "sic/Model/AbstractFilterNode.hh"
#include "sic/Model/AbstractFilterTree.hh"
#include "sic/Model/FilterNode.hh"

namespace sic {

/**
 * A Tree of Asset filters.
 */
class FilterTree : public sic::AbstractFilterTree {

private:
	sic::FilterNode rootNode;

public:
	/**
	 * Construct a FilterTree.
	 */
	FilterTree(sic::External::ID externalID)
		: sic::AbstractFilterTree(externalID) {}

	/**
	 * Get the root filter node of the tree.
	 */
	sic::AbstractFilterNode &getRootNode() { return rootNode; }
};

} // namespace sic

#endif // SIC_MODEL_FILTERTREE_H_
