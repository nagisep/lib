
#include <iostream>


#include <cmath>
#include "astroB.h"

sun_moon::timePairType sun_moon::moonPhasePos[4] = {{28, 0, 0}, {7, 0, 0}, {14, 0, 0}, {21, 0, 0}};

static const double dToR = 3.141592653 / 180.0;

static double norm360(double value)  // 0 .. 360 に正規化
{
  while(value < 0) value += 360;
  return std::fmod(value, 360.0);
}

static double norm180(double value)  // -180 .. 180 に正規化
{
  value = norm360(value);
  if (value > 180) value -= 360;
  return value;
}

double sun_moon::phase(bool addOffset) const
{
  double rikaku = norm360(moon(now).koudou_axis().keido - sun(now).koudou_axis().keido);
  double phaseValue = rikaku / (360.0 / 28.0);
  if ((addOffset) && (phaseValue < 3.5)) phaseValue += 28.0; 

  return phaseValue;
}

double sun_moon::ratio() const
{
  double rikaku = norm360(moon(now).koudou_axis().keido - sun(now).koudou_axis().keido);
  rikaku *= dToR;
  return (1.0 - (1.0 + std::cos(rikaku)) / 2.0) * 100.0;
}

static bool within(double a, double b, double c)
{
   if ((a < b) && (a < c)) return false;
   if ((a > b) && (a > c)) return false;
   return true;
}


// 未来に向けた探索 = 「今」は含まない
// 過去に向けた探索 = 「今」も含む
TDateTime sun_moon::findPoint(TDateTime now, bool toFuture, double phase)
{

  TimePhasePaireDef front;
  TimePhasePaireDef back;
  TimePhasePaireDef naka;
  TimePhasePaireDef wk;
  
  
  // front と back のポイントを探索する
  wk.timePos = now;
  wk.phase = sun_moon(wk.timePos).phase(true);

  if ((wk.phase == phase) && (! toFuture)) return wk.timePos;
  
  int direct; 
  double border;
  TimePhasePaireDef *nearPos;
  TimePhasePaireDef *farPos;
 
  if (toFuture)
  {
    direct =  1; // 時間は増加方向
    border = -3; // 最初、phaseより、ちょっと小さいポイントを探す
    nearPos   = &front; // 近いポイントは、「早い時刻」
    farPos    = &back;
  }
  else
  {
    direct = -1;
    border =  3;
    nearPos   = &back;
    farPos    = &front;
  }

  // 最初のポイントを探索・phase より未来（小さな）過去（大きな）ポイント
  while(1)
  {
    if (within(wk.phase, phase, phase + border)) break;
    wk.timePos += direct;
    wk.phase = sun_moon(wk.timePos).phase(true);
  }

  *nearPos = wk;

  // 次のポイント
  while(1)
    {
      if (within(wk.phase, phase, phase - border)) break;
      wk.timePos += direct;
      wk.phase = sun_moon(wk.timePos).phase(true);
    }

  *farPos = wk;
  // near と far の間に、狙うポイントがある

  double error = static_cast<double>(TDateTime(0, 0, 1, 0));
  
  while(1)
  {
    naka.timePos = (static_cast<double>(front.timePos) + static_cast<double>(back.timePos)) / 2;
    naka.phase = sun_moon(naka.timePos).phase(true) - phase;
    
    if (naka.phase == 0)      break;
    else if (naka.phase > 0)  back  = naka;
    else if (naka.phase < 0)  front = naka;

    if (static_cast<double>(back.timePos) - static_cast<double>(front.timePos) <= error) break;
  }

  return naka.timePos;
}


void sun_moon::makeMoonList() const
{
  for (int i = 0; i < 4; i++)
  {
     if((now >= moonPhasePos[i].last) && (now < moonPhasePos[i].next)) continue;
     // 今のリストでつじつまが合っている

     moonPhasePos[i].last = findPoint(now, false, moonPhasePos[i].phase); // now から過去にさかのぼって朔を探す
     moonPhasePos[i].next = findPoint(moonPhasePos[i].last + 1, true, moonPhasePos[i].phase); // 直前の朔から、未来向きに朔を探す
  }
}

