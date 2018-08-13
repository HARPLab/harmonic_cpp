/*
 * morsel_info.hpp
 *
 *  Created on: Aug 13, 2018
 *      Author: reubena
 */

#ifndef INCLUDE_HARMONIC_CPP_MORSEL_INFO_HPP_
#define INCLUDE_HARMONIC_CPP_MORSEL_INFO_HPP_

#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

namespace harmonic {

struct Morsel {
	cv::Mat location;
	std::string name;
};

struct MorselInfo {
	std::array<Morsel, 3> morsels;
	std::size_t stated_goal;
	bool goal_success;
	bool any_success;

	static MorselInfo load_from_file(boost::filesystem::path const & filename);
};


} // namespace harmonic



#endif /* INCLUDE_HARMONIC_CPP_MORSEL_INFO_HPP_ */
