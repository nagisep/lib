
// permanent counter
// �֘A�v�f�� c:\user\BCL_permanent_counter.ini �i�Œ�j��
// �ۑ����邱�Ƃŉi���I�J�E���^����������

// [] �ɂ́A�A�v���P�[�V�������̂̃t�@�C�����i__argv[0]�j���g��

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
    int get()      const {return myCount;} // �Öق̌^�ϊ������܂������Ȃ����p

    void operator ++(int);
    void operator --(int);

    void operator +=(int);
    void operator -=(int);

    void operator =(int);
};

#endif

