
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
   // �^����ꂽ���ɁA�^����ꂽ���o��ʉ߂���ꍇ�A���̎�����Ԃ�
   // �ʉ߂��Ȃ��ꍇ�́A0 ��Ԃ��B

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
   { // �����̕t�߂ɁA��v�_�����肻��
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
    // point �̓����A���͍��
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
    aDate += TDateTime(23, 59, 59, 0); // �����̍Ō�
    int lng = sun(aDate).koudou_axis().keido;
    result = sekkiDateType(is72Kou, lng / 5, aDate, false);
  }

  if (namePtr) (*namePtr) = getName(result);
  return result;
}

struct checkZassetsuType
{
  int koukei;        // ��ƂȂ�ߋC�̉��o
  int diff;          // ��ƂȂ�ߋC����̓���
  bool timeIsValid;  // ���������ĕԂ��G��
};

const checkZassetsuType kokeiForZassetsu[] = 
{
  { 297,  0, true}, // index =  0 �y�p����i�t�j
  {  27,  0, true}, // index =  1 �y�p����i�āj
  { 117,  0, true}, // index =  2 �y�p����i�H�j
  { 207,  0, true}, // index =  3 �y�p����i�~�j

  {  80,  0, true}, // index =  4 ���~
  { 100,  0, true}, // index =  5 ���Đ�

  {   0,  3, false}, // index =  6 �ފ݊J��
  { 180,  3, false}, // index =  7 �ފ݊J��
  {   0, -3, false}, // index =  8 �ފݓ���
  { 180, -3, false}, // index =  9 �ފݓ���

// 210���Ȃǂ� �u�`���v�́A���t�̓����܂߂ăJ�E���g���Ă���̂ŁA�����Z���鐔��������Ȃ����Ă����K�v������ 210�� �� ���t���܂߂āA210�� �� ���t�� 209����
  { 315,  -1, false}, //index = 10 �ߕ�
  { 315,  87, false}, //index = 11 ���\����

  { 315, 209, false}, //index = 12 ��S�\��
  { 315, 219, false}, //index = 13 ��S��\��

  {-1, 0, false},   // ���X�g�ɂ͂Ȃ����A�G�߂̂ЂƂƂ��āA index = 14 �́u����v�Ƃ���B
  {-1, 0, false}    // ���X�g�ɂ͂Ȃ����A�G�߂̂ЂƂƂ��āA index = 15 �́u�ĉz�v�Ƃ���B

};

const int hassakuIndex = 14;
const int nagoshiIndex = 15;
// ���t�Ō��肳���X�y�V�����G��
// 14 = ����ƁA 15 = �ĉz

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

    // ����̒T��
    // �����܂łŁA�u�����v���m�肵�Ă���΁A�����ɂ͏����͗��Ȃ�
    kyuuDateType wk = toKyuureki(aDate);
    if (wk.day == 1 && wk.month == 8)
    {
      result = sekkiDateType(isZassetsu, hassakuIndex, aDate, false);
      if (namePtr) (*namePtr) = getName(result);
      return result;
    }
    
    // �ĉz�̒T���B�u�A���v�����o����K�v������̂ŁA�������u�U���v�ŁA�������u�P���v��T������
    if (wk.month == 6 && wk.isMisoka)
    {
      result = sekkiDateType(isZassetsu, nagoshiIndex, aDate, false);
      if (namePtr) (*namePtr) = getName(result);
      return result;
    }
  }

  // ���t�̃L���b�V���Ƀq�b�g���Ă��܂����Ƃ��ł��A���O���Z�b�g�ł���悤�ɁA�����ł��Z�b�g����B
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
    aDay++; // ������ 00:00 ���|�C���g����
    sun_moon nowMoon(aDay);
    for(int i = 0; i < 4; i++)
    {
      TDateTime aTime = nowMoon.getMoonTime(i).last;
      if (static_cast<int>(aTime) == static_cast<int>(aDate)) // �����̒��̒��O�̃C�x���g�������Ȃ�
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
   // sekkiDateType �́A�f�t�H���g�R���X�g���N�^�� kind == isNone �ɐݒ肳���

   for (int i = 0; eventFunc[i]; i++)
   {
     result = eventFunc[i](aDate, namePtr);
     if (result.kind != isNone) return result;
   }

   return result;
}

