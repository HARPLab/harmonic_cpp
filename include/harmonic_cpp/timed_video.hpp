/*
 * timed_video.hpp
 *
 *  Created on: Jul 27, 2018
 *      Author: reubena
 */

#ifndef INCLUDE_HARMONIC_CPP_TIMED_VIDEO_HPP_
#define INCLUDE_HARMONIC_CPP_TIMED_VIDEO_HPP_

#include <opencv2/opencv.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/optional.hpp>

namespace harmonic {

struct TimedFrame {
	cv::Mat frame;
	double time;

	bool operator==(TimedFrame const & other) const {
		if (this == &other) return true;
		else return (cv::countNonZero(frame != other.frame) == 0) && (time == other.time);
	}
};

// forward-declare
struct TimedVideo;

struct TimedVideoIterator :
		public boost::iterator_facade<
			TimedVideoIterator,
			TimedFrame const,
			boost::incrementable_traversal_tag
		> {
	public:
		TimedVideoIterator() :
			impl() {}
		explicit TimedVideoIterator(TimedVideo * const impl) :
				impl(impl), cur_frame() {
			this->increment();
		}
	private:
		friend class boost::iterator_core_access;
		void increment();
		TimedFrame const & dereference() const {
			return cur_frame;
		}
		bool equal(TimedVideoIterator const & other) const {
			return (impl == other.impl) && (cur_frame == other.cur_frame);
		}

		TimedVideo * impl;
		TimedFrame cur_frame;
};


struct TimedVideo {


public:
	TimedVideo(std::string const & video_file, std::string const & time_file);

	typedef TimedVideoIterator iterator;

	iterator begin() { return iterator(this); }
	iterator end() const { return iterator(); }
	boost::optional<TimedFrame> next();

	inline cv::VideoCapture const & get_capture() const { return cap; }

private:
	cv::VideoCapture cap;
	std::vector<double> times;
	std::vector<double>::iterator next_time;

};



}



#endif /* INCLUDE_HARMONIC_CPP_TIMED_VIDEO_HPP_ */
