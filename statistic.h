
#if !defined(_BCL_LIB_STATISTIC)
#define      _BCL_LIB_STATISTIC

#include <cmath>

class notEnough
{ };

class statistic
{
   private:
     int    total;
     double sum;
     double x2Sum;

   public:
     statistic() { clear();  }

     void clear() 
     {
        total = 0;
        sum   = 0;
        x2Sum = 0;
     }

     void add(double v, int n = 1)
     {
        total += n;
        sum += (n * v);
        x2Sum += v * v * n;
     }

     statistic & operator +(double v)
     { 
       add(v);
       return *this;
     }

     void operator +=(double v)
     {
        *this = *this + v;
     }

     int count() {return total; }

     double average() throw (notEnough)
     {
        if (total) return (sum / total);
        throw (notEnough());
     }

     double variance() throw (notEnough)
     {
        if (total) return (x2Sum - total * average() * average()) / total;
        throw (notEnough());
     }

     double stdevia() throw (notEnough)
     {
        if (total) return std::sqrt(variance());
        throw (notEnough());
     }
 
};


#endif

