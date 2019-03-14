#if !defined(_BCL_CP2102_DEVICE)
#define      _BCL_CP2102_DEVICE

#include <windows.h>
#include <string>
#include <vector>
#include "CP210xManufacturingDLL.h"


struct deviceListType
{   int vid;
    int pid;
    int portNo;
    std::string deviceSerial;
    std::string productString;
};

class CP2102
{
 private:
   HANDLE handle;  
   char serialNumber[128];
   char productString[128];

 public:

   static int portNo(int vid, int pid, const char* ser);

   static unsigned long number();                                          // デバイスの総数を取得
   static struct deviceListType getADevice(int n);               // n 番目のデバイスの情報リストを入手
   static void getDeviceList(std::vector<deviceListType> *deviceList);
   static bool isHere(int vid, int pid, const char *ser);       // 条件を満たすデバイスが存在するかどうかを返す

   CP2102(unsigned short vid, unsigned short pid, const char *pSeri);  // Vied, Pid, device serial を指定してオープン
   CP2102(unsigned long n);                                      // CP210x_GetNumDevices() で得られる No. を指定してオープン
   ~CP2102();

   bool isValid() { return (handle);} 


   // ... get 
   const char *getSerialNumber();
   const char *getProductString();

   int getPid();
   int getVid();
   int getMaxPower();  // return by mA

   bool isSelfPower();
   bool isBusPower() { return ! isSelfPower(); }

   #include <cstring>
   // ... set
   bool setSerialNumber(char *val)  { return CP210x_SetSerialNumber(handle, val, std::strlen(val)) == CP210x_SUCCESS; }
   bool setProductString(char *val) { return CP210x_SetProductString(handle, val, std::strlen(val)) == CP210x_SUCCESS; }

   bool setPid(int val)            { return CP210x_SetPid(handle, val) == CP210x_SUCCESS; }
   bool setVid(int val)             { return CP210x_SetVid(handle, val) == CP210x_SUCCESS; }
   bool setMaxPower(val)            { return CP210x_SetMaxPower(handle, val / 2) == CP210x_SUCCESS; } 
                                      // CP210x_SetMaxPower の第２引数は、2mA 単位の指定なので、2 で割った値をセットしておく

   bool setSelfPower()              { return CP210x_SetSelfPower(handle, TRUE) == CP210x_SUCCESS; }
   bool setBusPower()               { return CP210x_SetSelfPower(handle, FALSE) == CP210x_SUCCESS; }

   void reset()                     { CP210x_Reset(handle); Sleep(1500); }
   // reset -> close のシーケンスには、1000ms 以上のディレイが必要
   // そのディレイは reset() の中で確保しておく
};

#endif


