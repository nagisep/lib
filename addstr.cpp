
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
   char buff[4096];

   std::string pre = "";
   std::string post = "";
   
   if (argc > 1)
   {
      pre = argv[1];
      pre += " ";
   }
  
   if (argc > 2)
   {
      post = " ";
      post += argv[2];
   }

   while(1)
   {
      std::string line = "";
      std::cin >> buff;
      if (! std::cin) break;
      line = pre + buff + post;
      std::cout << line.c_str() << "\n";
   }
   return 0;
}
