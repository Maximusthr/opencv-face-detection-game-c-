#include "../headers/FaceDetector.h"
#include <iostream>

FaceDetector::FaceDetector(std::string cascadeName){
    if (!cascade.load(cascadeName)){
        std::cerr << "Error: could not load cascade: " << cascadeName << std::endl;
        exit(-1);
    }


}

void FaceDetector::detect(cv::Mat grayFrame, std::vector<cv::Rect> &faces) {
    cascade.detectMultiScale(grayFrame, faces, 1.1, 4, 0, cv::Size(30, 30));
}