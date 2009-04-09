//-------------------------------------------------------------------------
// tcpcomm.h
//      This file contains definitions of "Win32" api functions provided by
//      the df32func.dll dynamic link library.
//
// This file is to be included when using socket networking in df32func.mk
//
// Copyright (c) 2006-2009, glyn@8kb.co.uk
//
// df32func/tcpcomm.h
//-------------------------------------------------------------------------

Define __tcpcomm_h__

//-------------------------------------------------------------------------
// External functions
//-------------------------------------------------------------------------

external_function ClientSocket "ClientSocket" df32func.dll dword port string host returns integer
external_function ServerSocket "ServerSocket" df32func.dll dword port returns integer
external_function AcceptClient "AcceptClient" df32func.dll returns integer
external_function Send "Send" df32func.dll dword socket string data returns integer
external_function Receive "Receive" df32func.dll dword socket pointer dataOut returns integer
external_function CloseConnection "CloseConnection" df32func.dll dword socket returns integer
external_function PseudoRand "PseudoRand" df32func.dll dword w returns integer
external_function RdtscRand "RdtscRand" df32func.dll returns integer
