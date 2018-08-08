# Scalable Investment Constraints

SIC provides a high-performance engine for evaluating if clients' portfolios remain in-line with their asset allocation models.

It is currently under development and is expected to both functionally change and be improved with optimisation.

## Prerequisites
SIC requires at a minimum the following libraries and tools to be installed:
- CMake 3.10 or higher
- Boost 1.67 or higher

To generate unit tests and benchmarks, the following libraries are necessary:
- Google Test / Google Mock
- Google Benchmark

Source code documentation is generated under the doc folder if Doxygen is installed.

## Build Instructions
It is possible to build any of the components with CMake explicitly.  However, the following bash scripts are provided for ease of use:

./script/build.sh - generate all documentation in the doc folder, benchmarks, use cases and unit test binaries in the bin folder
./script/clean_build.sh - purge the existing build files and rebuild
./script/debug_build.sh - generate debug binaries in the bin_debug folder
./script/install_hooks.sh - install into the local git repository a pre-commit hook ensuring unit tests pass

## Branching
This repository follows the [git flow](https://datasift.github.io/gitflow/IntroducingGitFlow.html) pattern of branching where the following nomenclature applies:

- master: public releases and hotfixes
- develop: main development stream that features are merged into or for small additions
- feature/X: isolated branches for developing new features
- experimental/X: experimental features and optimisations.  Notably, this includes advanced optimisations including parallelisation.

For example, to checkout, build and benchmark the parallelised version of SIC, you can execute the following:

```
git checkout experimental/parallelisation
./script/build.sh
./bin/benchmarks
```
