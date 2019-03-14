#if !defined(_BCL_LIBS_TREE)
#define      _BCL_LIBS_TREE

#include <fstream>

class iTree
{
  private:
    std::ofstream *otf;
    char myRoot[1024];

  public:
    iTree(char *fName, char *root);
    // root ���[�h���w�肵�āAfName �̃t�@�C���𐶐�����
    ~iTree();

    void writeRecord(char *header, char *body);
    // �w�b�_�Ɩ{�����w�肵�āA�P���R�[�h����������
    // �w�b�_�̊K�w�́A\x01 �ŋ�؂�iroot ���[�h�͂��Ȃ��j
};

#endif

