
/* ************************************************
ITX-D Avitab integration for FF A350
Ilias Tselios
version 1.0
************************************************ */
 
 #define XPLM300 = 1;  // This example requires SDK3.0
 
 #include "XPLMPlugin.h"
 #include "XPLMDisplay.h"
 #include "XPLMGraphics.h"
 #include "XPLMProcessing.h"
 #include "XPLMDataAccess.h"
 #include "XPLMMenus.h"
 #include "XPLMUtilities.h"
 #include "XPWidgets.h"
 #include "XPStandardWidgets.h"
 #include "XPLMScenery.h"

//Avitab datarefs init
static XPLMDataRef avitab_enable = NULL;				// avitab is enabled?
static XPLMDataRef avitab_pos_left = NULL;				// avitab left pos
static XPLMDataRef avitab_pos_bottom = NULL;			// avitab bottom pos
static XPLMDataRef avitab_width = NULL;					// avitab width
static XPLMDataRef avitab_height = NULL;				// avitab height

//FF A350 OIS datarefs init
static XPLMDataRef ois_page = NULL;						// OIS page number
static XPLMDataRef ois_position = NULL;					// OIS position
static XPLMDataRef ois_cursor = NULL;					// OIS cursor type
static XPLMDataRef dummyDisplay = NULL;					// Is dummy display on?

//X-Plane's dataref
static XPLMDataRef busVolts1 = NULL;					// OIS cursor type
 
	 int enabled;
	 int cursor;
	 float left_side;
	 float bottom_side;
 
	 float  MyFlightLoopCallBack(						//our flight loop callback
		 float elapsedMe,
		 float elapsedSim,
		 int counter,
		 void* refcon);

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
	 avitab_pos_left = XPLMFindDataRef("avitab/panel_enabled");
	 avitab_pos_left = XPLMFindDataRef("avitab/panel_enabled");
	 ois_page = XPLMFindDataRef("1-sim/ois/guage");
	 ois_position = XPLMFindDataRef("1-sim/misc/oisCycle");
	 ois_cursor = XPLMFindDataRef("1-sim/options/displayCursor");
	 dummyDisplay = XPLMFindDataRef("1-sim/lights/dummyScreen");
	 busVolts1 = XPLMFindDataRef("sim/cockpit2/electrical/bus_volts[0]");

	 XPLMSetDatai(avitab_enable, enabled);
	 XPLMSetDatai(ois_cursor, cursor);
	 XPLMSetDataf(avitab_pos_left, left_side);
	 XPLMSetDataf(avitab_pos_bottom, bottom_side);

	 //register the callback
	 XPLMRegisterFlightLoopCallback(MyFlightLoopCallBack, -1, NULL);  //Flight Loop Call Back Register

	 return 1;
 }
 
  
 PLUGIN_API void     XPluginStop(void)
 { 
	 XPLMUnregisterFlightLoopCallback(MyFlightLoopCallBack, NULL);	 //  Don't forget to unload this callback.  
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
 
 float  MyFlightLoopCallBack(						//our flight loop callback
							 float elapsedMe,
							 float elapsedSim,
							 int counter,
							 void* refcon)
 {
	 int oisPosition = XPLMGetDatai(ois_position);
	 int oisPage = XPLMGetDatai(ois_page);
	 float dummyScr = XPLMGetDataf(dummyDisplay);

	 if (oisPosition == 0 && oisPage == 5 && dummyScr > 0.0f)
	 {
		enabled = 1;
		cursor = 0;
		left_side = 75;
		bottom_side = -20;
	 }
	 else if (oisPosition == 1 && oisPage == 5 && dummyScr > 0.0f)
	 {
		 enabled = 1;
		 cursor = 0;
		 left_side = 75;
		 bottom_side = 620;
	 }
	 else
	 {
		 enabled = 1;
		 cursor = 0;
	 }


	/* XPLMSetDatai(avitab_enable, enabled);
	 XPLMSetDatai(ois_cursor, cursor);
	 XPLMSetDataf(avitab_pos_left, left_side);
	 XPLMSetDataf(avitab_pos_bottom, bottom_side);*/

	 return 0.001; //time to run our code in seconds
 }
