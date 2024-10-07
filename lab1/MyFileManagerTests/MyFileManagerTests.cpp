#include <gtest/gtest.h>
#include "../MyFileManagerLib/MyFileManager.h"

#include <cstring>

TEST(MyFileManagerTest, TestIsSameCD) {
    EXPECT_TRUE(is_same_CD("C:\\Users", "C:\\Program Files"));
    EXPECT_FALSE(is_same_CD("C:\\Users", "D:\\Program Files"));
}

TEST(MyFileManagerTest, TestIsAbsoluteChar) {
    EXPECT_TRUE(is_absolute("C:\\Windows"));
    EXPECT_FALSE(is_absolute("Windows\\System32"));
}

TEST(MyFileManagerTest, TestIsAbsoluteString) {
    EXPECT_TRUE(is_absolute(std::string("C:\\Windows")));
    EXPECT_FALSE(is_absolute(std::string("Windows\\System32")));
}

TEST(MyFileManagerTest, TestIsPathNormalChar) {
    EXPECT_TRUE(is_path_normal("valid_path"));
    EXPECT_FALSE(is_path_normal("invalid#path"));
    EXPECT_FALSE(is_path_normal("path/with/slash"));
}

TEST(MyFileManagerTest, TestIsPathNormalString) {
    EXPECT_TRUE(is_path_normal(std::string("valid_path")));
    EXPECT_FALSE(is_path_normal(std::string("invalid#path")));
}

TEST(MyFileManagerTest, TestJoinChar) {
    char* result = join("C:\\Users", "Documents");
    EXPECT_STREQ(result, "C:\\Users\\Documents");
    delete[] result;
}

TEST(MyFileManagerTest, TestJoinString) {
    std::string result = join(std::string("C:\\Users"), std::string("Documents"));
    EXPECT_EQ(result, "C:\\Users\\Documents");
}

TEST(MyFileManagerTest, TestAbsoluteChar) {
    char* abs_path = absolute("Documents");
    EXPECT_TRUE(is_absolute(abs_path));
    delete[] abs_path;
}

TEST(MyFileManagerTest, TestAbsoluteString) {
    std::string abs_path = absolute(std::string("Documents"));
    EXPECT_TRUE(is_absolute(abs_path));
}

TEST(MyFileManagerTest, TestRelativizeChar) {
    char* rel_path = relativize("C:\\Users\\John", "C:\\Users\\John\\Documents");
    EXPECT_STREQ(rel_path, "\\Documents");
    delete[] rel_path;
}

TEST(MyFileManagerTest, TestRelativizeString) {
    std::string rel_path = relativize(std::string("C:\\Users\\John"), std::string("C:\\Users\\John\\Documents"));
    EXPECT_EQ(rel_path, "Documents");

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}