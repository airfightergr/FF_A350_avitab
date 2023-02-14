
/* ************************************************
ITX-D Avitab integration for FF A350
Ilias Tselios
version 1.0
************************************************ */
 
 #define XPLM400 = 1;  // This example requires SDK4.0

#include <cstring>
 #include <cstddef>
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

//FF A350 OIS datarefs init
XPLMDataRef ois_page = NULL;						// OIS page number
XPLMDataRef ois_position = NULL;					// OIS position
XPLMDataRef ois_cursor = NULL;					// OIS cursor type
XPLMDataRef dummyDisplay = NULL;					// Is dummy display on?

//X-Plane's dataref
XPLMDataRef busVolts1 = NULL;					// OIS cursor type
 
	 int enabled;
	 int cursor;
	 float left_side;
	 float bottom_side;

	 int oisPosition;
	 int oisPage;
	 float dummyScr;
 
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
	 avitab_width = XPLMFindDataRef("avitab/panel_width");
	 avitab_height = XPLMFindDataRef("avitab/panel_enabled");
	 ois_page = XPLMFindDataRef("1-sim/ois/guage");
	 ois_position = XPLMFindDataRef("1-sim/misc/oisCycle");
	 ois_cursor = XPLMFindDataRef("1-sim/options/displayCursor");
	 dummyDisplay = XPLMFindDataRef("1-sim/lights/dummyScreen");
	 busVolts1 = XPLMFindDataRef("sim/cockpit2/electrical/bus_volts[0]");

	 //XPLMSetDatai(avitab_enable, enabled);
	 //XPLMSetDatai(ois_cursor, cursor);
	 //XPLMSetDataf(avitab_pos_left, left_side);
	 //XPLMSetDataf(avitab_pos_bottom, bottom_side);

//register the callback
	 XPLMRegisterFlightLoopCallback(MyFlightLoopCallBack, 1, NULL);  //Flight Loop Call Back Register

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
	 oisPosition = XPLMGetDatai(ois_position);
	 oisPage = XPLMGetDatai(ois_page);
	 dummyScr = XPLMGetDataf(dummyDisplay);

	 if (XPLMGetDatai(ois_position) == 0 && XPLMGetDatai(ois_page) == 5)
	 {
		enabled = 1;
		cursor = 0;
		left_side = 75;
		bottom_side = -20;
	 }
	 else if (XPLMGetDatai(ois_position) == 1 && XPLMGetDatai(ois_page) == 5 )
	 {
		 enabled = 1;
		 cursor = 0;
		 left_side = 75;
		 bottom_side = 620;
	 }
	 else
	 {
		 enabled = 0;
		 cursor = 1;
	 }

	 XPLMSetDatai(avitab_enable, enabled);
	 XPLMSetDatai(ois_cursor, cursor);
	 XPLMSetDataf(avitab_pos_left, left_side);
	 XPLMSetDataf(avitab_pos_bottom, bottom_side);

	 return 0.1; //time to run our code in seconds
 }
