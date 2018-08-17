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
					harmonic::Specifier::filter_or(
							harmonic::Specifier::TagFilter(harmonic::RUN),
							harmonic::Specifier::TagFilter(harmonic::CALIB))),
			std::ostream_iterator<harmonic::DataRun>(std::cout, "\n")
			);
}

