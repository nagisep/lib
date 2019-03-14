
// permanent counter
// 関連要素を c:\user\BCL_permanent_counter.ini （固定）に
// 保存することで永続的カウンタを実現する

// [] には、アプリケーション自体のファイル名（__argv[0]）を使う

#if !defined(_BCL_pCount)
#define      _BCL_pCount

#include <iniFiles.hpp>
#include <string>

class pCount
{

  private:
    int  myCount;
    std::string myCountName;

    void getCount();
    void putCount();

  public:
    pCount(const char *countName);
    ~pCount();

    operator int() const {return myCount;}
    int get()      const {return myCount;} // 暗黙の型変換がうまくいかない時用

    void operator ++(int);
    void operator --(int);

    void operator +=(int);
    void operator -=(int);

    void operator =(int);
};

#endif

