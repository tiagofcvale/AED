#include <stdio.h>
#include <string.h>

#include "error.h"
#include "LoadSave.h"

// Load Image. Assume type PBM if fname "*.pbm" otherwise PPM.
Image LoadImage(char* fname) {
  int length = strlen(fname);
  // File extension
  char* fext = fname + length - 3;

  Image img = NULL;

  if (strcmp(fext, "pbm") == 0 || strcmp(fext, "PBM") == 0) {
    img = ImageLoadPBM(fname);
  } else {
    img = ImageLoadPPM(fname);
  }

  return img;
}

// Save Image. Assume type PBM if fname "*.pbm" otherwise PPM.
Image SaveImage(Image img, char* fname) {
  int length = strlen(fname);
  // File extension
  char* fext = fname + length - 3;

  if (strcmp(fext, "pbm") == 0 || strcmp(fext, "PBM") == 0) {
    ImageSavePBM(img, fname);
  } else {
    ImageSavePPM(img, fname);
  }

  return img;
}

