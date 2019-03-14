
#include <system.hpp>
#include "moonPhase.h"

TDateTime sakuMoonTimeList[128];
static int maxList;

static int searchIndex(TDateTime nowTime)
{
  int i;
  for (i = 0; i < maxList; i++)
  {
	if (sakuMoonTimeList[i] > nowTime) break;
  }
  return i - 1;
}


void moonInit()
{

  maxList = 0;

  sakuMoonTimeList[maxList++] = TDateTime(2009, 1, 26)+TDateTime(16, 55, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2009, 2, 25)+TDateTime(10, 35, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2009, 3, 27)+TDateTime(1, 06, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2009, 4, 25)+TDateTime(12, 23, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2009, 5, 24)+TDateTime(21, 11, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2009, 6, 23)+TDateTime(4, 35, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2009, 7, 22)+TDateTime(11, 35, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2009, 8, 20)+TDateTime(19, 2, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2009, 9, 19)+TDateTime(3, 44, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2009, 10, 18)+TDateTime(14, 33, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2009, 11, 17)+TDateTime(4, 14, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2009, 12, 16)+TDateTime(21, 2, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 1, 15)+TDateTime(16, 11, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 2, 14)+TDateTime(11, 51, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 3, 16)+TDateTime(6, 1, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 4, 14)+TDateTime(21, 29, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 5, 14)+TDateTime(10, 4, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 6, 12)+TDateTime(20, 15, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 7, 12)+TDateTime(4, 40, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 8, 10)+TDateTime(12, 8, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 9, 8)+TDateTime(19, 30, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 10, 8)+TDateTime(3, 44, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 11, 6)+TDateTime(13, 52, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2010, 12, 6)+TDateTime(2, 36, 0, 0);
  sakuMoonTimeList[maxList++] = TDateTime(2099, 12, 31)+TDateTime(23, 59, 0, 0);
}

double getMoonPhase(TDateTime nowTime)
{
  static int    listIndex = 0;

  if (nowTime >= sakuMoonTimeList[listIndex + 1]) listIndex = searchIndex(nowTime);
  if (nowTime < sakuMoonTimeList[listIndex]) listIndex = searchIndex(nowTime);
 
  double base = static_cast<double>(sakuMoonTimeList[listIndex]);
  double now  = static_cast<double>(nowTime);

  return now - base;
}

