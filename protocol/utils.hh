/*
 * utils.hh
 *
 *  Created on: Nov 21, 2015
 *      Author: sdai
 */

#ifndef UTILS_HH_
#define UTILS_HH_
#include <fstream>
#include <iostream>
#include <string>
class binaryFile {
public:
	binaryFile(std::string& fn);
	binaryFile();
	void write(const unsigned char* byte, int size);
	unsigned char* read();
	std::string getFilename() const;
	void setFilename(std::string& filename);

private:
	std::string filename;
};



#endif /* UTILS_HH_ */
