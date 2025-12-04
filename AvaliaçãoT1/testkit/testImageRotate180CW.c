// TestImageRotate180CW - A program that uses the ImageRotate180CW function.
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
    error(1, 0, "Usage: testImageRotate180CW original_image rotated_image");
  }

  ImageInit();

  printf("Loading the original image\n");
  Image original_image = LoadImage(argv[1]);

  printf("Rotating\n");
  Image rotated_image = ImageRotate180CW(original_image);
  assert(rotated_image != NULL);
  assert(ImageHeight(original_image) == ImageHeight(rotated_image));
  assert(ImageWidth(original_image) == ImageWidth(rotated_image));
  assert(ImageColors(original_image) == ImageColors(rotated_image));

  printf("Saving the rotated image\n");
  SaveImage(rotated_image, argv[2]);

  // Housekeeping
  ImageDestroy(&original_image);
  ImageDestroy(&rotated_image);

  return 0;
}
