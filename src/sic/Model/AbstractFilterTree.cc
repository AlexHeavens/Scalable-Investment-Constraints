#include "sic/Model/AbstractFilterTree.hh"

namespace sic {

const sic::AbstractFilterNode *
AbstractFilterTree::node_iterator::getNextNode() const {

	sic::AbstractFilterNode *nextNode = nullptr;

	// Depth-first search.  Keep taking descendent nodes until we hit a leaf.
	if (!currentNode->isLeaf()) {

		nextNode = &currentNode->getChild(0);

	} else {

		// Try to take the next child of the current node's parent.
		const auto *parentNode = currentNode->getParentNode();
		const auto *prevNode = currentNode;
		const auto prevNodeIndex = prevNode->getParentIndex();
		auto nextNodeIndex = prevNodeIndex + 1;

		// When we reach the end a parent node's children, recurse up the tree
		// until we find a parent with remaining children to visit.
		while (parentNode != nullptr and
			   nextNodeIndex == parentNode->getChildCount()) {
			prevNode = parentNode;
			parentNode = parentNode->getParentNode();

			if (parentNode != nullptr) {

				const auto prevNodeIndex = prevNode->getParentIndex();
				nextNodeIndex = prevNodeIndex + 1;
			}
		}

		// nullptr parent indicates root node; the iteration is complete.
		if (parentNode != nullptr) {
			nextNode = &parentNode->getChild(nextNodeIndex);
		}
	}

	return nextNode;
}

} // namespace sic
