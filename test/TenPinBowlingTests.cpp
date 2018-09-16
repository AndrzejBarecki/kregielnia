#include <gtest/gtest.h>
#include "InputFile.hpp"
#include "TenPinBowling.hpp"
#include <string>
#include <numeric>

struct TenPinBowlingTests : public ::testing::Test
{
};

int sumVector(std::vector<int> const &vec)
{
    return std::accumulate(vec.begin(), vec.end(), 0);
}

TEST_F(TenPinBowlingTests, assertThatTravisCiBuildStatusIsPass)
{
    ASSERT_TRUE(true);
}

TEST_F(TenPinBowlingTests, assertThatLoadInputFileFunctionReadAllFiles)
{
    const std::string inputDirectoryPath = "test_files";
    const fs::path outputFilePath = "result.txt";
    int inFileCntr = 0;

    TenPinBowling testgame{"My Bowling Game!", {}, inputDirectoryPath, outputFilePath};
    // GIVEN
    InputFile::Remove(inputDirectoryPath);
    ASSERT_FALSE(fs::is_directory(inputDirectoryPath));
    ASSERT_TRUE(InputFile::Create(inputDirectoryPath));
    ASSERT_TRUE(fs::is_directory(inputDirectoryPath));
    ASSERT_FALSE(fs::is_empty(inputDirectoryPath));

    for(auto&p: fs::directory_iterator(inputDirectoryPath)) {
        inFileCntr++;
    }
    // WHEN
    ASSERT_TRUE(testgame.loadInputFiles());
    // THEN
    ASSERT_TRUE(inFileCntr == testgame.gamesCounter());
}

TEST_F(TenPinBowlingTests, assertSingleNumberConversion)
{
    TenPinBowling testgame{"My Bowling Game!", {}, "", ""};

    EXPECT_EQ(sumVector(testgame.getPlayerFrame("0")), 0);
    EXPECT_EQ(sumVector(testgame.getPlayerFrame("2")), 2);
    EXPECT_EQ(sumVector(testgame.getPlayerFrame("3")), 3);
    EXPECT_EQ(sumVector(testgame.getPlayerFrame("8")), 8);
}
TEST_F(TenPinBowlingTests, assertSingleMarkConversion)
{
    TenPinBowling testgame{"My Bowling Game!", {}, "", ""};

    EXPECT_EQ(sumVector(testgame.getPlayerFrame("X")), 10);
    EXPECT_EQ(sumVector(testgame.getPlayerFrame("-")), 0);
}
TEST_F(TenPinBowlingTests, assertSpareConversion)
{
    TenPinBowling testgame{"My Bowling Game!", {}, "", ""};

    EXPECT_EQ(sumVector(testgame.getPlayerFrame("3/")), 10);
}
TEST_F(TenPinBowlingTests, assertExampleConversion)
{
    TenPinBowling testgame{"My Bowling Game!", {}, "", ""};

    EXPECT_EQ(sumVector(testgame.getPlayerFrame("3-|X|4/|5")), 28);
    EXPECT_EQ(sumVector(testgame.getPlayerFrame("X|4-|3")), 17);
    EXPECT_EQ(sumVector(testgame.getPlayerFrame("34|X|0-")), 17);
}
TEST_F(TenPinBowlingTests, assertCorrectNameExtraction)
{
    TenPinBowling testgame{"My Bowling Game!", {}, "", ""};

    EXPECT_EQ(testgame.getPlayerName("Name1:X|4-|3"), "Name1");
    EXPECT_EQ(testgame.getPlayerName(":X|22|33"), "");
    EXPECT_EQ(testgame.getPlayerName(":"), "");
    EXPECT_EQ(testgame.getPlayerName(""), "");
}
TEST_F(TenPinBowlingTests, assertCorrectPointsExtractionFromGameDescription)
{
    TenPinBowling testgame{"My Bowling Game!", {}, "", ""};

    EXPECT_EQ(sumVector(testgame.getPlayerFrame("Name1:X|4-|3")), 17);
    EXPECT_EQ(sumVector(testgame.getPlayerFrame(":X|22|33")), 20);
    EXPECT_EQ(sumVector(testgame.getPlayerFrame(":")), 0);
    EXPECT_EQ(sumVector(testgame.getPlayerFrame("")), 0);
    EXPECT_EQ(sumVector(testgame.getPlayerFrame("N:1a-m|e/1:X|4-|3")), 17);
}