sun_moon::timePairType sun_moon::getMoonTime(int index) const
{
  // 一応ちょっとでもスレッドセーフに近づけるために、使う前に、newMoonList を確認する。
  if(index < 0) index = 0;
  if(index > 3) index = 3;
  makeMoonList();
  return moonPhasePos[index];
}

double sun_moon::age() const
{
  return static_cast<double>(now) - static_cast<double>(getNewMoon().last);
}

int sun_moon::ageInt(bool misokaFlag) const
{

  int nextNewMoonDate = static_cast<int>(getNewMoon().next);
  int nowDate         = static_cast<int>(now);

  if (nextNewMoonDate == nowDate) return 0;
  if (misokaFlag && (nextNewMoonDate - nowDate == 1)) return -1;
  return nowDate - static_cast<int>(getNewMoon().last);
}


struct factorList
{
  double A;
  double B;
  double C;
};

static double calcFactors(struct factorList list[], double t)
{
  double result = 0;

  for (int i = 0; list[i].A >= 0; i++)
  {
     struct factorList *ptr = &list[i];
     result += ptr->A * std::sin(dToR * norm360(ptr->B  + ptr->C * t));
  }

  return result;
}

ido_keido sun::koudou_axis() const
{
    ido_keido result;

    struct factorList list[] =
    {
        {0.0003, 329.7, 44.43 },
        {0.0003, 352.5, 1079.97 },
        {0.0004, 21.1, 720.02 },
        {0.0004, 157.3, 299.30 },
        {0.0004, 234.9, 315.56 },
        {0.0005, 291.2, 22.81 },
        {0.0005, 207.4, 1.50 },
        {0.0006, 29.8, 337.18 },
        {0.0007, 206.8, 30.35 },
        {0.0007, 153.3, 90.38 },
        {0.0008, 132.5, 659.29 },
        {0.0013, 81.4, 225.18 },
        {0.0015, 343.2, 450.37 },
        {0.0018, 251.3, 0.20 },
        {0.0018, 297.8, 4452.67 },
        {0.0020, 247.1, 329.64 },
        {0.0048, 234.95, 19.341 },
        {0.0200, 355.05, 719.981 },
        {-1, -1, -1},
    };

  double rm_sun = calcFactors(list, aTime::Julius_T(getNow()));
  rm_sun += (1.9146 - 0.00005 * aTime::Julius_T(getNow())) * std::sin( dToR * norm360( 357.538 + 359.991 * aTime::Julius_T(getNow()) ));
  rm_sun += norm360( 280.4603 + 360.00769 * aTime::Julius_T(getNow()) );

  result.keido =  norm360(rm_sun);
  result.ido   = 0;

  return result;
}

double sun::kyori() const
{
     struct factorList list[] =
    {
      {0.000007, 156, 329.6}, 
      {0.000007, 254, 450.4}, 
      {0.000013, 27.8, 4452.67}, 
      {0.000030, 90.0, 0}, 
      {0.000091, 265.1, 719.98}, 
      {-1, -1, -1}
    };

  double r_sun =  calcFactors(list, aTime::Julius_T(getNow()));
  r_sun += (0.007256 - 0.0000002 * aTime::Julius_T(getNow())) * std::sin( dToR * norm360( 267.54 + 359.991 * aTime::Julius_T(getNow()))) ;
  r_sun = std::pow(10,r_sun);

  return r_sun;
}

