
#if !defined(_BCL_ASTRO_B)
#define      _BCL_ASTRO_B

#include <queue>

#include <system.hpp>

  struct TimePhasePaireDef
  {
     TDateTime timePos;
     double    phase;
  };

 struct ido_keido
 {
   double ido;
   double keido;
 };

namespace aTime
{
  double Julius_T(TDateTime now);
  double Julius_D(TDateTime now)  {return static_cast<double>(now) - static_cast<int>(now);}
  double getKouseiJi(TDateTime now, ido_keido pos);
  double ep(TDateTime now) {return 23.439291 - 0.0001300042 * Julius_T(now);}
};


class astro
{
   private:

     TDateTime now;

     bool isBorder(bool increse, double saki, double ato) const;
     // 日の出・日の入り（など）の境界点有無を返す

     std::queue<TDateTime> getAkeKureTime(ido_keido chijou, bool increase, double (astro::*koudo_kijun)(double fukaku), double fukaku);
     // 内部で、now を（一時的に）変更するので、const ではない

     void setDateTime(TDateTime input_now) {now = input_now;}

   protected:
     TDateTime getNow() const {return now; }

   public:
     astro(TDateTime input_now) {setDateTime(input_now); }
     double getJikaku(ido_keido chijou) const;

     virtual ido_keido koudou_axis()    const = 0;
     virtual double kyori()             const = 0;
     virtual double shisa()             const = 0;

	 double deiriKoudo(double fukaku);     // fukaku は関数の形を合わせるため・実際には使用しない
	 double akekureKoudo(double fukaku);   // fukaku の規定値は明六つ・暮六つに相当

     virtual double hankeiHosei()       const = 0; // 出没計算の際の視半径分補正値（月は視半径補正しないので常にゼロを返す）


     ido_keido sekidou_axis() const;

     double koudo(ido_keido chijou) const;
     double houi(ido_keido chijou) const;

     // 与えられた日付での南中時刻を返す
     // 南中しない場合、 0 を返す
     TDateTime minami(ido_keido chijou);

     std::queue<TDateTime>  de(ido_keido chijou) { return getAkeKureTime(chijou, true,  &astro::deiriKoudo, 0.585556); }
     std::queue<TDateTime> iri(ido_keido chijou) { return getAkeKureTime(chijou, false, &astro::deiriKoudo, 0.585556); }

     std::queue<TDateTime> ake(ido_keido chijou, double fukaku = 7.361)  { return getAkeKureTime(chijou, true,  &astro::akekureKoudo, fukaku); }
     std::queue<TDateTime> kure(ido_keido chijou, double fukaku = 7.361) { return getAkeKureTime(chijou, false, &astro::akekureKoudo, fukaku); }
};

class sun : public astro
{
   public:
     sun(TDateTime input_now) : astro(input_now) { }

     virtual ido_keido koudou_axis() const;
     virtual double kyori() const;
     virtual double shisa() const;

     virtual double hankeiHosei() const {return 0.266994 / kyori();}
};

class moon : public astro
{
   public:
     moon(TDateTime input_now) : astro(input_now) { }

     virtual ido_keido koudou_axis() const;
     virtual double kyori() const { return 0;}
     virtual double shisa() const;

     virtual double hankeiHosei() const {return 0;}
     // 月は、中心点で出没を判定するので、視半径補正は不要（常にゼロ）
};

class sun_moon
{

  public:
     struct timePairType
     {
       double phase;
       TDateTime last;
       TDateTime next;
     };

  private:
     static timePairType moonPhasePos[4];
     // 新月・上弦・満月・下弦

     static TDateTime findPoint(TDateTime now, bool toFuture, double phase);

     void  makeMoonList() const;
     const TDateTime now;

  public:
     sun_moon(TDateTime input_now) : now(input_now) { }

     double phase(bool addOffset = false) const; 
     // addOffset は、主に朔（など）の時刻算出用
     // 朔の瞬間における月相の不連続を（見かけ上）なくすため、
     // addOffset == true なら、0 - (28) を、3.5 - (31.5) に変換する
     double age() const;
     int ageInt(bool misokaFlag = false) const;
     // misokaFlag が true の時、朔の前日には、月齢として -1 （晦日として使用）を返す

     double ratio() const;
     // 輝面比（100% に換算したもの）

     timePairType getMoonTime(int index) const;

     timePairType getNewMoon()  const {return getMoonTime(0);}
     timePairType getFullMoon() const {return getMoonTime(2);}
     timePairType getFirstHalfMoon() const {return getMoonTime(1);}
     timePairType getLastHalfMoon()  const  {return getMoonTime(3);}
};

#endif

