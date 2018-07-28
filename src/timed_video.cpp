/*
 * timed_video.cpp
 *
 *  Created on: Jul 27, 2018
 *      Author: reubena
 */

#include "harmonic_cpp/timed_video.hpp"

#include "cnpy.h"

namespace harmonic {

TimedVideo::TimedVideo(std::string const & video_file, std::string const & time_file) :
	cap(video_file),
	times(cnpy::npy_load(time_file).as_vec<double>()),
	next_time(times.begin()) {
	// empty
}

void TimedVideoIterator::increment() {
	boost::optional<TimedFrame> next_val = this->impl->next();
	if (next_val) {
		this->cur_frame = std::move(*next_val);
	} else {
		this->cur_frame = TimedFrame();
		this->impl = nullptr;
	}
}

boost::optional<TimedFrame> TimedVideo::next() {
	TimedFrame res;
	if (this->cap.read(res.frame)) {
		res.time = *(this->next_time++);
		return boost::optional<TimedFrame>(std::move(res));
	} else {
		return boost::optional<TimedFrame>();
	}
}

} // namespace harmonic


