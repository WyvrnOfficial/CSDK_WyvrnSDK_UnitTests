 #ifndef _WYVRNSDKTYPES_H_
 #define _WYVRNSDKTYPES_H_

#include <Windows.h>
#define WIN32_LEAN_AND_MEAN
  
 #pragma once
  
 typedef LONG            RZRESULT;

  
 namespace WyvrnSDK
 {
     typedef struct APPINFOTYPE
     {
         wchar_t Title[256];           
         wchar_t Description[1024];
         struct Author
         {
             wchar_t Name[256];
             wchar_t Contact[256];
         } Author;
         //SupportedDevice = 
         //    0x01 | // Keyboards
         //    0x02 | // Mice
         //    0x04 | // Headset
         //    0x08 | // Mousepads
         //    0x10 | // Keypads
         //    0x20   // ChromaLink devices
         const DWORD SupportedDevice = 63;
         //Category = 
         //    0x01 | // App
         //    0x02 | // Game
         DWORD Category;             
     } APPINFOTYPE;
 }
  
 #endif
