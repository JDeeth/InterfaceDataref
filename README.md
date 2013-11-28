InterfaceDataref
================

This is an X-Plane plugin for creating new datarefs to serve as special-purpose interfaces for X-Plane.

Currently it has two features:

 - *AliasDataref*, which creates a new dataref which acts as an alias for an existing one. This is to circumvent the 58-character limit for dataref identifiers on the Teensy hardware boards. 
 - *CommandFromDataref*, which creates new datarefs which trigger existing commands. This is for ExtPlane clients, as ExtPlane currently doesn't interface with X-Plane Commands. Write 1 to the dataref to trigger the command once, 2 to hold the command on, and 0 to release the command.

These new interface datarefs are currently hardcoded as illustrated in `main.cpp`. Todo is implement a file parser to load these values from an input text file.
 
Jack