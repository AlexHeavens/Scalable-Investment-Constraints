#include "sic/Model/FilterNode.hh"

sic::AbstractFilterNode &
sic::FilterNode::addChild(std::unique_ptr<const sic::Filter> childFilter) {

	std::unique_ptr<sic::AbstractFilterNode> newNode(
		new sic::FilterNode(std::move(childFilter), this));
	childNodes.push_back(std::move(newNode));
	return *childNodes.back();
}

sic::AbstractFilterNode *
sic::FilterNode::filterToChild(const sic::AbstractAsset &asset) {

	sic::AbstractFilterNode *matchNode = nullptr;
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

sic::Iterators<sic::AbstractFilterNode::ChildIterator>
sic::FilterNode::getChildIterators() {
	return sic::Iterators<sic::AbstractFilterNode::ChildIterator>(
		childNodes.begin(), childNodes.end());
}

sic::Iterators<sic::AbstractFilterNode::ConstChildIterator>
sic::FilterNode::getChildIterators() const {
	return sic::Iterators<sic::AbstractFilterNode::ConstChildIterator>(
		childNodes.cbegin(), childNodes.cend());
}
