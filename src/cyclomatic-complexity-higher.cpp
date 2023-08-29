// scatr-check: CXX-R1000
#include <iostream>

// [CXX-R1000]: "`function` has a cyclomatic complexity of 15 with 'medium' risk"
int function(int x) {
	switch (x % 10) {
		case 0:
		case 2:
		case 4:
		case 6:
		case 8:
			std::cout << "even" << std::endl;
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 9:
			std::cout << "odd" << std::endl;
			break;
		default:
			break;
	}
	switch (x % 10) {
		case 0:
		case 2:
		case 4:
		case 6:
		case 8:
			std::cout << "even" << std::endl;
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 9:
			std::cout << "odd" << std::endl;
			break;
		default:
			break;
	}
	switch (x % 10) {
		case 0:
		case 2:
		case 4:
		case 6:
		case 8:
			std::cout << "even" << std::endl;
			break;
		case 1:
		case 3:
		case 5:
		case 7:
		case 9:
			std::cout << "odd" << std::endl;
			break;
		default:
			break;
	}

	if (true || false) {
		std::cout << "true || false" << std::endl;
	}
	for (;;) {}
	while (0) {}
	do {} while(0);
	return -1;
}
