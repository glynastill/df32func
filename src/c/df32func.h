/*-------------------------------------------------------------------------
 * df32func.h
 *     df32func extension definitions
 *
 * Copyright (c) 2007-2009, glyn@8kb.co.uk
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

#endif /* _DF32FUNC_H_ */
