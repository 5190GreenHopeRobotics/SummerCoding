/*
 * test.cpp
 *
 *  Created on: Jul 11, 2015
 *      Author: sdai
 */
#include <iostream>
#include "transmission.h"

using namespace std;

int main() {
	transmissionPacket tp;
	commandData cmd;
	cmd.setCommand(3);
	cmd.setParameter(2);
	tp.setData(cmd);

	commandData newData = interpreter::interpretCommandData(tp);
	std::cout << "cmd:" << (short)newData.getCommand() << " param:" << (short)newData.getParameter() << std::endl;
}

