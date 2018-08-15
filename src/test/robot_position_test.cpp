/*
 * robot_position_test.cpp
 *
 *  Created on: Aug 3, 2018
 *      Author: reubena
 */

#include "harmonic_cpp/accessors.hpp"

int main(int argc, char** argv) {
	harmonic::DataRun run("../src/test/data");
	harmonic::RobotPositionData pos = run.robot_position();
	std::cout << std::setprecision(15);
	std::cout << pos.get_start_time() << "-" << pos.get_end_time() << std::endl;
	std::cout << pos.get_start_time() << ":\n\t" << pos.get_position(pos.get_start_time()).data << std::endl;
	auto const mid_time = (pos.get_start_time() + pos.get_end_time())/2.;
	std::cout << mid_time << ":\n\t" << pos.get_position(mid_time).data << std::endl;

}

