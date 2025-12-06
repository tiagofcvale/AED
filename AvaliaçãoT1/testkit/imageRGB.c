/// imageRGB - A simple image module for handling RGB images,
///            pixel color values are represented using a look-up table (LUT)
///
/// This module is part of a programming project
/// for the course AED, DETI / UA.PT
///
/// You may freely use and modify this code, at your own risk,
/// as long as you give proper credit to the original and subsequent authors.
///
/// The AED Team <jmadeira@ua.pt, jmr@ua.pt, ...>
/// 2025

// Student authors (fill in below):
// NMec:125804
// Name:Dinis Filipe da Silva Néri Marques Carvalho
// NMec:119832
// Name:Vasco Peixoto Araújo
//
// Date:15/11/2025
//

#include "imageRGB.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PixelCoords.h"
#include "PixelCoordsQueue.h"
#include "PixelCoordsStack.h"
#include "instrumentation.h"

// The data structure
//
// A RGB image is stored in a structure containing 5 fields:
// Two integers store the image width and height.
// The next field is a pointer to an array that stores the pointers
// to the image rows.
//
// Clients should use images only through variables of type Image,
// which are pointers to the image structure, and should not access the
// structure fields directly.

// FIXED SIZE of LUT for storing RGB triplets
#define FIXED_LUT_SIZE 1000

// Internal structure for storing RGB images
struct image {
  uint32 width;
  uint32 height;
  uint16** image;  // pointer to an array of pointers referencing the image rows
  uint16 num_colors;  // the number of colors (i.e., pixel labels) used
  rgb_t* LUT;         // table storing (R,G,B) triplets
};

// Design by Contract

// This module follows "design-by-contract" principles.
// `assert` is used to check function preconditions, postconditions
// and type invariants.
// This helps to find programmer errors.

/// Defensive Error Handling

// In this module, only functions dealing with memory allocation or file
// (I/O) operations use defensive techniques.
//
// When one of these functions detects a memory or I/O error,
// it immediately prints an error message and aborts the program.
// This is a Fail-Fast strategy.
//
// You may use the `check` function to check a condition
// and exit the program with an error message if it is false.
// Note that it works similarly to `assert`, but cannot be disabled.
// It should be used to detect "external" uncontrolable errors,
// and not for "internal" programmer errors.
//
// See how it's used in ImageLoadPBM, for example.

// Check a condition and if false, print failmsg and exit.
static void check(int condition, const char* failmsg) {
  if (!condition) {
    perror(failmsg);
    exit(errno || 255);
  }
}

/// Init Image library.  (Call once!)
/// Currently, simply calibrate instrumentation and set names of counters.
void ImageInit(void) {  ///
  InstrCalibrate();
  InstrName[0] = "pixmem";  // InstrCount[0] will count pixel array acesses
  // Name other counters here...
}

// Macros to simplify accessing instrumentation counters:
#define PIXMEM InstrCount[0]
// Add more macros here...

// TIP: Search for PIXMEM or InstrCount to see where it is incremented!

/// Auxiliary (static) functions

static Image AllocateImageHeader(uint32 width, uint32 height) {
  // Create the header of an image data structure
  // Allocate the array of pointers to rows
  // And the look-up table

  Image newHeader = malloc(sizeof(struct image));
  // Error handling
  check(newHeader != NULL, "malloc");

  newHeader->width = width;
  newHeader->height = height;

  // Allocating the array of pointers to image rows
  newHeader->image = malloc(height * sizeof(uint16*));
  // Error handling
  check(newHeader->image != NULL, "Alloc failed ->image array");

  // Allocating the LUT
  newHeader->LUT = malloc(FIXED_LUT_SIZE * sizeof(rgb_t));
  // Error handling
  check(newHeader->LUT != NULL, "Alloc failed ->LUT array");

  // Initialize LUT with 2 fixed colors
  newHeader->num_colors = 2;
  newHeader->LUT[0] = 0xffffff;  // RGB WHITE
  newHeader->LUT[1] = 0x000000;  // RGB BLACK

  return newHeader;
}

// Allocate row of background (label=0) pixels
static uint16* AllocateRowArray(uint32 size) {
  uint16* newArray = calloc((size_t)size, sizeof(uint16));
  // Error handling
  check(newArray != NULL, "AllocateRowArray");

  return newArray;
}

