#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include "game_object.h"

#pragma once
class Game {
    public:
        cv::CascadeClassifier cascade;
        cv::Mat frame, gray_frame, smallFrame, background;
        game_object derzas, beer;
     
        Game();
        Game(cv::CascadeClassifier cascade, cv::Mat frame, cv::Mat gray_frame,
             cv::Mat smallFrame, cv::Mat background, game_object derzas, game_object beer;
        )
};
