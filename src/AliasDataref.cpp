#include "aliasdataref.h"

AliasDataref::AliasDataref(
    const std::string &longIdent,
    const std::string &shortIdent ) {

  _longIdent = longIdent;
  _shortIdent = shortIdent;

}

void AliasDataref::start() {
  _longDR = XPLMFindDataRef(_longIdent.c_str());
  _shortDR = XPLMFindDataRef(_shortIdent.c_str());

  if (_longDR && !_shortDR) {
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
}

void AliasDataref::stop() {
  XPLMUnregisterDataAccessor(_shortDR);
}

void AliasDataref::registerWithDRE() {
  XPLMPluginID PluginID = XPLMFindPluginBySignature("xplanesdk.examples.DataRefEditor");
  if (PluginID != XPLM_NO_PLUGIN_ID) {
    if (_longDR && _shortDR)
      XPLMSendMessageToPlugin(PluginID, MSG_ADD_DATAREF, (void*)_shortIdent.c_str());
  }
}
