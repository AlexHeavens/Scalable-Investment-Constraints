#ifndef SIC_ABSTRACTPORTFOLIOFACTORY_H_
#define SIC_ABSTRACTPORTFOLIOFACTORY_H_

#include <memory>

#include "sic/AbstractPortfolio.hh"

namespace sic {

/**
 * Interface for a factory producing Portfolios.
 */
class AbstractPortfolioFactory {

public:
	/**
	 * Generate a new Portfolio.
	 */
	virtual std::unique_ptr<sic::AbstractPortfolio> create();

	/**
	 * Destroy the factory.
	 */
	virtual ~AbstractPortfolioFactory();
};

} // namespace sic

#endif // SIC_ABSTRACTPORTFOLIOFACTORY_H_
