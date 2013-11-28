#ifndef COMMANDFROMDATAREF_H
#define COMMANDFROMDATAREF_H

#include <XPLMPlugin.h>
#include <XPLMDataAccess.h>
#include <XPLMUtilities.h>
#include <string>

#define MSG_ADD_DATAREF 0x01000000

class CommandFromDataref
{
public:
    CommandFromDataref(
            const std::string &outCmd,
            const std::string &inDR );
    void start();
    void stop();
    void registerWithDRE();

private:
    enum CMD_STATE {
        CMD_STATE_LOWEST = 0,

        RELEASE = 0,
        RUN_ONCE = 1,
        HOLD_ON = 2,

        CMD_STATE_HIGHEST = HOLD_ON
    };

    int _phase;
    std::string _inDRIdent;
    std::string _outCmdIdent;

    XPLMDataRef _inDR;
    XPLMCommandRef _outCmd;

    static int _readInt( void* ref ) {
        return ((CommandFromDataref*)ref)->_phase;
    }

    static void _writeInt( void* ref, int inValue ) {
        if( inValue < CMD_STATE_LOWEST ) inValue = 0;
        if( inValue > CMD_STATE_HIGHEST ) inValue = 2;
        ((CommandFromDataref*)ref)->_newInput( inValue );
    }

    void _newInput( const int& inValue );
};

#endif // COMMANDFROMDATAREF_H
