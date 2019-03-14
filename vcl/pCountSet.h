
#if !defined(BCL_PCOUNT_SET)
#define      BCL_PCOUNT_SET

#include <StdCtrls.hpp>
#include <vector>

template <class intType> struct LabelCountPairDef
{
  TLabel *label;
  intType *count;
  LabelCountPairDef(TLabel *aLabel, intType *aCount) : label(aLabel), count(aCount) { }
};

template <class intType> class countSetType
{
  private:
    std::vector <LabelCountPairDef<intType> > countList;
 
  public:
    void disp()
    {
      for(int i = 0; i < countList.size(); i++)
        countList[i].label->Caption = static_cast<int>(*countList[i].count);
    }

    void push(LabelCountPairDef<intType> aPair) 
    { 
      countList.push_back(aPair);
      disp();
    }
    
    void push(TLabel *aLabel, intType *aCount)
    { 
       countList.push_back(LabelCountPairDef<intType>(aLabel, aCount));
       disp();
    }

	void clear(bool withConfirm = true)
	{
	  if ((! withConfirm) || Application->MessageBox(L"Do you want to reset couner ?", L"info", MB_YESNO) == IDYES)
		for(int i = 0; i < countList.size(); i++)
		  *countList[i].count = 0;

	  disp();
	}

};

#endif