ido_keido moon::koudou_axis() const
{
    ido_keido result;

    struct factorList listBM[] =
    {
      {0.0005, 307, 19.4},
      {0.0026, 55.0, 19.34},
      {0.0040, 119.5, 1.33},
      {0.0043, 322.1, 19.36},
      {0.0267, 234.95, 19.341},
      {-1, -1, -1}
    };

    double bm = calcFactors(listBM, aTime::Julius_T(getNow()));

    struct factorList list_ido[] =
    {
      {0.0003, 234, 19268.0}, 
      {0.0003, 146, 3353.3}, 
      {0.0003, 107, 18149.4}, 
      {0.0003, 205, 22642.7}, 
      {0.0004, 147, 14097.4}, 
      {0.0004, 13, 9325.4}, 
      {0.0004, 81, 10242.6}, 
      {0.0004, 238, 23281.3}, 
      {0.0004, 311, 9483.9}, 
      {0.0005, 239, 4193.4}, 
      {0.0005, 280, 8485.3}, 
      {0.0006, 52, 13617.3}, 
      {0.0006, 224, 5590.7}, 
      {0.0007, 294, 13098.7}, 
      {0.0008, 326, 9724.1}, 
      {0.0008, 70, 17870.7}, 
      {0.0010, 18.0, 12978.66}, 
      {0.0011, 138.3, 19147.99}, 
      {0.0012, 148.2, 4851.36}, 
      {0.0012, 38.4, 4812.68}, 
      {0.0013, 155.4, 379.35}, 
      {0.0013, 95.8, 4472.03}, 
      {0.0014, 219.2, 299.96}, 
      {0.0015, 45.8, 9964.00}, 
      {0.0015, 211.1, 9284.69}, 
      {0.0016, 135.7, 420.02}, 
      {0.0017, 99.8, 14496.06}, 
      {0.0018, 270.8, 5192.01}, 
      {0.0018, 243.3, 8206.68}, 
      {0.0019, 230.7, 9244.02}, 
      {0.0021, 170.1, 1058.66}, 
      {0.0022, 331.4, 13377.37}, 
      {0.0025, 196.5, 8605.38}, 
      {0.0034, 319.9, 4433.31}, 
      {0.0042, 103.9, 18509.35}, 
      {0.0043, 307.6, 5470.66}, 
      {0.0082, 144.9, 3713.33}, 
      {0.0088, 176.7, 4711.96}, 
      {0.0093, 277.4, 8845.31}, 
      {0.0172, 3.18, 14375.997}, 
      {0.0326, 328.96, 13737.362}, 
      {0.0463, 172.55, 698.667}, 
      {0.0554, 194.01, 8965.374}, 
      {0.1732, 142.427, 4073.3220}, 
      {0.2777, 138.311, 60.0316}, 
      {0.2806, 228.235, 9604.0088}, 
      {-1, -1, -1}
    };

  double bt_moon = calcFactors(list_ido, aTime::Julius_T(getNow()));
  bt_moon +=  5.1282 * std::sin( dToR * norm360( 93.273 + 4832.0202 * aTime::Julius_T(getNow()) + bm));

  result.ido = bt_moon;

    struct factorList listAM[] =
    {
      {0.0040, 119.5,  1.33},
      {0.0020,  55.0, 19.34},
      {0.0006,  71.0,  0.2},
      {0.0006,  54.0, 19.3},
      {-1, -1, -1}
    };

    double Am = calcFactors(listAM, aTime::Julius_T(getNow()));

    struct factorList list[] =
    {
      {1.2740, 100.738, 4133.3536}, 
      {0.6583, 235.700, 8905.3422}, 
      {0.2136, 269.926, 9543.9773}, 
      {0.1856, 177.525, 359.9905}, 
      {0.1143, 6.546, 9664.0404}, 
      {0.0588, 214.22, 638.635}, 
      {0.0572, 103.21, 3773.363}, 
      {0.0533, 10.66, 13677.331}, 
      {0.0459, 238.18, 8545.352}, 
      {0.0410, 137.43, 4411.998}, 

      {0.0348, 117.84, 4452.671}, 
      {0.0305, 312.49, 5131.979}, 
      {0.0153, 130.84, 758.698}, 
      {0.0125, 141.51, 14436.092}, 
      {0.0110, 231.59, 4892.052}, 
      {0.0107, 336.44, 13038.696}, 
      {0.0100, 44.89, 14315.966}, 
      {0.0085, 201.5, 8266.71}, 
      {0.0079, 278.2, 4493.34}, 
      {0.0068, 53.2, 9265.32}, 

      {0.0052, 197.2, 319.32}, 
      {0.0050, 295.4, 4812.66}, 
      {0.0048, 235.0, 19.34}, 
      {0.0040, 13.2, 13317.34}, 
      {0.0040, 145.6, 18449.32}, 
      {0.0040, 119.5, 1.33}, 
      {0.0039, 111.3, 17810.68}, 
      {0.0037, 349.1, 5410.62}, 
      {0.0027, 272.5, 9183.99}, 
      {0.0026, 107.2, 13797.39}, 

      {0.0024, 211.9, 988.63}, 
      {0.0024, 252.8, 9224.66}, 
      {0.0022, 240.6, 8185.36}, 
      {0.0021, 87.5, 9903.97}, 
      {0.0021, 175.1, 719.98}, 
      {0.0021, 105.6, 3413.37}, 
      {0.0020, 55.0, 19.34}, 
      {0.0018, 4.1, 4013.29}, 
      {0.0016, 242.2, 18569.38}, 
      {0.0012, 339.0, 12678.71}, 

      {0.0011, 276.5, 19208.02}, 
      {0.0009, 218, 8586.0}, 
      {0.0008, 188, 14037.3}, 
      {0.0008, 204, 7906.7}, 
      {0.0007, 140, 4052.0}, 
      {0.0007, 275, 4853.3}, 
      {0.0007, 216, 278.6}, 
      {0.0006, 128, 1118.7}, 
      {0.0005, 247, 22582.7}, 
      {0.0005, 181, 19088.0}, 

      {0.0005, 114, 17450.7}, 
      {0.0005, 332, 5091.3}, 
      {0.0004, 213, 398.7}, 
      {0.0004, 278, 120.1}, 
      {0.0004, 71, 9584.7}, 
      {0.0004, 20, 720.0}, 
      {0.0003, 83, 3814.0}, 
      {0.0003, 66, 3494.7}, 
      {0.0003, 147, 18089.3}, 
      {0.0003, 311, 5492.0}, 

      {0.0003, 161, 40.7}, 
      {0.0003, 280, 23221.3}, 

      {-1, -1, -1}
    };

    double  moonRamda = 218.3161 + 4812.67881 * aTime::Julius_T(getNow())  + 6.2887 * std::sin(dToR * norm360(134.961 + 4771.9886 * aTime::Julius_T(getNow()) + Am));
    moonRamda += calcFactors(list, aTime::Julius_T(getNow()));

    result.keido =norm360(moonRamda);
    return result;
}

