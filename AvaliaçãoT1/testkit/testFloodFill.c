// TestFloodFill - A program that uses the ImageRegionFilling* functions
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
  if ((argc - 7) % 3 != 0) {
    error(1, 0,
      "Usage: testFloodFill [R|S|Q] orig_img result_img [u v lbl]...");
  }

  char opt = argv[1][0];
  char *original_file = argv[2];
  char *result_file = argv[3];

  ImageInit();

  printf("Loading the original image\n");
  Image img = LoadImage(original_file);

  for (int i = 4; i < argc; i += 3) {
    int u = atoi(argv[i]);
    int v = atoi(argv[i+1]);
    int lbl = atoi(argv[i+2]);

    char* functionName = NULL;
    FillingFunction function = NULL;

    switch (opt) {
    case 'R': case 'r':
      functionName = "ImageRegionFillingRecursive";
      function = ImageRegionFillingRecursive;
      break;
    case 'S': case 's':
      functionName = "ImageRegionFillingWithSTACK";
      function = ImageRegionFillingWithSTACK;
      break;
    case 'Q': case 'q':
      functionName = "ImageRegionFillingWithQUEUE";
      function = ImageRegionFillingWithQUEUE;
      break;
    default:
      error(1, 0, "opt must be R, S or Q!");
    }

    printf("%s(img, %d, %d, %d)...\n", functionName, u, v, lbl);
    int num_pixels = function(img, u, v, lbl);
    printf("Number_of_labeled_pixels = %d\n", num_pixels);
  }

  ImageRAWPrint(img);
  printf("Saving the resulting image\n");
  ImageSavePPM(img, result_file);

  // Housekeeping
  ImageDestroy(&img);

  return 0;
}
