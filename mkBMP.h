
#if !defined(_BCL_MKBMP)
#define      _BCL_MKBMP

#include <string>
#include <vector>

struct colorSetDef
{
  int R;
  int G;
  int B;
};

struct dotLineDef
{
  colorSetDef color;
  int dotCount;
};

class mkBMP
{
  private:
   std::string fileName;
   std::vector<dotLineDef> pixLines;

   int xSize;
   int ySize;

  public:
   mkBMP(char *fname, int ySize);
   void add(colorSetDef color, int dotCount);
   void make();
};

#endif
