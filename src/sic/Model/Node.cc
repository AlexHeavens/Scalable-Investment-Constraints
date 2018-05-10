#include "sic/Model/Node.hh"

sic::Model::Node &sic::Model::Node::addChild() {

	std::unique_ptr<sic::Model::Node> newNode(new sic::Model::Node(this));
	childNodes.push_back(std::move(newNode));
	return *childNodes.back();
}
