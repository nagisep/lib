
#if !defined(_BCL_ASTRO_KYU)
#define      _BCL_ASTRO_KYU

#include <u:/lib/astrob.h>

struct kyuuDateType 
{
   int year;
   int month;
   int day;
   bool isUruu;
   bool isMisoka;
};

kyuuDateType toKyuureki(TDateTime today, int *chuuCount = 0);

#endif

