/*
Copyright © 2011-2012 Clint Bellanger

This file is part of FLARE.

FLARE is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

FLARE is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
FLARE.  If not, see http://www.gnu.org/licenses/
*/

#include "FileParser.h"


using namespace std;


FileParser::FileParser() : new_section(false) {
}

bool FileParser::open(const string& filename) {
	infile.open(filename.c_str(), ios::in);
	return infile.is_open();
}

void FileParser::close() {
	if (infile.is_open())
		infile.close();
}

/**
 * Advance to the next key pair
 * Take note if a new section header is encountered
 *
 * @return false if EOF, otherwise true
 */
bool FileParser::next() {

	string starts_with;
	new_section = false;

	while (!infile.eof()) {

		// if infile goes invalid on this read, this will return "" anyway.
		line = getLine(infile);

		// skip ahead if this line is empty
		if (line.length() == 0) continue;

		starts_with = line.at(0);

		// skip ahead if this line is a comment
		if (starts_with == "#") continue;

		// set new section if this line is a section declaration
		if (starts_with == "[") {
			new_section = true;
			section = parse_section_title(line);

			// keep searching for a key-pair
			continue;
		}

		// this is a keypair. Perform basic parsing and return
		parse_key_pair(line, key, val);
		return true;

	}

	// hit the end of file
	return false;
}

/**
 * Get an unparsed, unfiltered line from the input file
 */
string FileParser::getRawLine() {
	if (!infile.eof()) {
		return getLine(infile);
	}
	return "";
}

string FileParser::nextValue() {
	if (val == "") {
		return ""; // not found
	}
	string s;
	size_t seppos = val.find_first_of(',');
	if (seppos == string::npos) {
		s = val;
		val = "";
	}
	else {
		s = val.substr(0, seppos);
		val = val.substr(seppos+1, val.length());
	}
	return s;
}

FileParser::~FileParser() {
	close();
}
