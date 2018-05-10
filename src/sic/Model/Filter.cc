#include "sic/Model/Filter.hh"

bool sic::Model::Filter::evaluate(const sic::Asset &asset) const {
	return predicate(asset);
}
