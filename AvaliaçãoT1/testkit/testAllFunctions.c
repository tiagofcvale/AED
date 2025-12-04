// TestAllFunctions - A program that uses all developed functions.
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
    error(1, 0, "Usage: TestAllFunctions");
  }

  ImageInit();

  // Creating and displaying some images

  printf("1) ImageCreate\n");
  Image white_image = ImageCreate(100, 100);
  assert(ImageWidth(white_image) == 100);
  assert(ImageHeight(white_image) == 100);
  assert(ImageColors(white_image) == 2);
  // ImageRAWPrint(white_image);

  printf("2) ImageCreateChess(black) + ImageSavePBM\n");
  Image image_chess_1 = ImageCreateChess(150, 120, 30, 0x000000);  // black
  assert(ImageWidth(image_chess_1) == 150);
  assert(ImageHeight(image_chess_1) == 120);
  assert(ImageColors(image_chess_1) == 2);
  // ImageRAWPrint(image_chess_1);
  ImageSavePBM(image_chess_1, "chess_image_1.pbm");

  printf("3) ImageCreateChess(red) + ImageSavePPM\n");
  Image image_chess_2 = ImageCreateChess(20, 20, 8, 0xff0000);  // red
  assert(ImageWidth(image_chess_2) == 20);
  assert(ImageHeight(image_chess_2) == 20);
  assert(ImageColors(image_chess_2) == 3);
  // ImageRAWPrint(image_chess_2);
  ImageSavePPM(image_chess_2, "chess_image_2.ppm");

  printf("4) ImageCreateChess(all black)\n");
  Image black_image = ImageCreateChess(100, 100, 100, 0x000000);  // all black
  assert(ImageWidth(black_image) == 100);
  assert(ImageHeight(black_image) == 100);
  assert(ImageColors(black_image) == 2);
  // ImageRAWPrint(black_image);
  // ImageSavePBM(black_image, "black_image.pbm");

  printf("5) ImageCopy + ImageIsEqual\n");
  Image copy_image = ImageCopy(image_chess_1);
  assert(copy_image != NULL);
  assert(ImageIsEqual(copy_image, image_chess_1));
  // ImageRAWPrint(copy_image);
  // ImageSavePBM(copy_image, "copy_image.pbm");

  printf("6) ImageLoadPBM\n");
  Image image_1 = ImageLoadPBM("img/feep.pbm");
  // ImageRAWPrint(image_1);

  printf("7) ImageLoadPPM\n");
  Image image_2 = ImageLoadPPM("img/feep.ppm");
  // ImageRAWPrint(image_2);

  printf("8) ImageCreatePalete\n");
  Image image_3 = ImageCreatePalete(4 * 32, 4 * 32, 4);
  ImageSavePPM(image_3, "palete.ppm");

  printf("9) ImageIsEqual\n");
  assert(ImageIsEqual(image_chess_1, image_chess_2) == 0);
  assert(ImageIsEqual(image_1, image_2) == 0);
  assert(ImageIsEqual(image_chess_1, copy_image) == 1);

  printf("10) ImageIsDifferent\n");
  assert(ImageIsDifferent(image_chess_1, image_chess_2) == 1);
  assert(ImageIsDifferent(image_1, image_2) == 1);
  assert(ImageIsDifferent(image_chess_1, copy_image) == 0);

  printf("11) ImageRotate90CW\n");
  Image image_1_rotated_90 = ImageRotate90CW(image_1);
  assert(ImageWidth(image_1) == ImageHeight(image_1_rotated_90));
  assert(ImageHeight(image_1) == ImageWidth(image_1_rotated_90));
  // ImageSavePBM(image_1_rotated_90, "feep90.pbm");

  Image image_1_rotated_180 = ImageRotate90CW(image_1_rotated_90);
  assert(ImageWidth(image_1) == ImageWidth(image_1_rotated_180));
  assert(ImageHeight(image_1) == ImageHeight(image_1_rotated_180));
  // ImageSavePBM(image_1_rotated_180, "feep180.pbm");

  Image image_1_rotated_270 = ImageRotate90CW(image_1_rotated_180);
  assert(ImageWidth(image_1) == ImageHeight(image_1_rotated_270));
  assert(ImageHeight(image_1) == ImageWidth(image_1_rotated_270));
  // ImageSavePBM(image_1_rotated_270, "feep270.pbm");

  Image image_1_rotated_360 = ImageRotate90CW(image_1_rotated_270);
  assert(ImageIsEqual(image_1, image_1_rotated_360) == 1);
  // ImageSavePBM(image_1_rotated_360, "feep360.pbm");

  printf("12) ImageRotate180CW\n");
  Image image_2_rotated_180 = ImageRotate180CW(image_2);
  assert(ImageWidth(image_2) == ImageWidth(image_2_rotated_180));
  assert(ImageHeight(image_2) == ImageHeight(image_2_rotated_180));
  // ImageSavePPM(image_2_rotated_180, "feep180.ppm");
  Image image_2_rotated_360 = ImageRotate180CW(image_2_rotated_180);
  assert(ImageIsEqual(image_2, image_2_rotated_360) == 1);
  // ImageSavePPM(image_2_rotated_360, "feep360.ppm");

  printf("13) ImageRegionFillingRecursive\n");
  // Create image with the full LUT
  // BUT with just ONE color being used
  Image image_10_10 = ImageCreatePalete(10, 10, 10);
  // ImageRAWPrint(image_10_10);

  int num_labeled_pixels = ImageRegionFillingRecursive(image_10_10, 0, 0, 3);
  assert(num_labeled_pixels == 10 * 10);
  // ImageRAWPrint(image_10_10);

  printf("14) ImageRegionFillingWithSTACK\n");
  // Create image with the full LUT
  // BUT with just ONE color being used
  Image image_15_15 = ImageCreatePalete(15, 15, 15);
  // ImageRAWPrint(image_15_15);

  num_labeled_pixels = ImageRegionFillingWithSTACK(image_15_15, 0, 0, 3);
  assert(num_labeled_pixels == 15 * 15);
  // ImageRAWPrint(image_15_15);

  printf("15) ImageRegionFillingWithQUEUE\n");
  // Create image with the full LUT
  // BUT with just ONE color being used
  Image image_20_20 = ImageCreatePalete(20, 20, 20);
  // ImageRAWPrint(image_20_20);

  num_labeled_pixels = ImageRegionFillingWithQUEUE(image_20_20, 0, 0, 3);
  assert(num_labeled_pixels == 20 * 20);
  // ImageRAWPrint(image_20_20);

  printf("16) ImageSegmentation with RECURSIVE Region Growing\n");
  Image segmented_image_1 = ImageLoadPPM("img/ooo.ppm");
  // ImageRAWPrint(segmented_image_1);

  int num_regions =
      ImageSegmentation(segmented_image_1, ImageRegionFillingRecursive);
  assert(num_regions == 4);
  // ImageRAWPrint(segmented_image_1);
  // ImageSavePPM(segmented_image_1, "oooSegRecAlg.ppm");

  printf("17) ImageSegmentation with Region Growing with STACK\n");
  Image temp_image = ImageLoadPPM("img/ooo.ppm");
  Image segmented_image_2 = ImageRotate90CW(temp_image);
  // ImageRAWPrint(image_segmented_2);

  num_regions =
      ImageSegmentation(segmented_image_2, ImageRegionFillingWithSTACK);
  assert(num_regions == 4);
  // ImageRAWPrint(segmented_image_2);
  // ImageSavePPM(segmented_image_2, "oooSegSTACKAlg.ppm");

  printf("18) ImageSegmentation with Region Growing with QUEUE\n");
  Image segmented_image_3 = ImageLoadPPM("img/ooo.ppm");
  // ImageRAWPrint(image_segmented_3);

  num_regions =
      ImageSegmentation(segmented_image_3, ImageRegionFillingWithQUEUE);
  assert(num_regions == 4);
  // ImageRAWPrint(segmented_image_3);
  // ImageSavePPM(segmented_image_3, "oooSegQUEUEAlg.ppm");

  printf("19) ImageSegmentation : Maze image\n");
  Image segmented_image_4 = ImageLoadPBM("img/maze20x20b.pbm");
  // ImageRAWPrint(image_segmented_4);
  Image segmented_image_5 = ImageCopy(segmented_image_4);
  Image segmented_image_6 = ImageCopy(segmented_image_4);

  num_regions =
      ImageSegmentation(segmented_image_4, ImageRegionFillingRecursive);
  assert(num_regions == 5);
  ImageSavePPM(segmented_image_4, "mazeSegRecAlg.ppm");

  num_regions =
      ImageSegmentation(segmented_image_5, ImageRegionFillingWithSTACK);
  assert(num_regions == 5);
  assert(ImageIsEqual(segmented_image_4, segmented_image_5));
  ImageSavePPM(segmented_image_5, "mazeSegSTACKAlg.ppm");

  num_regions =
      ImageSegmentation(segmented_image_6, ImageRegionFillingWithQUEUE);
  assert(num_regions == 5);
  assert(ImageIsEqual(segmented_image_4, segmented_image_6));
  ImageSavePPM(segmented_image_6, "mazeSegQUEUEAlg.ppm");

  // Housekeeping

  printf("20) ImageDestroy\n");
  ImageDestroy(&white_image);
  ImageDestroy(&black_image);
  ImageDestroy(&copy_image);
  ImageDestroy(&image_chess_1);
  ImageDestroy(&image_chess_2);
  ImageDestroy(&image_1);
  ImageDestroy(&image_2);
  ImageDestroy(&image_3);
  ImageDestroy(&image_1_rotated_90);
  ImageDestroy(&image_1_rotated_180);
  ImageDestroy(&image_1_rotated_270);
  ImageDestroy(&image_1_rotated_360);
  ImageDestroy(&image_2_rotated_180);
  ImageDestroy(&image_2_rotated_360);
  ImageDestroy(&image_10_10);
  ImageDestroy(&image_15_15);
  ImageDestroy(&image_20_20);
  ImageDestroy(&segmented_image_1);
  ImageDestroy(&temp_image);
  ImageDestroy(&segmented_image_2);
  ImageDestroy(&segmented_image_3);
  ImageDestroy(&segmented_image_4);
  ImageDestroy(&segmented_image_5);
  ImageDestroy(&segmented_image_6);

  return 0;
}
