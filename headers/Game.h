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
        cv::Mat frame;
        cv::VideoCapture cap;
        Game_object derzas, beer, water;
        std::chrono::time_point<std::chrono::steady_clock> startTime;
        std::vector<std::string> imgs;
        std::vector<int> scores;
        int x, y;
        bool flip;
        int score;
        int timeLimit;

        // Game();
        // Game(cv::CascadeClassifier cascade, cv::Mat frame, cv::Mat gray_frame,
        //      cv::Mat smallFrame, cv::Mat background, game_object derzas, game_object beer;
             
        // )
        Game(std::string windowName, std::string cascadeName, std::string backgroundFilename, std::string beerFilename,
             std::string caipirinhaFilename, std::string derzasFilename, std::string waterFilename, std::string whiskeyFilename, 
             std::string wineFilename, bool flip);

        void run();
        void showMenu();
        void showScore();
        void showRank();
        void loadData();
        void saveData();
};
