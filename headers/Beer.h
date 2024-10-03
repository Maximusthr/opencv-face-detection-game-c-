#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

#pragma once
class game_object {
    public:
        int x, y;
        std::string path;
        Mat beer = imread(path, IMREAD_UNCHANGED);
        Beer();
        Beer(std::string path);    
};
