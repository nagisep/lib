
#include "astrob.h"
#include <iostream>

int wmain()
{
  ido_keido chijou;
  chijou.ido   = 35.0167;
  chijou.keido = 135.75;

  TDateTime today = TDateTime(2015, 3, 31);
  TDateTime wk = sun(today).minami(chijou);

  
  std::cout << "ans. " << static_cast<double>(wk);

  return 0;
}
