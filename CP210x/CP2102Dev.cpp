
#include <string>
#include <cstdio>
#include "CP2102Dev.h"


bool CP2102::isHere(int vid, int pid, const char* ser)
{
   CP2102 dev(vid, pid, ser);
   if (dev.isValid()) return true;
   else               return false;
}

int CP2102::portNo(int vid, int pid, const char* ser)
{

    // AN197 Åi http://www.silabs.com/public/documents/tpub_doc/anote/Microcontrollers/Interface/en/AN197.pdf Åj
    // Ç…ÇÊÇÈ
    HKEY tmpKey;
    HKEY tmpPortKey;

    char portKeyString[1024];
    DWORD valtype;
    char* portString;
    DWORD length = 100;
	portString = new char[101];
    int portNum = -1;

	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet", 0 , KEY_READ, &tmpKey))
    {
//       std::sprintf(portKeyString, "Enum\\USB\\Vid_%04x&Pid_%04x&Mi_00\\%s_00\\Device Parameters\\",vid, pid, ser);
	   std::sprintf(portKeyString, "Enum\\USB\\Vid_%04x&Pid_%04x\\%s\\Device Parameters\\",vid, pid, ser);
	   if (ERROR_SUCCESS == RegOpenKey(tmpKey, portKeyString, &tmpPortKey))
	   {
         if (ERROR_SUCCESS == RegQueryValueEx(tmpPortKey, "PortName", NULL, &valtype, (unsigned char *)portString, &length));
         {
            portNum = atoi(portString + 3);
         }
         // Make sure to close all open keys for cleanup
         RegCloseKey(tmpPortKey);
       }
       RegCloseKey(tmpKey);
    }
    RegCloseKey(HKEY_LOCAL_MACHINE);

    return portNum;
}


unsigned long CP2102::number()
{
   unsigned long num;
   if (CP210x_GetNumDevices(&num) != CP210x_SUCCESS) return 0;
   else return num;
}


void CP2102::getDeviceList(std::vector<deviceListType> *deviceList)
{
   CP2102 *ptr;
   deviceListType aDev;

   deviceList->clear();
   for(unsigned long i = 0; i < number(); i++)
   {
      ptr = new CP2102(i);
      if(ptr->isValid())
      {
        aDev.vid = ptr->getVid();
        aDev.pid = ptr->getPid();
        aDev.portNo = portNo(ptr->getVid(), ptr->getPid(), ptr->getSerialNumber());
        aDev.deviceSerial = ptr->getSerialNumber();
        aDev.productString = ptr->getProductString();
        deviceList->push_back(aDev);
     }
     delete ptr;
   }
}

CP2102::CP2102(unsigned long n)
{
   handle = 0;
   if (n >= number()) return;
   // ãKíËÇÃîÕàÕì‡Çæ
   if (CP210x_Open(n, &handle) != CP210x_SUCCESS)
   { // open Ç…é∏îsÇµÇΩ
     handle = 0;
     return;
   }
}

CP2102::CP2102(unsigned short vid, unsigned short pid, const char *pSeri)
{
   unsigned long num;
   handle = 0;
   for(unsigned long i = 0; i < number(); i++)
   {
     if (CP210x_Open(i, &handle) == CP210x_SUCCESS)
     {
       unsigned short Vid;
       unsigned short Pid;
       unsigned char len;
       char PSerial[100];

       CP210x_GetDeviceVid(handle, &Vid);
       CP210x_GetDevicePid(handle, &Pid);
       CP210x_GetDeviceSerialNumber(handle, PSerial, &len);
       PSerial[len] = '\0';
	   if ( (Vid == vid) && (Pid == pid) && (static_cast<std::string>(PSerial) == pSeri))
		 return;

       CP210x_Close(handle);
     }
     handle = 0;
   }
}

CP2102::~CP2102()
{
  if (handle)  CP210x_Close(handle);
}

const char *CP2102::getSerialNumber()
{
  unsigned char len;
  CP210x_GetDeviceSerialNumber(handle, serialNumber, &len);
  serialNumber[len] = '\0';
  return serialNumber;
}

const char *CP2102::getProductString()
{
  unsigned char len;
  CP210x_GetDeviceProductString(handle, productString, &len);
  productString[len] = '\0';
  return productString;
}

int CP2102::getPid()
{
  unsigned short val;
  CP210x_GetDevicePid(handle, &val);
  return val;
} 

int CP2102::getVid()
{
  unsigned short val;
  CP210x_GetDeviceVid(handle, &val);
  return val;
} 

int CP2102::getMaxPower()  // return bu mA
{
  unsigned char val;
  CP210x_GetMaxPower(handle, &val);
  return val * 2;  // convert to mA
} 

bool CP2102::isSelfPower()
{
  int val;
  CP210x_GetSelfPower(handle, &val);
  return val;
} 

