#include "pch.h"
#include "globalWindow.h"


int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	if (argc > 1) {
		std::string f = '*' + std::string(argv[1]) + '*';
		for (size_t i = 2; i < argc; ++i)
			f += ":*" + std::string(argv[i]) + '*';
		::testing::GTEST_FLAG(filter) = f;
	}
	int res = RUN_ALL_TESTS();
	globalWindow->setActive(true);
	globalWindow->close();
	globalWindow->setActive(false);
	return res;
}
