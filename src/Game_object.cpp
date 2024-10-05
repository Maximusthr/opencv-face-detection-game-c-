#include "../headers/Game_object.h"

Game_object::Game_object() {}
// game_object::game_object(std::string path) {
//     this->path = path;
// }

void Game_object::load(std::string filename){
    image = cv::imread(filename, cv::IMREAD_UNCHANGED);
    if (image.empty()){
        std::cerr << "Error: Could not load image: " << filename << std::endl;
        exit(-1);
    }
}

void Game_object::draw(cv::Mat frame, int x, int y) {
    // Verifica se a imagem cabe dentro do frame
    if (y + image.rows >= frame.rows || x + image.cols >= frame.cols)
        return;

    if (image.channels() == 4) { // Imagem com canal alpha
        std::vector<cv::Mat> layers;
        cv::split(image, layers);
        cv::Mat mask = layers[3]; // Canal alpha
        layers.pop_back(); // Remove o canal alpha
        cv::Mat rgb;
        cv::merge(layers, rgb);
        rgb.copyTo(frame(cv::Rect(x, y, rgb.cols, rgb.rows)), mask);
    } else {
        image.copyTo(frame(cv::Rect(x, y, image.cols, image.rows)));
    }
}

int Game_object::getWidth(){
    return image.cols;
}

int Game_object::getHeight(){
    return image.rows;
}