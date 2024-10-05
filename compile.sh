g++ teste.cpp src/*.cpp `pkg-config --cflags opencv4` `pkg-config --libs --static opencv4` -o teste
./teste