// TestImageCopy - A program that uses the ImageCopy function.
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
    error(1, 0, "Usage: testImageCopy original_filename copy_filename");
  }

  ImageInit();

  printf("Loading the original image\n");
  Image original_image = LoadImage(argv[1]);

  printf("Copying\n");
  Image copy_image = ImageCopy(original_image);
  assert(copy_image != NULL);
  assert(copy_image != original_image);
  assert(ImageHeight(original_image) == ImageHeight(copy_image));
  assert(ImageWidth(original_image) == ImageWidth(copy_image));
  assert(ImageColors(original_image) == ImageColors(copy_image));

  printf("Destroying the original image\n");
  ImageDestroy(&original_image);

  printf("Saving the copy image\n");
  SaveImage(copy_image, argv[2]);

  // Housekeeping
  ImageDestroy(&copy_image);

  return 0;
}
