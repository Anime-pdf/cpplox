#include <filesystem>

#include <gtest/gtest.h>
#include <../include/interpreter.h>
#include <fstream>
#include <string>

class InterpreterFileTest :
        public testing::TestWithParam<std::string> {
};

TEST_P(InterpreterFileTest, RunSourceFile) {
    std::string Filename = GetParam();
    std::ifstream File(Filename);
    ASSERT_TRUE(File.is_open()) << "Could not open test file: " << Filename;

    std::string Source((std::istreambuf_iterator<char>(File)),
                       std::istreambuf_iterator<char>());

    std::stringstream Result;

    Lox::CInterpreter::AttachOutStream(&Result);
    Lox::CInterpreter::Run(Source);

    std::string ExpectedFile = Filename + ".expected";
    std::ifstream ExpectedStream(ExpectedFile);
    ASSERT_TRUE(ExpectedStream.is_open()) << "Could not open expected file: " << ExpectedFile;

    std::string Expected((std::istreambuf_iterator<char>(ExpectedStream)),
                         std::istreambuf_iterator<char>());

    EXPECT_EQ(Result.str(), Expected);
}

INSTANTIATE_TEST_SUITE_P(
    SourceFiles,
    InterpreterFileTest,
    ::testing::Values(
        "source/empty.lox",
        "source/parentheses.lox",
        "source/literals.lox"
    )
);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
