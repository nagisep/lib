
#include "sekki.h";

extern const sekki::nameDef sekkiName[];
extern const sekki::nameDef zassetsuName[];
extern const sekki::nameDef moonPhaseName[];
extern const sekki::nameDef kou72Name[];

static double koukeiAddOffset(double koukei)
{
   if (koukei < 8) koukei += 360;
   return koukei;
}

static TDateTime get_a_SekkiTime(TDateTime day, int koukei, TDateTime dayEnd = 0)
{
   // 与えられた日に、与えられた黄経を通過する場合、その時刻を返す
   // 通過しない場合は、0 を返す。

   if (static_cast<int>(dayEnd) == 0) dayEnd = day;

   TDateTime today = static_cast<int>(day);
   TDateTime endDate = static_cast<int>(dayEnd);
   const double error = static_cast<double>(TDateTime(0, 0, 1, 0));

   if (koukei == 0) koukei = 360;

   TimePhasePaireDef last;
   last.timePos  = today - TDateTime(0, 30, 0, 0);
   last.phase    = koukeiAddOffset(sun(last.timePos).koudou_axis().keido) - koukei;

   TimePhasePaireDef next;
   next.timePos  =  endDate + 1;
   next.timePos  += TDateTime(0, 30, 0, 0);
   next.phase    = koukeiAddOffset(sun(next.timePos).koudou_axis().keido) - koukei;

   TimePhasePaireDef naka;

   if ((last.phase < 0) && (next.phase > 0))
   { // 今日の付近に、一致点がありそう
     while(1)
     {
       naka.timePos = (static_cast<double>(last.timePos) + static_cast<double>(next.timePos)) / 2;
       naka.phase   = koukeiAddOffset(sun(naka.timePos).koudou_axis().keido) - koukei;

       if (naka.phase == 0)     break;
       else if (naka.phase > 0) next = naka;
       else                     last = naka;
   
       if (static_cast<double>(next.timePos) - static_cast<double>(last.timePos) <= error) break;
	 }

	 if (static_cast<int>(today)   > static_cast<int>(naka.timePos)) return 0;
	 if (static_cast<int>(endDate) < static_cast<int>(naka.timePos)) return 0;
     return naka.timePos;
   }
   return 0;
}

struct kyuuTsukiType
{
  TDateTime saku;
  TDateTime misoka;
};

static kyuuTsukiType getSakuMisoka(TDateTime point)
{
  kyuuTsukiType result;

  int nextSaku = static_cast<int>(sun_moon(point).getNewMoon().next);
  if (nextSaku == static_cast<int>(point))
  {
    // point の日が、実は朔日
    result.saku = nextSaku;
    result.misoka = static_cast<int>(sun_moon(nextSaku + 1).getNewMoon().next);
    result.misoka--;
  }
  else
  {
	result.saku = static_cast<int>(sun_moon(point).getNewMoon().last);
	result.misoka = nextSaku - 1;
  }

  return result;
}

sekki::nameDef sekki::getName(sekki::sekkiDateType wk)
{
  static sekki::nameDef noname;
  noname.name = "";
  noname.comment = "";

  switch(wk.kind)
  {
    case sekki::isNone      : return noname;
    case sekki::is24Sekki   : return sekkiName[wk.index];
    case sekki::is72Kou     : return kou72Name[wk.index];
    case sekki::isZassetsu  : return zassetsuName[wk.index];
    case sekki::isMoonEvent : return moonPhaseName[wk.index];
  }
  return noname;
}

sekki::sekkiDateType sekki::get24Sekki(TDateTime aDate, nameDef *namePtr)
{
  static TDateTime checkedDate = 0;
  static sekkiDateType result;

  if (static_cast<int>(aDate) != static_cast<int>(checkedDate))
  {
    checkedDate = aDate;
    result = sekkiDateType(isNone);
    for (int i = 0; i < 24; i++)
    {
      TDateTime wk =  get_a_SekkiTime(aDate, i * 15);
      if (static_cast<int>(wk) != 0)
      {
        result = sekkiDateType(is24Sekki, i, wk, true);
        break;
      }
    }
  }

  if (namePtr) (*namePtr) = getName(result);
  return result;
}


sekki::sekkiDateType sekki::get72kou(TDateTime aDate, nameDef *namePtr)
{
  static TDateTime checkedDate = 0;
  static sekkiDateType result;

  if (static_cast<int>(aDate) != static_cast<int>(checkedDate))
  {
    checkedDate = aDate;
    aDate = static_cast<int>(aDate);
    aDate += TDateTime(23, 59, 59, 0); // 今日の最後
    int lng = sun(aDate).koudou_axis().keido;
    result = sekkiDateType(is72Kou, lng / 5, aDate, false);
  }

  if (namePtr) (*namePtr) = getName(result);
  return result;
}

struct checkZassetsuType
{
  int koukei;        // 基準となる節気の黄経
  int diff;          // 基準となる節気からの日数
  bool timeIsValid;  // 時刻をつけて返す雑節
};

