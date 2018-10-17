/*
 * accessors.hpp
 *
 *  Created on: Jul 27, 2018
 *      Author: reubena
 */

#ifndef INCLUDE_HARMONIC_CPP_ACCESSORS_HPP_
#define INCLUDE_HARMONIC_CPP_ACCESSORS_HPP_

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/iterator/filter_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/lexical_cast.hpp>

#include "harmonic_cpp/morsel_info.hpp"
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

enum TAG {
	RUN,
	CHECK,
	CALIB
};

boost::filesystem::path get_path_from_tag(TAG tag) {
	switch (tag) {
	case RUN: return TAG_RUN_FS;
	case CHECK: return TAG_CHECK_FS;
	case CALIB: return TAG_CALIB_FS;
	default: throw std::runtime_error("Unknown tag: " + boost::lexical_cast<std::string>(tag));
	}
}

HARMONIC_CPP_MAKE_VAL(DIR, TEXT_DATA, "text_data");
HARMONIC_CPP_MAKE_VAL(DIR, STATS, "stats");
HARMONIC_CPP_MAKE_VAL(DIR, KEYPOINTS, "keypoints");
HARMONIC_CPP_MAKE_VAL(DIR, VIDEOS, "videos");
HARMONIC_CPP_MAKE_VAL(DIR, PROCESSED, "processed");

struct Specifier {
	struct PartId {
		typedef std::size_t value;
		inline static value get(Specifier const & spec) { return spec.part_id; }
		inline static value parse(std::string const & val) { return boost::lexical_cast<value>(val); }
	};
	struct Tag {
		typedef TAG value;
		inline static value get(Specifier const & spec) { return spec.tag; }
		static value parse(std::string const & val);
	};
	struct RunId {
		typedef std::size_t value;
		inline static value get(Specifier const & spec) { return spec.run_id; }
		inline static value parse(std::string const & val) { return boost::lexical_cast<value>(val); }
	};

	template <class T>
	struct FilterBase {
		FilterBase(typename T::value const & filt) : filt(filt) {}
		inline bool operator()(Specifier const & spec) const { return T::get(spec) == filt; }
	private:
		typename T::value filt;
	};
	template <class Filter1, class Filter2>
	struct FilterAnd {
		FilterAnd(Filter1 filt1, Filter2 filt2) : filt1(filt1), filt2(filt2) {}
		inline bool operator()(Specifier const & spec) const { return filt1(spec) && filt2(spec); }
	private:
		Filter1 filt1;
		Filter2 filt2;
	};
	template <class F1, class F2>
	static FilterAnd<F1, F2> filter_and(F1 f1, F2 f2) { return FilterAnd<F1, F2>(f1, f2); }

	template <class Filter1, class Filter2>
	struct FilterOr {
		FilterOr(Filter1 filt1, Filter2 filt2) : filt1(filt1), filt2(filt2) {}
		inline bool operator()(Specifier const & spec) const { return filt1(spec) || filt2(spec); }
	private:
		Filter1 filt1;
		Filter2 filt2;
	};
	template <class F1, class F2>
	static FilterOr<F1, F2> filter_or(F1 f1, F2 f2) { return FilterOr<F1, F2>(f1, f2); }

	typedef FilterBase<PartId> PartIdFilter;
	typedef FilterBase<Tag> TagFilter;
	typedef FilterBase<RunId> RunIdFilter;

	PartId::value part_id;
	Tag::value tag;
	RunId::value run_id;

	std::string get_identifier() const;
};

struct DataRun {
	DataRun(char const * root_dir, boost::optional<Specifier> const & spec = boost::optional<Specifier>()) : DataRun(boost::filesystem::path(root_dir), spec) {}
	DataRun(std::string const & root_dir, boost::optional<Specifier> const & spec = boost::optional<Specifier>()) : DataRun(boost::filesystem::path(root_dir), spec) {}
	DataRun(boost::filesystem::path const & root_dir, boost::optional<Specifier> const & spec = boost::optional<Specifier>());

	HARMONIC_CPP_MAKE_ACCESSOR(root, ".")

