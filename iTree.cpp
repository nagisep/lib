#include "iTree.h"
#include <cstring>
#include <cstdio>

iTree::iTree(char *fName, char *root)
{
  otf = new std::ofstream(fName);
  (*otf) << root << '\x05' << '0' << '\x06' << '1' << '\n';
  std::strcpy(myRoot, root);
}

iTree::~iTree()
{
  delete otf;
}

void iTree::writeRecord(char *header, char *body)
{
  char hBuff[1024];

  // root + 1 ƒŒƒxƒ‹‚È‚ç open , ‚»‚êˆÈŠO‚È‚ç close ‚ÌŽ}‚É‚·‚é

  std::sprintf(hBuff, "%s\x01%s", myRoot, header);
  if (std::strchr(header, '\x01') == 0)
  { // 1 level
         (*otf) << hBuff << '\x05' << '0' << '\x06' << '1' << '\x07' << '0' << '\n';
  }
  else
  {
    // ‚»‚êˆÈŠO
         (*otf) << hBuff << '\x05' << '0' << '\x06' << '0' << '\x07' << '0' << '\n';
  }

  (*otf) << "\x04\x0d\x0a";
  for(int i = 0; i < std::strlen(body); i++)
  {
     if (body[i] == '\n')
       (*otf) << "\x0d\x0a";
     else
       (*otf) << body[i];
  }
    (*otf) << "\x0d\x0d\x0a";
    (*otf) << "\x04\x0d\x0a";

};