TEST_F(TenPinBowlingTests, assertCorrectGameStatusForOneLine)
{
    TenPinBowling testgame{"My Bowling Game!", {}, "", ""};

    EXPECT_EQ(testgame.getPlayerStatus(""), Status::NotStarted);
    EXPECT_EQ(testgame.getPlayerStatus(":"), Status::InProgress);
    EXPECT_EQ(testgame.getPlayerStatus(":1"), Status::InProgress);
    EXPECT_EQ(testgame.getPlayerStatus(":X|22|33"), Status::InProgress);
    EXPECT_EQ(testgame.getPlayerStatus("Name1:X|4-|3"), Status::InProgress);
    EXPECT_EQ(testgame.getPlayerStatus("Name2:00|00|00|00|00|00|00|00|00|00"), Status::Finished);
    EXPECT_EQ(testgame.getPlayerStatus("Name2:00|00|00|00|00|00|00|00|00|0"), Status::InProgress);
    EXPECT_EQ(testgame.getPlayerStatus("Name2:00|00|00|00|00|00|00|00|00|55"), Status::InProgress);
    EXPECT_EQ(testgame.getPlayerStatus("Name2:00|00|00|00|00|00|00|00|00|54"), Status::Finished);
    EXPECT_EQ(testgame.getPlayerStatus("Name2:00|00|00|00|00|00|00|00|00|X"), Status::InProgress);
    EXPECT_EQ(testgame.getPlayerStatus("Name2:00|00|00|00|00|00|00|00|00|55||"), Status::InProgress);
    EXPECT_EQ(testgame.getPlayerStatus("Name2:00|00|00|00|00|00|00|00|00|55||5"), Status::Finished);
    EXPECT_EQ(testgame.getPlayerStatus("Name2:00|00|00|00|00|00|00|00|00|55||55"), Status::Finished);
}

TEST_F(TenPinBowlingTests, assertCorrectNameExtractionIfContainsColon)
{
    TenPinBowling testgame{"My Bowling Game!", {}, "", ""};

    EXPECT_EQ(testgame.getPlayerName("Name:1:X|4-|3"), "Name:1");
    EXPECT_EQ(testgame.getPlayerName("N:1a-m|e/1:X|4-|3"), "N:1a-m|e/1");
}

TEST_F(TenPinBowlingTests, assertValidPlayer)
{
    TenPinBowling testgame{"My Bowling Game!", {}, "", ""};
    ASSERT_FALSE(testgame.isValidPlayer(""));
    ASSERT_TRUE(testgame.isValidPlayer(":"));
    ASSERT_TRUE(testgame.isValidPlayer("name:"));
    ASSERT_FALSE(testgame.isValidPlayer("na$e:"));
    ASSERT_TRUE(testgame.isValidPlayer("name:-"));
    ASSERT_TRUE(testgame.isValidPlayer("name:X"));
    ASSERT_TRUE(testgame.isValidPlayer("name:/"));
    ASSERT_TRUE(testgame.isValidPlayer("name:-X"));
    ASSERT_TRUE(testgame.isValidPlayer("name:XX"));
    ASSERT_TRUE(testgame.isValidPlayer("name:/X"));
    ASSERT_FALSE(testgame.isValidPlayer("name:a"));
    ASSERT_TRUE(testgame.isValidPlayer("name:3"));
    ASSERT_TRUE(testgame.isValidPlayer("name:11|"));
    ASSERT_TRUE(testgame.isValidPlayer("name:11|XX"));
    ASSERT_FALSE(testgame.isValidPlayer("name:11|XX||"));
    ASSERT_TRUE(testgame.isValidPlayer("name:11|XX|2-"));
    ASSERT_FALSE(testgame.isValidPlayer("name:11|XX|2-||"));
    ASSERT_TRUE(testgame.isValidPlayer("name:11|XX|33|44|55|66|77|88|99|00||"));
    ASSERT_FALSE(testgame.isValidPlayer("name:11|XX|33|44|55|66|77|88|99|00|||"));
    ASSERT_TRUE(testgame.isValidPlayer("name:11|XX|33|44|55|66|77|88|99|00||X"));
    ASSERT_TRUE(testgame.isValidPlayer("name:11|XX|33|44|55|66|77|88|99|00||X8"));
    ASSERT_FALSE(testgame.isValidPlayer("name:11|XX|33|44|55|66|77|88|99|00||X8X"));
}

TEST_F(TenPinBowlingTests, assertCorrectStatusForEachLane)
{
    std::vector<int> frames = {};
    Player p1("A", frames, 12, Status::InProgress);
    Player p2("A", frames, 12, Status::InProgress);
    Game g1, g2, g3;
    g1.push_back(p1); g1.push_back(p2);
    std::get<3>(p1) = Status::Finished;
    std::get<3>(p2) = Status::Finished;
    g2.push_back(p1); g2.push_back(p2);
    std::get<3>(p1) = Status::InProgress;
    std::get<3>(p2) = Status::Finished;
    g3.push_back(p1); g3.push_back(p2);

    EXPECT_EQ(getLaneStatus(g1), "game in progress");
    EXPECT_EQ(getLaneStatus(g2), "game finished");
    EXPECT_EQ(getLaneStatus(g3), "game in progress");
}

TEST_F(TenPinBowlingTests, assertCorrectFileOutput)
{
    const std::string inputDirectoryPath = "../test/test_files/test_files_in";
    const fs::path outputFilePath = "../test/test_files/result.txt";

    TenPinBowling testgame{"My Bowling Game!", {}, inputDirectoryPath, outputFilePath};

    testgame.loadInputFiles();
    testgame.outputResults(false);

    std::string outputString[6];
    outputString[0] = "## Lane 1: game in progress ##";
    outputString[1] = "A 20";
    outputString[2] = "B 20";
    outputString[3] = "## Lane 2: game finished ##";
    outputString[4] = outputString[1];
    outputString[5] = outputString[2];


    std::ifstream fileStream(outputFilePath);
    int it = 0;
    for (std::string singleLine; std::getline(fileStream, singleLine); ) {
        EXPECT_EQ(singleLine, outputString[it]);
        it++;
    }
}
