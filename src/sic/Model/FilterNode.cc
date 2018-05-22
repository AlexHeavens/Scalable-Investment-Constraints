#include "sic/Model/FilterNode.hh"

sic::Model::AbstractFilterNode &sic::Model::FilterNode::addChild() {

	std::unique_ptr<sic::Model::AbstractFilterNode> newNode(
		new sic::Model::FilterNode(this));
	childNodes.push_back(std::move(newNode));
	return *childNodes.back();
}

sic::Iterators<sic::Model::FilterNode::ChildIterator>
sic::Model::FilterNode::getChildIterators() {
	return sic::Iterators<sic::Model::FilterNode::ChildIterator>(
		childNodes.begin(), childNodes.end());
}
