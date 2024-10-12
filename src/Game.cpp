#include "../headers/Game.h"
#include <iostream>
#include <cstdlib>
#include <vector>

Game::Game(std::string windowName, std::string cascadeName, std::string backgroundFilename, 
           std::string beerFilename, std::string caipirinhaFilename, std::string derzasFilename, std::string waterFilename, 
           std::string whiskeyFilename, std::string wineFilename, bool flip)
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
    cv::resize(beer.image, beer.image, cv::Size(50, 50));
    cv::resize(water.image, water.image, cv::Size(50, 50));
    cv::resize(derzas.image, derzas.image, cv::Size(100, 130));

    imgs.push_back(wineFilename);
    imgs.push_back(whiskeyFilename);
    imgs.push_back(caipirinhaFilename);
    imgs.push_back(beerFilename);
}

void Game::showMenu() {
    while (true) {
        frame = cv::Mat::zeros(480, 640, CV_8UC3);

        // Exibe o título do menu
        cv::putText(frame, "Menu Principal", cv::Point(80, 200), 
                    cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(255, 255, 255), 2);
        cv::putText(frame, "1 - Iniciar Jogo", cv::Point(80, 300), 
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
        cv::putText(frame, "2 - Ranking", cv::Point(80, 350), 
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
        cv::putText(frame, "3 - Sair", cv::Point(80, 400), 
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);

        cv::imshow(windowName, frame);

        int key = cv::waitKey(0);
        if (key == '1') {
            run();
            break;
        } else if (key == '2') {
            showRank();
            break;
        } else if (key == '3') {
            cv::destroyWindow(windowName);
            exit(0);
        }
    }
}

void Game::showScore() {
    while (true) {
        frame = cv::Mat::zeros(480, 640, CV_8UC3);

        // Exibe o score final
        std::string scoreText = "Pontuacao Final: " + std::to_string(score);
        cv::putText(frame, scoreText, cv::Point(80, 200), 
                    cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(255, 255, 255), 2);

        cv::putText(frame, "1 - Jogar Novamente", cv::Point(80, 300), 
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
        cv::putText(frame, "2 - Ranking", cv::Point(80, 350), 
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
        cv::putText(frame, "3 - Sair", cv::Point(80, 400), 
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);

        cv::imshow(windowName, frame);

        score = 0;

        int key = cv::waitKey(0);
        if (key == '1') {
            run();
            break;
        } else if (key == '2') {
            showRank();
            break;
        } else if (key == '3') {
            cv::destroyWindow(windowName);
            exit(0);
        }
    }
}

void Game::showRank() {
    int auxY = 150;
    while (true) {
        frame = cv::Mat::zeros(480, 640, CV_8UC3);

        cv::putText(frame, "1 - Voltar", cv::Point(80, 100), 
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
        

        
        for(int i = 0; i < scores.size(); i++) {
            cv::putText(frame, std::to_string(i+1) + ": " + std::to_string(scores[i]), cv::Point(80, auxY), 
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
            auxY += 50;
        }  

        cv::imshow(windowName, frame);

        int key = cv::waitKey(0);
        if (key == '1') {
            showMenu();
            break;
        }
    }
}

void Game::run() {

    this->score = 0;
    this->startTime = std::chrono::steady_clock::now();
    this->timeLimit = 10;

    int fx = rand() % (640 - beer.getWidth());
    int fy = rand() % (480 - beer.getHeight());

    int gx = rand() % (640 - water.getWidth());
    int gy = rand() % (480 - water.getHeight());  

    while (true) {
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Error: Blank frame grabbed" << std::endl;
            break;
        }

        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedTime = currentTime - startTime;
        double remainingTime = timeLimit - elapsedTime.count();

        if (remainingTime <= 0) {
            scores.push_back(score);
            std::sort(scores.rbegin(), scores.rend());
            std::cout << "Tempo esgotado! Sua pontuação: " << score << std::endl;
            showScore();  // Chama a função de exibição do score quando o tempo acaba
            break;
        }

        if (flip) cv::flip(frame, frame, 1);

        cv::Mat gray_frame;
        cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
        background.copyTo(frame);

        // cerveja
        beer.draw(frame, fx, fy);
        cv::Rect beerRect(fx, fy, beer.getWidth(), beer.getHeight());

        // agua
        water.draw(frame, gx, gy);
        cv::Rect waterRect(gx, gy, water.getWidth(), water.getHeight());

        // derzas
        derzas.draw(frame, x, y);
        cv::Rect derzasRect(x, y, derzas.getWidth(), derzas.getHeight());

        // rosto
        std::vector<cv::Rect> faces;
        faceDetector.detect(gray_frame, faces);

        for (const auto& face : faces) {
            cv::rectangle(frame, face, cv::Scalar(255, 0, 0), 2);

            if (!(face.y + derzas.getHeight() >= 480 || face.y <= 0) && 
                !(face.x + derzas.getWidth() >= 640 || face.x <= 0)) {
                x = face.x; y = face.y;
            }

            if ((derzasRect & beerRect).area() > 0) {
                std::cout << "Colidiu" << std::endl;
                static int i = 0;
                if (i+1 < imgs.size()) {
                    i++;
                    beer.load(imgs[i]);
                    cv::resize(beer.image, beer.image, cv::Size(50, 50));
                } else {
                    i = 0;
                    beer.load(imgs[i]);
                    cv::resize(beer.image, beer.image, cv::Size(50, 50));
                }
                rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
                fx = rand() % (frame.cols - beer.getWidth());
                fy = rand() % (frame.rows - beer.getHeight());
                if (i == 3) {
                    system("mpg123 -f 500 ./audios/drink2.mp3 &");
                    score += 4;
                } else {
                    system("mpg123 -f 500 ./audios/drink.mp3 &");
                    score++;
                }
            }

            if ((derzasRect & waterRect).area() > 0) {
                std::cout << "Colidiu agua" << std::endl;
                rectangle(frame, face, cv::Scalar(0, 0, 255), 2);
                system("mpg123 -f 500 ./audios/damage.mp3 &");
                gx = rand() % (frame.cols - water.getWidth());
                gy = rand() % (frame.rows - water.getHeight());
                score -= 3;
            }

            if (gx < face.x && gx < (680 - water.getWidth())) gx += 1;
            if (gy < face.y && gy < (480 - water.getHeight())) gy += 1;
            if (gx > face.x && gx > 1) gx -= 1;
            if (gy > face.y && gy > 1) gy -= 1;

            break;
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
