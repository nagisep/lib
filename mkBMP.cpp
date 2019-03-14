
#include "mkBMP.h"
#include <cstdio>

unsigned char hedder[] =
{
  'B', 'M',
  100, 0, 0, 0,   // file size 4byte 下位ビットから
  0, 0, 0, 0,     // 予約領域
  54, 0, 0, 0,    // data までのオフセット

  // ここからデータヘッダ
  40, 0, 0, 0,
  5,  0, 0, 0,   // 画像の幅
  2,  0, 0, 0,   // 画像の高さ
  1,  0,         // プレーン数
  24, 0,         // カラービット数
  0,  0,  0, 0,  // 非圧縮
  30, 0,  0, 0,  // 画像データのサイズ 高さ×幅×３（RGB）
  0,  0,  0, 0,  // 水平解像度
  0,  0,  0, 0,  // 垂直解像度
  0,  0,  0, 0,  // パレットの色数
  0,  0,  0, 0,
};


mkBMP::mkBMP(char *fname, int y_size)
{
  fileName = fname;
  ySize    = y_size;
}

void mkBMP::add(colorSetDef color, int dotCount)
{
  dotLineDef wk;
  wk.color = color;
  wk.dotCount = dotCount;

  pixLines.push_back(wk);
}

void mkBMP::make()
{
   xSize = 0;
   for(auto eachValue : pixLines) xSize += eachValue.dotCount;

   hedder[18] = xSize % 0x100;
   hedder[19] = xSize / 0x100;

   hedder[22] = ySize % 0x100;
   hedder[23] = ySize / 0x100;

   int imageDataSize = xSize * ySize * 3; // 正味の画像データ
   imageDataSize += (xSize % 4) * ySize;

   hedder[34] = imageDataSize % 0x100;
   hedder[35] = imageDataSize / 0x100;

   int fileSize = imageDataSize + 54;
   hedder[2] = fileSize % 0x100;
   hedder[3] = fileSize / 0x100;

   std::FILE *fp = std::fopen(fileName.c_str(), "wb");

   for(int i = 0; i < 54; i++)
     std::putc(hedder[i], fp);

   int dotCount = 0;

   for(int i = 0; i < ySize; i++)
   {
     for(auto eachValue : pixLines)
     {
        for(int k = 0; k < eachValue.dotCount; k++)
        {
          std::putc(eachValue.color.B, fp);
          std::putc(eachValue.color.G, fp);
          std::putc(eachValue.color.R, fp);
          dotCount += 3;
        }
     }

     while(dotCount % 4)
     {
        std::putc(0, fp);
        dotCount++;
     }
     dotCount = 0;
   }
}
