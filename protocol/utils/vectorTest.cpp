/*
 * vectorTest.cpp
 *
 *  Created on: Aug 1, 2015
 *      Author: sdai
 */

#include "vector.h"
#include <iostream>
int main() {
	frc5190::vector<int> vec;
	for(int i=0;i<3;i++) {
		vec.prepend(i);
		for(int* i=vec.begin();i!= vec.end();i++) {
				std::cout <<*i << "." << std::endl;
		}
		std::cout << "size:" << vec.getSize() << std::endl;
	}
	std::cout << "insert" << std::endl;
	int insert = 1;
	vec.insert(insert, 1);
	for(int* i=vec.begin();i!= vec.end();i++) {
		std::cout <<*i << "." << std::endl;
	}
	std::cout << "removal" << std::endl;
	vec.remove(0);
	for(int* i=vec.begin();i!= vec.end();i++) {
		std::cout <<*i << "." << std::endl;
	}

	std::cout << "cpy ctor" << std::endl;
	frc5190::vector<int> v = vec;
	for(int* i=v.begin();i!= v.end();i++) {
		std::cout <<*i << "." << std::endl;
	}

	std::cout << "assignment" << std::endl;
	v = vec;
	for(int* i=vec.begin();i!= vec.end();i++) {
		std::cout <<*i << "." << std::endl;
	}
}