	// Directories
	HARMONIC_CPP_MAKE_ACCESSOR(text_data, DIR_TEXT_DATA_FS)
	HARMONIC_CPP_MAKE_ACCESSOR(stats, DIR_STATS_FS)
	HARMONIC_CPP_MAKE_ACCESSOR(keypoints, DIR_KEYPOINTS_FS)
	HARMONIC_CPP_MAKE_ACCESSOR(videos, DIR_VIDEOS_FS)
	HARMONIC_CPP_MAKE_ACCESSOR(processed, DIR_PROCESSED_FS)

	// Pupil videos
	HARMONIC_CPP_MAKE_ACCESSOR(pupil_world, DIR_VIDEOS_FS / "world.avi")
	HARMONIC_CPP_MAKE_ACCESSOR(pupil_world_ts, DIR_VIDEOS_FS / "world_timestamps.npy")

	inline TimedVideo pupil_world() const {return TimedVideo(pupil_world_path(), pupil_world_ts_path());}

	// Stats
	HARMONIC_CPP_MAKE_ACCESSOR(run_info, DIR_STATS / "run_info.yaml")

	// Text data
	HARMONIC_CPP_MAKE_ACCESSOR(robot_position, DIR_TEXT_DATA / "robot_position.csv")
	HARMONIC_CPP_MAKE_ACCESSOR(morsel_info, DIR_TEXT_DATA / "morsel.yaml")


	inline RobotPositionData robot_position() const {return RobotPositionData::load_from_file( robot_position_path() );}
	inline MorselInfo morsel_info() const { return MorselInfo::load_from_file( morsel_info_path() ); }

	static bool check_valid(boost::filesystem::path const & path);

	inline boost::optional<Specifier> const & specifier() { return spec; }

	std::string get_identifier() const;

private:
	DataRun();
	boost::filesystem::path root_dir;
	boost::optional<Specifier> spec;

	friend class DataRunChecker;
	friend class DataRunPrinter;
	friend std::ostream & operator<<(std::ostream & ostr, DataRun const & run);
};

std::ostream & operator<<(std::ostream & ostr, DataRun const & run);

struct DataRunChecker {
	bool operator()(boost::filesystem::path const & path) const {
		return DataRun::check_valid(path);
	}
};
struct DataRunCreator {
	DataRun operator()(boost::filesystem::path const & path) const {
		return DataRun(path);
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


	struct Lookup {
	public:
		inline DataRun operator()(Specifier const & spec) const {
			return Lookup::lookup(root_path, spec);
		}
	private:
		Lookup() : root_path() {
			throw std::runtime_error("not accessible!");
		}
		Lookup(boost::filesystem::path const & path) : root_path(path) {}
		static DataRun lookup(boost::filesystem::path const & root_path, Specifier const & spec);
		boost::filesystem::path root_path;
		friend class HarmonicDataset;
	};


	inline DataRun get(Specifier const & specifier) const {
		return Lookup::lookup(root_path, specifier);
	}
	inline DataRun operator()(Specifier const & specifier) const {
		return get(specifier);
	}
	inline Lookup getter() const {
		return Lookup(this->root_path);
	}
	inline auto transformer() const -> decltype(boost::adaptors::transformed(Lookup())) {
		return boost::adaptors::transformed(getter());
	}

	template <class Range,
		typename boost::enable_if< boost::has_range_const_iterator<Range>, int >::type = 0>
	inline auto get(Range const & r) const
		-> decltype(r | transformer()) {
		return r | transformer();
	}

	inline auto get() const -> decltype(get(std::vector<Specifier>())) {
		return get(all_data);
	}

	template <class Range, class Filter>
	inline auto get(Range const & r, Filter filt) const -> decltype(get(r | boost::adaptors::filtered(filt))) {
		return get( r | boost::adaptors::filtered(filt));
	}

	template <class Filter,
		typename boost::disable_if< boost::has_range_const_iterator<Filter>, int >::type = 0>
	inline auto get(Filter filt) const -> decltype(get(std::vector<Specifier>(), filt)) {
		return get(all_data, filt);
	}

	inline std::vector<Specifier> const & data() const {
		return all_data;
	}

private:

	boost::filesystem::path root_path;
	std::vector<Specifier> all_data;
};


}// namespace harmonic

#endif /* INCLUDE_HARMONIC_CPP_ACCESSORS_HPP_ */
