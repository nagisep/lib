
#include <iostream>

#include "pCount.h"

void putLine(pCount &c1, pCount &c2)
{
  std::cout << "OK = " << c1 << " and NG = " << c2 << "\n";
}

int main()
{
   pCount OK("OK");
   pCount NG("NG");

   putLine(OK, NG);

   OK++;
   putLine(OK, NG);

   NG++;
   putLine(OK, NG);

   OK += 2;
   NG -= 3;
   putLine(OK, NG);

   return 0;
}



