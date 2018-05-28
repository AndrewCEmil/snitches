#./display_image WellIcon.png
./make.sh
#./display_image ./movie0.mov ./movie1.mov ./movie0.mov
#./dft WellIcon.png FT.png
./dft_video movie0.mov frames

/opt/ImageMagick/bin/convert -loop 0 frames/*.jpeg output.gif
