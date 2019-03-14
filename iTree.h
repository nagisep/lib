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
    // root ワードを指定して、fName のファイルを生成する
    ~iTree();

    void writeRecord(char *header, char *body);
    // ヘッダと本文を指定して、１レコードを書き込む
    // ヘッダの階層は、\x01 で区切る（root ワードはつけない）
};

#endif

