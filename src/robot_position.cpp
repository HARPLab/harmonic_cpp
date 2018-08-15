#include "harmonic_cpp/robot_position.hpp"

#include <fstream>
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "harmonic_cpp/utils.hpp"

namespace harmonic {

std::map<std::string, std::size_t> const JOINT_INDEX_MAP {
		{"mico_link_base", 0},
		{"mico_link_1", 1},
		{"mico_link_2", 2},
		{"mico_link_3", 3},
		{"mico_link_4", 4},
		{"mico_link_5", 5},
		{"mico_link_hand", 6},
		{"mico_end_effector", 7},
		{"mico_tool", 8},
		{"mico_fork", 9}
};

std::vector<std::string> const JOINT_NAMES {
	"mico_link_base",
	"mico_link_1",
	"mico_link_2",
	"mico_link_3",
	"mico_link_4",
	"mico_link_5",
	"mico_link_hand",
	"mico_end_effector",
	"mico_tool",
	"mico_fork"
};

std::vector<std::string> const SUFFIXES {
	"_x",
	"_y",
	"_z"
};

std::vector<std::string> const DESIRED_HEADERS {
	"timestamp",
	"mico_link_base_x",
	"mico_link_base_y",
	"mico_link_base_z",
	"mico_link_1_x",
	"mico_link_1_y",
	"mico_link_1_z",
	"mico_link_2_x",
	"mico_link_2_y",
	"mico_link_2_z",
	"mico_link_3_x",
	"mico_link_3_y",
	"mico_link_3_z",
	"mico_link_4_x",
	"mico_link_4_y",
	"mico_link_4_z",
	"mico_link_5_x",
	"mico_link_5_y",
	"mico_link_5_z",
	"mico_link_hand_x",
	"mico_link_hand_y",
	"mico_link_hand_z",
	"mico_end_effector_x",
	"mico_end_effector_y",
	"mico_end_effector_z",
	"mico_tool_x",
	"mico_tool_y",
	"mico_tool_z",
	"mico_fork_x",
	"mico_fork_y",
	"mico_fork_z"
};

RobotPositionData RobotPositionData::load_from_file(std::string const & infile) {
	std::ifstream instream(infile);
	std::string line;
	std::getline(instream, line, '\n');

	if (instream.good()) {
		// build the header map

		std::vector<std::string> const headers(parse_csv_line(line));

		std::map<std::string, std::size_t> index_map;
		std::transform(DESIRED_HEADERS.begin(), DESIRED_HEADERS.end(),
				std::inserter(index_map, index_map.begin()),
				[&headers] (std::string const & header) {
			auto it = std::find(headers.begin(), headers.end(), header);
			if (it == headers.end()) {
				throw std::runtime_error("File missing required header: " + header);
			}
			return std::make_pair(header, std::distance(headers.begin(), it));
		});

		// Read the data
		RobotPositionData data;
		while (true) {
			std::getline(instream, line, '\n');
//			std::cout << "Processing line:\n" << line << std::endl;
			if (!instream.good()) break;

			std::vector<std::string> const line_data(parse_csv_line(line));

			TimeType time = boost::lexical_cast<double>(line_data[index_map.at("timestamp")]);
			std::vector<std::vector<double> > values;

			for ( auto const & joint : JOINT_NAMES) {
				std::vector<double> joint_vals;
				joint_vals.reserve(3);
				for ( auto const & suffix : SUFFIXES ) {
					joint_vals.push_back(boost::lexical_cast<double>(line_data[index_map.at(joint + suffix)]));
				}
				values.push_back(std::move(joint_vals));
			}

			data.data.emplace(time, PositionData(JOINT_NAMES, values));
		}

		return data;

	} else {
		throw std::runtime_error("Failed to read from file " + infile);
	}

}

RobotPositionData::PositionData::PositionData() :
	data(JOINT_INDEX_MAP.size(), 3, CV_64F) {}

RobotPositionData::PositionData::PositionData(std::vector<std::string> const & joints, std::vector<std::vector<double> > const & values) :
	data(3, JOINT_INDEX_MAP.size(), CV_64F) {
	auto joint = joints.begin();
	auto value = values.begin();
	while (joint != joints.end() && value != values.end()) {
		cv::Mat(*value).copyTo(this->data.col(JOINT_INDEX_MAP.at(*joint)));
		++joint; ++value;
	}
	data = data.t();
}


RobotPositionData::TimeType RobotPositionData::get_start_time() const {
	return this->data.begin()->first;
}

RobotPositionData::TimeType RobotPositionData::get_end_time() const {
	return this->data.rbegin()->first;
}

RobotPositionData::PositionData RobotPositionData::get_position(RobotPositionData::TimeType const & timestamp) const {
	RobotPositionData::ConstIteratorType lower = this->data.lower_bound(timestamp);

//	std::for_each(this->data.begin(), this->data.end(), [&] (RobotPositionData::ContainerType::value_type const & pair) {
//		std::cout << "t: " << std::setprecision(16) << pair.first << std::endl;
//	});

//	std::cout << "searched for " << std::setprecision(16) << timestamp << " (min: " << this->data.begin()->first << " max: " << this->data.rbegin()->first << ")" << std::endl;

	if (lower == this->data.end()) {
//		std::cout << "No lower bound found, returning end" << std::endl;
		return this->data.rbegin()->second;
	} else if (lower == this->data.begin()) {
//		std::cout << "No upper bound found, returning lower" << std::endl;
		return lower->second;
	} else if (lower->first == timestamp) {
//		std::cout << "Found exactly" << std::endl;
		return lower->second;
	} else {
//		std::cout << "Interpolating" << std::endl;
		auto pre = lower; --pre;
		return RobotPositionData::interpolate(lower->first, lower->second, pre->first, pre->second, timestamp);
	}
}

RobotPositionData::PositionData RobotPositionData::interpolate(RobotPositionData::TimeType const & t1, RobotPositionData::PositionData const & d1,
		RobotPositionData::TimeType const & t2, RobotPositionData::PositionData const & d2, RobotPositionData::TimeType const & t) {
	double const factor = (t-t1)/(t2-t1);
//	std::cout << "Interpolating btw " << std::setprecision(16) << t1 << ", " << t2 << ": d=" << (t2-t1) << " n=" << (t-t1) << " f=" << factor << std::endl;

	RobotPositionData::PositionData result;
	result.data = (1-factor)*d1.data + factor*d2.data;
	return result;
}

} // namespace harmonic
