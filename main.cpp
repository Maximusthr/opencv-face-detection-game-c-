#include "./headers/Game.h"

int main() {
    std::string windowName = "Derzu Adventures";
    std::string cascadeName = "haarcascade_frontalface_default.xml";
    std::string backgroundFilename = "./images/background4.png";
    std::string beerFilename = "./images/beer.png";
    std::string caipirinhaFilename = "./images/caipirinha.png";
    std::string derzasFilename = "./images/derzas.png";
    std::string waterFilename = "./images/water.png";
    std::string whiskeyFilename = "./images/whiskey.png";
    std::string wineFilename = "./images/wine.png";

    bool flip = true;

    srand(time(0));

    Game game = Game(windowName, cascadeName, backgroundFilename, beerFilename, caipirinhaFilename, derzasFilename, waterFilename, whiskeyFilename, wineFilename, flip);
    game.run();

    return 0;
}
