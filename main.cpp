
// Add a noise to temperature sensor as a new Dataref
//Based on Xplane SDK Sample Codes
//Published by Alireza Ghaderi p30planets@gmail.com

 
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
static XPLMDataRef avitab_width = NULL;				// avitab width
static XPLMDataRef avitab_height = NULL;				// avitab height

//FF A350 OIS datarefs init
static XPLMDataRef ois_page = NULL;					// OIS page number
static XPLMDataRef ois_position = NULL;				// OIS position
static XPLMDataRef ois_cursor = NULL;					// OIS cursor type
static XPLMDataRef dummyDisplay = NULL;				// Is dummy display on?

//X-Plane's dataref
static XPLMDataRef busVolts1 = NULL;					// OIS cursor type
 
 
float  AvitabControl(float elapsedMe, float elapsedSim, int counter, void * refcon);  //  Declare callback to register dataref 

 PLUGIN_API int XPluginStart(
	 char *        outName,
	 char *        outSig,
	 char *        outDesc)
 {

	 // Plugin Info
	 strcpy(outName, "Avitab_FF_A350");
	 strcpy(outSig, "itxd.integrations.ffa350Avitab");
	 strcpy(outDesc, "Avitab Integration for FF A350");

   //	     sensorTempRef = XPLMRegisterDataAccessor("realSensor/temp_c",
		 //xplmType_Float,                                  // The types we support
		 //1,                                             // Writable
		 //NULL, NULL,      // Integer accessors
		 //getSensorVal, setSensorVal,          // Float accessors
		 //NULL, NULL,                                    // Doubles accessors
		 //NULL, NULL,                                    // Int array accessors
		 //NULL, NULL,                                    // Float array accessors
		 //NULL, NULL,                                    // Raw data accessors
		 //NULL, NULL);                                   // Refcons not used

	 

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

	 //register the callback
	 //XPLMRegisterFlightLoopCallback(RegDataRefInDRE, 1, NULL);   // This FLCB will register our custom dataref in DRE
	 //XPLMPluginID PluginID = XPLMFindPluginBySignature("xplanesdk.examples.DataRefEditor");

	 XPLMRegisterFlightLoopCallback(AvitabControl, 1, NULL);  //Flight Loop Call Back Register

	 return 1;
 }
 
  
 PLUGIN_API void     XPluginStop(void)
 { 
	 //XPLMUnregisterDataAccessor(sensorTempRef);
	 XPLMUnregisterFlightLoopCallback(AvitabControl, NULL);	 //  Don't forget to unload this callback.  
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
 
 float  AvitabControl(float elapsedMe, float elapsedSim, int counter, void* refcon)
 {
	 int oisPosition		= XPLMGetDatai(ois_position);
	 int oisPage			= XPLMGetDatai(ois_page);
	 float bus1Volts		= XPLMGetDataf(busVolts1);
	 float dummyScr			= XPLMGetDataf(dummyDisplay);

	 if (oisPosition == 0)
	 {
		 if (oisPage == 5)
		 {
			 if (dummyScr == 1)
			 {
				 XPLMSetDatai(avitab_enable, 1);
				 XPLMSetDatai(ois_cursor, 0);
			 }
			 else
			 {
				 XPLMSetDatai(avitab_enable, 0);
				 XPLMSetDatai(ois_cursor, 1);
			 }
		 }
		 else
		 {
			 XPLMSetDatai(avitab_enable, 0);
			 XPLMSetDatai(ois_cursor, 1);
		 }

		 XPLMSetDataf(avitab_pos_left, 75);
		 XPLMSetDataf(avitab_pos_bottom, -20);
	 }
	 else
	 {
		 if (oisPage == 5)
		 {
			 if (dummyScr == 1)
			 {
				 XPLMSetDatai(avitab_enable, 1);
				 XPLMSetDatai(ois_cursor, 0);
			 }
			 else
			 {
				 XPLMSetDatai(avitab_enable, 0);
				 XPLMSetDatai(ois_cursor, 1);
			 }
		 }
		 XPLMSetDataf(avitab_pos_left, 75);
		 XPLMSetDataf(avitab_pos_bottom, 620);
	 }

	 return 1.0;
 }

 //float     getSensorVal(void* inRefcon)
 //{
 //    return sensorTemp;
 //}
 //
 //void	setSensorVal(void* inRefcon, float inValue)
 //{
 //     sensorTemp = inValue;
 //}


 
 
 //  This single shot FLCB registers our custom dataref in DRE
 //float RegDataRefInDRE(float elapsedMe, float elapsedSim, int counter, void * refcon)
 //{
	// float idealTempVal = XPLMGetDataf(idealTempRef);
	// float tempT = (idealTempVal + randomizer(-TEMP_NOISE_AMP * idealTempVal, TEMP_NOISE_AMP*idealTempVal));
	// XPLMSetDataf(sensorTempRef, tempT);
 //    return 1;    
 //}


 //float randomizer(float LO, float HI) {
	// return LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
 //}
