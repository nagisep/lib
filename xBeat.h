#if !defined(_BCL_LIB_XBEAT)
#define      _BCL_LIB_XBEAT

#include <system.hpp>

namespace xBeat
{
  double xBeat(TDateTime now = TDateTime().CurrentDateTime());
  AnsiString xBeatStr(TDateTime now);
  AnsiString xBeatStr();

  // xBeatStr �̂ق��́A�������t���������g���ƁA�Ȃ����A�R���p�C����������B
  // xBeat()����������Ȃ�
}

#endif


