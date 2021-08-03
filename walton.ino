//<App !Start!>
// FILE: [walton.ino]
// Created by GUIslice Builder version: [0.16.b011]
//
// GUIslice Builder Generated File
//
// For the latest guides, updates and support view:
// https://github.com/ImpulseAdventure/GUIslice
//
//<App !End!>

// ------------------------------------------------
// Headers to include
// ------------------------------------------------
#include "walton_GSLC.h"
#include <BlockDriver.h>
#include <FreeStack.h>
#include <MinimumSerial.h>
#include <SdFat.h>
#include <SdFatConfig.h>
#include <sdios.h>
#include <SysCall.h>
char    acTxt[31];
char    acTxt2[31];
int nVal1;

// ------------------------------------------------
// Program Globals
// ------------------------------------------------

// Save some element references for direct access
//<Save_References !Start!>
gslc_tsElemRef* m_pElemListbox1   = NULL;
gslc_tsElemRef* m_pListSlider1    = NULL;
//<Save_References !End!>

// Define debug message function
static int16_t DebugOut(char ch) { if (ch == (char)'\n') Serial.println(""); else Serial.write(ch); return 0; }

// file system object
SdFat sd1;

SdFile root;
SdFile file;
SdFile dir;

//defining a simple class using methods of sdFat library to handle the directory and file listing logic
class sdList{
  public: 
    static void set_dir()
    {
      // Open root directory
      if (!dir.open("/")){
        sd1.errorHalt("dir.open failed");
      }
    }
    
    static void set_dir(char param2[])
    {
      // Open root directory
      if (!dir.open(param2)){
          dir.open("/");
          sd1.errorHalt("dir.open failed, returning to root");
               
      }
      
    }
  
    static void update_list(){
      
      //use the directory set from set_dir() and update the listbox list
      // Open next file in root.
      // Warning, openNext starts at the current position of dir so a
      // rewind may be necessary in your application.
      gslc_ElemXListboxReset(&m_gui,m_pElemListbox1);
      char name_of_dir[30] PROGMEM={0};
      char listofdir[50][30] PROGMEM={0};
      char name_of_file[30] PROGMEM={0};
      char list_of_file[50][30] PROGMEM={0};
      int i=0; 
      int j=0;
      while (file.openNext(&dir, O_RDONLY)) {
        
        file.printFileSize(&Serial);
        Serial.write(' ');
        file.printModifyDateTime(&Serial);
        Serial.write(' ');
        file.printName(&Serial);
        //
        if (file.isDir()&&!file.isHidden()) 
        { 
          // Indicate a directory.
          
          file.getName(name_of_dir,128);
          Serial.write('/');
          Serial.println();
          Serial.println("printing the directory name from 2D array");
          strcat(name_of_dir, "/");
          Serial.println(name_of_dir);
          strcpy(listofdir[i], name_of_dir);
          i++;
        }
        if (file.isFile())
        {
          //it is a file
          file.getName(name_of_file,128);
          strcpy(list_of_file[j], name_of_file);
          j++;
        }
        Serial.println();
        file.close();
      }
      
      if (dir.getError()) {
        Serial.println("openNext failed");
      } else {
        Serial.println("Done!");
      
      Serial.println("printing list of directories only");
        i=0;
        while(i<=49){
        if(listofdir[i][0]=='\0')
        {
          Serial.println("no more directories");
          break;
        }
        Serial.println(listofdir[i]);
        i++;
        }
        Serial.println("printing list of files only");
        i=0;
        while(i<=49){
        if(list_of_file[i][0]=='\0')
        {
          Serial.println("no more files");
          break;
        }
        Serial.println(list_of_file[i]);
        i++;
        }
        //total rows:
        
        i=0;
        j=0;   //added after subdirectory listing failed
        while(i<=49){
        if(listofdir[i][0]=='\0')
        {
          Serial.println("no more directories");
          break;
        }
      
          sprintf(acTxt,listofdir[i]);
          Serial.println("acTxt value is:");
          Serial.println(acTxt);
            if (gslc_ElemXListboxInsertItemAt(&m_gui,m_pElemListbox1,i,acTxt)) {
              // successfully added new file in the list
              j=i+1;
              }
              else
              {
                Serial.println("item entry failed");
                }   
        i++;
        }
        
        i=0;
        
        Serial.println("before adding files in list, here is the first file name:");
        Serial.println(list_of_file[0]);
        while(i<=49){
        if(list_of_file[i][0]=='\0')
        {
          Serial.println("no more files");
          break;
        }
      
          sprintf(acTxt,list_of_file[i]);
            if (gslc_ElemXListboxInsertItemAt(&m_gui,m_pElemListbox1,j,acTxt)) {
              // successfully added new country
              }
              else
              {
                Serial.println("item entry failed");
                }  
        i++;
        j++;
        } 
      }
      dir.close();
    }
};
bool setting_done=false;
// ------------------------------------------------
// Callback Methods
// ------------------------------------------------
// Common Button callback
bool CbBtnCommon(void* pvGui,void *pvElemRef,gslc_teTouch eTouch,int16_t nX,int16_t nY)
{
  // Typecast the parameters to match the GUI and element types
  gslc_tsGui*     pGui     = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui,pElemRef);

  if ( eTouch == GSLC_TOUCH_UP_IN ) {
    // From the element's ID we can determine which button was pressed.
    switch (pElem->nId) {
//<Button Enums !Start!>
      case E_ELEM_BTN1:
              sdList::set_dir();
              sdList::update_list();
        break;
      case E_ELEM_BTN2:
            
              if(setting_done)
                { sdList::set_dir(acTxt2);
                  sdList::update_list();
                  setting_done=false;
                }
                else
                {
                  
                }
              
        break;
      case E_ELEM_BTN4:
          nVal1 = gslc_ElemXSliderGetPos(pGui,m_pListSlider1);
          nVal1=nVal1-1;
          if(nVal1<0)
          nVal1=0;
          gslc_ElemXListboxSetScrollPos(pGui, m_pElemListbox1, nVal1);
          gslc_ElemXSliderSetPos(pGui, m_pListSlider1, nVal1);
        break;
      case E_ELEM_BTN5:
          nVal1 = gslc_ElemXSliderGetPos(pGui,m_pListSlider1);
          nVal1=nVal1+1;
          if(nVal1>100)
          nVal1=100;
          gslc_ElemXListboxSetScrollPos(pGui, m_pElemListbox1, nVal1);
          gslc_ElemXSliderSetPos(pGui, m_pListSlider1, nVal1);
        break;
//<Button Enums !End!>
      default:
        break;
    }
  }
  return true;
}

