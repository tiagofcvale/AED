// TestImageIsEqual - A program that uses the ImageIsEqual function.
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

#include "LoadSave.h"

int main(int argc, char* argv[]) {
  program_name = argv[0];
  if (argc != 3) {
    error(1, 0, "Usage: testImageIsEqual first_image second_image");
  }

  ImageInit();

  printf("Loading the two images\n");
  Image image_1 = LoadImage(argv[1]);
  Image image_2 = LoadImage(argv[2]);

  printf("Comparing the two images\n");
  if (ImageIsEqual(image_1, image_2)) {
    printf("The images are EQUAL\n");
  } else {
    printf("The images are NOT EQUAL\n");
  }

  // Housekeeping
  ImageDestroy(&image_1);
  ImageDestroy(&image_2);

  return 0;
}
