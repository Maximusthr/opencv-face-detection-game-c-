#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace cv;

string cascadeName;
string wName = "Game";

// Função para desenhar uma imagem (img) em um frame (frame), nas posições xPos e yPos
void drawImage(Mat frame, Mat img, int xPos, int yPos) {
    // Verifica se a imagem cabe dentro do frame
    if (yPos + img.rows >= frame.rows || xPos + img.cols >= frame.cols)
        return;

    Mat mask;           // Máscara para transparência
    vector<Mat> layers; // Vetor para armazenar os canais da imagem

    // Separar os canais da imagem (R, G, B, e possivelmente o canal alpha)
    split(img, layers);
    
    // Verifica se a imagem possui canal alpha (transparência)
    if (layers.size() == 4) {
        // Separar os canais RGB
        Mat rgb[3] = { layers[0],layers[1],layers[2] };
        mask = layers[3]; // O canal alpha é usado como máscara
        merge(rgb, 3, img);  // Combinar os canais RGB
        // Copiar a imagem para o frame, usando a máscara para manter a transparência
        img.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols), mask);
    } else {
        // Se não houver canal alpha, apenas copiar a imagem para o frame
        img.copyTo(frame.rowRange(yPos, yPos + img.rows).colRange(xPos, xPos + img.cols));
    }
}

int x=640/2, y=480/2;


int main(int argc, const char** argv){
    srand(time(0));

    CascadeClassifier cascade;
    cascadeName = "haarcascade_frontalface_default.xml";

    if (!cascade.load(cascadeName)){
        cout << "Error: Could not load classifier cascade: " << cascadeName << endl;
        return -1;
    }

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Error: Could not open the camera" << endl;
        return -1;
    }

    // Ajustar resolução e taxa de quadros
    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(CAP_PROP_FPS, 30);

    Mat frame, gray_frame, smallFrame;

    Mat background = imread("background.png");
    if (background.empty()){
        cout << "Error: Could not load derzas" << endl;
        return -1;
    }

    if (background.channels() == 4){
        cvtColor(background, background, COLOR_BGRA2BGR);
    }

    resize(background, background, Size(640, 480));

    Mat beer = imread("cerveja_desenho.png", IMREAD_UNCHANGED);
    if (beer.empty()){
        cout << "Error: Could not load derzas" << endl;
        return -1;
    }

    Mat derzas = imread("derzas.png", IMREAD_UNCHANGED);
    if (derzas.empty()){
        cout << "Error: Could not load derzas" << endl;
        return -1;
    }

    resize(beer, beer, Size(100, 100));
    resize(derzas, derzas, Size(200, 200));
    
    int fx = rand() % (640 - beer.cols);
    int fy = rand() % (480 - beer.rows);
    
    while (true) {
        cap >> frame;  // Capture frame


        if (frame.empty()) {
            cerr << "Error: Blank frame grabbed" << endl;
            break;
        }

        cvtColor(frame, gray_frame, COLOR_BGR2GRAY);

        background.copyTo(frame);

        //drawImage(combined, beer, fx, fy);
        drawImage(frame, beer, fx, fy);


        vector<Rect> faces;
        cascade.detectMultiScale(gray_frame, faces, 1.1, 4); 
        Rect beerRect = Rect(fx, fy, 100, 100);

        drawImage(frame, derzas, x, y);

        for (const auto& face : faces) {
            //rectangle(frame, face, Scalar(255, 0, 0), 2); // Desenhar retângulo em azul
            rectangle(frame, face, Scalar(255, 0, 0), 2);
            //drawImage(frame, derzas, face.x, face.y);
            if (!(face.y+derzas.rows >= 480 || face.y <= 0) && !(face.x+derzas.cols >= 680 || face.x <= 0)) {
                x = face.x; y = face.y;
            }
            
            if ((face & beerRect).area() > 0) {
                cout << "Colidiu" << endl;
                rectangle(frame, face, Scalar(0, 0, 255), 2);
                system("mpg123 -f 500 bebida.mp3 &");
                fx = rand() % (frame.cols - beer.cols);
                fy = rand() % (frame.rows - beer.rows);
            }
        }

        imshow("Deuzu Adventures", frame);

        // Use um valor de delay mínimo no waitKey para não atrasar a captura
        if (waitKey(1) >= 0) {
            break;
        }
    }

    return 0;
}
