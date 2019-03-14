

#if !defined(_BCL_BOT_SEKKI_BASE_VAL)
#define      _BCL_BOT_SEKKI_BASE_VAL

#include <system.hpp>
#include <vector>
#include <u:/lib/astrob.h>
#include <u:/lib/kyuureki.h>


namespace sekki
{

    struct nameDef
    {
      char *name;
      char *comment;
    };

    enum sekkiKindType {isNone, is24Sekki, is72Kou, isZassetsu, isMoonEvent, isElse};

    struct sekkiDateType
    {  
       sekkiKindType kind;
       int           index;
       TDateTime     dayTime;
       bool          timeIsValid;

       sekkiDateType(sekkiKindType m_kind = isNone, int m_index = 0, TDateTime m_dayTime = 0, bool m_valid = false):
         kind(m_kind), index(m_index), dayTime(m_dayTime), timeIsValid(m_valid) { }
    };

    // aDate の日に該当するものがあれば、その情報を返す。
    // さらに、namePtr != 0 のとき、そこに、name と comment を持ったデータを返す
    sekkiDateType get24Sekki(TDateTime aDate, nameDef *namePtr = 0);
    sekkiDateType get72kou(TDateTime aDate, nameDef *namePtr = 0);
    sekkiDateType getZassetsu(TDateTime aDate, nameDef *namePtr = 0);
    sekkiDateType getMoonEvent(TDateTime aDate, nameDef *namePtr = 0);
    sekkiDateType getSekkiEventName(TDateTime aDate, nameDef *namePtr = 0);

    nameDef       getName(sekki::sekkiDateType wk);

};

#endif