double moon::shisa() const
{
    struct factorList list[] =
    {
      {0.0003, 227, 4412.0}, 
      {0.0004, 194, 3773.4}, 
      {0.0005, 329, 8545.4}, 
      {0.0009, 100.0, 13677.3}, 
      {0.0028, 0.0, 9543.98}, 
      {0.0078, 325.7, 8905.34}, 
      {0.0095, 190.7, 4133.35}, 
      {0.0518, 224.98, 4771.989}, 
      {0.9507, 90, 0}, 
      {-1, -1, -1}
    };
  return calcFactors(list, aTime::Julius_T(getNow()));
}



// 経過ユリウス年の計算

double aTime::Julius_T(TDateTime now)
{
  unsigned short year, month, day;
  now.DecodeDate(&year, &month, &day);
  unsigned short yearPure = year;

  if ((month == 1) || (month == 2))
  {
     year--;
     month += 12;  // １月２月のパッチ
  }
  year -= 2000;

  double K_japan = 365.0 * year + 30.0 * month + day - 33.875 + std::floor(3.0 * (month + 1) / 5.0) + std::floor(year / 4.0);
  double deltaT = 57.0 + 0.8 * (yearPure - 1990);
  return (K_japan + aTime::Julius_D(now) + deltaT / 86400.0) / 365.25;
}

