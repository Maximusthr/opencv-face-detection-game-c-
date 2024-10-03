#include "../headers/game..h"

game::Game(){};
Game(cv::CascadeClassifier cascade, cv::Mat frame, cv::Mat gray_frame,
     cv::Mat smallFrame, cv::Mat background, game_object derzas, game_object beer;
) {
    this->cascade = cascade;
    this->frame = frame;
    this->gray_frame = gray_frame;
    this->smallFrame = smallFrame;
    this->background = background;
    this->derzas = derzas;
    this->beer = beer;
}
