#if !defined(_BCL_LIB_XBEAT)
#define      _BCL_LIB_XBEAT

#include <system.hpp>

namespace xBeat
{
  double xBeat(TDateTime now = TDateTime().CurrentDateTime());
  AnsiString xBeatStr(TDateTime now);
  AnsiString xBeatStr();

  // xBeatStr のほうは、初期化付き引数を使うと、なぜか、コンパイラがこける。
  // xBeat()もかもしれない
}

#endif


