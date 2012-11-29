#ifndef ALIASDATAREF_H
#define ALIASDATAREF_H

#include <string>
#include <XPLMPlugin.h>
#include <XPLMDataAccess.h>

#define MSG_ADD_DATAREF 0x01000000

class AliasDataref {
public:
  // Class constructor.
  /// \param longIdent The identifier of the existing dataref.
  /// \param shortIdent The identifier of the new alias dataref you want to create.
  AliasDataref(
      const std::string &longIdent,
      const std::string &shortIdent );

  // Call this function within XPluginStart.
  // It registers the data accessors.
  void start();

  // Call this in XPluginStop.
  void stop();

  // If desired, call this once in a flight-loop callback to register
  // the alias dataref with DataRefEditor.
  void registerWithDRE();

private:
  std::string _shortIdent;
  std::string _longIdent;

  XPLMDataRef _shortDR;
  XPLMDataRef _longDR;

  // Data getter/setter functions
  static int _readInt(void * ref) {
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

#endif // ALIASDATAREF_H
