#if !defined(__BCL_ASTRO)
#define      __BCL_ASTRO

#include <system.hpp>
#include <cmath>

const double AUDistance = 149597870.691;

class astro
{
  private:
    void ut2jd();
    // 結果は、jDate, jTime に書き出す

    long jDate;
    double jTime;

    virtual void pos() = 0;
    // jDate, jTime をもとに、lng, lat, distance を決定する

  protected:

    double distance;
    double lng;
    double lat;

    TDateTime pureDate; // 引数そのもの
    TDateTime myDate;   // UTC に変換したもの

    astro(TDateTime dtime);
    void init() {pos();}

  public:
    double getJulius()     const {return jDate + jTime; }
    double getDistance()   const {return distance;}                  // 距離（単位 km）
    double getAUDistance() const {return distance / AUDistance;}     // 距離（単位 AU）
    double getLng()        const {return lng;}                       // 経度
    double getLat()        const {return lat;}                       // 緯度
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
    double getPhase() const;  // 月相を返す
    double getAge()   const;  // 月齢を返す
};

#endif
