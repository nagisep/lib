
#include "xBeat.h"
#include <SysUtils.hpp>

char *week[] =  {"???", "Sun,", "Mon,", "Tue,", "Wed,", "Thu,", "Fri,", "Sat,"};
char *month[] = {"???", "Jan ", "Feb ", "Mar ", "Apr ", "May ", "Jun ", "Jul ",
						"Aug ", "Sep ", "Oct ", "Nov ", "Dec "};

double xBeat::xBeat(TDateTime now)
{
   now -= TDateTime(8, 0, 0, 0);
   return (static_cast<double>(now) - static_cast<int>(now)) * 1000;
}

AnsiString xBeat::xBeatStr()
{ return xBeatStr(Now()); }

AnsiString xBeat::xBeatStr(TDateTime now)
{
   AnsiString beatValue;
   unsigned short yyyy, mmmm, dddd;
   double beatTime = xBeat(now);

   now -= TDateTime(8, 0, 0, 0);
   now.DecodeDate(&yyyy, &mmmm, &dddd);

   beatValue += week[now.DayOfWeek()];
   beatValue += FormatFloat(" 0# ", dddd);
   beatValue += month[mmmm];
   beatValue += yyyy;
   beatValue += " @";
   beatValue += FormatFloat("00#", beatTime);

   return beatValue;
}


