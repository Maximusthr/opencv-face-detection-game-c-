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
           std::string beerFilename, std::string derzasFilename, std::string waterFilename, bool flip)
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
    water.load(waterFilename);
    derzas.load(derzasFilename);
    cv::resize(beer.image, beer.image, cv::Size(80, 80));
    cv::resize(water.image, water.image, cv::Size(80, 80));
    cv::resize(derzas.image, derzas.image, cv::Size(150, 150));
}


void Game::run() {

    this->score = 0;
    this->startTime = std::chrono::steady_clock::now();
    this->timeLimit = 60;

    int fx = rand() % (640 - beer.getWidth());
    int fy = rand() % (480 - beer.getHeight());
    
    int gx = rand() % (640 - water.getWidth());
    int gy = rand() % (480 - water.getHeight());  


    while (true) {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Error: Blank frame grabbed" << std::endl;
            break;
        }
        
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;
        double remainingTime = timeLimit - elapsedTime.count();

        // std::cout << "timeLimit: " << timeLimit << std::endl;
        // std::cout << "Elapsed Time: " << elapsedTime.count() << std::endl;
        // std::cout << "Remaining Time: " << remainingTime << std::endl;

        if (remainingTime <= 0) {
            std::cout << "Tempo esgotado! Sua pontuação: " << score << std::endl;
            break;
        }


        if (flip) cv::flip(frame, frame, 1);

        cv::Mat gray_frame;
        cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
        background.copyTo(frame);

        //cerveja
        beer.draw(frame, fx, fy);
        cv::Rect beerRect(fx, fy, beer.getWidth(), beer.getHeight());

        //agua
        water.draw(frame, gx, gy);
        cv::Rect waterRect(gx, gy, water.getWidth(), water.getHeight());

        //derzas
        derzas.draw(frame, x, y);
        cv::Rect derzasRect(x, y, derzas.getWidth(), derzas.getHeight());
        
        //rosto
        std::vector<cv::Rect> faces;
        faceDetector.detect(gray_frame, faces);
        
        // if (faces.empty()) {
        //     std::cout << "Nenhum rosto detectado." << std::endl;
        // } else {
        //     std::cout << "Rostos detectados: " << faces.size() << std::endl;
        // }

        for (const auto& face  : faces) {
            cv::rectangle(frame, face, cv::Scalar(255, 0, 0), 2);
            
            if (!(face.y + derzas.getHeight() >= 480 || face.y <= 0) && 
                !(face.x + derzas.getWidth() >= 640 || face.x <= 0)) {
                x = face.x; y = face.y;
            }


            if ((derzasRect & beerRect).area() > 0) {
                std::cout << "Colidiu" << std::endl;
                rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
                system("mpg123 -f 500 bebida.mp3 &");
                fx = rand() % (frame.cols - beer.getWidth());
                fy = rand() % (frame.rows - beer.getHeight());
                score++;
            } 

            if ((derzasRect & waterRect).area() > 0) {
                std::cout << "Colidiu agua" << std::endl;
                rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
                system("mpg123 -f 500 agua.mp3 &");
                gx = rand() % (frame.cols - water.getWidth());
                gy = rand() % (frame.rows - water.getHeight());
                score -= 3;
            }

            if ( gx < face.x  && gx < (680 - water.getWidth())) gx += 1;
            if ( gy < face.y && gy < (480 - water.getHeight()) ) gy += 1;
            if ( gx > face.x && gx > 1) gx -= 1;
            if ( gy > face.y && gy > 1) gy -= 1;
           
        }

        std::string scoreText = "Score: " + std::to_string(score);
        cv::putText(frame, scoreText, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
        std::string timeText = "Tempo: " + std::to_string(static_cast<int>(remainingTime));
        cv::putText(frame, timeText, cv::Point(460, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);

        cv::imshow(windowName, frame);
        if (cv::waitKey(1) >= 0) {
            break;
        }
    }
}
