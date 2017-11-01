// main.cpp - run tests of technical analysis library
// Copyright (c) 2011 KALX, LLC. All rights reserved. No warranty is made.
#include <iostream>

void fms_test_technical(void);

int
main()
{
	try {
		fms_test_technical();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;

		return -1;
	}

	return 0;
}