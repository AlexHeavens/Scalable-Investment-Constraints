#include "sic/Model/FilterNode.hh"

sic::AbstractFilterNode &
sic::FilterNode::addChild(std::unique_ptr<const sic::Filter> childFilter) {

	std::unique_ptr<sic::AbstractFilterNode> newNode(
		new sic::FilterNode(std::move(childFilter), this));
	childNodes.push_back(std::move(newNode));
	return *childNodes.back();
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
