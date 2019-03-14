
#include "strlib.h"

AnsiString toStr(int v, int n)
{
   AnsiString result = AnsiString::FormatFloat("##################0,",v);
   if (n > 0)
        while(result.Length() < n)  result = " " + result;
   return result;
}


