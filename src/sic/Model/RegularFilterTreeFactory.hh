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
	sic::AbstractAsset::Class nextClassJump;

	/**
	 * Generate the child nodes and filters of a given parent node.
	 *
	 * This will add nodeDegree - 1 arbitrary AssetClassFilter nodes and one
	 * final AllAssetsFilter node as a catch all.
	 */
	void generateNode(sic::AbstractFilterNode &node, unsigned currentDepth);

public:
	/**
	 * Create a new Factory.
	 */
	RegularFilterTreeFactory(unsigned depth, unsigned nodeDegree)
		: depth(depth), nodeDegree(nodeDegree), nextClassJump(nodeClassJump) {}

	std::unique_ptr<sic::FilterTree>
	create(sic::External::ID externalID) override;

	void create(sic::FilterTree &filterTree) override;

	/**
	 * Return the classes necessary to follow a path through the generated tree.
	 *
	 * @param path the child choice at each step through the tree.
	 * @return Classes necessary for an asset to follow this path through the
	 * tree.
	 */
	std::unique_ptr<sic::AbstractAsset::ClassSet>
	getPathClasses(const std::vector<unsigned> &path) const;
};
} // namespace sic

#endif // SIC_REGULARFILTERTREEFACTORY_H_
