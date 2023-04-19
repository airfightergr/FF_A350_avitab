#include "XPLMMenus.h"
#include <string.h>
#if IBM
	#include <windows.h>
#endif

#ifndef XPLM300
	#error This is made to be compiled against the XPLM300 SDK
#endif

#include <stdio.h>
#include <stdlib.h>
#include "XPLMDataAccess.h"
#include "XPLMPlugin.h"
#include "XPLMProcessing.h"

XPLMDataRef avitab_panel_enable;
XPLMDataRef avitab_pos_left;
XPLMDataRef avitab_pos_bottom;
XPLMDataRef avitab_width;
XPLMDataRef avitab_height;
XPLMDataRef FFA350_ois_page;
XPLMDataRef FFA350_ois_position;
XPLMDataRef FFA350_ois_cursor;
XPLMDataRef bus1_volts;
XPLMDataRef dummy_display;

float MyFlightLoopCallBack(float elapsedMe, float elapsedSim, int counter, void * refcon);

PLUGIN_API int XPluginStart(
						char *		outName,
						char *		outSig,
						char *		outDesc)
{
	strcpy(outName, "FFA350_avitab");
	strcpy(outSig, "tselios.A350_avitab");
	strcpy(outDesc, "Avitab integration to Fight Factor A350");

	avitab_panel_enable 	= XPLMFindDataRef("avitab/panel_enabled");
	avitab_pos_left		= XPLMFindDataRef("avitab/panel_left");
	avitab_pos_bottom	= XPLMFindDataRef("avitab/panel_bottom");
	avitab_width		= XPLMFindDataRef("avitab/panel_width");
	avitab_height		= XPLMFindDataRef("avitab/panel_height");
	FFA350_ois_page		= XPLMFindDataRef("1-sim/ois/guage");
	FFA350_ois_position	= XPLMFindDataRef("1-sim/misc/oisCycle");
	FFA350_ois_cursor	= XPLMFindDataRef("1-sim/options/displayCursor");
	bus1_volts		= XPLMFindDataRef("sim/cockpit2/electrical/bus_volts[0]");
	dummy_display		= XPLMFindDataRef("1-sim/lights/dummyScreen");
	// 
	// XPLMSetDatai(avitab_panel_enable, 0);
	// XPLMSetDatai(FFA350_ois_cursor, 1);
	// XPLMSetDataf(avitab_pos_left, 75);
	// XPLMSetDataf(avitab_pos_bottom, -20);
	//

//create and register our function to run on flight loop
XPLMRegisterFlightLoopCallback(MyFlightLoopCallBack, 1.0, NULL);

	return 1;
}

PLUGIN_API int XPluginEnable(void)
{
	return 1;
}

float MyFlightLoopCallBack(	float elapsedMe, 
							float elapsedSim, 
							int counter, 
							void * refcon)
{
int oisPos 		= XPLMGetDatai(FFA350_ois_position);
int oisPage	 	= XPLMGetDatai(FFA350_ois_page);
int dummyDis	= XPLMGetDataf(dummy_display);

float busV_values[8];
float busV			= XPLMGetDatavf(bus1_volts, busV_values, 0, 1);

	if (FFA350_ois_page == 5)
	{
		XPLMSetDatai(avitab_panel_enable, 1);
		XPLMSetDatai(FFA350_ois_cursor, 0);
	}
else
	{
		XPLMSetDatai(avitab_panel_enable, 0);
		XPLMSetDatai(FFA350_ois_cursor, 1);
	}

	//Position
	if (FFA350_ois_page == 0)
	{
		XPLMSetDatai(avitab_pos_left, 75);
		XPLMSetDatai(avitab_pos_bottom, -20);
		XPLMSetDatai(avitab_width, 930);
		XPLMSetDatai(avitab_height, 980);
	}
	else
	{
		XPLMSetDatai(avitab_pos_left, 75);
		XPLMSetDatai(avitab_pos_bottom, 620);
		XPLMSetDatai(avitab_width, 930);
		XPLMSetDatai(avitab_height, 980);
	}
	return 1.0;
}		//FlightLoop

PLUGIN_API void	XPluginStop(void)
{
}

PLUGIN_API void XPluginDisable(void)
{
}

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam)
{
}
