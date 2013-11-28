//xplm
#include <XPLMPlugin.h>
#include <XPLMUtilities.h>
#include <XPLMDataAccess.h>
#include <XPLMProcessing.h>

//us
#include <string>
#include <vector>

#include "aliasdataref.h"
#include "commandfromdataref.h"

using namespace std;

// Using a vector makes it easier to call .start() and .stop() in
// XPluginStart and XPluginStop, but it is not completely necessary.
vector<AliasDataref> aliasDR;
vector<CommandFromDataref> commandDR;

// Flight Loop Call-Back (FLCB)
float RunOnce ( float, float, int, void * );

PLUGIN_API int XPluginStart(
        char * outName,
        char * outSig,
        char * outDesc) {

    char pluginName[] = "InterfaceDataref";
    char pluginSig[]  = "Dozer.InterfaceDataref";
    char pluginDesc[] = "Creates aliases for datarefs, and datarefs which trigger commands.";
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

    commandDR.push_back( CommandFromDataref( "Dozer/baro_up", "sim/instruments/barometer_up" ) );

    // Step through vector of AliasDatarefs and .start each one
    vector<AliasDataref>::iterator adItr;
    for (adItr = aliasDR.begin(); adItr != aliasDR.end(); adItr++) {
        (*adItr).start();
    }

    vector<CommandFromDataref>::iterator cdItr;
    for( cdItr = commandDR.begin(); cdItr != commandDR.end(); cdItr++ ){
        (*cdItr).start();
    }

    // Register FLCB
    XPLMRegisterFlightLoopCallback(RunOnce, 1.0, NULL); //call after 1 second

    return 1;
}

PLUGIN_API void XPluginStop(void) {

    // Step through vector of AliasDatarefs and .stop each one
    vector<AliasDataref>::iterator adItr;
    for (adItr = aliasDR.begin(); adItr != aliasDR.end(); adItr++) {
        (*adItr).stop();
    }

    vector<CommandFromDataref>::iterator cdItr;
    for( cdItr = commandDR.begin(); cdItr != commandDR.end(); cdItr++ ){
        (*cdItr).stop();
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
    vector<AliasDataref>::iterator adItr;
    for (adItr = aliasDR.begin(); adItr != aliasDR.end(); adItr++) {
        (*adItr).registerWithDRE();
    }

    vector<CommandFromDataref>::iterator cdItr;
    for( cdItr = commandDR.begin(); cdItr != commandDR.end(); cdItr++ ){
        (*cdItr).registerWithDRE();
    }

    return 0;  // Flight loop is called only once.
}


