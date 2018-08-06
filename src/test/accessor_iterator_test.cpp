#include "harmonic_cpp/accessors.hpp"
#include <iostream>


int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Usage: accessor_iterator_test <directory>" << std::endl;
		return 1;
	}

	std::for_each(
			harmonic::DataRunIterator(argv[1]),
			harmonic::DataRunIterator(),
			[] (harmonic::DataRun const & data_run) {
		std::cout << data_run.root_path() << std::endl;
	});
}