/// Find color label for given RGB color in img LUT.
/// Return the label or -1 if not found.
static int LUTFindColor(Image img, rgb_t color) {
  for (uint16 index = 0; index < img->num_colors; index++) {
    if (img->LUT[index] == color) return index;
  }
  return -1;
}

/// Return color label for RGB color in img LUT.
/// Finds existing color or allocs new one!
static int LUTAllocColor(Image img, rgb_t color) {
  int index = LUTFindColor(img, color);
  if (index < 0) {
    check(img->num_colors < FIXED_LUT_SIZE, "LUT Overflow");
    index = img->num_colors++;
    img->LUT[index] = color;
  }
  return index;
}

/// Return a pseudo-random successor of the given color.
static rgb_t GenerateNextColor(rgb_t color) {
  return (color + 7639) & 0xffffff;
}

/// Image management functions

/// Create a new RGB image. All pixels with the background WHITE color.
///   width, height: the dimensions of the new image.
/// Requires: width and height must be non-negative.
///
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
Image ImageCreate(uint32 width, uint32 height) {
  assert(width > 0);
  assert(height > 0);

  // Just two possible pixel colors
  Image img = AllocateImageHeader(width, height);

  // Creating the image rows
  for (uint32 i = 0; i < height; i++) {
    img->image[i] = AllocateRowArray(width);  // Alloc all WHITE row
  }

  return img;
}

/// Create a new RGB image, with a color chess pattern.
/// The background is WHITE.
///   width, height: the dimensions of the new image.
///   edge: the width and height of a chess square.
///   color: the foreground color.
/// Requires: width, height and edge must be non-negative.
///
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
Image ImageCreateChess(uint32 width, uint32 height, uint32 edge, rgb_t color) {
  assert(width > 0);
  assert(height > 0);
  assert(edge > 0);

  Image img = ImageCreate(width, height);

  // Alloc color in LUT.
  uint8 label = LUTAllocColor(img, color);

  // Assigning the color to each image pixel

  // Pixel (0, 0) gets the chosen color label
  for (uint32 i = 0; i < height; i++) {
    uint32 I = i / edge;
    for (uint32 j = 0; j < width; j++) {
      uint32 J = j / edge;
      img->image[i][j] = (I + J) % 2 ? 0 : label;
    }
  }

  // Return the created chess image
  return img;
}

/// Create an image with a palete of generated colors.
Image ImageCreatePalete(uint32 width, uint32 height, uint32 edge) {
  assert(width > 0);
  assert(height > 0);
  assert(edge > 0);

  Image img = ImageCreate(width, height);

  // Fill LUT with generated colors
  rgb_t color = 0x000000;
  while (img->num_colors < FIXED_LUT_SIZE) {
    color = GenerateNextColor(color);
    img->LUT[img->num_colors++] = color;
  }

  // number of tiles
  uint32 wtiles = width / edge;

  // Pixel (0, 0) gets the chosen color label
  for (uint32 i = 0; i < height; i++) {
    uint32 I = i / edge;
    for (uint32 j = 0; j < width; j++) {
      uint32 J = j / edge;
      img->image[i][j] = (I * wtiles + J) % FIXED_LUT_SIZE;
    }
  }

  return img;
}

/// Destroy the image pointed to by (*imgp).
///   imgp : address of an Image variable.
/// If (*imgp)==NULL, no operation is performed.
///
/// Ensures: (*imgp)==NULL.
void ImageDestroy(Image* imgp) {
  assert(imgp != NULL);

  Image img = *imgp;

  for (uint32 i = 0; i < img->height; i++) {
    free(img->image[i]);
  }
  free(img->image);
  free(img->LUT);
  free(img);

  *imgp = NULL;
}

