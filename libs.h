
#if !defined(_BLC_USER_LIBS)
#define      _BLC_USER_LIBS

namespace libs
{
  // �ύX�v�ۂ̃`�F�b�N�ȂǂɎg�p����
  // if (isNotEqaualAndSet<int>(*dest, src))
  // �̗p�Ɏg�p���āA�u�������Ȃ��Ƃ��Ɏ��s����v���Ƃ��ł���B
  // �`�F�b�N��A dest == src �ƂȂ�B
  // dest ������������꓾�邱�Ƃ𖾎����邽�߁Adest �́A�|�C���^�Ƃ��Ă���
  template <class T> bool isNotEqualAndSet(T *dest, T src)
  {
	bool result = (*dest != src);
	*dest = src;
	return result;
  }

  // target �� min �� max �̊Ԃɉ������߂�B
  // �P�Ɍ��E�𒴂���������؂�̂Ă邾��
  template <class T> void normalize(T *target, T min, T max)
  {
    if (*target < min) *target = min;
    if (*target > max) *target = max;
  }

  // ��萔�ȉ��̃t�B�{�i�b�`�����������������
  // �ʐM���g���C�Ԋu�̐����ȂǂɎg�p����B
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

