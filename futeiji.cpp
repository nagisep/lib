
#include "futeiji.h"

// nowTime �̒l���Alocate ���Q�Ƃ��Ȃ���A�s�莞�@�̎����ƌ��Ȃ��āATDateTime �^�ŕԂ��B
// JST ��p

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
   // �K�v�Ȃ���
   // ���t���ς��΁A�܂��A�擾����
   // �����̖閾���i�����Z�j�Ɠ����i���Z�j
   todayAke  =  sun(today).ake(locate).front();
   todayKure =  sun(today).kure(locate).front();

   // �ǉ��E�O���̕��Z��
   lastKure =  sun(today - 1).kure(locate).front();

   // ����ɁA�����̖����Z��
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
