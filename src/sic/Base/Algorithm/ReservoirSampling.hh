#ifndef SIC_RESERVOIRSAMPLING_H_
#define SIC_RESERVOIRSAMPLING_H_

#include <memory>
#include <random>
#include <stdexcept>
#include <vector>

#include "sic/Base/Iterators.hh"

namespace sic {

/**
 * Sample uniformly an arbitrary range in a single pass.
 *
 * Random integers are taken via std::default_random_engine
 * (implementation-specific).
 *
 * @param iterators collection to sample from.
 * @param sampleCount number of samples to take.
 *
 * @return a vector of pointers to the sampled items from the iterators
 * collection.
 */
template <typename Item>
static std::unique_ptr<std::vector<const Item *>>
resevoirSample(sic::Iterators<Item> iterators, size_t sampleCount) {

	auto samples = std::make_unique<std::vector<const Item *>>(sampleCount);

	std::default_random_engine randomEngine;
	size_t itemsSeen = 0;
	size_t replaceIndex = 0;

	while (iterators.remaining()) {
		itemsSeen++;

		std::uniform_int_distribution<size_t> intDistr(1, itemsSeen);
		if (intDistr(randomEngine) <= sampleCount) {
			samples->at(replaceIndex) = &(*iterators.current());
			replaceIndex = (replaceIndex + 1) % sampleCount;
		}

		iterators.current()++;
	}

	if (itemsSeen < sampleCount) {
		throw std::invalid_argument(
			"Population size (" + std::to_string(itemsSeen) +
			") is smaller than requested sample count (" +
			std::to_string(sampleCount) + ").");
	}

	return samples;
}

} // namespace sic

#endif // SIC_RESERVOIRSAMPLING_H_
