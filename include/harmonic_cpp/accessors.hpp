/*
 * accessors.hpp
 *
 *  Created on: Jul 27, 2018
 *      Author: reubena
 */

#ifndef INCLUDE_HARMONIC_CPP_ACCESSORS_HPP_
#define INCLUDE_HARMONIC_CPP_ACCESSORS_HPP_

#include <boost/filesystem.hpp>

#include "harmonic_cpp/robot_position.hpp"
#include "harmonic_cpp/timed_video.hpp"

namespace harmonic {

#define HARMONIC_CPP_MAKE_VAL(type, name, val)\
	constexpr char type ## _ ## name [] = val; \
	const boost::filesystem::path type ## _ ## name ## _FS( type ## _ ## name)
#define HARMONIC_CPP_MAKE_ACCESSOR(name, real_path)\
	inline boost::filesystem::path name ## _path_fs() const { return this->root_dir / real_path; } \
	inline std::string name ## _path () const { return name ## _path_fs().native(); }

HARMONIC_CPP_MAKE_VAL(TAG, RUN, "run");
HARMONIC_CPP_MAKE_VAL(TAG, CHECK, "check");
HARMONIC_CPP_MAKE_VAL(TAG, CALIB, "calib");

HARMONIC_CPP_MAKE_VAL(DIR, TEXT_DATA, "text_data");
HARMONIC_CPP_MAKE_VAL(DIR, STATS, "stats");
HARMONIC_CPP_MAKE_VAL(DIR, KEYPOINTS, "keypoints");
HARMONIC_CPP_MAKE_VAL(DIR, VIDEOS, "videos");

struct DataRun {
	DataRun(std::string const & root_dir);

	// Pupil videos
	HARMONIC_CPP_MAKE_ACCESSOR(pupil_world, DIR_VIDEOS_FS / "world.mp4")
	HARMONIC_CPP_MAKE_ACCESSOR(pupil_world_ts, DIR_VIDEOS_FS / "world_timestamps.npy")

	inline TimedVideo pupil_world() const { return TimedVideo(pupil_world_path(), pupil_world_ts_path()); }

	// Stats
	HARMONIC_CPP_MAKE_ACCESSOR(run_info, DIR_STATS / "run_info.yaml")

	// Text data
	HARMONIC_CPP_MAKE_ACCESSOR(robot_position, DIR_TEXT_DATA / "robot_position.csv")

	// TODO: make this throw an exception rather than return an optional
	inline boost::optional<RobotPositionData> robot_position() const { return RobotPositionData::load_from_file( robot_position_path()); }

private:
	bool check_valid() const;
	boost::filesystem::path root_dir;
};

// TODO!
//std::vector<DataRun> find_data(std::string const & base_path);


} // namespace harmonic



#endif /* INCLUDE_HARMONIC_CPP_ACCESSORS_HPP_ */
