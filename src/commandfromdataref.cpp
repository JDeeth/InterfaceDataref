#include "commandfromdataref.h"

CommandFromDataref::CommandFromDataref(
        const std::string &outCmd,
        const std::string &inDR ) {
    _inDRIdent = inDR;
    _outCmdIdent = outCmd;
    _phase = 0;
}


void CommandFromDataref::start() {
    _inDR = XPLMFindDataRef( _inDRIdent.c_str() );
    _outCmd = XPLMFindCommand( _outCmdIdent.c_str() );

    if (_outCmd && !_inDR) {
        _inDR = XPLMRegisterDataAccessor(
                    _inDRIdent.c_str(),
                    xplmType_Int,
                    true,
                    &CommandFromDataref::_readInt, &CommandFromDataref::_writeInt,
                    0, 0,
                    0, 0,
                    0, 0,
                    0, 0,
                    0, 0,
                    (void *)this, (void *)this);
    }

}

void CommandFromDataref::stop() {
    XPLMUnregisterDataAccessor( _inDR );
}

void CommandFromDataref::_newInput( const int& inValue ) {

    _phase = RELEASE;
    switch( inValue ) {
    case RELEASE:
        XPLMCommandEnd( _outCmd );
        break;
    case RUN_ONCE:
        XPLMCommandOnce( _outCmd );
        break;
    case HOLD_ON:
        XPLMCommandBegin( _outCmd );
        _phase = HOLD_ON;
        break;
    }
}

void CommandFromDataref::registerWithDRE() {
  XPLMPluginID PluginID = XPLMFindPluginBySignature("xplanesdk.examples.DataRefEditor");
  if (PluginID != XPLM_NO_PLUGIN_ID) {
    if (true)//(_inDR && _outCmd)
      XPLMSendMessageToPlugin(PluginID, MSG_ADD_DATAREF, (void*)_inDRIdent.c_str());
  }
}
