#include "TenPinBowling.hpp"

TenPinBowling::TenPinBowling(std::string const & name, std::vector<Game> const & game)
    :name_(name)
    ,games_(game)
{}

TenPinBowling::~TenPinBowling()
{}

bool TenPinBowling::loadInputFiles(std::string const &path)
{
    std::string filePath;
    std::vector<Player> game{};
    std::string playerName{};
    std::vector<int> playerFrames{};
    int score = 0;
    enum Status status{Status::NotStarted};
    for (auto & p : fs::directory_iterator(path)) {
        filePath = p.path();
        std::ifstream fileStream(filePath);
        for (std::string singleLine; std::getline(fileStream, singleLine); ) {
            //TODO::cout only for debug remove in release version!
            std::cout <<singleLine<<std::endl;

            //TODO:
            //1. Implement TenPinBowling::getPlayerName function
            //2. Implement TenPinBowling::getPlayerFrames function
            //3. Implement TenPinBowling::getScore function (this function should also calculate status)
            //4. Implement TenPinBowling::validateGame function

            auto singlePlayer = std::make_tuple(playerName, playerFrames, score, status);
            game.push_back(singlePlayer);
        }
        games_.push_back(game);
    }
    return true;
}

int TenPinBowling::gamesCntr()
{
    return games_.size();
}
