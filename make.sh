g++ $(pkg-config --cflags --libs opencv) display_image.cpp -o display_image
g++ $(pkg-config --cflags --libs opencv) dft.cpp -o dft

