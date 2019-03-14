
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
  // �Ō�́A�u����̔N�v����̂��߂ɁA�f�R�[�h���Ă���

  // ���t�̎Z�o
  result.day = sun_moon(today).ageInt() + 1;
  result.isMisoka = (sun_moon(today).ageInt(true) == -1) ? true : false;

  // ���̎Z�o
  // ���̎Z�o�ɐ旧���Atoday ���C���N�������g����
  // ��̎��ԎZ�o�̊�́A���̓��̏I���i�������̍ŏ��j�ł���K�v������
  // ���O�̍�́A�������Ȃ�A���̓��̒��ɂ���̂ŁA�{���́A���̓��� 23:59.99 
  today ++;
  
  sun_moon::timePairType newMoonPair = sun_moon(today).getNewMoon();
  
  // �����œ���ꂽ���������ƂɁA����̈ꃖ������؂�B
  // �������A�ꃖ���̋�؂�́u����܂ޓ��̌ߑO0���v�Ȃ̂ŁA������ 0:00 �ɂ��Ă����K�v������B
  newMoonPair.last = static_cast<int>(newMoonPair.last);
  double lastDegree = sun(newMoonPair.last).koudou_axis().keido;

  // �I���̉��o�|�C���g�́A����̗�������� 0:00 �łȂ���΂Ȃ�Ȃ�
  newMoonPair.next = static_cast<int>(newMoonPair.next);
  double nextDegree = sun(newMoonPair.next).koudou_axis().keido;

  if (nextDegree < lastDegree) nextDegree += 360; // 360�x�����؂������̉��

  int cntChuu = 0;
  int memberChuu[2];

  // ���̒T��
  for (int iDegree = 0; iDegree < 420; iDegree += 30)
  { // 360�x���܂�����P�[�X�ɑΉ����邽�߁A�p�x�̏���� 420�x
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
    // �������܂�Ȃ� �� �ЂƂO�̒���������̒�
    // �����́A�i���̌��肳�ꂽ�j�O���̗����Ȃ̂ŁA�ŏ��̒����Y������

     result.month = toMonth(setMonth);
     result.isUruu = false;
  }
  else if (cntChuu == 0)
  {
     int cnt;
     // �O���̏󋵂𒲂ׂȂ��ƌ���ł��Ȃ�
     TDateTime dateWk = newMoonPair.last - 1;    // �O���̊A���ɋA��
     kyuuDateType wk = toKyuureki(dateWk, &cnt); // �O���̌������炤
     switch(cnt)
     {
       case 0: // error
               result.month = 0;
               result.isUruu = false;
               break;

       case 1: // �����͉[
               result.month = wk.month;
               result.isUruu = true;
               break;

       case 2: // �����͑O���̗���
               result.month = wk.month + 1;
               if (result.month > 12) result.month -= 12;
               result.isUruu = false;
      }
  }

  // �Ō�ɔN�����肷��
  // ����̌����V��̌��Ȃ�A�u���N�v�ƌ��Ȃ��B

  result.year = year;
  if(result.month > month) result.year--;
  return result;
}

