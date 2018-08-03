/*
 * timed_video.cpp
 *
 *  Created on: Aug 3, 2018
 *      Author: reubena
 */


#include "harmonic_cpp/accessors.hpp"
#include "opencv2/opencv.hpp"
#include <boost/lexical_cast.hpp>

int main(int argc, char** argv) {
	harmonic::TimedVideo vid = harmonic::DataRun("../src/test/data").pupil_world();
	std::for_each(vid.begin(), vid.end(), [] (harmonic::TimedFrame const & frame) {
		cv::Mat img = frame.frame.clone();
		cv::putText(img, boost::lexical_cast<std::string>(frame.time), cv::Point2i(10, 100), cv::HersheyFonts::FONT_HERSHEY_PLAIN, 1., cv::Scalar(0, 255, 0));
		cv::imshow("Test", img);
		cv::waitKey(1);
	});
	cv::waitKey(-1);
}