const checkZassetsuType kokeiForZassetsu[] = 
{
  { 297,  0, true}, // index =  0 土用入り（春）
  {  27,  0, true}, // index =  1 土用入り（夏）
  { 117,  0, true}, // index =  2 土用入り（秋）
  { 207,  0, true}, // index =  3 土用入り（冬）

  {  80,  0, true}, // index =  4 入梅
  { 100,  0, true}, // index =  5 半夏生

  {   0,  3, false}, // index =  6 彼岸開け
  { 180,  3, false}, // index =  7 彼岸開け
  {   0, -3, false}, // index =  8 彼岸入り
  { 180, -3, false}, // index =  9 彼岸入り

// 210日などの 「～日」は、立春の日を含めてカウントしているので、引き算する数字を一つ少なくしておく必要がある 210日 → 立春を含めて、210日 → 立春の 209日後
  { 315,  -1, false}, //index = 10 節分
  { 315,  87, false}, //index = 11 八十八夜

  { 315, 209, false}, //index = 12 二百十日
  { 315, 219, false}, //index = 13 二百二十日

  {-1, 0, false},   // リストにはないが、雑節のひとつとして、 index = 14 は「八朔」とする。
  {-1, 0, false}    // リストにはないが、雑節のひとつとして、 index = 15 は「夏越」とする。

};

const int hassakuIndex = 14;
const int nagoshiIndex = 15;
// 日付で決定されるスペシャル雑節
// 14 = 八朔と、 15 = 夏越

sekki::sekkiDateType sekki::getZassetsu(TDateTime aDate, nameDef *namePtr)
{
  static TDateTime checkedDate = 0;
  static sekkiDateType result;

  if (static_cast<int>(aDate) != static_cast<int>(checkedDate))
  {
    checkedDate = aDate;
    result = sekkiDateType(isNone);

    for(int i = 0; kokeiForZassetsu[i].koukei >= 0; i++)
    {
      int baseDateData = static_cast<int>(aDate);
      baseDateData -= kokeiForZassetsu[i].diff;
      
      TDateTime wk =  get_a_SekkiTime(baseDateData, kokeiForZassetsu[i].koukei);
      if (static_cast<int>(wk) != 0)
      {
        result = sekkiDateType(isZassetsu, i, wk, kokeiForZassetsu[i].timeIsValid);
        if (namePtr) (*namePtr) = getName(result);
        return result;
      }
    }

    // 八朔の探索
    // ここまでで、「今日」が確定していれば、ここには処理は来ない
    kyuuDateType wk = toKyuureki(aDate);
    if (wk.day == 1 && wk.month == 8)
    {
      result = sekkiDateType(isZassetsu, hassakuIndex, aDate, false);
      if (namePtr) (*namePtr) = getName(result);
      return result;
    }
    
    // 夏越の探索。「晦日」を検出する必要があるので、当日が「６月」で、翌日が「１日」を探索する
    if (wk.month == 6 && wk.isMisoka)
    {
      result = sekkiDateType(isZassetsu, nagoshiIndex, aDate, false);
      if (namePtr) (*namePtr) = getName(result);
      return result;
    }
  }

  // 日付のキャッシュにヒットしてしまったときでも、名前をセットできるように、ここでもセットする。
  if (namePtr) (*namePtr) = getName(result);
  return result;
}


sekki::sekkiDateType sekki::getMoonEvent(TDateTime aDate, nameDef *namePtr)
{
  static sekkiDateType result;
  static TDateTime checkedDate = 0;

  if (static_cast<int>(aDate) != static_cast<int>(checkedDate))
  {
    checkedDate = aDate;
    result = sekkiDateType(isNone);
    TDateTime aDay = static_cast<int>(aDate);
    aDay++; // 翌日の 00:00 をポイントする
    sun_moon nowMoon(aDay);
    for(int i = 0; i < 4; i++)
    {
      TDateTime aTime = nowMoon.getMoonTime(i).last;
      if (static_cast<int>(aTime) == static_cast<int>(aDate)) // 明日の朝の直前のイベントが今日なら
      {
         result = sekkiDateType(isMoonEvent, i, aTime, true);
         break;
      }
    }
  }

  if (namePtr) (*namePtr) = getName(result);
  return result;
}

sekki::sekkiDateType sekki::getSekkiEventName(TDateTime aDate, nameDef *namePtr)
{
   sekkiDateType (*eventFunc[])(TDateTime aDate, nameDef *namePtr) = {get24Sekki, getZassetsu, getMoonEvent, get72kou, 0};
   sekkiDateType result;
   // sekkiDateType は、デフォルトコンストラクタで kind == isNone に設定される

   for (int i = 0; eventFunc[i]; i++)
   {
     result = eventFunc[i](aDate, namePtr);
     if (result.kind != isNone) return result;
   }

   return result;
}

