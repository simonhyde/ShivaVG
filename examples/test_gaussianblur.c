#include "test.h"
#include <VG/openvg.h>
#include <VG/vgu.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "./img_data.c"

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT   240

enum deviationType {
   NONE,
   DEFAULT,
   X,
   Y
} deviationType;


void display(void)
{
   VGImage srcImage, dstImage;
   VGfloat white[] = { 0.0f, 0.0f, 0.0f, 1.0f };

   vgSetfv(VG_CLEAR_COLOR, 4, white);
   vgClear(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

   srcImage = vgCreateImage(VG_sRGBA_8888, SCREEN_WIDTH, SCREEN_HEIGHT, VG_IMAGE_QUALITY_BETTER);
   vgImageSubData( srcImage, cimg, SCREEN_WIDTH*4, VG_sABGR_8888, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

   dstImage = vgCreateImage(VG_sRGBA_8888, SCREEN_WIDTH, SCREEN_HEIGHT, VG_IMAGE_QUALITY_BETTER);

   switch (deviationType) {
   case DEFAULT:
      vgGaussianBlur(dstImage, srcImage, 10.0f, 10.0f, VG_TILE_PAD);
      vgDrawImage(dstImage);
      break;
   case X:
      vgGaussianBlur( dstImage, srcImage, 10.0f, 1.0f, VG_TILE_PAD);
      vgDrawImage(dstImage);
      break;
   case Y:
      vgGaussianBlur( dstImage, srcImage, 1.0f, 10.0f, VG_TILE_PAD);
      vgDrawImage(dstImage);
      break;
   default:
      vgDrawImage(srcImage);
      break;

   }
   vgDestroyImage( srcImage );
   vgDestroyImage( dstImage );
}

const char commands[] =
   "Commands\n"
   "H - this help\n"
   "d - default (stdDevX == sdtDevY)\n"
   "x - stdDevX > stdDevY\n"
   "y - sdtDevY > sstdDevX\n"
   "n - none \n" ;

const char help[] = "Press H for a list of commands";
void key(unsigned char code, int x, int y)
{
   static int open = 0;
   switch (tolower(code)) {
   case 'd':
      deviationType = DEFAULT;
      break;
   case 'x':
      deviationType = X;
      break;
   case 'y':
      deviationType = Y;
      break;
   case 'n':
      deviationType = NONE;
      break;
   case 'h':
      /* Show help */
      if (!open) {
         testOverlayString(commands);
         open = 1;
      } else {
         testOverlayString(help);
         open = 0;
      }
      break;
   }

}

int main(int argc, char *argv[])
{
   testInit(argc, argv, SCREEN_WIDTH, SCREEN_HEIGHT , "ShivaVG: gaussian blur example");
   display();
   testCallback(TEST_CALLBACK_DISPLAY, (CallbackFunc) display);
   testCallback(TEST_CALLBACK_KEY, (CallbackFunc) key);
   testOverlayColor(1, 1, 1, 1);

   testRun();

   return EXIT_SUCCESS;
}