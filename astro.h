#if !defined(__BCL_ASTRO)
#define      __BCL_ASTRO

#include <system.hpp>
#include <cmath>

const double AUDistance = 149597870.691;

class astro
{
  private:
    void ut2jd();
    // ���ʂ́AjDate, jTime �ɏ����o��

    long jDate;
    double jTime;

    virtual void pos() = 0;
    // jDate, jTime �����ƂɁAlng, lat, distance �����肷��

  protected:

    double distance;
    double lng;
    double lat;

    TDateTime pureDate; // �������̂���
    TDateTime myDate;   // UTC �ɕϊ���������

    astro(TDateTime dtime);
    void init() {pos();}

  public:
    double getJulius()     const {return jDate + jTime; }
    double getDistance()   const {return distance;}                  // �����i�P�� km�j
    double getAUDistance() const {return distance / AUDistance;}     // �����i�P�� AU�j
    double getLng()        const {return lng;}                       // �o�x
    double getLat()        const {return lat;}                       // �ܓx
};

class sun : public astro
{
  private:
    virtual void pos();

  public:
    sun(TDateTime dtime);
};

class moon : public astro
{
  private:
    virtual void pos();

    static void makeNewMoonList(TDateTime now);

	static TDateTime lastNewMoonTime;
    static TDateTime nextNewMoonTime;

  public:
	static TDateTime getLastNewMoon(TDateTime now);
    static TDateTime getNextNewMoon(TDateTime now);

    moon(TDateTime dtime);
    double getPhase() const;  // ������Ԃ�
    double getAge()   const;  // �����Ԃ�
};

#endif
