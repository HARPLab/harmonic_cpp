/*
 * morsel_info.cpp
 *
 *  Created on: Aug 13, 2018
 *      Author: reubena
 */

#include "harmonic_cpp/morsel_info.hpp"

#include "yaml-cpp/yaml.h"
#include <boost/lexical_cast.hpp>

namespace harmonic {

void load_mat(YAML::Node const & node, cv::Mat & mat) {
	mat = (cv::Mat_<double>(4,4) <<
			node[0][0].as<double>() , node[0][1].as<double>() , node[0][2].as<double>() , node[0][3].as<double>(),
			node[1][0].as<double>() , node[1][1].as<double>() , node[1][2].as<double>() , node[1][3].as<double>(),
			node[2][0].as<double>() , node[2][1].as<double>() , node[2][2].as<double>() , node[2][3].as<double>(),
			node[3][0].as<double>() , node[3][1].as<double>() , node[3][2].as<double>() , node[3][3].as<double>()
		);
}

MorselInfo MorselInfo::load_from_file(boost::filesystem::path const & filename) {
	YAML::Node morsels = YAML::LoadFile(filename.native());
	MorselInfo info;

	for (unsigned int i=0; i<3; ++i) {
		std::string const name = "morsel" + boost::lexical_cast<std::string>(i);
		info.morsels[i].name = name;
		if (morsels[name] && !morsels[name].IsNull()) {
			load_mat(morsels[name], info.morsels[i].location);
		} else {
			info.morsels[i].location = cv::Mat();
		}
	}

	info.stated_goal = morsels["stated_goal"].as<std::size_t>();
	info.any_success = morsels["any_success"].as<bool>();
	info.goal_success = morsels["goal_success"].as<bool>();

	return info;
}


} // namespace harmonic


