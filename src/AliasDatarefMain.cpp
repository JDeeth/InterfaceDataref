#define XPLM200 1

//xplm
#include <XPLMPlugin.h>
//#include <XPLMUtilities.h>
#include <XPLMDataAccess.h>
#include <XPLMProcessing.h>

//ppl
//#include <settings.h>
//#include <pluginpath.h>
//#include <logwriter.h>
//#include <logichandler.h>
//#include <owneddata.h>

//us
#include <string>
#include <vector>
//#include <sstream>
//#include <fstream>
//#include <boost/tokenizer.hpp>

#include "aliasdataref.h"

//using namespace PPL;
using namespace std;
//using namespace boost;



// Using a vector makes it easier to call .start() and .stop() in
// XPluginStart and XPluginStop, but it is not completely necessary.
vector<AliasDataref> aliasDR;


// Flight Loop Call-Back (FLCB)
float RunOnce ( float, float, int, void * );



PLUGIN_API int XPluginStart(
    char * outName,
    char * outSig,
    char * outDesc) {

  char pluginName[] = "AliasDataref";
  char pluginSig[]  = "Dozer.AliasDataref";
  char pluginDesc[] = "";
  strcpy_s(outName, sizeof(pluginName), pluginName);
  strcpy_s(outSig,  sizeof(pluginSig),  pluginSig);
  strcpy_s(outDesc, sizeof(pluginDesc), pluginDesc);

  // Here we define our datarefs in the source code as elements of the
  // aliasDR vector.
  aliasDR.push_back(AliasDataref("sim/cockpit2/engine/actuators/prop_rotation_speed_rad_sec",
                                 "Dozer/propspeed"));

  aliasDR.push_back(AliasDataref("sim/cockpit/radios/transponder_light",
                                 "Dozer/xplight"));

  aliasDR.push_back(AliasDataref("sim/cockpit2/gauges/actuators/barometer_setting_in_hg_pilot",
                                 "Dozer/barosetting"));

  // these next two should not be created or registered with DRE.

  // non-existant long dataref:
  aliasDR.push_back(AliasDataref("blargle/blargle/blargle",
                                 "Dozer/blargle"));

  // duplicate short dataref:
  aliasDR.push_back(AliasDataref("sim/cockpit2/radios/indicators/nav1_has_dme",
                                 "Dozer/xplight"));


  // Step through vector of AliasDatarefs and .start each one
  vector<AliasDataref>::iterator itr;
  for (itr = aliasDR.begin(); itr != aliasDR.end(); itr++) {
    (*itr).start();
  }

  // Register FLCB
  XPLMRegisterFlightLoopCallback(RunOnce, 1.0, NULL); //call after 1 second

  return 1;
}



PLUGIN_API void XPluginStop(void) {

  // Step through vector of AliasDatarefs and .stop each one
  vector<AliasDataref>::iterator itr;
  for (itr = aliasDR.begin(); itr != aliasDR.end(); itr++) {
    (*itr).stop();
  }

  // Unregister FLCB
  XPLMUnregisterFlightLoopCallback(RunOnce, NULL);
}



// These functions don't need to do anything, but must exist.
PLUGIN_API void XPluginDisable(void) {}
PLUGIN_API int XPluginEnable(void) { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID, long, void *) {}



// FLCB body
float RunOnce(float, float, int, void *) {
  // All we use this for is to register all the aliases with DataRefEditor

  vector<AliasDataref>::iterator itr;
  for (itr = aliasDR.begin(); itr != aliasDR.end(); itr++) {
    (*itr).registerWithDRE();
  }

  return 0;  // Flight loop is called only once.
}


