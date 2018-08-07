/*
 * accessors.hpp
 *
 *  Created on: Jul 27, 2018
 *      Author: reubena
 */

#ifndef INCLUDE_HARMONIC_CPP_ACCESSORS_HPP_
#define INCLUDE_HARMONIC_CPP_ACCESSORS_HPP_

#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/algorithm.hpp>

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
HARMONIC_CPP_MAKE_VAL(DIR, PROCESSED, "processed");

struct DataRun {
	DataRun(char const * root_dir) : DataRun(boost::filesystem::path(root_dir)) {}
	DataRun(std::string const & root_dir) : DataRun(boost::filesystem::path(root_dir)) {}
	DataRun(boost::filesystem::path const & root_dir);

	HARMONIC_CPP_MAKE_ACCESSOR(root, ".")

	// Directories
	HARMONIC_CPP_MAKE_ACCESSOR(text_data, DIR_TEXT_DATA_FS)
	HARMONIC_CPP_MAKE_ACCESSOR(stats, DIR_STATS_FS)
	HARMONIC_CPP_MAKE_ACCESSOR(keypoints, DIR_KEYPOINTS_FS)
	HARMONIC_CPP_MAKE_ACCESSOR(videos, DIR_VIDEOS_FS)
	HARMONIC_CPP_MAKE_ACCESSOR(processed, DIR_PROCESSED_FS)

	// Pupil videos
	HARMONIC_CPP_MAKE_ACCESSOR(pupil_world, DIR_VIDEOS_FS / "world.mp4")
	HARMONIC_CPP_MAKE_ACCESSOR(pupil_world_ts, DIR_VIDEOS_FS / "world_timestamps.npy")

	inline TimedVideo pupil_world() const {return TimedVideo(pupil_world_path(), pupil_world_ts_path());}

	// Stats
	HARMONIC_CPP_MAKE_ACCESSOR(run_info, DIR_STATS / "run_info.yaml")

	// Text data
	HARMONIC_CPP_MAKE_ACCESSOR(robot_position, DIR_TEXT_DATA / "robot_position.csv")

	inline RobotPositionData robot_position() const {return RobotPositionData::load_from_file( robot_position_path() );}

private:
	DataRun();
	bool check_valid() const;
	boost::filesystem::path root_dir;

	friend class DataRunChecker;
	friend class DataRunPrinter;
};

struct DataRunChecker {
	bool operator()(boost::filesystem::path const & path) const {
		DataRun tester; // must use default constructor to prevent exception-initialization
		tester.root_dir = path;
		return tester.check_valid();
	}
};
struct DataRunCreator {
	DataRun operator()(boost::filesystem::path const & path) const {
		return DataRun(path);
	}
};
struct DataRunPrinter {
	std::string operator()(DataRun const & run) const {
		return run.root_dir.native();
	}
};

typedef boost::transform_iterator<DataRunCreator,
			boost::filter_iterator<
				DataRunChecker,
				boost::filesystem::directory_iterator>
		> DataRunIterator;
typedef boost::transform_iterator<DataRunCreator,
			boost::filter_iterator<
				DataRunChecker,
				boost::filesystem::recursive_directory_iterator>
		> DataRunRecursiveIterator;

DataRunIterator data_run_iterator() {
	return DataRunIterator();
}
DataRunIterator data_run_iterator(std::string const & base_path) {
	return DataRunIterator(boost::filesystem::directory_iterator(base_path));
}
boost::iterator_range<DataRunIterator> data_run_range(std::string const & base_path) {
	return boost::make_iterator_range(data_run_iterator(base_path), data_run_iterator());
}


DataRunRecursiveIterator data_run_recursive_iterator() {
	return DataRunRecursiveIterator();
}
DataRunRecursiveIterator data_run_recursive_iterator(std::string const & base_path) {
	return DataRunRecursiveIterator(boost::filesystem::recursive_directory_iterator(base_path));
}
boost::iterator_range<DataRunRecursiveIterator> data_run_recursive_range(std::string const & base_path) {
	return boost::make_iterator_range(data_run_recursive_iterator(base_path), data_run_recursive_iterator());
}


struct HarmonicDataset {
	HarmonicDataset(std::string const & root_path);


};


}// namespace harmonic

#endif /* INCLUDE_HARMONIC_CPP_ACCESSORS_HPP_ */
