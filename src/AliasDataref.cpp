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

//using namespace PPL;
using namespace std;
using namespace boost;



class AliasDataref {
public:
  AliasDataref(const std::string &longIdent, const std::string &shortIdent ) {
    _shortIdent = shortIdent;
    _longIdent = longIdent;
  }

  void registerDR() {
    _longDR = XPLMFindDataRef(_longIdent.c_str());

    _shortDR = XPLMRegisterDataAccessor(
          _shortIdent.c_str(),
          XPLMGetDataRefTypes(_longDR),
          XPLMCanWriteDataRef(_longDR),
          &AliasDataref::_readInt,      &AliasDataref::_writeInt,
          &AliasDataref::_readFloat,    &AliasDataref::_writeFloat,
          &AliasDataref::_readDouble,   &AliasDataref::_writeDouble,
          &AliasDataref::_readIntArr,   &AliasDataref::_writeIntArr,
          &AliasDataref::_readFloatArr, &AliasDataref::_writeFloatArr,
          &AliasDataref::_readBytes,    &AliasDataref::_writeBytes,
          (void *)this, (void *)this);
  }

  void unregisterDR() {
    XPLMUnregisterDataAccessor(_shortDR);
  }

  void meetDRE() {
    XPLMPluginID PluginID = XPLMFindPluginBySignature("xplanesdk.examples.DataRefEditor");
    if (PluginID != XPLM_NO_PLUGIN_ID)
    {
      XPLMSendMessageToPlugin(PluginID, MSG_ADD_DATAREF, (void*)_shortIdent.c_str());
    }
  }

private:
  std::string _shortIdent;
  std::string _longIdent;

  XPLMDataRef _shortDR;
  XPLMDataRef _longDR;
  static int _readInt(void * ref) {
    ///AliasDataref* that = (AliasDataref*)ref;
    return XPLMGetDatai(((AliasDataref*)ref)->_longDR);
  }
  static void _writeInt(void * ref, int inValue) {
    XPLMSetDatai(((AliasDataref*)ref)->_longDR, inValue);
  }

  static float _readFloat(void * ref) {
    return XPLMGetDataf(((AliasDataref*)ref)->_longDR);
  }
  static void _writeFloat(void * ref, float inValue) {
    XPLMSetDataf(((AliasDataref*)ref)->_longDR, inValue);
  }

  static double _readDouble(void * ref) {
    return XPLMGetDatad(((AliasDataref*)ref)->_longDR);
  }
  static void _writeDouble(void * ref, double inValue) {
    XPLMSetDatad(((AliasDataref*)ref)->_longDR, inValue);
  }

  static int _readIntArr      (void * ref, int * outValues, int inOffset, int inMax) {
    return XPLMGetDatavi(((AliasDataref*)ref)->_longDR, outValues, inOffset, inMax);
  }

  static void _writeIntArr    (void * ref, int * inValues, int inOffset, int inCount) {
    XPLMSetDatavi(((AliasDataref*)ref)->_longDR, inValues, inOffset, inCount);
  }

  static int _readFloatArr    (void * ref, float * outValues, int inOffset, int inMax) {
    return XPLMGetDatavf(((AliasDataref*)ref)->_longDR, outValues, inOffset, inMax);
  }

  static void _writeFloatArr  (void * ref, float * inValues, int inOffset, int inCount) {
    XPLMSetDatavf(((AliasDataref*)ref)->_longDR, inValues, inOffset, inCount);
  }

  static int _readBytes       (void * ref, void * outValue, int inOffset, int inMaxLength) {
    return XPLMGetDatab(((AliasDataref*)ref)->_longDR, outValue, inOffset, inMaxLength);
  }

  static void _writeBytes     (void * ref, void * inValue, int inOffset, int inLength) {
    XPLMSetDatab(((AliasDataref*)ref)->_longDR, inValue, inOffset, inLength);
  }

};

AliasDataref propSpeed("sim/cockpit2/engine/actuators/prop_rotation_speed_rad_sec", "Dozer/propspeed");
AliasDataref xpLight("sim/cockpit/radios/transponder_light", "Dozer/xplight");static

//FLCB
float RunOnce ( float, float, int, void * );

PLUGIN_API int XPluginStart(
    char * outName,
    char * outSig,
    char * outDesc) {

  strcpy(outName, "AliasDataref");
  strcpy(outSig,  "Dozer.AliasDataref");
  strcpy(outDesc, "");

  propSpeed.registerDR();
  xpLight.registerDR();

  //string inputFile = thisPluginPath.prependPlanePath("/RTDatarefDev.cfg");
  //openFile(inputFile);

  //FLCB
  XPLMRegisterFlightLoopCallback(RunOnce, 1.0, NULL); //call after 1 second

  return 1;
}

PLUGIN_API void XPluginStop(void) {

  propSpeed.unregisterDR();
  xpLight.unregisterDR();

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

  propSpeed.meetDRE();
  xpLight.meetDRE();

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



