#include <gtest/gtest.h>

#include <string>

TEST(Equal, CStrEqual) {
	EXPECT_STREQ("123", "123");
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}