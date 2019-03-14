
#include "nanaData.h"

// src を delim 毎に分割して dst に格納する
void split(const std::string& src, std::vector<std::string>& dst, const std::string& delim, const bool getNull=false)
{
  std::string::size_type lastPos = 0;
  std::string::size_type foundPos;
  std::string::size_type delimLen = delim.size();
  while( (foundPos=src.find(delim,lastPos))!=std::string::npos )
  {
    if( ! getNull && foundPos==lastPos )
    {
      lastPos += delimLen;
      continue;
    }
    dst.push_back( src.substr(lastPos,foundPos-lastPos) );
    lastPos = foundPos+delimLen;
  }
  // 最終検索位置が分割対象の最後尾まで達していない場合
  if( lastPos < src.size()-1 )
  {   // 最終検索位置から後ろをベクターに格納
      dst.push_back( src.substr(lastPos) );
  }
}

void nanaData::pop_node(nodeType node, int level)
{
  for(int i = 0; i < level; i++)
    otf << ".";

  otf << node.name.c_str() << "\n";

  for(auto p : node.body) otf << (p.c_str()) << "\n";
  for(auto p : node.child) pop_node(p, level + 1);


}

void nanaData::addLine(std::string aLine)
{
  if (currentNode) currentNode->body.push_back(aLine);
}

void nanaData::find_node(nodeType *node, std::vector<std::string> nodeTitleList, unsigned int level)
{
  if (level >= nodeTitleList.size())
  { // タイトルリストはすべてマッチした
    currentNode = node;
    return;
  }

  nodeType *nodePtr = 0;
  std::string tergetName = nodeTitleList[level];
  std::vector<nodeType> &check = node->child;

  for(std::vector<nodeType>::iterator p = check.begin(); p < check.end(); p++)
  {
    if(p->name == tergetName) 
    {  
       nodePtr = &(*p);
       break;
    }
  }

  if (! nodePtr)  // 一致する名前がなかった
  {
    nodeType nextNode;
    nextNode.name = tergetName;
    check.push_back(nextNode);
    nodePtr = &check.back();
  }
  find_node(nodePtr, nodeTitleList, level + 1);
}


void nanaData::setNode(std::vector<std::string> nodeTitleList)
{
  unsigned int level = 0;
  if(nodeTitleList.size() == 0) return;
  nodeType *nodePtr = 0;

  std::string tergetName = nodeTitleList[0];

  for(std::vector<nodeType>::iterator p = base.begin(); p < base.end(); p++)
  {
    if(p->name == tergetName) 
    {  
       nodePtr = &(*p);
       break;
    }
  }

  if (! nodePtr)  // 一致する名前がなかった
  {
    nodeType nextNode;
    nextNode.name = tergetName;
    base.push_back(nextNode);
    nodePtr = &base.back();
  }
  find_node(nodePtr, nodeTitleList, level + 1);
}

void nanaData::setNode(std::string indexList)
{
  std::vector<std::string> indexVector;
  split(indexList, indexVector, "/");
  setNode(indexVector);
}

void nanaData::save()
{
  otf.open(fileName.c_str());

  int level = 1;
  for (std::vector<nodeType>::iterator p = base.begin(); p < base.end(); p++)
    pop_node(*p, level);
}

void nanaData::sortChild(std::vector<nodeType> &v, int n, int level)
{
  if ((n > 0) && (level >= n)) return;
  
  for(std::vector<nodeType>::iterator ptr = v.begin(); ptr < v.end(); ptr++)
  {
    std::sort(ptr->child.begin(), ptr->child.end());
    sortChild((ptr->child), n, level + 1);
  }
}

void nanaData::sort(int n)
{
  std::sort(base.begin(), base.end());
  sortChild(base, n, 1);
}

