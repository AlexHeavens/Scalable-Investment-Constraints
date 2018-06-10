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

sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>
sic::FilterNode::getChildIterators() {
	sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>::It begin(
		childNodes.begin());
	const sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>::It end(
		childNodes.end());
	return sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>(begin, end);
}

sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>
sic::FilterNode::getChildIterators() const {
	sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>::It begin(
		childNodes.cbegin());
	const sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>::It end(
		childNodes.cend());

	return sic::Iterators<std::unique_ptr<sic::AbstractFilterNode>>(begin, end);
}
