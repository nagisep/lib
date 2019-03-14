
#include "kyuureki.h"

static int toMonth(int degree)
{
  int result = degree / 30;
  result += 2;
  if (result > 12) result -= 12;
  return result;
}


kyuuDateType toKyuureki(TDateTime today, int *chuuCount)
{
  kyuuDateType result;

  today = static_cast<int>(today);

  unsigned short year, month, day;
  today.DecodeDate(&year, &month, &day);
  // 最後の、「旧暦の年」判定のために、デコードしておく

  // 日付の算出
  result.day = sun_moon(today).ageInt() + 1;
  result.isMisoka = (sun_moon(today).ageInt(true) == -1) ? true : false;

  // 月の算出
  // 月の算出に先立ち、today をインクリメントする
  // 朔の時間算出の基準は、その日の終わり（＝翌日の最初）である必要がある
  // 直前の朔は、旧暦一日なら、その日の中にあるので、本来は、その日の 23:59.99 
  today ++;
  
  sun_moon::timePairType newMoonPair = sun_moon(today).getNewMoon();
  
  // ここで得られた時刻をもとに、旧暦の一ヶ月を区切る。
  // ただし、一ヶ月の区切りは「朔を含む日の午前0時」なので、時刻は 0:00 にしておく必要がある。
  newMoonPair.last = static_cast<int>(newMoonPair.last);
  double lastDegree = sun(newMoonPair.last).koudou_axis().keido;

  // 終わりの黄経ポイントは、旧暦の翌月一日の 0:00 でなければならない
  newMoonPair.next = static_cast<int>(newMoonPair.next);
  double nextDegree = sun(newMoonPair.next).koudou_axis().keido;

  if (nextDegree < lastDegree) nextDegree += 360; // 360度を横切った時の回避

  int cntChuu = 0;
  int memberChuu[2];

  // 中の探索
  for (int iDegree = 0; iDegree < 420; iDegree += 30)
  { // 360度をまたがるケースに対応するため、角度の上限は 420度
    if(lastDegree <= iDegree && iDegree < nextDegree)
	{
	  memberChuu[cntChuu++] = iDegree % 360;
	}
  }

  if (chuuCount) *chuuCount = cntChuu;

  if (cntChuu == 1)
  {
    result.month = toMonth(memberChuu[0]);
    result.isUruu = false;
  }
  else if (cntChuu == 2)
  { 
    int setMonth = -1;
    if (memberChuu[0] % 90 == 0) setMonth = memberChuu[0];
    if (memberChuu[1] % 90 == 0) setMonth = memberChuu[1];
    if (setMonth < 0) setMonth = memberChuu[0]; 
    // 月が決まらない → ひとつ前の中が月決定の中
    // 今月は、（月の決定された）前月の翌月なので、最初の中が該当する

     result.month = toMonth(setMonth);
     result.isUruu = false;
  }
  else if (cntChuu == 0)
  {
     int cnt;
     // 前月の状況を調べないと決定できない
     TDateTime dateWk = newMoonPair.last - 1;    // 前月の晦日に帰り
     kyuuDateType wk = toKyuureki(dateWk, &cnt); // 前月の月をもらう
     switch(cnt)
     {
       case 0: // error
               result.month = 0;
               result.isUruu = false;
               break;

       case 1: // 今月は閏
               result.month = wk.month;
               result.isUruu = true;
               break;

       case 2: // 今月は前月の翌月
               result.month = wk.month + 1;
               if (result.month > 12) result.month -= 12;
               result.isUruu = false;
      }
  }

  // 最後に年を決定する
  // 旧暦の月＞新暦の月なら、「去年」と見なす。

  result.year = year;
  if(result.month > month) result.year--;
  return result;
}

