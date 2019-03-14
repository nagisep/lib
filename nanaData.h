
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

    // n 階層までをソートする
    // n = 0 はトップ階層。デフォルトの  n = 1 は、最初の子階層
    // ただし、body には手を触れない
    // n < 0 の場合、全階層をソートする
    void sort(int n = 1); 
    void sortAll() { sort(-1); }

    // nodeTitleList で指定される vecotr をcurrentNode とする。
    void setNode(std::vector<std::string> nodeTitleList);
    void setNode(std::string nodeList);

    // ボディテキストの追加
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
