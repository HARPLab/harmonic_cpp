/*
 * utils.cpp
 *
 *  Created on: Aug 3, 2018
 *      Author: reubena
 */

#include "harmonic_cpp/utils.hpp"

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace harmonic {

std::vector<std::string> parse_csv_line(std::string const & line) {
	std::string const trim_line = boost::algorithm::trim_copy(line);
	boost::tokenizer<boost::escaped_list_separator<char> > tk(
			trim_line,
			boost::escaped_list_separator<char>('\\', ',', '\"'));
	return std::vector<std::string>(tk.begin(), tk.end());
}

} // namespace harmonic


