/*
 * dataset_test.cpp
 *
 *  Created on: Aug 7, 2018
 *      Author: reubena
 */

#include <harmonic_cpp/accessors.hpp>
#include <iostream>

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Usage: dataset_test <path>" << std::endl;
		return 1;
	}

	harmonic::HarmonicDataset dataset(argv[1]);
	boost::copy(
			dataset.get(
					harmonic::HarmonicDataset::Specifier::filter_or(
							harmonic::HarmonicDataset::Specifier::TagFilter(harmonic::RUN),
							harmonic::HarmonicDataset::Specifier::TagFilter(harmonic::CALIB)))
				| boost::adaptors::transformed(harmonic::DataRunPrinter()),
			std::ostream_iterator<std::string>(std::cout, "\n")
			);
}