//<Checkbox Callback !Start!>
//<Checkbox Callback !End!>
//<Keypad Callback !Start!>
//<Keypad Callback !End!>
//<Spinner Callback !Start!>
//<Spinner Callback !End!>
bool CbListbox(void* pvGui, void* pvElemRef, int16_t nSelId)
{
  gslc_tsGui*     pGui     = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui, pElemRef);
  char            acTxt[MAX_STR + 1];
  
  if (pElemRef == NULL) {
    return false;
  }

  // From the element's ID we can determine which listbox was active.
  switch (pElem->nId) {
//<Listbox Enums !Start!>
    case E_ELEM_LISTBOX1:
      if (nSelId != XLISTBOX_SEL_NONE) {
        gslc_ElemXListboxGetItem(&m_gui,m_pElemListbox1,nSelId,acTxt2,sizeof(acTxt2));
        //check if dir
        /*if (!dir.open(acTxt2))
        {
            Serial.println("directory open failed from listbox")
        }*/
        setting_done=true;
        //sdList::set_dir(acTxt2);
        
        //enter should work
      } 
      else{
        setting_done=false;
        acTxt2[0]='/';
      }
      break;

//<Listbox Enums !End!>
    default:
      break;
  }
  return true;
}
//<Draw Callback !Start!>
//<Draw Callback !End!>

// Callback function for when a slider's position has been updated
bool CbSlidePos(void* pvGui,void* pvElemRef,int16_t nPos)
{
  gslc_tsGui*     pGui     = (gslc_tsGui*)(pvGui);
  gslc_tsElemRef* pElemRef = (gslc_tsElemRef*)(pvElemRef);
  gslc_tsElem*    pElem    = gslc_GetElemFromRef(pGui,pElemRef);
  int16_t         nVal;

  // From the element's ID we can determine which slider was updated.
  switch (pElem->nId) {
//<Slider Enums !Start!>
    case E_LISTSCROLL1:
      // Fetch the slider position
      nVal = gslc_ElemXSliderGetPos(pGui,m_pListSlider1);
      gslc_ElemXListboxSetScrollPos(pGui, m_pElemListbox1, nVal);
      break;

//<Slider Enums !End!>
    default:
      break;
  }

  return true;
}
//<Tick Callback !Start!>
//<Tick Callback !End!>


const uint8_t chipSelect = SS;
unsigned long last_time=0;
void setup()
{
  // ------------------------------------------------
  // Initialize
  // ------------------------------------------------
  Serial.begin(115200);
  // Wait for USB Serial 
  delay(1000);  // NOTE: Some devices require a delay after Serial.begin() before serial port can be used
  

  // ------------------------------------------------
  // Create graphic elements
  // ------------------------------------------------
  InitGUIslice_gen();
  gslc_Update(&m_gui);
  gslc_SetPageCur(&m_gui, E_PG_MAIN);
  
  //delay(300);
  
  gslc_SetPageCur(&m_gui, E_PG2);
  gslc_Update(&m_gui);
  gslc_InitDebug(&DebugOut);
  // Initialize the SD.
  if (!sd1.begin(chipSelect, SD_SCK_MHZ(8))) {
    sd1.initErrorHalt(&Serial);
  }
  
  sdList::set_dir();
  sdList::update_list();
}

// -----------------------------------
// Main event loop
// -----------------------------------
void loop()
{

  // ------------------------------------------------
  // Update GUI Elements
  // ------------------------------------------------
  
  //TODO - Add update code for any text, gauges, or sliders
  
  // ------------------------------------------------
  // Periodically call GUIslice update function
  // ------------------------------------------------
  gslc_Update(&m_gui);


    if(millis()-last_time>3000)
    {
      last_time=millis();
        
        Serial.println(freeMemory()); //print the free RAM left in Arduino every 3 seconds
    }
    
}

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
