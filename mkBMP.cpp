
#include "mkBMP.h"
#include <cstdio>

unsigned char hedder[] =
{
  'B', 'M',
  100, 0, 0, 0,   // file size 4byte ���ʃr�b�g����
  0, 0, 0, 0,     // �\��̈�
  54, 0, 0, 0,    // data �܂ł̃I�t�Z�b�g

  // ��������f�[�^�w�b�_
  40, 0, 0, 0,
  5,  0, 0, 0,   // �摜�̕�
  2,  0, 0, 0,   // �摜�̍���
  1,  0,         // �v���[����
  24, 0,         // �J���[�r�b�g��
  0,  0,  0, 0,  // �񈳏k
  30, 0,  0, 0,  // �摜�f�[�^�̃T�C�Y �����~���~�R�iRGB�j
  0,  0,  0, 0,  // �����𑜓x
  0,  0,  0, 0,  // �����𑜓x
  0,  0,  0, 0,  // �p���b�g�̐F��
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

   int imageDataSize = xSize * ySize * 3; // �����̉摜�f�[�^
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
