#define MSG_ADD_DATAREF 0x01000000
#define XPLM200 1

//xplm
#include <XPLMPlugin.h>
#include <XPLMUtilities.h>
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
#include <sstream>
#include <fstream>
#include <boost/tokenizer.hpp>

//xplmType_Unknown	0	Data of a type the current XPLM doesn't do.
//xplmType_Int	1	A single 4-byte integer, native endian.
//xplmType_Float	2	A single 4-byte float, native endian.
//xplmType_Double	4	A single 8-byte double, native endian.
//xplmType_FloatArray	8	An array of 4-byte floats, native endian.
//xplmType_IntArray	16	An array of 4-byte integers, native endian.
//xplmType_Data	32	A variable block of data.

//using namespace PPL;
using namespace std;
using namespace boost;

XPLMDataRef longDR;
XPLMDataRef shortDR;

int readInt(void *) {
  return XPLMGetDatai(longDR);
}
void writeInt(void *, int inValue) {
  XPLMSetDatai(longDR, inValue);
}

float readFloat(void *) {
  return XPLMGetDataf(longDR);
}
void writeFloat(void *, float inValue) {
  XPLMSetDataf(longDR, inValue);
}

double readDouble(void *) {
  return XPLMGetDatad(longDR);
}
void writeDouble(void *, double inValue) {
  XPLMSetDatad(longDR, inValue);
}

int readIntArr      (void *, int * outValues, int inOffset, int inMax) {
  return XPLMGetDatavi(longDR, outValues, inOffset, inMax);
}

void writeIntArr    (void *, int * inValues, int inOffset, int inCount) {
  XPLMSetDatavi(longDR, inValues, inOffset, inCount);
}

int readFloatArr    (void *, float * outValues, int inOffset, int inMax) {
  return XPLMGetDatavf(longDR, outValues, inOffset, inMax);
}

void writeFloatArr  (void *, float * inValues, int inOffset, int inCount) {
  XPLMSetDatavf(longDR, inValues, inOffset, inCount);
}

int readBytes       (void *, void * outValue, int inOffset, int inMaxLength) {
  return XPLMGetDatab(longDR, outValue, inOffset, inMaxLength);
}

void writeBytes     (void *, void * inValue, int inOffset, int inLength) {
  XPLMSetDatab(longDR, inValue, inOffset, inLength);
}



//FLCB
float RunOnce ( float, float, int, void * );

PLUGIN_API int XPluginStart(
    char * outName,
    char * outSig,
    char * outDesc) {

  strcpy(outName, "AliasDataref");
  strcpy(outSig,  "Dozer.AliasDataref");
  strcpy(outDesc, "");

  longDR = XPLMFindDataRef("sim/cockpit2/engine/actuators/prop_rotation_speed_rad_sec");

  shortDR = XPLMRegisterDataAccessor(
        "Dozer/propspeed",
        XPLMGetDataRefTypes(longDR),
        XPLMCanWriteDataRef(longDR),
        readInt, writeInt,
        readFloat, writeFloat,
        readDouble, writeDouble,
        readIntArr, writeIntArr,
        readFloatArr, writeFloatArr,
        readBytes, writeBytes,
        0, 0);



  //string inputFile = thisPluginPath.prependPlanePath("/RTDatarefDev.cfg");
  //openFile(inputFile);

  //FLCB
  XPLMRegisterFlightLoopCallback(RunOnce, 1.0, NULL); //call after 1 second

  return 1;
}

PLUGIN_API void XPluginStop(void) {
  XPLMUnregisterDataAccessor(shortDR);

  //FLCB
  XPLMUnregisterFlightLoopCallback(RunOnce, NULL);

  //vector<OwnedData<int>*>::iterator vi;
  //for (vi = ownedInt.begin(); vi != ownedInt.end(); vi++) {
  //  delete *vi;
  //}
  //vector<OwnedData<double>*>::iterator vd;
  //for (vd = ownedDouble.begin(); vd != ownedDouble.end(); vd++) {
  //  delete *vd;
  //}
}

PLUGIN_API void XPluginDisable(void) {}

PLUGIN_API int XPluginEnable(void) { return 1; }

PLUGIN_API void XPluginReceiveMessage(XPLMPluginID, long, void *) {}

//FLCB
float RunOnce(float, float, int, void *) {
  XPLMPluginID PluginID = XPLMFindPluginBySignature("xplanesdk.examples.DataRefEditor");
  if (PluginID != XPLM_NO_PLUGIN_ID)
  {
    XPLMSendMessageToPlugin(PluginID, MSG_ADD_DATAREF, (void*)"Dozer/propspeed");
  }

  return 0;  // Flight loop is called only once!
}




//vector<OwnedData<int>*> ownedInt;
//vector<OwnedData<double>*> ownedDouble;

//PluginPath thisPluginPath;
//Settings switchIni(thisPluginPath.prependPlanePath("/RuntimeDataref.ini"), 1, 0);

//string cleanDrIdent(const string& unclean) {
//  string clean = "";
//  for(string::const_iterator i = unclean.begin();
//      i != unclean.end();
//      i++) {
//    if ( isalnum(*i)    ||
//         (*i) == '/'    ||
//         (*i) == '_'
//         )
//      clean.push_back(*i);
//  }
//  return clean;
//}

//XPLMDataTypeID isDataType (const string *a) {
//  if(a->compare(0,3,"int") == 0) {
//    return xplmType_Int;
//  }
//  if(a->compare(0,6,"double") == 0 ||
//     a->compare(0,5,"float") == 0 ) {
//    return xplmType_Double;
//  }

//  return xplmType_Unknown;
//}

//void createDataref(const XPLMDataTypeID &type, const string * ident) {
//  switch(type) {
//    case xplmType_Int:
//      ownedInt.push_back(new OwnedData<int>(cleanDrIdent(*ident), ReadWrite, true));
//      break;
//    case xplmType_Double:
//      ownedDouble.push_back(new OwnedData<double>(cleanDrIdent(*ident), ReadWrite, true));
//      break;
//  } //switch
//}

//int openFile(string fileName) {

//  ifstream modelFile;
//  modelFile.open(fileName.c_str(), ios_base::in);
//  if (!modelFile){
//    return EXIT_FAILURE;
//  }

//  typedef tokenizer<char_separator<char> >
//      tokenizer;
//  char_separator<char> sep(" ");

//  string newLine;

//  while(getline(modelFile, newLine)) {

//    tokenizer tokens(newLine, sep);
//    tokenizer::iterator tok;

//    tok = tokens.begin();
//    XPLMDataTypeID type;

//    // First token
//    if (tok == tokens.end()) {
//      continue;
//    } else {
//      type = isDataType(&(*tok));
//      if (type == xplmType_Unknown)
//        continue;
//      tok++;
//    } // first token not blank

//    if (tok == tokens.end()) {
//      continue;
//    } else {
//      createDataref(type, &(*tok));
//    } // second token not blank

//  } //while

//  modelFile.close();

//  return EXIT_SUCCESS;
//}