/// Create a deep copy of the image pointed to by img.
///   img : address of an Image variable.
///
/// On success, a new copied image is returned.
/// (The caller is responsible for destroying the returned image!)
Image ImageCopy(const Image img) {
  assert(img != NULL);

  Image copia = malloc(sizeof(struct image)); //alocar memória para a copia da imagem
  assert(copia !=NULL);  //garantir que foi alocada memória para a cópia

  copia->width = img->width; //copiar o comprimento da imagem
  copia->height = img->height; //copiar a altura da imagem
  copia->num_colors = img ->num_colors; //copiar o numero de cores da imagem 

  copia->image = malloc(copia->height * sizeof(uint16*));//alocar memoria para os pixeis
  assert(copia->image != NULL);

  copia->LUT = malloc(FIXED_LUT_SIZE * sizeof(rgb_t));  //alocar memória para a LUT
  assert(copia->LUT != NULL);
  for (uint16 i = 0; i < copia->num_colors; i++) {
      copia->LUT[i] = img->LUT[i];  //copiar a cor usada em cada pixel
  }

  for (uint32 i = 0; i < copia->height; i++) {
    copia->image[i] = malloc(copia->width * sizeof(uint16)); //alocar memoria para cada pixel
    assert(copia->image[i] != NULL);
    for (uint32 j = 0; j < copia->width; j++) {
        copia->image[i][j] = img->image[i][j]; //copiar os pixeis da imagem 
    }
  }
  

  return copia; //devolve a imagem que copiamos
}

/// Printing on the console

/// These functions do not modify the image and never fail.

/// Output the raw RGB image (i.e., print the integer value of pixel).
void ImageRAWPrint(const Image img) {
  printf("width = %d height = %d\n", (int)img->width, (int)img->height);
  printf("num_colors = %d\n", (int)img->num_colors);
  printf("RAW image\n");

  // Print the pixel labels of each image row
  for (uint32 i = 0; i < img->height; i++) {
    for (uint32 j = 0; j < img->width; j++) {
      printf("%2d", img->image[i][j]);
    }
    // At current row end
    printf("\n");
  }

  printf("LUT:\n");
  // Print the LUT (R,G,B) values
  for (int i = 0; i < (int)img->num_colors; i++) {
    rgb_t color = img->LUT[i];
    int r = color >> 16 & 0xff;
    int g = color >> 8 & 0xff;
    int b = color & 0xff;
    printf("%3d -> (%3d,%3d,%3d)\n", i, r, g, b);
  }

  printf("\n");
}

/// PBM file operations --- For BW images

// See PBM format specification: http://netpbm.sourceforge.net/doc/pbm.html

//
static void unpackBits(int nbytes, const uint8 bytes[], uint8 raw_row[]) {
  // bitmask starts at top bit
  int offset = 0;
  uint8 mask = 1 << (7 - offset);
  while (offset < 8) {  // or (mask > 0)
    for (int b = 0; b < nbytes; b++) {
      raw_row[8 * b + offset] = (bytes[b] & mask) != 0;
    }
    mask >>= 1;
    offset++;
  }
}

static void packBits(int nbytes, uint8 bytes[], const uint8 raw_row[]) {
  // bitmask starts at top bit
  int offset = 0;
  uint8 mask = 1 << (7 - offset);
  while (offset < 8) {  // or (mask > 0)
    for (int b = 0; b < nbytes; b++) {
      if (offset == 0) bytes[b] = 0;
      bytes[b] |= raw_row[8 * b + offset] ? mask : 0;
    }
    mask >>= 1;
    offset++;
  }
}

// Match and skip 0 or more comment lines in file f.
// Comments start with a # and continue until the end-of-line, inclusive.
// Returns the number of comments skipped.
static int skipComments(FILE* f) {
  char c;
  int i = 0;
  while (fscanf(f, "#%*[^\n]%c", &c) == 1 && c == '\n') {
    i++;
  }
  return i;
}

/// Load a raw PBM file.
/// Only binary PBM files are accepted.
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
Image ImageLoadPBM(const char* filename) {  ///
  int w, h;
  char c;
  FILE* f = NULL;
  Image img = NULL;

  check((f = fopen(filename, "rb")) != NULL, "Open failed");
  // Parse PBM header
  check(fscanf(f, "P%c ", &c) == 1 && c == '4', "Invalid file format");
  skipComments(f);
  check(fscanf(f, "%d ", &w) == 1 && w >= 0, "Invalid width");
  skipComments(f);
  check(fscanf(f, "%d", &h) == 1 && h >= 0, "Invalid height");
  check(fscanf(f, "%c", &c) == 1 && isspace(c), "Whitespace expected");

  // Allocate image
  img = AllocateImageHeader((uint32)w, (uint32)h);

  // Read pixels
  int nbytes = (w + 8 - 1) / 8;  // number of bytes for each row
  // using VLAs...
  uint8 bytes[nbytes];
  uint8 raw_row[nbytes * 8];
  for (uint32 i = 0; i < img->height; i++) {
    check(fread(bytes, sizeof(uint8), nbytes, f) == (size_t)nbytes,
          "Reading pixels");
    unpackBits(nbytes, bytes, raw_row);
    img->image[i] = AllocateRowArray((uint32)w);
    for (uint32 j = 0; j < (uint32)w; j++) {
      img->image[i][j] = (uint16)raw_row[j];
    }
  }

  fclose(f);
  return img;
}

