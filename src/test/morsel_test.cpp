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
	harmonic::MorselInfo vid = harmonic::DataRun("../src/test/data").morsel_info();

	std::cout
		<< vid.morsels[0].name << ":\n"
		<< vid.morsels[0].location << "\n"
		<< vid.morsels[1].name << ":\n"
		<< vid.morsels[1].location << "\n"
		<< vid.morsels[2].name << ":\n"
		<< vid.morsels[2].location << "\n"
		<< "stated_goal: " << vid.stated_goal << "\n"
		<< "goal_success: " << vid.goal_success << "\n"
		<< "any_success: " << vid.any_success
		<< std::endl;
}


