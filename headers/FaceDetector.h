#pragma once

#include "opencv2/objdetect.hpp"
#include <string>
#include <vector>

class FaceDetector {
    public:
        cv::CascadeClassifier cascade;

        FaceDetector(const std::string cascadeName);
        void detect(const cv::Mat grayFrame, std::vector<cv::Rect> &faces);
};
