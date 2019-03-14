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

   static unsigned long number();                                          // �f�o�C�X�̑������擾
   static struct deviceListType getADevice(int n);               // n �Ԗڂ̃f�o�C�X�̏�񃊃X�g�����
   static void getDeviceList(std::vector<deviceListType> *deviceList);
   static bool isHere(int vid, int pid, const char *ser);       // �����𖞂����f�o�C�X�����݂��邩�ǂ�����Ԃ�

   CP2102(unsigned short vid, unsigned short pid, const char *pSeri);  // Vied, Pid, device serial ���w�肵�ăI�[�v��
   CP2102(unsigned long n);                                      // CP210x_GetNumDevices() �œ����� No. ���w�肵�ăI�[�v��
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
                                      // CP210x_SetMaxPower �̑�Q�����́A2mA �P�ʂ̎w��Ȃ̂ŁA2 �Ŋ������l���Z�b�g���Ă���

   bool setSelfPower()              { return CP210x_SetSelfPower(handle, TRUE) == CP210x_SUCCESS; }
   bool setBusPower()               { return CP210x_SetSelfPower(handle, FALSE) == CP210x_SUCCESS; }

   void reset()                     { CP210x_Reset(handle); Sleep(1500); }
   // reset -> close �̃V�[�P���X�ɂ́A1000ms �ȏ�̃f�B���C���K�v
   // ���̃f�B���C�� reset() �̒��Ŋm�ۂ��Ă���
};

#endif


