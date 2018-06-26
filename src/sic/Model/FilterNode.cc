#include "sic/Model/FilterNode.hh"

namespace sic {

sic::AbstractFilterNode &
FilterNode::addChild(std::unique_ptr<const sic::Filter> childFilter) {

	std::unique_ptr<sic::AbstractFilterNode> newNode(new sic::FilterNode(
		filterTree, std::move(childFilter), this, childNodes.size()));
	childNodes.push_back(std::move(newNode));
	return *childNodes.back();
}

const sic::AbstractFilterNode *
FilterNode::filterToChild(const sic::AbstractAsset &asset) const {

	const sic::AbstractFilterNode *matchNode = nullptr;
	auto childIt = getChildIterators();
	while (childIt.current() != childIt.end()) {

		const auto &currentChild = *childIt.current();
		if (currentChild->getFilter().evaluate(asset)) {
			matchNode = currentChild.get();
			break;
		}

		childIt.current()++;
	}

	return matchNode;
}

sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>
FilterNode::getChildIterators() {
	return sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>(childNodes);
}

sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>
FilterNode::getChildIterators() const {
	return sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>(childNodes);
}

} // namespace sic