/// Save image to PBM file.
/// On success, returns nonzero.
/// On failure, a partial and invalid file may be left in the system.
int ImageSavePBM(const Image img, const char* filename) {  ///
  assert(img != NULL);
  assert(img->num_colors == 2);

  int w = (int)img->width;
  int h = (int)img->height;
  FILE* f = NULL;

  check((f = fopen(filename, "wb")) != NULL, "Open failed");
  check(fprintf(f, "P4\n%d %d\n", w, h) > 0, "Writing header failed");

  // Write pixels
  int nbytes = (w + 8 - 1) / 8;  // number of bytes for each row
  // using VLAs...
  uint8 bytes[nbytes];
  uint8 raw_row[nbytes * 8];
  for (uint32 i = 0; i < img->height; i++) {
    for (uint32 j = 0; j < img->width; j++) {
      raw_row[j] = (uint8)img->image[i][j];
    }
    // Fill padding pixels with WHITE
    memset(raw_row + w, WHITE, nbytes * 8 - w);
    packBits(nbytes, bytes, raw_row);
    check(fwrite(bytes, sizeof(uint8), nbytes, f) == (size_t)nbytes,
          "Writing pixels failed");
  }

  // Cleanup
  fclose(f);

  return 0;
}

/// PPM file operations --- For RGB images

/// Load a raw PPM file.
/// Only ASCII PPM files are accepted.
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
Image ImageLoadPPM(const char* filename) {
  assert(filename != NULL);
  int w, h;
  int levels;
  char c;
  FILE* f = NULL;

  check((f = fopen(filename, "rb")) != NULL, "Open failed");
  // Parse PPM header
  check(fscanf(f, "P%c ", &c) == 1 && c == '3', "Invalid file format");
  skipComments(f);
  check(fscanf(f, "%d ", &w) == 1 && w >= 0, "Invalid width");
  skipComments(f);
  check(fscanf(f, "%d", &h) == 1 && h >= 0, "Invalid height");
  skipComments(f);
  check(fscanf(f, "%d", &levels) == 1 && 0 <= levels && levels <= 255,
        "Invalid depth");
  check(fscanf(f, "%c", &c) == 1 && isspace(c), "Whitespace expected");

  // Allocate image
  Image img = ImageCreate((uint32)w, (uint32)h);

  // Read pixels
  for (uint32 i = 0; i < img->height; i++) {
    for (uint32 j = 0; j < img->width; j++) {
      int r, g, b;
      check(fscanf(f, "%d %d %d", &r, &g, &b) == 3 && 0 <= r && r <= levels &&
                0 <= g && g <= levels && 0 <= b && b <= levels,
            "Invalid pixel color");
      rgb_t color = r << 16 | g << 8 | b;
      uint16 index = LUTAllocColor(img, color);
      img->image[i][j] = index;
      // printf("[%u][%u]: (%d,%d,%d) -> %u (%6x)\n", i, j, r,g,b, index,
      // color);
    }
    fprintf(f, "\n");
  }

  fclose(f);
  return img;
}

