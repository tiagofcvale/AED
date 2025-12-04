// imageRGBTest - A program that performs some operations on RGB images.
//
// This program is an example use of the imageRGB module,
// a programming project for the course AED, DETI / UA.PT
//
// You may freely use and modify this code, NO WARRANTY, blah blah,
// as long as you give proper credit to the original and subsequent authors.
//
// The AED Team <jmadeira@ua.pt, jmr@ua.pt, ...>
// 2025

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "imageRGB.h"
#include "instrumentation.h"

int main(int argc, char* argv[]) {
  program_name = argv[0];
  if (argc != 1) {
    error(1, 0, "Usage: imageRGBTest");
  }

  InstrName[0] = "counter";
  InstrCalibrate();

  ImageInit();

  // Creating and displaying some images

  printf("1) ImageCreate\n");
  Image white_image = ImageCreate(100, 100);
  // ImageRAWPrint(white_image);

  printf("2) ImageCreateChess(black)+ ImageSavePBM\n");
  Image image_chess_1 = ImageCreateChess(150, 120, 30, 0x000000);  // black
  // ImageRAWPrint(image_chess_1);
  ImageSavePBM(image_chess_1, "chess_image_1.pbm");

  printf("3) ImageCreateChess(red) + ImageSavePPM\n");
  Image image_chess_2 = ImageCreateChess(20, 20, 8, 0xff0000);  // red
  ImageRAWPrint(image_chess_2);
  ImageSavePPM(image_chess_2, "chess_image_2.ppm");

  printf("4) ImageCreateChess(all black)\n");
  Image black_image = ImageCreateChess(100, 100, 100, 0x000000);  // all black
  // ImageRAWPrint(black_image);
  ImageSavePBM(black_image, "black_image.pbm");

  printf("5) ImageCopy\n");
  Image copy_image = ImageCopy(image_chess_1);
  // ImageRAWPrint(copy_image);
  if (copy_image != NULL) {
    ImageSavePBM(copy_image, "copy_image.pbm");
  }

  printf("6) ImageLoadPBM\n");
  Image image_1 = ImageLoadPBM("img/feep.pbm");
  ImageRAWPrint(image_1);

  printf("7) ImageLoadPPM\n");
  Image image_2 = ImageLoadPPM("img/feep.ppm");
  ImageRAWPrint(image_2);

  printf("8) ImageCreatePalete\n");
  Image image_3 = ImageCreatePalete(4 * 32, 4 * 32, 4);
  ImageSavePPM(image_3, "palete.ppm");

  printf("9) ImageIsEqual\n");
  Image images1[8] = {
    ImageLoadPPM("img/img1_64.ppm"),
    ImageLoadPPM("img/img1_128.ppm"),
    ImageLoadPPM("img/img1_192.ppm"),
    ImageLoadPPM("img/img1_256.ppm"),
    ImageLoadPPM("img/img1_320.ppm"),
    ImageLoadPPM("img/img1_384.ppm"),
    ImageLoadPPM("img/img1_448.ppm"),
    ImageLoadPPM("img/img1_512.ppm"),
  };
  Image images2[8] = {
    ImageLoadPPM("img/img2_64.ppm"),
    ImageLoadPPM("img/img2_128.ppm"),
    ImageLoadPPM("img/img2_192.ppm"),
    ImageLoadPPM("img/img2_256.ppm"),
    ImageLoadPPM("img/img2_320.ppm"),
    ImageLoadPPM("img/img2_384.ppm"),
    ImageLoadPPM("img/img2_448.ppm"),
    ImageLoadPPM("img/img2_512.ppm"),
  };
  for (int i = 0; i < 8; i++) {
	  printf("%d, ", i);
	  if (ImageIsEqual(images1[i], images1[i])) {
		  printf(", True\n");
	  } else {
		  printf(", False\n");
	  }
  }
  for (int i = 0; i < 8; i++) {
	  printf("%d, ", i);
	  if (ImageIsEqual(images1[i], images2[i])) {
		  printf(", True\n");
	  } else {
		  printf(", False\n");
	  }
  }
  for (int i = 0; i < 8; i++) {
    ImageDestroy(&(images1[i]));
    ImageDestroy(&(images2[i]));
  }
  printf("10) ImageRotate90CW\n");
  Image original = ImageLoadPPM("img/feep.ppm");
  Image rotated90 = ImageRotate90CW(original);
  printf("IMAGEM ORIGINAL\n");
  ImageRAWPrint(original);
  printf("IMAGEM ROTACIONADA 90 GRAUS\n");
  ImageRAWPrint(rotated90);

  printf("11) ImageRotate180CW\n");
  Image rotated180 = ImageRotate180CW(original);
  printf("IMAGEM ORIGINAL\n");
  ImageRAWPrint(original);
  printf("IMAGEM ROTACIONADA 180 GRAUS\n");
  ImageRAWPrint(rotated180);

  printf("12) ImageRegionFillingRecursive\n");
  Image image_4 = ImageLoadPBM("img/maze.pbm");
  InstrReset();
  ImageSegmentation(image_4, ImageRegionFillingRecursive);
  InstrPrint();
  ImageSavePPM(image_4, "maze_out1.ppm");
  ImageDestroy(&image_4);

  printf("13) ImageRegionFillingWithSTACK\n");
  image_4 = ImageLoadPBM("img/maze.pbm");
  InstrReset();
  ImageSegmentation(image_4, ImageRegionFillingWithSTACK);
  InstrPrint();
  ImageSavePPM(image_4, "maze_out2.ppm");
  ImageDestroy(&image_4);

  printf("14) ImageRegionFillingWithQUEUE\n");
  image_4 = ImageLoadPBM("img/maze.pbm");
  InstrReset();
  ImageSegmentation(image_4, ImageRegionFillingWithQUEUE);
  InstrPrint();
  ImageSavePPM(image_4, "maze_out3.ppm");
  ImageDestroy(&image_4);

  ImageDestroy(&white_image);
  ImageDestroy(&black_image);
  if (copy_image != NULL) {
    ImageDestroy(&copy_image);
  }
  ImageDestroy(&image_chess_1);
  ImageDestroy(&image_chess_2);
  ImageDestroy(&image_1);
  ImageDestroy(&image_2);
  ImageDestroy(&image_3);
  ImageDestroy(&original);
  ImageDestroy(&rotated90);
  ImageDestroy(&rotated180);

  return 0;
}
