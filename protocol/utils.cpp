/*
 * utils.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: sdai
 */

#include "utils.hh"
#include <vector>
binaryFile::binaryFile() {

}

binaryFile::binaryFile(std::string& fn) {
	this->filename = fn;
}

binaryFile::binaryFile(std::string&& fn) {
	this->filename = fn;
}

void binaryFile::write(const unsigned char* byte, int size) {
	std::ofstream of(filename,std::ofstream::out | std::ofstream::binary);
	of.write((const char*) byte, size);
	of.flush();
	of.close();
}

unsigned char* binaryFile::read() {
	std::ifstream input(filename,std::ifstream::in | std::ifstream::binary);
	std::vector<unsigned char> buffer;
	while(true) {
		unsigned char buf;
		input >> std::noskipws >> buf;
		buffer.push_back(buf);
		if(input.eof()) {
			break;
		}
	}
	unsigned char* result = new unsigned char[buffer.size()];
	for(int i=0;i<buffer.size();++i) {
		result[i] = buffer[i];
	}
	return result;
}

std::string binaryFile::getFilename() const {
	return filename;
}

void binaryFile::setFilename(std::string& filename) {
	this->filename = filename;
}



