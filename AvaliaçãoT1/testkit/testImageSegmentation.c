// TestImageSegmentation - A program that uses the ImageSegmentation
// function.
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
    error(1, 0, "Usage: testImageSegmentation original_image result_image");
  }

  ImageInit();

  printf("Loading the original image\n");
  Image original_image = LoadImage(argv[1]);

  printf("Image Segmentation with RECURSIVE Region Growing\n");
  Image segmented_image_1 = ImageCopy(original_image);
  int num_regions_1 =
      ImageSegmentation(segmented_image_1, ImageRegionFillingRecursive);

  printf("Num regions = %d\n", num_regions_1);

  ImageRAWPrint(segmented_image_1);

  printf("Saving the segmented image\n");
  SaveImage(segmented_image_1, argv[2]);

  printf("Image Segmentation with Region Growing with STACK\n");
  Image segmented_image_2 = ImageCopy(original_image);
  int num_regions_2 =
      ImageSegmentation(segmented_image_2, ImageRegionFillingWithSTACK);
  assert(num_regions_1 == num_regions_2);
  assert(ImageIsEqual(segmented_image_1, segmented_image_2));

  printf("Image Segmentation with Region Growing with QUEUE\n");
  Image segmented_image_3 = ImageCopy(original_image);
  int num_regions_3 =
      ImageSegmentation(segmented_image_3, ImageRegionFillingWithQUEUE);
  assert(num_regions_1 == num_regions_3);
  assert(ImageIsEqual(segmented_image_1, segmented_image_3));

  // Housekeeping
  ImageDestroy(&original_image);
  ImageDestroy(&segmented_image_1);
  ImageDestroy(&segmented_image_2);
  ImageDestroy(&segmented_image_3);

  return 0;
}
