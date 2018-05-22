#include "sic/Model/FilterNode.hh"

sic::Model::AbstractFilterNode &sic::Model::FilterNode::addChild() {

	std::unique_ptr<sic::Model::AbstractFilterNode> newNode(
		new sic::Model::FilterNode(this));
	childNodes.push_back(std::move(newNode));
	return *childNodes.back();
}