ido_keido astro::sekidou_axis() const
{
  ido_keido result;
  ido_keido taiyou = koudou_axis();

  taiyou.ido   *= dToR;
  taiyou.keido *= dToR;
  double epValue = aTime::ep(getNow()) * dToR;

  double sekkeiSin = std::sin(taiyou.ido) * std::cos(epValue) + std::cos(taiyou.ido) * std::sin(taiyou.keido) * std::sin(epValue);

  result.ido      = std::asin(sekkeiSin);  // 返値は ±90度（相当範囲）

  double sinSide  = std::cos(taiyou.ido) * std::sin(taiyou.keido) * std::cos(epValue) - std::sin(taiyou.ido) * std::sin(epValue);
  double cosSide  = std::cos(taiyou.ido) * std::cos(taiyou.keido);
  result.keido    = std::atan2(sinSide, cosSide);

  result.ido   /= dToR;
  result.keido /= dToR;

  return result;
}

// 地方恒星時の計算

double aTime::getKouseiJi(TDateTime now, ido_keido pos)
{
  double JT = aTime::Julius_T(now);
  double JD = aTime::Julius_D(now);
  double result = 325.4606 + 360.007700536 * JT +0.00000003879 * JT * JT + 360.0 * JD + pos.keido;
  return norm360(result);
}

double astro::getJikaku(ido_keido chijou) const
{
  ido_keido sekidou = sekidou_axis();
  double jikaku = aTime::getKouseiJi(now, chijou) - sekidou.keido;
  return norm180(jikaku);
}

double sun::shisa() const
{
  return 0.0024428 / kyori();
}


double astro::deiriKoudo(double fukaku)
{
  double S = hankeiHosei();      // 視半径
  double P = shisa();            // 視差
  double E = 0.0;                // 観測地点の高度補正（０に固定）
  return - S - E - fukaku + P;
}

double astro::akekureKoudo(double fukaku)
{ // 明け暮れの高度は、地平線下のため、大気による浮き上がり補正も不要
  // 本来、「月」には、無意味な定義ではある
  double P = shisa();
  double E = 0.0;
  return - fukaku - E + P;
}

double astro::koudo(ido_keido chijou) const
{
   ido_keido sekidou = sekidou_axis();
   double jikaku = getJikaku(chijou);
   
   double koudoSin = std::sin(sekidou.ido * dToR) * std::sin(chijou.ido * dToR)
                   + std::cos(sekidou.ido * dToR) * std::cos(chijou.ido * dToR) * std::cos(jikaku * dToR);
   return std::asin(koudoSin) / dToR;
}

double astro::houi(ido_keido chijou) const
{
  ido_keido sekidou = sekidou_axis();
  double jikaku = getJikaku(chijou);

  sekidou.ido *= dToR;
  jikaku      *= dToR;

  double sinSide  = - std::cos(sekidou.ido) * std::sin(jikaku);
  double cosSide  = std::sin(sekidou.ido) *  std::cos(chijou.ido * dToR) - std::cos(sekidou.ido) * std::sin(chijou.ido * dToR) * std::cos(jikaku);
  return norm360(std::atan2(sinSide, cosSide) / dToR);
}

TDateTime astro::minami(ido_keido chijou)
{
  TDateTime nowSave = now;


  TDateTime today = static_cast<int>(now);

  TimePhasePaireDef mae = {0, 0};
  TimePhasePaireDef ato = {0, 360};

  for(double time = 0; time < 1; time += 0.128)
  { // 二分法の最初の候補決定
    TDateTime wk = today + time;
    setDateTime(wk);
    double wkHoui = houi(chijou);
    if(wkHoui < 180 && mae.phase <= wkHoui)
    {
       mae.timePos = wk;
       mae.phase   = wkHoui;
    }
    else if (wkHoui >= 180 && ato.phase >= wkHoui)
    {
       ato.timePos = wk;
       ato.phase   = wkHoui;
    }
  }

  // 南中点がない
  setDateTime(nowSave); // ここで抜けてしまう可能性があるから
  if (static_cast<int>(mae.timePos) == 0) return 0;
  if (static_cast<int>(ato.timePos) == 0) return 0;
  if (static_cast<double>(mae.timePos) > static_cast<double>(ato.timePos)) return 0;

   // 二分法による南中点の探索
   TimePhasePaireDef naka;
   while(1)
   {
     naka.timePos = (static_cast<double>(mae.timePos) + static_cast<double>(ato.timePos)) / 2;
     setDateTime(naka.timePos);
     naka.phase = houi(chijou);
     if(naka.phase < 180)
        mae = naka;
     else
        ato = naka;
   
     if(std::fabs(static_cast<double>(ato.phase) - static_cast<double>(mae.phase)) < 0.01) break;
   }

   setDateTime(nowSave);
   return naka.timePos;
}


