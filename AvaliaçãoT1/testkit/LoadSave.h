#ifndef _LoadSave_H
#define _LoadSave_H

#include "imageRGB.h"

// Load Image. Assume type PBM if fname "*.pbm" otherwise PPM.
Image LoadImage(char* fname);

// Save Image. Assume type PBM if fname "*.pbm" otherwise PPM.
Image SaveImage(Image img, char* fname);

#endif
