#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include "opencv2/videoio.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include "Game_object.h"
#include "FaceDetector.h"


class Game {
    public:
        std::string windowName;
        FaceDetector faceDetector;
        cv::Mat background;
        cv::VideoCapture cap;
        Game_object derzas, beer, water;
        std::chrono::time_point<std::chrono::steady_clock> startTime;
        int x, y;
        bool flip;
        int score;
        int timeLimit;

        // Game();
        // Game(cv::CascadeClassifier cascade, cv::Mat frame, cv::Mat gray_frame,
        //      cv::Mat smallFrame, cv::Mat background, game_object derzas, game_object beer;
             
        // )
        Game(std::string windowName, std::string cascadeName, std::string backgroundFilename, std::string beerFilename, std::string derzasFilename, std::string waterFilename, bool flip);
        void run();
};