bool astro::isBorder(bool increase, double saki, double ato) const
{
   if (increase && (saki < 0) && (ato > 0)) return true; // 日の出の境界点がある
   if ((! increase) && (saki > 0) && (ato < 0)) return true; // 日の入りの境界点がある
   return false;
}

std::queue<TDateTime> astro::getAkeKureTime(ido_keido chijou, bool increase, double (astro::*koudo_kijun)(double fukaku), double fukaku)
{
   // この関数の中で、メンバー now を変更する。
   // 抜け出すときには、now を必ず復元すること。

   std::queue<TDateTime> result;

   TDateTime nowSave = now;
   TDateTime today = static_cast<int>(now);

   TDateTime saki = today - TDateTime(0, 30, 0, 0); // 前日の 23:30 あたりからスタートする
   TDateTime last = today + 1;
   last += TDateTime(1, 0, 0 ,0); // 翌日の 1:00 （この前でチェックをやめる）
   TDateTime ato;

   setDateTime(saki);

   double sakiKoudo = koudo(chijou) -  (this->*koudo_kijun)(fukaku);

   for (ato = today; ato < last; ato += TDateTime(0, 30, 0, 0))
   {
     setDateTime(ato);
     double atoKoudo   = koudo(chijou) -  (this->*koudo_kijun)(fukaku);

     if (atoKoudo == 0)
     { // 太陽は地平線ぴったりだが、登りと下りの区別が必要
       if (static_cast<int>(ato) == static_cast<int>(today)) // その時刻が、本当に、「今日」で
       {
         if (increase && (sakiKoudo < 0))     result.push(ato); // 昇りつつあるときの地平線ぴったり
         if ((! increase) && (sakiKoudo > 0)) result.push(ato); // 下りつつあるときの地平線ぴったり
       }
     }
     else if (isBorder(increase, sakiKoudo, atoKoudo))
     { // 出入りのポイントを見つけた

       TDateTime diff = TDateTime(0, 0, 1, 0);
       TDateTime naka;

       TDateTime checkSaki = saki;
       TDateTime checkAto  = ato;
       
       while(1)
       {
          TDateTime checkNaka   = (static_cast<double>(checkSaki) + static_cast<double>(checkAto)) / 2.0;
          setDateTime(checkNaka);
          double nakaKoudo  = koudo(chijou) -  (this->*koudo_kijun)(fukaku);

          if (nakaKoudo == 0)
          {
            checkSaki = checkAto = checkNaka; // たまたま、その瞬間になってしまったら、そこで計算終了
          }
          else if (nakaKoudo < 0) // 中央値が地平線下
          {
            if (increase) checkSaki = checkNaka; // まだ昇りきっていないので、こちらを、「先」に
            else          checkAto  = checkNaka; // 下ってしまっているので、こちらを、「後」に
          }
          else  // 中央値が地平線の上
          {
            if (increase) checkAto  = checkNaka;
            else          checkSaki = checkNaka;
          }

          if (static_cast<double>(checkAto) - static_cast<double>(checkSaki) <= static_cast<double>(diff))
          {
            if (static_cast<int>(checkNaka) == static_cast<int>(today)) // そのポイントが「今日」なら、答として採用
               result.push(checkNaka);

            break;
          }
       }
     }

     saki = ato;
     sakiKoudo = atoKoudo;
   }

   setDateTime(nowSave);
   return result;
}
