#ifndef SIC_PARALLELISM_H_
#define SIC_PARALLELISM_H_

#include <thread>

namespace sic {

/**
 * Parameters of parallel evalauting in SIC.
 */
struct ParallelParameters {
	const std::size_t threadCount;
	const bool serial;

	/**
	 * Construct ParallelParameters.
	 */
	ParallelParameters(std::size_t threadCount = getMaxThreadCount(),
					   bool serial = false)
		: threadCount(threadCount), serial(serial) {

		if (threadCount == 0) {
			throw std::invalid_argument(
				"ParallelParameters: threadCount must be >= 1");
		}
		if (serial and threadCount > 1) {
			throw std::invalid_argument(
				"ParallelParameters: serial requires threadCount to be 1");
		}
	}

	/**
	 * The maximum number of threads to make efficient use of the system's
	 * cores.
	 */
	static std::size_t getMaxThreadCount() {
		std::size_t threadCount = std::thread::hardware_concurrency();

		// Zero implies thread count is unknown.
		if (threadCount == 0) {
			threadCount = 1;
		}

		return threadCount;
	}
};

} // namespace sic

#endif // SIC_PARALLELISM_H_
