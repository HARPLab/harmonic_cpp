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

DataRun::DataRun(boost::filesystem::path const & root_dir) : root_dir(root_dir) {
	if (!this->check_valid()) {
		throw std::runtime_error("Supplied path " + root_dir.native() + " was not a valid HARMONIC data directory.");
	}
}

bool DataRun::check_valid() const {
	// TODO: probably something more sophisticated?
	return boost::filesystem::is_regular_file(this->run_info_path_fs());
}

} // namespace harmonic
