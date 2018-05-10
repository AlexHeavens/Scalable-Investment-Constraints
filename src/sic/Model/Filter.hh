#ifndef SIC_MODEL_FILTER_H_
#define SIC_MODEL_FILTER_H_

#include "sic/Portfolio/Asset.hh"

namespace sic::Model {

/**
 * Asset Filter for placement in a FilterTree.
 */
class Filter {
public:
	/// A predicate function to return if an Asset passes the filter.
	typedef bool (*Pred)(const sic::Asset &);

private:
	sic::Model::Filter::Pred predicate;

public:
	/**
	 * Construct a Filter.
	 *
	 * @param predicate function determining Assets that pass the filter.
	 */
	Filter(sic::Model::Filter::Pred predicate) : predicate(predicate) {}

	/**
	 * If a given Asset passes the filter.
	 */
	bool evaluate(const sic::Asset &asset) const;
};

} // namespace sic::Model

#endif // SIC_MODEL_FILTER_H_
