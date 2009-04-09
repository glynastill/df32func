//-------------------------------------------------------------------------
// df32func.mk
//		Primary source file for DateFlex 3.2 Console Mode "df32func" extentions
//
// This is the file to be compiled by the dfcomp command
//
// Copyright (c) 2006-2009, glyn@8kb.co.uk
// 
// df32func/df32func.mk
//-------------------------------------------------------------------------
//#NOISY 99
//#NOISY 1

#REM ----------------------------------------------------------------------
#REM Compiling df32func.inc
#REM If precompiling ensure df32func.flp is with your 
#REM bytecode and df32func.pki is with your packages
#REM ----------------------------------------------------------------------

//-------------------------------------------------------------------------
// Set dataflex epoch and set date format to 4 digit years
//-------------------------------------------------------------------------

set_date_attribute date4_state to dftrue
set_date_attribute epoch_value to 80
set_date_attribute sysdate4_state to dftrue

//-------------------------------------------------------------------------
// Standard DF32 Console Mode packages 
//-------------------------------------------------------------------------

Use CASE                                        //this package allows the use of switch statements in DataFlex
Use UI                                          //this package contains all the user interface / data classes for OO DataFlex
Use MONARRAY                                    //this package allows the easy monitoring of an array
Use TIMER                                       //this package allows the use of timers to control events
Use DLL                                         //this package allows import of dynamic link libraries, N.B. It contains fake
                                                //WORD and BYTE types that are incorrect for use in C struct types
//-------------------------------------------------------------------------
// Defines used to control some compile time behaviour
//-------------------------------------------------------------------------

//Define enable_dfassert
Define no_backslash_quote
Define default_file_channel						for 9

//-------------------------------------------------------------------------
// Global variables used to control some runtime behaviour
//-------------------------------------------------------------------------

// Turn on any debug lines at runtime
indicator show_debug_lines

//Used by set msxsl function (string.inc) - set to true to keep msxsl binary in one particular place
indicator g_bMsxslPresent                       
string g_sMsxslEngine

//used by runtime (date.inc)
integer g_nRuntime_entry 

//used by replacement GET_FILE_MOD_TIME command (depmacro.inc)                     
string  g_sGetfileModTime

indicate g_bMsxslPresent false                  
move "c:\msxsl.exe" to g_sMsxslEngine
move 0 to g_nRuntime_entry
indicate show_debug_lines false


//-------------------------------------------------------------------------
// Include main content
//-------------------------------------------------------------------------

//Including header file win32.h
#INCLUDE win32.h
//Including include file tcpcom.h
#INCLUDE tcpcomm.h
//Including include file console.h
#INCLUDE console.h
//Including include file encode.h
#INCLUDE encode.h
//Including include file errors.h
#INCLUDE errors.h
//Including include file macro.inc
#INCLUDE macro.inc
//Including include file math.inc
#INCLUDE math.inc
//Including include file win32.inc
#INCLUDE win32.inc
//Including include file string.inc
#INCLUDE string.inc
//Including include file date.inc
#INCLUDE date.inc
//Including include file console.inc
#INCLUDE hash.inc
//Including include file data.inc
#INCLUDE data.inc
//Including include file depmacro.inc
#INCLUDE console.inc
//Including include file hash.inc
#INCLUDE depmacro.inc
//Including include file tstamp.inc
#INCLUDE tstamp.inc
//Including include file encode.inc
#INCLUDE encode.inc
