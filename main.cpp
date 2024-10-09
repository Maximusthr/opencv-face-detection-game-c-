#include "./headers/Game.h"

int main() {
    std::string windowName = "Derzu Adventures";
    std::string cascadeName = "haarcascade_frontalface_default.xml";
    std::string backgroundFilename = "background4.png";
    std::string beerFilename = "cerveja_desenho.png";
    std::string waterFilename = "water.png";
    std::string derzasFilename = "derzas.png";

    bool flip = true;

    srand(time(0));

    Game game(windowName, cascadeName, backgroundFilename, beerFilename, derzasFilename, waterFilename, flip);
    game.run();

    return 0;
}
