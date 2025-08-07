#include "senbonzakura/file_writer.hpp"

#include <string>
#include <vector>

#include <gtest/gtest.h>

TEST(FileWriterTest, FileWriterConstructorTest) {
  std::vector<std::string> programs_filepaths{
      "/some/path/to/file_1.eta", "/this/is/another/path/to/a/file_2.eta",
      "/a/third/path/to/yet/another/file_3.eta"};
  std::string output_directory{"/path/to/the/output/directory/"};
  FileWriter file_writer{programs_filepaths, output_directory};

  EXPECT_EQ(file_writer.GetProgramFilepaths(), programs_filepaths);
  EXPECT_EQ(file_writer.GetOutputDirectory(), output_directory);
}
