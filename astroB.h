
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
     // ���̏o�E���̓���i�Ȃǁj�̋��E�_�L����Ԃ�

     std::queue<TDateTime> getAkeKureTime(ido_keido chijou, bool increase, double (astro::*koudo_kijun)(double fukaku), double fukaku);
     // �����ŁAnow ���i�ꎞ�I�Ɂj�ύX����̂ŁAconst �ł͂Ȃ�

     void setDateTime(TDateTime input_now) {now = input_now;}

   protected:
     TDateTime getNow() const {return now; }

   public:
     astro(TDateTime input_now) {setDateTime(input_now); }
     double getJikaku(ido_keido chijou) const;

     virtual ido_keido koudou_axis()    const = 0;
     virtual double kyori()             const = 0;
     virtual double shisa()             const = 0;

	 double deiriKoudo(double fukaku);     // fukaku �͊֐��̌`�����킹�邽�߁E���ۂɂ͎g�p���Ȃ�
	 double akekureKoudo(double fukaku);   // fukaku �̋K��l�͖��Z�E��Z�ɑ���

     virtual double hankeiHosei()       const = 0; // �o�v�v�Z�̍ۂ̎����a���␳�l�i���͎����a�␳���Ȃ��̂ŏ�Ƀ[����Ԃ��j


     ido_keido sekidou_axis() const;

     double koudo(ido_keido chijou) const;
     double houi(ido_keido chijou) const;

     // �^����ꂽ���t�ł̓쒆������Ԃ�
     // �쒆���Ȃ��ꍇ�A 0 ��Ԃ�
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
     // ���́A���S�_�ŏo�v�𔻒肷��̂ŁA�����a�␳�͕s�v�i��Ƀ[���j
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
     // �V���E�㌷�E�����E����

     static TDateTime findPoint(TDateTime now, bool toFuture, double phase);

     void  makeMoonList() const;
     const TDateTime now;

  public:
     sun_moon(TDateTime input_now) : now(input_now) { }

     double phase(bool addOffset = false) const; 
     // addOffset �́A��ɍ�i�Ȃǁj�̎����Z�o�p
     // ��̏u�Ԃɂ����錎���̕s�A�����i��������j�Ȃ������߁A
     // addOffset == true �Ȃ�A0 - (28) ���A3.5 - (31.5) �ɕϊ�����
     double age() const;
     int ageInt(bool misokaFlag = false) const;
     // misokaFlag �� true �̎��A��̑O���ɂ́A����Ƃ��� -1 �i�A���Ƃ��Ďg�p�j��Ԃ�

     double ratio() const;
     // �P�ʔ�i100% �Ɋ��Z�������́j

     timePairType getMoonTime(int index) const;

     timePairType getNewMoon()  const {return getMoonTime(0);}
     timePairType getFullMoon() const {return getMoonTime(2);}
     timePairType getFirstHalfMoon() const {return getMoonTime(1);}
     timePairType getLastHalfMoon()  const  {return getMoonTime(3);}
};

#endif

