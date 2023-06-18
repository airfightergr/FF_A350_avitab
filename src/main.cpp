
/* ************************************************
ITX-D Avitab integration for FF A350
Ilias Tselios
version 1.0
************************************************ */
 
#define XPLM400 = 1;  // This example requires SDK4.0

#include <cstring>
#include <cstddef>
#include <cstdio>
#include <XPLMPlugin.h>
#include <XPLMDisplay.h>
#include <XPLMGraphics.h>
#include <XPLMProcessing.h>
#include <XPLMDataAccess.h>
#include <XPLMMenus.h>
#include <XPLMUtilities.h>
#include <XPWidgets.h>
#include <XPStandardWidgets.h>
#include <XPLMScenery.h>

//Avitab datarefs init
 XPLMDataRef avitab_enable = NULL;				// avitab is enabled?
 XPLMDataRef avitab_pos_left = NULL;				// avitab left pos
 XPLMDataRef avitab_pos_bottom = NULL;			// avitab bottom pos
 XPLMDataRef avitab_width = NULL;					// avitab width
 XPLMDataRef avitab_height = NULL;				// avitab height
 XPLMDataRef avitab_brit = NULL;

//FF A350 OIS datarefs init
static XPLMDataRef ois_page = NULL;						// OIS page number
static XPLMDataRef ois_position = NULL;					// OIS position
static XPLMDataRef ois_cursor = NULL;					// OIS cursor type
static XPLMDataRef dummyDisplay = NULL;					// Is dummy display on?

//X-Plane's dataref
static XPLMDataRef busVolts1 = NULL;					// OIS cursor type

float left_side, bottom_side, dummyScr;
int enabled, oisPosition, cursor, oisPage, oisCur;

int oisP, oisPs;

float  AvitabEnable( float elapsedMe, float elapsedSim, int counter, void* Refcon ) //our flight loop callback
{
    // enable avitab when page == 5 and set cursor to mouse
	ois_page = XPLMFindDataRef("1-sim/ois/guage");
	int oisP = XPLMGetDatai(ois_page);
	ois_cursor = XPLMFindDataRef("1-sim/options/displayCursor");
	
	if (oisP == 5)
	{
		XPLMSetDatai(avitab_enable, 1);
		XPLMSetDatai(ois_cursor, 0);
	}
	else
	{
		XPLMSetDatai(avitab_enable, 0);
		XPLMSetDatai(ois_cursor, 1);
	}

	 return 0.1f; //time to run our code in seconds
 }


 float AvitabPosition( float elapsedMe, float elapsedSim, int counter, void* Refcon ) //our flight loop callback)
 {
// set avitab position based on OIS position
	ois_position = XPLMFindDataRef("1-sim/misc/oisCycle");
	int oisPs = XPLMGetDatai(ois_position);

	avitab_pos_bottom = XPLMFindDataRef("avitab/panel_bottom");

	 if (oisPs == 0) 
	{	
		XPLMSetDatai(avitab_pos_left, 75);
		XPLMSetDatai(avitab_pos_bottom, -20);
	}
	else if (oisPs == 1)
	{
		XPLMSetDatai(avitab_pos_left, 75);
		XPLMSetDatai(avitab_pos_bottom, 620);
	}

	return 0.1f; //time to run our code in seconds
 }


float AvitabBrit( float elapsedMe, float elapsedSim, int counter, void* Refcon ) //our flight loop callback)
{
// control brightness
	dummyDisplay = XPLMFindDataRef("1-sim/lights/dummyScreen");
	avitab_brit = XPLMFindDataRef("avitab/brightness");

	XPLMSetDataf(avitab_brit, XPLMGetDataf(dummyDisplay));
 	
	return 0.1f; //time to run our code in seconds
}

// int     getOisPos(void* inRefcon);
// int     getOisPage(void* inRefcon);
// float   getdummySrc(void* inRefcon);

// void    setOisPos(void* inRefcon, int outValue);
// void    setAvitabLeftPos(void* inRefcon, int outValue);
// void    setAvitabBottomPos(void* inRefcon, int outValue);

 PLUGIN_API int XPluginStart(
	 char *        outName,
	 char *        outSig,
	 char *        outDesc)
 {
	 // Plugin Info
	 strcpy(outName, "Avitab_FF_A350");
	 strcpy(outSig, "itxd.integrations.ffa350Avitab");
	 strcpy(outDesc, "Avitab Integration for FF A350");


// Find and intialize our  dataref
	 avitab_enable = XPLMFindDataRef("avitab/panel_enabled");
	 avitab_pos_left = XPLMFindDataRef("avitab/panel_left");
	 avitab_pos_bottom = XPLMFindDataRef("avitab/panel_bottom");
	 avitab_width = XPLMFindDataRef("avitab/panel_width");
	 avitab_height = XPLMFindDataRef("avitab/panel_enabled");
	 ois_page = XPLMFindDataRef("1-sim/ois/guage");
	 ois_position = XPLMFindDataRef("1-sim/misc/oisCycle");
	 ois_cursor = XPLMFindDataRef("1-sim/options/displayCursor");
	 dummyDisplay = XPLMFindDataRef("1-sim/lights/dummyScreen");
	 busVolts1 = XPLMFindDataRef("sim/cockpit2/electrical/bus_volts[0]");

     oisPosition = XPLMGetDatai(ois_position);
     oisPage = XPLMGetDatai(ois_page);
     dummyScr = XPLMGetDataf(dummyDisplay);
     oisCur = XPLMGetDatai(ois_cursor);

	 //XPLMSetDatai(avitab_enable, enabled);
	 //XPLMSetDatai(ois_cursor, cursor);
	 //XPLMSetDataf(avitab_pos_left, left_side);
	 //XPLMSetDataf(avitab_pos_bottom, bottom_side);

//register the callback
	XPLMRegisterFlightLoopCallback(AvitabEnable, -1, NULL);  //Flight Loop Call Back Register
 	XPLMRegisterFlightLoopCallback(AvitabPosition, -1, NULL);  //Flight Loop Call Back Register
	XPLMRegisterFlightLoopCallback(AvitabBrit, -1, NULL);  //Flight Loop Call Back Register
	 return 1;
 }


 PLUGIN_API void     XPluginStop(void)
 {
	XPLMUnregisterFlightLoopCallback(AvitabEnable, NULL);	 //  Don't forget to unload this callback.
	XPLMUnregisterFlightLoopCallback(AvitabPosition, NULL);	 //  Don't forget to unload this callback.
 }

 PLUGIN_API void XPluginDisable(void)
 {
 }

 PLUGIN_API int XPluginEnable(void)
 {
     return 1;
 }

 PLUGIN_API void XPluginReceiveMessage(XPLMPluginID    inFromWho,
                                      long             inMessage,
                                      void *           inParam)
 {

 }

