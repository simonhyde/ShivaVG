#include "test.h"
#include "lodepng.h"
#include <ctype.h>

VGPath src;
VGPath dst;
VGPaint srcFill;
VGPaint dstFill;
VGImage isrc;
VGImage idst;

VGfloat srcColor[4] = { 0.12f, 0.58f, 0.95f, 1.0f}; // #2196f3
VGfloat dstColor[4] = { 0.91f, 0.11f, 0.38f, 1.0f}; // #e91e63

#ifndef IMAGE_DIR
#define IMAGE_DIR "./"
#endif

VGBlendMode blends[5] = {
   VG_BLEND_SRC,
   VG_BLEND_SRC_OVER,
   VG_BLEND_DST_OVER,
   VG_BLEND_SRC_IN,
   VG_BLEND_DST_IN
};


void createOperands(void)
{
  VGfloat clear[] = {1.0f, 0.0f, 0.0f, 0.0f};

  src = testCreatePath();
  vguEllipse(src, 30,30,40,40);

  dst = testCreatePath();
  vguRect(dst, 0,0,40,30);

  srcFill = vgCreatePaint();
  vgSetParameterfv(srcFill, VG_PAINT_COLOR, 4, srcColor);

  dstFill = vgCreatePaint();
  vgSetParameterfv(dstFill, VG_PAINT_COLOR, 4, dstColor);

  SH_DEBUG("try to load src image\n");
  unsigned int wsrc, hsrc;
  unsigned char *src;
  unsigned int err = lodepng_decode32_file(&src, &wsrc, &hsrc, IMAGE_DIR"test_blend_src.png");

  if (err != 0) {
     printf("Something goes wrong %s\n",lodepng_error_text(err));
     exit(EXIT_FAILURE);
  }

  SH_DEBUG("try to load dst image\n");
  unsigned int wdst, hdst;
  unsigned char *dst;
  err = lodepng_decode32_file(&dst, &wdst, &hdst, IMAGE_DIR"test_blend_dst.png");
  if (err != 0) {
     printf("Something goes wrong %s\n",lodepng_error_text(err));
     exit(EXIT_FAILURE);
  }

  // Remember png are big-endian
  isrc = vgCreateImage(VG_sRGBA_8888, wsrc,
                       hsrc, VG_IMAGE_QUALITY_BETTER);

  vgImageSubData(isrc, src, wsrc * 4, VG_sRGBA_8888,
                  0,0, wsrc, hsrc);
  SH_DEBUG("src image loaded\n");

  idst = vgCreateImage(VG_sRGBA_8888, wdst,
                       hdst, VG_IMAGE_QUALITY_BETTER);
  vgImageSubData(idst, dst, wdst * 4 , VG_sRGBA_8888,
                  0,0, wdst, hdst);
  SH_DEBUG("dst image loaded\n");
}

void
display(float interval)
{
   VGfloat clear[] = {0.0f, 0.0f, 0.0f, 1.0f};

   vgSetfv(VG_CLEAR_COLOR, 4, clear);
   vgClear(0, 0, testWidth(), testHeight());

   vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
   vgLoadIdentity();

   vgSeti(VG_BLEND_MODE, VG_BLEND_SRC_IN);
   vgDrawImage(isrc);
   vgSeti(VG_BLEND_MODE, VG_BLEND_SRC_OVER);
   vgDrawImage(idst);

}

void
key(unsigned char code, int x, int y)
{
   if (tolower(code) == 'q') {
      exit(EXIT_SUCCESS);
   }

}

int
main(int argc, char *argv[])
{
   testInit(argc, argv, 400, 400, "ShivaVG: Blending Test");
   testCallback(TEST_CALLBACK_DISPLAY, (CallbackFunc) display);
   testCallback(TEST_CALLBACK_KEY, (CallbackFunc) key);

   testOverlayColor(1, 1, 1, 1);
   /*
    * testOverlayString("Not implemented yet");
    */

   createOperands();
   testRun();

   return EXIT_SUCCESS;
}
