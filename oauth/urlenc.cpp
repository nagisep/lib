
#include <string>
#include <cstring>

#include "urlenc.h"

void URLEnc(const char *inBuff, char *outBuff, bool delChar, bool without_sharp)
{
  char *nonEscape_01 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.~";
  char *nonEscape_02 = "%abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_.~";
  // �������́A�G���R�[�h�~�X�̋~�ϗp�Ȃ̂ŁA% �́i���ɃG���R�[�h����Ă�����̂́j�ăG���R�[�h���Ȃ�

  char *nonEscape = (without_sharp) ? nonEscape_02 :  nonEscape_01;

  std::string line = "";
  while(*inBuff)
  {
    char c;
    char lBuff[16];
    std::string orgLine = "";

    c = *inBuff++;

    if (std::strchr(nonEscape, c))
    {
      lBuff[0] = c;
      lBuff[1] = '\0';
    }
    else if (! delChar)
    { int i = c;
      i &= 0x00ff;
      std::sprintf(lBuff, "%%%02X", i);
    }
    else
      lBuff[0] = '\0';

    line += lBuff;
  }

  std::sprintf(outBuff, "%s", line.c_str());

}

