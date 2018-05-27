#ifndef SIC_REGULARFILTERTREEFACTORY_H_
#define SIC_REGULARFILTERTREEFACTORY_H_

#include <memory>

#include "sic/Model/AbstractFilterTreeFactory.hh"

namespace sic {

/**
 * Generates FilterTrees with fixed depths and node degrees.
 */
class RegularFilterTreeFactory : public sic::AbstractFilterTreeFactory {

private:
	const unsigned depth;
	const unsigned nodeDegree;

	static constexpr sic::AbstractAsset::Class nodeClassJump = 100;
	sic::AbstractAsset::Class nextClass, nextClassJump;

	/**
	 * Generate the child nodes and filters of a given parent node.
	 */
	void generateNode(sic::AbstractFilterNode &node, unsigned currentDepth);

public:
	/**
	 * Create a new Factory.
	 */
	RegularFilterTreeFactory(unsigned depth, unsigned nodeDegree)
		: depth(depth), nodeDegree(nodeDegree), nextClass(0),
		  nextClassJump(nodeClassJump) {}

	std::unique_ptr<sic::FilterTree>
	create(sic::External::ID externalID) override;

	void create(sic::FilterTree &filterTree) override;
};
} // namespace sic

#endif // SIC_REGULARFILTERTREEFACTORY_H_