/// Save image to PPM file.
/// On success, returns nonzero.
/// On failure, a partial and invalid file may be left in the system.
int ImageSavePPM(const Image img, const char* filename) {
  assert(img != NULL);

  int w = (int)img->width;
  int h = (int)img->height;
  FILE* f = NULL;

  check((f = fopen(filename, "wb")) != NULL, "Open failed");
  check(fprintf(f, "P3\n%d %d\n255\n", w, h) > 0, "Writing header failed");

  // The pixel RGB values
  for (uint32 i = 0; i < img->height; i++) {
    for (uint32 j = 0; j < img->width; j++) {
      uint16 index = img->image[i][j];
      rgb_t color = img->LUT[index];
      int r = color >> 16 & 0xff;
      int g = color >> 8 & 0xff;
      int b = color & 0xff;
      fprintf(f, "  %3d %3d %3d", r, g, b);
    }
    fprintf(f, "\n");
  }

  // Cleanup
  fclose(f);

  return 0;
}

/// Information queries

/// These functions do not modify the image and never fail.

/// Get image width
uint32 ImageWidth(const Image img) {
  assert(img != NULL);
  return img->width;
}

/// Get image height
uint32 ImageHeight(const Image img) {
  assert(img != NULL);
  return img->height;
}

/// Get number of image colors
uint16 ImageColors(const Image img) {
  assert(img != NULL);
  return img->num_colors;
}

/// Image comparison

/// These functions do not modify the images and never fail.

/// Check if img1 and img2 represent equal images.
/// NOTE: The same rgb color may correspond to different LUT labels in
/// different images!
int ImageIsEqual(const Image img1, const Image img2) {
  assert(img1 != NULL);
  assert(img2 != NULL);

  uint32 width_img1 = img1->width;  //width_img1 = comprimento da imagem 1
  uint32 height_img1 = img1 ->height; //height_img1 = altura da imagem 1
  uint32 width_img2 = img2->width; //comprimento da imagem 2
  uint32 height_img2 = img2->height;  //altura da imagem 2

  if (width_img1 != width_img2 || height_img1 != height_img2){
    return 0; //se as medidas das imagens forem diferentes, nunca podem ser iguais.
  }

  for (uint32 i =0; i < height_img1;i++){
    for(uint32 j = 0; j < width_img1;j++){
      InstrCount[0]++; // contador para verificar o número de comparações que o programa faz para diferentes imagens.

      uint16 pixel1 = img1->image[i][j];  //obter o pixel da img1 com aquela posição
      uint16 pixel2 = img2->image[i][j];  //obter o pixel da img2 com aquela posição
      rgb_t color1 = img1->LUT[pixel1]; //obter as cores RGB do pixel da imagem 1
      rgb_t color2 = img2->LUT[pixel2]; //obter as cores RGB do pixel da imagem 2
      if (color1 != color2){  //se as cores forem diferentes, as imagens são diferentes
        return 0;             
      }
    }
  }
  

  return 1; //imagens iguais se as dimensões e as cores de cada pixel das duas imagens forem iguais
  //Nesta função, o melhor caso é que a diferença ocorre no primeiro pixel que é comparado
  //O pior caso é em que todas as comparações sejam feitas (independentemente das imagens serem iguais ou diferentes)
  //Neste último caso, o número de comparações seria o comprimento * altura da imagem

}

int ImageIsDifferent(const Image img1, const Image img2) {
  assert(img1 != NULL);
  assert(img2 != NULL);

  return !ImageIsEqual(img1, img2);
}

/// Geometric transformations

/// These functions apply geometric transformations to an image,
/// returning a new image as a result.
///
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)

/// Rotate 90 degrees clockwise (CW).
/// Returns a rotated version of the image.
/// Ensures: The original img is not modified.
///
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
Image ImageRotate90CW(const Image img) {
  assert(img != NULL);

  Image img_rotated = malloc(sizeof(struct image));
  assert (img_rotated != NULL); //garante que a memória foi alocada para a nova imagem

  img_rotated -> width = img-> height; //ao rodar a imagem 90º, a largura vira a altura
  img_rotated -> height = img -> width;  //e a altura vira a largura
  img_rotated -> num_colors = img -> num_colors; //numero de cores mantém-se

  img_rotated -> LUT = malloc(FIXED_LUT_SIZE * sizeof(rgb_t));
  assert (img_rotated->LUT !=NULL);

  for (uint32 c = 0; c < img_rotated->num_colors;c++){
    img_rotated->LUT[c] = img->LUT[c];  //copiar cada cor
  }

  img_rotated ->image = malloc(img_rotated->height * sizeof(uint16*)); //Alocar a matriz dos pixeis
  assert (img_rotated ->image != NULL);

  for (uint32 i = 0; i < img_rotated ->height;i++){
    img_rotated ->image[i] = malloc(img_rotated->width * sizeof(uint16));
    assert (img_rotated->image[i] != NULL);
    for (uint32 j = 0; j < img_rotated ->width;j++){
      uint32 coluna = i;  //coluna original
      uint32 linha = img->height -1-j; //linha original
      img_rotated ->image[i][j] = img->image[linha][coluna];  //copiar os pixeis para as novas posições
    }
  }

  return img_rotated;
}

