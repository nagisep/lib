
#include "futeiji.h"

// nowTime の値を、locate を参照しながら、不定時法の時刻と見なして、TDateTime 型で返す。
// JST 専用

#include <u:/lib/libs.h>

TDateTime futeiji(TDateTime nowTime, ido_keido locate)
{
  static TDateTime todayAke  =  0;
  static TDateTime todayKure =  0;
  static TDateTime lastKure =  0;
  static TDateTime nextAke =  0;

  static TDateTime checkToday = 0;

  TDateTime today = static_cast<int>(nowTime);

  if(libs::isNotEqualAndSet<TDateTime>(&checkToday, today))
  {
   // 必要なもの
   // 日付が変われば、また、取得する
   // 当日の夜明け（明け六つ）と日暮れ（暮れ六つ）
   todayAke  =  sun(today).ake(locate).front();
   todayKure =  sun(today).kure(locate).front();

   // 追加・前日の暮れ六つ
   lastKure =  sun(today - 1).kure(locate).front();

   // さらに、翌日の明け六つ
   nextAke =  sun(today + 1).ake(locate).front();
  }

  TDateTime offSet;
  TDateTime start;
  TDateTime end;

  if (lastKure < nowTime && nowTime <= todayAke)
  {
    offSet  = today - 1;
    offSet  += TDateTime(18, 0, 0, 0);
    start   = lastKure;
    end     = todayAke;
  }
  else if (todayAke < nowTime && nowTime <= todayKure)
  {
    offSet  = today + TDateTime(6, 0, 0, 0);
    start   = todayAke;
    end     = todayKure;
  }
  else
  {
    offSet  = today + TDateTime(18, 0, 0, 0);
    start   = todayKure;
    end     = nextAke;
  }

  double timeLen = end - start;
  double lenFromStart = nowTime - start;
  lenFromStart /= timeLen;
  lenFromStart *= 0.5;
  offSet += lenFromStart;

  return offSet;
}
