

#include "pCount.h"

const char *iniFile = "c:/user/BCL_permanent_counter.ini";

void pCount::getCount()
{
   TIniFile *ini = new TIniFile(iniFile);

   if (__argv)
     myCount = ini->ReadInteger(__argv[0], myCountName.c_str(), 0);
   else
     myCount = ini->ReadInteger(_wargv[0], myCountName.c_str(), 0);

   delete ini;
}

void pCount::putCount()
{
   TIniFile *ini = new TIniFile(iniFile);

   if (__argv)
     ini->WriteInteger(__argv[0], myCountName.c_str(), myCount);
   else
     ini->WriteInteger(_wargv[0], myCountName.c_str(), myCount);

   delete ini;
}

pCount::pCount(const char *countName)
{
   myCountName = countName;
   getCount();
}

pCount::~pCount()
{  }


void pCount::operator ++(int)
{
   myCount++;
   putCount();
}


void pCount::operator --(int)
{
   myCount--;
   putCount();
}

void pCount::operator +=(int v)
{
   myCount += v;
   putCount();
}

void pCount::operator -=(int v)
{
   myCount -= v;
   putCount();
}

void pCount::operator =(int v)
{
   myCount = v;
   putCount();
}

