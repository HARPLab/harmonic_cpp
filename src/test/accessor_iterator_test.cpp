#include "harmonic_cpp/accessors.hpp"
#include <iostream>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Usage: accessor_iterator_test <directory>" << std::endl;
		return 1;
	}

	boost::copy(
			harmonic::data_run_recursive_range(argv[1]) |
				boost::adaptors::transformed(harmonic::DataRunPrinter()),
			std::ostream_iterator<std::string>(std::cout, "\n")
			);
//	std::for_each(
//			harmonic::data_run_recursive_iterator(argv[1]),
//			harmonic::data_run_recursive_iterator(),
//			[] (harmonic::DataRun const & data_run) {
//		std::cout << data_run.root_path() << std::endl;
//	});
}
