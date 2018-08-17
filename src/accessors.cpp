/*
 * accessors.cpp
 *
 *  Created on: Jul 27, 2018
 *      Author: reubena
 */

#include "harmonic_cpp/accessors.hpp"

#include <boost/regex.hpp>
#include <sstream>

namespace harmonic {

DataRun::DataRun() : root_dir() {
	// Default constructor for empty (invalid) runs
}

DataRun::DataRun(boost::filesystem::path const & root_dir, boost::optional<Specifier> const & spec) : root_dir(root_dir), spec(spec) {
	if (!DataRun::check_valid(root_dir)) {
		throw std::runtime_error("Supplied path " + root_dir.native() + " was not a valid HARMONIC data directory.");
	}
}

bool DataRun::check_valid(boost::filesystem::path const & root_dir) {
	// TODO: probably something more sophisticated?
	return boost::filesystem::is_regular_file(root_dir / DIR_STATS / "run_info.yaml");
}


std::ostream & operator<<(std::ostream & ostr, DataRun const & run) {
	ostr << "[";
	if (run.spec) {
		ostr << "p" << run.spec->part_id << ":" << run.spec->run_id << " (" << get_path_from_tag(run.spec->tag).native() << ")";
	} else {
		ostr << "DataRun";
	}
	ostr << ": " << run.root_path() << "]";
	return ostr;
}

Specifier::Tag::value Specifier::Tag::parse(const std::string & val) {
	if (val == TAG_RUN) {
		return RUN;
	} else if (val == TAG_CHECK) {
		return CHECK;
	} else if (val == TAG_CALIB) {
		return CALIB;
	} else {
		throw std::runtime_error("Unknown tag: " + val);
	}
}

HarmonicDataset::HarmonicDataset(std::string const & root_path) :
			root_path(root_path),
			all_data() {
	boost::regex const part_reg("p(\\d+)");
	boost::regex const tag_reg("(calib|check|run)");
	boost::regex const run_reg("(\\d+)");

	Specifier spec;

	namespace fs = boost::filesystem;
	// Part ID
	std::for_each(
			fs::directory_iterator(this->root_path),
			fs::directory_iterator(),
			[&] (fs::path const & part_path) {

		boost::smatch part_match;
		if (boost::regex_match(part_path.filename().native(), part_match, part_reg)) {
			spec.part_id = Specifier::PartId::parse(part_match[1]);

			// Tag
			std::for_each(
					fs::directory_iterator(part_path),
					fs::directory_iterator(),
					[&] (fs::path const & tag_path) {

				boost::smatch tag_match;
				if (boost::regex_match(tag_path.filename().native(), tag_match, tag_reg)) {
					spec.tag = Specifier::Tag::parse(tag_match[1]);

					std::for_each(
							fs::directory_iterator(tag_path),
							fs::directory_iterator(),
							[&] (fs::path const & base_path) {

						boost::smatch run_match;
						if (boost::regex_match(base_path.filename().native(), run_match, run_reg)) {
							spec.run_id = Specifier::RunId::parse(run_match[1]);

							if (DataRun::check_valid(base_path)) {
								this->all_data.push_back(spec);
							}
						}

					});

				}

			});
		}

	});
}


DataRun HarmonicDataset::Lookup::lookup(boost::filesystem::path const & root_path, Specifier const & specifier) {
	std::stringstream str;
	str << "p" << specifier.part_id;
	boost::filesystem::path const part_path(str.str());

	str.str("");
	str << std::setfill('0') << std::setw(3) << specifier.run_id;

	return DataRun(
			root_path / part_path
				/ get_path_from_tag(specifier.tag)
				/ str.str(),
			boost::optional<Specifier>(specifier)
			);

}

} // namespace harmonic
