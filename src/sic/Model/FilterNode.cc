#include "sic/Model/FilterNode.hh"

sic::AbstractFilterNode &
sic::FilterNode::addChild(std::unique_ptr<const sic::Filter> childFilter) {

	std::unique_ptr<sic::AbstractFilterNode> newNode(
		new sic::FilterNode(std::move(childFilter), this));
	childNodes.push_back(std::move(newNode));
	return *childNodes.back();
}

sic::Iterators<sic::FilterNode::ChildIterator>
sic::FilterNode::getChildIterators() {
	return sic::Iterators<sic::FilterNode::ChildIterator>(childNodes.begin(),
														  childNodes.end());
}
