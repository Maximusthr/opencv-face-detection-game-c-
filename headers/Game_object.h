#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

class Game_object {
    public:
        cv::Mat image;
        Game_object();
        void load(std::string filename);
        void draw(cv::Mat frame, int x, int y);
        int getWidth();
        int getHeight();
        
        // int x, y;
        // std::string path;
        // Mat Game_object = imread(path, IMREAD_UNCHANGED);
        // Rect rect;
        // Game_object();
        // Game_object(std::string path);    
};
