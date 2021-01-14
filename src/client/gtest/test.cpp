#include "pch.h"
#include "main.h"
TEST(TestCaseName1, TestName) {
	EXPECT_EQ(1, 1);
}

TEST(TestCaseName2, TestName) {
	EXPECT_TRUE(true);
}

TEST(TestCaseName3, TestName) {
	EXPECT_EQ(split("asdfasdf", "f")[1], "asd");
}

TEST(TestCaseName4, TestName) {
    VTile v(2, 3);
    EXPECT_EQ(v.x, 2);
    EXPECT_EQ(v.y, 3);
}

TEST(TestCaseName5, TestName) {
	int x = 0;
	WalkClickEvent::subscribe(new EventObserver<WalkClickEvent>(
		[&](WalkClickEvent* ev) {
			++x;
		}
	));
	for (int i = 0; i < 5; ++i)
		WalkClickEvent(VTile()).emit();
	EXPECT_EQ(x, 5);
}

TEST(TestCaseName6, TestName) {
	Item item(7, "Copper Ore", 27, false, 3, 1);
	EXPECT_EQ(item.id, 7);
}
