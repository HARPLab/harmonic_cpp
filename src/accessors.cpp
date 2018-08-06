/*
 * accessors.cpp
 *
 *  Created on: Jul 27, 2018
 *      Author: reubena
 */

#include "harmonic_cpp/accessors.hpp"

namespace harmonic {

DataRun::DataRun() : root_dir() {
	// Default constructor for empty (invalid) runs
}

DataRun::DataRun(std::string const & root_dir) : root_dir(root_dir) {
	if (!this->check_valid()) {
		throw std::runtime_error("Supplied path " + root_dir + " was not a valid HARMONIC data directory.");
	}
}

bool DataRun::check_valid() const {
	// TODO: probably something more sophisticated?
	return boost::filesystem::is_regular_file(this->run_info_path_fs());
}

DataRunIterator::DataRunIterator(std::string const & base_path,
		boost::filesystem::symlink_option opt) :
	iter(boost::filesystem::recursive_directory_iterator(base_path, opt)) {
	this->get_next();
}

void DataRunIterator::increment() {
	++(this->iter);
	this->get_next();
}

void DataRunIterator::get_next() {
	// increment until either:
	//   (1) we've found a valid data run
	//   (2) we're out of files
	while (this->iter != boost::filesystem::recursive_directory_iterator()) {
		this->data_run.root_dir = *(this->iter);
		if (this->data_run.check_valid()) {
			break;
		}
		++(this->iter);
	}
}

} // namespace harmonic
