#include "pch.h"

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	//if (argc > 1) {
	//	std::string f = '*' + std::string(argv[1]) + '*';
	//	for (int i = 2; i < argc; ++i)
	//		f = f + ":*" + std::string(argv[i]) + '*';
	//	::testing::GTEST_FLAG(filter) = f;
	//}
	int err = RUN_ALL_TESTS();
	return err;
}
