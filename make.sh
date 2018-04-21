g++ $(pkg-config --cflags --libs opencv) display_image.cpp -o display_image
g++ $(pkg-config --cflags --libs opencv) dft.cpp -o dft
g++ $(pkg-config --cflags --libs opencv) dft_video.cpp -o dft_video

