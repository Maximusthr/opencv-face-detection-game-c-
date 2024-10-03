#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

#pragma once
class Game_object {
    public:
        int x, y;
        std::string path;
        Mat Game_object = imread(path, IMREAD_UNCHANGED);
        Rect rect;
        Game_object();
        Game_object(std::string path);    
};
