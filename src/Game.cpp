#include "../headers/Game.h"
#include <iostream>
#include <cstdlib>
#include <vector>

// game::Game(){};
// Game(cv::CascadeClassifier cascade, cv::Mat frame, cv::Mat gray_frame,
//      cv::Mat smallFrame, cv::Mat background, game_object derzas, game_object beer;
// ) {
//     this->cascade = cascade;
//     this->frame = frame;
//     this->gray_frame = gray_frame;
//     this->smallFrame = smallFrame;
//     this->background = background;
//     this->derzas = derzas;
//     this->beer = beer;
// }


Game::Game(std::string windowName, std::string cascadeName, std::string backgroundFilename, 
           std::string beerFilename, std::string derzasFilename)
    : faceDetector(cascadeName), x(640/2), y(480/2), cap(0) { // Inicializa x e y no centro da tela

    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open the camera" << std::endl;
        exit(-1);
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS, 30);

    background = cv::imread(backgroundFilename);
    if (background.empty()) {
        std::cerr << "Error: Could not load background" << std::endl;
        exit(-1);
    }
    // if (background.channels() == 4){
    //     cvtColor(background, background, COLOR_BGRA2BGR);
    // }
    cv::resize(background, background, cv::Size(640, 480));

    beer.load(beerFilename);
    derzas.load(derzasFilename);
    cv::resize(beer.image, beer.image, cv::Size(80, 80));
    cv::resize(derzas.image, derzas.image, cv::Size(150, 150));
}


void Game::run() {
    int fx = rand() % (640 - beer.getWidth());
    int fy = rand() % (480 - beer.getHeight());

    while (true) {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Error: Blank frame grabbed" << std::endl;
            break;
        }

        cv::Mat gray_frame;
        cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
        background.copyTo(frame);

        //cerveja
        beer.draw(frame, fx, fy);
        cv::Rect beerRect(fx, fy, beer.getWidth(), beer.getHeight());

        //derzas
        derzas.draw(frame, x, y);
        
        //rosto
        std::vector<cv::Rect> faces;
        faceDetector.detect(gray_frame, faces);
        
        if (faces.empty()) {
            std::cout << "Nenhum rosto detectado." << std::endl;
        } else {
            std::cout << "Rostos detectados: " << faces.size() << std::endl;
        }

        for (const auto& face  : faces) {
            cv::rectangle(frame, face, cv::Scalar(255, 0, 0), 2);
            
            if (!(face.y + derzas.getHeight() >= 480 || face.y <= 0) && 
                !(face.x + derzas.getWidth() >= 640 || face.x <= 0)) {
                x = face.x; y = face.y;
            }

            if ((face & beerRect).area() > 0) {
                std::cout << "Colidiu" << std::endl;
                rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
                fx = rand() % (frame.cols - beer.getWidth());
                fy = rand() % (frame.rows - beer.getHeight());
            }
        }

        cv::imshow(windowName, frame);
        if (cv::waitKey(1) >= 0) {
            break;
        }
    }
}