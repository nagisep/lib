
#if !defined(_BLC_USER_LIBS)
#define      _BLC_USER_LIBS

namespace libs
{
  // 変更要否のチェックなどに使用する
  // if (isNotEqaualAndSet<int>(*dest, src))
  // の用に使用して、「等しくないときに実行する」ことができる。
  // チェック後、 dest == src となる。
  // dest が書き換えられ得ることを明示するため、dest は、ポインタとしている
  template <class T> bool isNotEqualAndSet(T *dest, T src)
  {
	bool result = (*dest != src);
	*dest = src;
	return result;
  }

  // target を min と max の間に押し込める。
  // 単に限界を超えた部分を切り捨てるだけ
  template <class T> void normalize(T *target, T min, T max)
  {
    if (*target < min) *target = min;
    if (*target > max) *target = max;
  }

  // 一定数以下のフィボナッチ数列を順次生成する
  // 通信リトライ間隔の生成などに使用する。
  class fibSequence
  {
    private:
      int n1;
      int n2;
      int max;
      bool isOver() { return ((max > 0) && (n1 > max)); }

    public:
      fibSequence(int m_max = 0) : n1(0), n2(1), max(m_max) { }
      void clear() {n1 = 0; n2 = 1;}

      int get()
      {
        int wk = n1 + n2;
        n1 = n2;
        n2 = wk;
        if (isOver()) n1 = n2 = max;
        return n1;
      }
  };

}

#endif

