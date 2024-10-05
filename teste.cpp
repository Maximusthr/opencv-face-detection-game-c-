#include "./headers/Game.h"

int main() {
    std::string windowName = "Deuzu Adventures";
    std::string cascadeName = "haarcascade_frontalface_default.xml";
    std::string backgroundFilename = "background.png";
    std::string beerFilename = "cerveja_desenho.png";
    std::string derzasFilename = "derzas.png";

    Game game(windowName, cascadeName, backgroundFilename, beerFilename, derzasFilename);
    game.run();

    return 0;
}
