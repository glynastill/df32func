/*-------------------------------------------------------------------------
 * df32func.h
 *     df32func extension definitions
 *
 * Copyright (c) 2007-2015, glyn@8kb.co.uk
 * Author: Glyn Astill <glyn@8kb.co.uk>
 *
 *-------------------------------------------------------------------------
 */

#ifndef _DF32FUNC_H_
#define _DF32FUNC_H_

#define DLLIMPORT __declspec (dllexport)

DLLIMPORT void CloseConnection(int socket);
DLLIMPORT int ClientSocket(int PortNo, char* IPAddress);
DLLIMPORT int Send(int socket, char* data);
DLLIMPORT int Receive(int socket, char *pData);
DLLIMPORT int ServerSocket(int PortNo);
DLLIMPORT int AcceptClient();
DLLIMPORT unsigned int PseudoRand(unsigned int w);
DLLIMPORT unsigned int RdtscRand();
DLLIMPORT int GetTzi (TCHAR* zone, TCHAR *result);
DLLIMPORT int RegexpMatch(const char *str, const char *pattern, const char *flags, int errors);
DLLIMPORT int RegexpMatches(const char *str, const char *pattern, const char *flags, char *output, int output_len, int errors);
DLLIMPORT int RegexpReplace(const char *str, const char *pattern, const char *replacement, const char *flags, char *output, int output_len, int errors);

#endif /* _DF32FUNC_H_ */