/// Rotate 180 degrees clockwise (CW).
/// Returns a rotated version of the image.
/// Ensures: The original img is not modified.
///
/// On success, a new image is returned.
/// (The caller is responsible for destroying the returned image!)
Image ImageRotate180CW(const Image img) {
  assert(img != NULL);

  Image img_rotated = malloc(sizeof(struct image));
  assert (img_rotated !=NULL);
  
  img_rotated ->width = img->width; //ao rodar 180º, os tamanhos mantêm-se
  img_rotated ->height = img->height;
  img_rotated ->num_colors = img->num_colors; //as cores também

  img_rotated ->LUT = malloc(FIXED_LUT_SIZE * sizeof(rgb_t));
  assert(img_rotated ->LUT !=NULL);
  for (uint32 i = 0; i < img_rotated ->num_colors; i++){
    img_rotated->LUT[i] = img ->LUT[i]; //copiar cada cor
  }

  img_rotated ->image = malloc (img_rotated->height * sizeof(uint16*)); //Alocar array de linhas desta nova imagem
  assert(img_rotated->image !=NULL);

  for(uint32 nova_linha = 0; nova_linha < img_rotated->height;nova_linha++){
    img_rotated->image[nova_linha] = malloc(img_rotated->width * sizeof(uint16));
    assert(img_rotated ->image[nova_linha] != NULL);

    for(uint32 nova_coluna =0; nova_coluna < img_rotated->width; nova_coluna++){
      uint32 linha =img->height -1 - nova_linha;  //linha invertida
      uint32 coluna = img ->width -1 - nova_coluna; //coluna invertida
      img_rotated -> image[nova_linha][nova_coluna] = img->image[linha][coluna];
    }
  }
  
  return img_rotated;
}

/// Check whether pixel coords (u, v) are inside img.
/// ATTENTION
///   u : column index
///   v : row index
int ImageIsValidPixel(const Image img, int u, int v) {
  return 0 <= u && u < (int)img->width && 0 <= v && v < (int)img->height;
}

/// Region Growing

/// The following three *RegionFilling* functions perform region growing
/// using some variation of the 4-neighbors flood-filling algorithm:
///   Given the coordinates (u, v) of a seed pixel,
///   fill all similarly-colored adjacent pixels with a new color label.
///
/// All of these functions receive the same arguments:
///   img: The image to operate on (and modify).
///   u, v: the coordinates of the seed pixel.
///   label: the new color label (LUT index) to fill the region with.
///
/// And return: the number of labeled pixels.

/// Each function carries out a different version of the algorithm.

/// Region growing using the recursive flood-filling algorithm.
int ImageRegionFillingRecursive(Image img, int u, int v, uint16 label) {
  assert(img != NULL);
  assert(ImageIsValidPixel(img, u, v));
  assert(label < FIXED_LUT_SIZE);

  uint16 pixel = img->image[u][v]; 

  // Se o pixel não pertencer ao background, não é preenchido
  if (pixel != WHITE){
        return 0;
  }
  //Preenche o pixel
  img->image[u][v] = label;

    
  ImageRegionFillingRecursive(img, u + 1, v,label);
  ImageRegionFillingRecursive(img,u - 1, v,label);
  ImageRegionFillingRecursive(img, u, v + 1,label);
  ImageRegionFillingRecursive(img, u,v - 1, label);  //Chamar a função para os 4 pixeis adjacentes ao selecionado e fazer recursão

  return 1;
}

