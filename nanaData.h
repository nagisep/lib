
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

struct nodeType
{
  std::string name;
  std::vector<nodeType> child;
  std::vector<std::string> body;
  bool operator <(nodeType another)
  {
    return name < another.name;
  }
};

class nanaData
{
  private:
    void find_node(nodeType *nodePtr, std::vector<std::string> nodeTitleList, unsigned int level);
    void pop_node(nodeType node, int level);

    std::string fileName;
    std::vector<nodeType> base;
    nodeType *currentNode;
    
    std::ofstream otf;
    void sortChild(std::vector<nodeType> &v, int n, int level);

  public:

    nanaData(std::string afName) : fileName(afName)
    { 
      currentNode = nullptr;
    }

    void save();

    // n �K�w�܂ł��\�[�g����
    // n = 0 �̓g�b�v�K�w�B�f�t�H���g��  n = 1 �́A�ŏ��̎q�K�w
    // �������Abody �ɂ͎��G��Ȃ�
    // n < 0 �̏ꍇ�A�S�K�w���\�[�g����
    void sort(int n = 1); 
    void sortAll() { sort(-1); }

    // nodeTitleList �Ŏw�肳��� vecotr ��currentNode �Ƃ���B
    void setNode(std::vector<std::string> nodeTitleList);
    void setNode(std::string nodeList);

    // �{�f�B�e�L�X�g�̒ǉ�
    void addLine(std::string line);
    void operator +=(std::string line)
    { addLine(line); }

    nanaData &operator[](std::vector<std::string> index) 
    {
      setNode(index);
      return *this;
    }

    nanaData &operator[](std::string index)
    {
      setNode(index);
      return *this;
    }
};
