
#include "sic/UseCase/TraditionalAAContext.hh"
#include "sic/UseCases.hh"

int main() {

	sic::TraditionalAAContext useCase;
	auto &context = useCase.getEvaluationContext();
	std::size_t maxAssetCount = context.getAssetCache().size();
	std::size_t maxFilterCount = context.getFilterTreeCache().size();

	sic::UseCase::time("FilterAssets", [&]() {
		sic::UseCase::filterAssets(context, maxFilterCount, maxAssetCount);
	});

	return 0;
}