/// Region growing using a STACK of pixel coordinates to
/// implement the flood-filling algorithm.
int ImageRegionFillingWithSTACK(Image img, int u, int v, uint16 label) {
  assert(img != NULL);
  assert(ImageIsValidPixel(img, u, v));
  assert(label < FIXED_LUT_SIZE);

  uint16 pixel = img->image[u][v];

  if (pixel != WHITE){
    return 0; //só preenche o pixel se este ainda não estiver preenchido
  }

  Stack* stack = StackCreate(img->width * img->height);
  assert(stack != NULL);
  StackPush(stack,(PixelCoords){u,v});  //coloca o pixel inicial na stack
  int contador_stack = 0;

  while (!StackIsEmpty(stack)){ //executar enquanto houver pixeis na stack
    PixelCoords p =StackPop(stack);
    int x = p.u;  //coordenada da coluna
    int y = p.v;  //coordenada da linha

    if (!ImageIsValidPixel(img,x,y)){
      continue;
    }
    if (img ->image[y][x] != WHITE){
      continue;
    }

    img->image[y][x] = label;
    contador_stack++;

    StackPush(stack, (PixelCoords){x+1,y});
    StackPush(stack, (PixelCoords){x-1,y});
    StackPush(stack, (PixelCoords){x,y+1});
    StackPush(stack, (PixelCoords){x,y-1}); //pixeis adjacentes ao escolhido
  }

  StackDestroy(&stack); //destroi a stack depois de preencher a imagem e coloca o ponteiro a NULL

  return contador_stack; //devolver a quantidade de pixeis que foram preenchidos
}

/// Region growing using a QUEUE of pixel coordinates to
/// implement the flood-filling algorithm.
int ImageRegionFillingWithQUEUE(Image img, int u, int v, uint16 label) {
  assert(img != NULL);
  assert(ImageIsValidPixel(img, u, v));
  assert(label < FIXED_LUT_SIZE);

  uint16 pixel = img->image[u][v];

  if (pixel != WHITE){
    return 0; 
  } //garante que o pixel tem de ser preenchido antes de avançar com o programa

  Queue* queue =QueueCreate(img->width * img->height); //Queue com capacidade para os pixeis da imagem
  assert(queue !=NULL);

  QueueEnqueue(queue,(PixelCoords){u,v}); //adiciona o pixel inicial
  int contador_queue = 0;

  while (!QueueIsEmpty(queue)){
    PixelCoords p = QueueDequeue(queue);
    int x = p.u; //coordenada da coluna
    int y = p.v; //coordenada da linha

    if (!ImageIsValidPixel(img,x,y)){ //ignora todos os pixeis fora da imagem
      continue;
    }
    
    if (img->image[y][x] != WHITE){ //apenas preenche pixeis que ainda não foram preenchidos, ou seja, qualquer pixel branco
      continue;
    }
    img->image[y][x] = label;
    contador_queue++;

    QueueEnqueue(queue, (PixelCoords){x+1,y});
    QueueEnqueue(queue, (PixelCoords){x-1,y});
    QueueEnqueue(queue, (PixelCoords){x,y+1});
    QueueEnqueue(queue, (PixelCoords){x,y-1}); //adiciona os pixeis adjacentes ao escolhido

  }
  QueueDestroy(&queue);


  return contador_queue; //devolve o número de pixeis preenchidos
}

/// Image Segmentation

/// Label each WHITE region with a different color.
/// - WHITE (the background color) has label (LUT index) 0.
/// - Use GenerateNextColor to create the RGB color for each new region.
///
/// One of the region filling functions above is passed as the
/// last argument, using a function pointer.
///
/// Returns the number of image regions found.
int ImageSegmentation(Image img, FillingFunction fillFunct) {
  assert(img != NULL);
  assert(fillFunct != NULL);

  int regioes = 0;    //conta regioes
  rgb_t cor_atual = 0; // cor inicial para gerar outras cores novas

  for (uint32 v = 0; v < img->height;v++){ //percore as linhas
    for(uint32 u = 0; u< img->width;u++){ //percorre as colunas

      if (img->image[v][u] == WHITE){
        cor_atual = GenerateNextColor(cor_atual); //altera a cor atual para outra cor
        uint16 cor_pixel = LUTAllocColor(img, cor_atual);
        int pixel_preenchido = fillFunct(img,u,v,cor_pixel);
        if (pixel_preenchido > 0){
          regioes++;
        }
      }
    }
  }

  return regioes;
}
