//-------------------------------------------------------------------------
// win32.h
//      This file contains definitions of "Win32" api functions as well as
//      their related constants and structs.  These functions are exposed as
//      dynamic link libraries provided on systems running Windows XP or greater.
//
// This file is to be included when using Win32 capabilities in df32func.mk
//
// Copyright (c) 2006-2015, glyn@8kb.co.uk
// 
// df32func/win32.h
//-------------------------------------------------------------------------

Define __win32_h__

//-------------------------------------------------------------------------
// External functions
//-------------------------------------------------------------------------

external_function GetDateFormat "GetDateFormatA" kernel32.dll dword LCID dword dwFlags pointer lpsSystemTime pointer lpFormat pointer lpDateStr integer cchDate returns integer
external_function GetTimeFormat "GetTimeFormatA" kernel32.dll dword LCID dword dwFlags pointer lpsSystemTime pointer lpFormat pointer lpTimeStr integer cchTime returns integer
external_function GetFileTime "GetFileTime" kernel32.dll handle hFileHandle pointer lpCreationTime pointer lpLastAccessTime pointer lpLastWriteTime returns integer
external_function FileTimeToSystemTime "FileTimeToSystemTime" kernel32.dll pointer lpFileTime Pointer lpSystemTime returns integer
external_function FileTimeToLocalFileTime "FileTimeToLocalFileTime" kernel32.dll pointer lpFileTime Pointer lpSystemTime returns integer
external_function FindFirstFile "FindFirstFileA" kernel32.dll pointer lpFileName pointer lpFindFileData returns handle
external_function FindNextFile "FindNextFileA" kernel32.dll handle hFindFile pointer lpFindFileData returns integer
external_function FindClose "FindClose" kernel32.dll handle hFindFile returns integer
external_function LockFile "LockFile" kernel32.dll handle hFile dword dwFileOffsetLow dword dwFileOffsetHigh dword nNumberOfBytesToLockLow dword nNumberOfBytesToLockHigh returns integer
external_function UnlockFile "UnlockFile" kernel32.dll handle hFile dword dwFileOffsetLow dword dwFileOffsetHigh dword nNumberOfBytesToLockLow dword nNumberOfBytesToLockHigh returns integer
external_function SHFileOperation "SHFileOperationA" shell32.dll pointer lpFileOp returns integer
external_function32 GetTempPath "GetTempPathA" kernel32.dll integer nBufferLength pointer lpBuffer_ptr returns integer
external_function GetSystemDirectory "GetSystemDirectoryA" kernel32.dll pointer lpBuffer integer uSize returns integer
external_function32 mciSendString "mciSendStringA" winmm.dll pointer lpstrCommand pointer lpstrReturnString integer uReturnLength integer hwndCallback returns integer
external_function ExitProcessEx  "ExitProcess" Kernel32.dll integer iExitCode returns integer
external_function GetComputername "GetComputerNameA" kernel32.dll pointer sBuffer pointer lSize returns integer
external_function WNetGetUser "WNetGetUserA" MPR.dll pointer lpName pointer lpUserName string  lpnLength returns DWord
external_function SHBrowseForFolder "SHBrowseForFolder" shell32.dll pointer lpsBrowseInfo returns dword
external_function SHGetPathFromIDList "SHGetPathFromIDList" shell32.dll pointer pidList pointer lpBuffer returns dWord
external_function CoTaskMemFree "CoTaskMemFree" ole32.dll pointer pV returns integer
external_function GetPID "_getpid" msvcrt.dll returns integer
external_function getShortPathName "GetShortPathNameA" kernel32.dll pointer lpszLongPath pointer lpszShortPath integer cchBuffer returns integer
external_function SetConsoleTitle "SetConsoleTitleA" Kernel32.dll string lpszTitle returns integer
external_function FindWindow "FindWindowA" user32.dll pointer lpszClassName string lpszWindowName returns handle
external_function GetSystemMenu "GetSystemMenu" user32.dll handle hwnd dword bRevert returns dword
external_function EnableMenuItem "EnableMenuItem" user32.dll handle hmenu integer uIDEnableItem integer uEnable returns integer
external_function ShowWindow "ShowWindow" user32.dll handle hWnd integer nCmdShow returns integer
external_function ShellExecute "ShellExecuteA" shell32.dll handle hWnd pointer lpOperation pointer lpFile pointer lpParameters pointer lpDirectory integer nShowCmd returns integer
external_function CreateProcess "CreateProcessA" kernel32.dll pointer lpAN pointer lpCL pointer lpPA pointer lpTA integer bIH dword dwCF pointer lpE pointer lpCD pointer lpSI pointer lpPi returns integer
external_function OpenProcess "OpenProcess" kernel32.dll dword dwDesiredAccessas integer bInheritHandle dword dwProcId returns handle
external_function TerminateProcess "TerminateProcess" kernel32.dll handle hProcess integer uExitCode returns integer
external_function CloseHandle "CloseHandle" kernel32.dll handle hObject returns integer
external_function WaitForSingleObject "WaitForSingleObject" kernel32.dll handle hHandle dword dwMilliseconds returns integer
external_function32 Message_Beep "MessageBeep" user32.dll integer iSound returns integer
external_function32 ExitWindowsEx "ExitWindowsEx" user32.dll integer uFlags integer dwReserved returns integer
external_function lOpen "_lopen" kernel32.dll string lpPathName integer iReadWrite returns integer
external_function lClose "_lclose" kernel32.dll handle hFile returns integer
external_function GetLastError "GetLastError" kernel32.dll returns integer
external_function CreateFile "CreateFileA" kernel32.dll pointer lpFNname dword dwDAccess dword dwSMode pointer lpSecAttrib dword dwCreationDisp dword dwFlagsAndAttrib handle hTemplateFile returns handle
external_function GetFileSize "GetFileSize" kernel32.dll handle hFile pointer lpFileSizeHigh returns integer
external_function SetFilePointer "SetFilePointer" kernel32.dll handle hFile dword lDistanceToMove pointer lpDistanceToMoveHigh dword dwMoveMethod returns handle
external_function ReadFile "ReadFile" kernel32.dll handle hFile pointer lpBuffer integer nNumberOfBytesToRead pointer lpNumberOfBytesRead pointer lpOverlapped returns integer
external_function CopyMemory "RtlMoveMemory" kernel32.dll pointer pDst pointer pSrc integer byteLen returns integer
external_function EnumProcesses "EnumProcesses" psapi.dll pointer lpidProcess integer cb pointer cbNeeded returns integer
external_function EnumProcessModules "EnumProcessModules" psapi.dll  handle hProcess pointer lphModule integer cb integer cbNeeded returns integer
external_function WideCharToMultiByte "WideCharToMultiByte" kernel32.dll integer cp dword dwF pointer lpWCS integer cchWC pointer lpMBS integer cchMB string dC string uDC returns integer
external_function CharToOem "CharToOemA" user32.dll pointer lpszSrc pointer lpszDst returns integer
external_function OemToChar "OemToCharA" user32.dll pointer lpszSrc pointer lpszDst returns integer
external_function GetSystemTime "GetSystemTime" kernel32.dll Pointer lpGST returns VOID_TYPE
external_function GetTickCount "GetTickCount" kernel32.dll returns dWord
external_function32 CoCreateGuid "CoCreateGuid" ole32.dll pointer pGUIDStructure returns word
external_function32 StringFromGUID2 "StringFromGUID2" ole32.dll pointer pGUIDStructure pointer lpstrClsId integer cbMax returns dword
external_function MsiQueryProductState "MsiQueryProductStateA" msi.dll string szProduct returns integer
external_function MilliSleep "Sleep" kernel32.dll integer dwMilliseconds returns integer
external_function SetLastError "SetLastError" kernel32.dll dword dwErrCode returns integer
external_function FormatMessage "FormatMessageA" kernel32.dll integer dwFlags pointer lpSource dword dwMessageId dword dwLanguageId pointer lpBuffer integer nSize dword Arguments returns integer
external_function GetProcessMemoryInfo "GetProcessMemoryInfo" PSAPI.DLL dword l_hProcess pointer ppsmemCounters dword cb returns integer
external_function MultiByteToWideChar "MultiByteToWideChar" kernel32.dll integer cp dword dwF pointer lpWCS integer cchWC pointer lpMBS integer cchMB string dC string uDC returns integer
external_function GetDiskFreeSpace "GetDiskFreeSpaceA" kernel32.dll string lpRootPathName pointer lpSectorsPC pointer lpBytesPS pointer lpNumberOfFreeClusters pointer lpTotalNOC returns integer
external_function InternetCanonicalizeUrl "InternetCanonicalizeUrlA" wininet.dll pointer lpszUrl pointer lpszBuffer pointer lpdwBufferLength dword dwFlags returns integer
external_function CryptAcquireContext "CryptAcquireContextA" advapi32.dll pointer phProv string pszContainer string pszProvider dword dwProvType dword dwFlags returns integer
external_function CryptReleaseContext "CryptReleaseContext" advapi32.dll pointer phProv dword dwFlags returns integer
external_function CryptCreateHash "CryptCreateHash" advapi32.dll handle hProv dword Algid handle hKey dword dwFlags pointer phHash returns integer
external_function CryptDestroyHash "CryptDestroyHash" advapi32.dll handle hHash returns integer
external_function CryptHashData "CryptHashData" advapi32.dll handle hHash pointer pbData dword dwDataLen dword dwFlags returns integer
external_function CryptGetHashParam "CryptGetHashParam" advapi32.dll handle hHash dword dwParam pointer pbData pointer pdwDataLen dword dwFlags returns integer
external_function CryptEnumProviders "CryptEnumProvidersA" advapi32.dll dword dwIndex pointer pdwReserved dword dwFlags pointer pdwProvType pointer pcbProvName pointer pszProvName returns integer
external_function CryptBinaryToString "CryptBinaryToStringA" crypt32.dll dword pbBinary dword cbBinary dword dwFlags pointer pszString pointer pcchString returns integer
external_function CryptGetProvParam "CryptGetProvParam" advapi32.dll handle hProv dword dwParam pointer pbData pointer pdwDataLen dword dwFlags returns integer
external_function CryptContextAddRef "CryptGetProvParam" advapi32.dll handle hProv dword pdwReserved dword dwFlags returns integer
external_function CryptImportKey "CryptImportKey" advapi32.dll handle hProv pointer pbData pointer pdwDataLen dword hPubKey dword dwFlags pointer phKey returns integer
external_function CryptExportKey "CryptExportKey" advapi32.dll handle hKey handle hExpKey dword dwBlobType dword dwFlags pointer pbData pointer pdwDataLen returns integer
external_function CryptDeriveKey "CryptDeriveKey" advapi32.dll handle hProv dword Algid handle hHash dword dwFlags pointer phKey returns integer
external_function CryptDestroyKey "CryptDestroyKey" advapi32.dll handle hKey returns integer
external_function CryptEncrypt "CryptEncrypt" advapi32.dll handle hKey handle hHash dword bFinal dword dwFlags pointer pbData pointer pdwDataLen dword dwBufLen returns integer
external_function CryptDecrypt "CryptDecrypt" advapi32.dll handle hKey handle hHash dword bFinal dword dwFlags pointer pbData pointer pdwDataLen returns integer
external_function CryptSetKeyParam "CryptSetKeyParam" advapi32.dll handle hKey dword dwParam pointer pbData dword dwFlags returns integer
external_function CryptBinaryToString "CryptBinaryToStringA" crypt32.dll dword pbBinary dword cbBinary dword dwFlags pointer pszString pointer pcchString returns integer
external_function CryptStringToBinary "CryptStringToBinaryA" crypt32.dll pointer pszString dword cchString dword dwFlags pointer pbBinary pointer pcbBinary pointer pdwSkip pointer pdwFlags returns integer
external_function GetVersionEx "GetVersionExA" kernel32.dll pointer lpVersionInfo returns integer
external_function GetSystemTime "GetSystemTime" kernel32.dll pointer lpSystemTime returns integer
external_function GetTimeZoneInformation "GetTimeZoneInformation" kernel32.dll pointer lpTimeZoneInformation returns integer

//-------------------------------------------------------------------------
// Constants
//-------------------------------------------------------------------------

// MSI Constants
#REPLACE INSTALLSTATE_UNKNOWN               |CI$-0000001   // No action to be taken on the feature or component.
#REPLACE INSTALLSTATE_BROKEN                |CI$00000000   // The feature is broken
#REPLACE INSTALLSTATE_ADVERTISED            |CI$00000001   //  Advertised feature
#REPLACE INSTALLSTATE_ABSENT                |CI$00000002   //  The feature is not installed.
#REPLACE INSTALLSTATE_LOCAL                 |CI$00000003   //  The feature is installed locally.
#REPLACE INSTALLSTATE_DEFAULT               |CI$00000005   //  The product is to be installed with all features installed to the default states specified in the Feature table.

// MCI Constants
#REPLACE SIMPLE_BEEP                        |CI$-0000001    // SimpleBeep = -1,
#REPLACE WINMDOWS_OK                        |CI$00000000    // A standard windows OK beep = 0x00,
#REPLACE WINDOWS_QUESTION                   |CI$00000020    // A standard windows Question beep = 0x20,
#REPLACE WINDOWS_EXCLAMATION                |CI$00000030    // A standard windows Exclamation beep = 0x30,
#REPLACE WINDOWS_ASTERISK                   |CI$00000040    // A standard windows Asterisk beep = 0x40,

// ExitWindowsEx Constants
#REPLACE EWX_LOGOFF                         |CI$00000000    // logoff
#REPLACE EWX_SHUTDOWN                       |CI$00000001    // shutdown
#REPLACE EWX_REBOOT                         |CI$00000002    // Reboot
#REPLACE EWX_FORCE                          |CI$00000004    // Force shutdown

// SHFileOperation Constants
#REPLACE MaxDword                           |CI$FFFFFFFF
#REPLACE FO_COPY                            |CI$00000002    //  FO_COPY              &H2     Copies a file or folder
#REPLACE FO_DELETE                          |CI$00000003    //  FO_DELETE            &H3     Deletes a file or folder
#REPLACE FO_MOVE                            |CI$00000001    //  FO_MOVE              &H1     Moves a file or folder
#REPLACE FO_RENAME                          |CI$00000004    //  FO_RENAME            &H4     Renames a file or folder
#REPLACE FOF_ALLOWUNDO                      |CI$00000040    //  FOF_ALLOWUNDO        &H40    Used with Rename. When used with Delete the files get sent to the Recycle Bin.
#REPLACE FOF_FILESONLY                      |CI$00000080    //  FOF_FILESONLY        &H80    Only allows files.
#REPLACE FOF_NOCONFIRMATION                 |CI$00000010    //  FOF_NOCONFIRMATION   &H10    Does not display the Delete or Overwrite confirmation dialog.
#REPLACE FOF_SILENT                         |CI$00000004    //  FOF_SILENT           &H4     Does not display the Windows animation while performing the opperation.
#REPLACE FOF_SIMPLEPROGRESS                 |CI$00000100    //  FOF_SIMPLEPROGRESS   &H100   Does not display filenames.
#REPLACE FOF_NOCOPYSECURITYATTRIBS          |CI$00000800    //  Do not copy NT file Security Attributes
#REPLACE FOF_NOERRORUI                      |CI$00000400    //  Do not display a dialog to the user if an error occurs.
#REPLACE FOF_NOCONFIRMMKDIR                 |CI$00000200    //  Do not ask the user to confirm the creation of a new directory if the operation requires one to be created

// FolderBrowse Constants
#REPLACE BIF_RETURNONLYFSDIRS               |CI$00000001    // Window will only return when user selects a filesystem folder
#REPLACE BIF_DONTGOBELOWDOMAIN              |CI$00000002    // Do not include network folders below the domain level
#REPLACE BIF_STATUSTEXT                     |CI$00000004    // Sets the window label to the chosen folder
#REPLACE BIF_RETURNFSANCESTORS              |CI$00000008    // Only return file system directories.
#REPLACE BIF_EDITBOX                        |CI$00000010    // Show edit, so user can edit path
#REPLACE BIF_BROWSEFORCOMPUTER              |CI$00001000    // Only return computers
#REPLACE BIF_BROWSEFORPRINTER               |CI$00002000    // Only return printers
#REPLACE BIF_BROWSEINCLUDEFILES             |CI$00004000    // Display files as well as folders

// GDI Window Constants
#REPLACE SC_CLOSE                           |CI$0000F060
#REPLACE MF_BYCOMMAND                       |CI1
#REPLACE MF_ENABLED                         |CI0
#REPLACE MF_GRAYED                          |CI1

// ShellExecute Constants
#REPLACE SW_HIDE                            |CI00       // Hides the window and activates another window;
#REPLACE SW_MAXIMIZE                        |CI01       // Activates and displays a window. If the window is minimized or maximized, Windows restores it to its original size and position. An application should specify this flag when displaying the window for the first time.
#REPLACE SW_MINIMIZE                        |CI02       // Minimizes the specified window and activates the next top-level window in the Z order
#REPLACE SW_RESTORE                         |CI03       // Maximizes the specified window
#REPLACE SW_SHOW                            |CI04       // Displays a window in its most recent size and position. The active window remains active.
#REPLACE SW_SHOWDEFAULT                     |CI05       // Activates the window and displays it in its current size and position
#REPLACE SW_SHOWMAXIMIZED                   |CI06       // Minimizes the specified window and activates the next top-level window in the Z order
#REPLACE SW_SHOWMINIMIZED                   |CI07       // Displays the window as a minimized window. The active window remains active
#REPLACE SW_SHOWMINNOACTIVE                 |CI08       // Displays the window in its current state. The active window remains active
#REPLACE SW_SHOWNA                          |CI09       // Activates and displays the window. If the window is minimized or maximized, Windows restores it to its original size and position. An application should specify this flag when restoring a minimized window.
#REPLACE SW_SHOWNOACTIVATE                  |CI10       // Displays a window in its most recent size and position. The active window remains active.
#REPLACE SW_SHOWNORMAL                      |CI11       // Activates and displays a window. If the window is minimized or maximized, Windows restores it to its original size and position. An application should specify this flag when displaying the window for the first time.

// CreateProcess constants
#REPLACE SYNCHRONIZE                        |CI1048576
#REPLACE NORMAL_PRIORITY_CLASS              |CI$00000020
#REPLACE HEXNULL                            |CI$00000000
#REPLACE HEXTRUE                            |CI$00000001
#REPLACE STRINGNULL                         |CS''
#REPLACE INFINITE                           |CI$-0000001    // Wait until process terminates

#REPLACE CREATE_NEW_CONSOLE                 |CI$00000010    // The new process has a new console, instead of inheriting the parent's console.
#REPLACE CREATE_NEW_PROCESS_GROUP           |CI$00000200    // Not supported.
#REPLACE CREATE_SEPARATE_WOW_VDM            |CI$00000800    // Not supported.
#REPLACE CREATE_SHARED_WOW_VDM              |CI$00001000    // Not supported.
#REPLACE CREATE_SUSPENDED                   |CI$00000004    // The primary thread of the new process is created in a suspended state, and does not run until the ResumeThread function is called.
#REPLACE CREATE_UNICODE_ENVIRONMENT         |CI000000400    // Not supported.
#REPLACE DEBUG_PROCESS                      |CI$00000001    // If this flag is set, the calling process is treated as a debugger, and the new process is a process being debugged. Child processes of the new process are also debugged.
                                                            // The system notifies the debugger of all debug events that occur in the process being debugged.
                                                            // If you create a process with this flag set, only the calling thread (the thread that called CreateProcess) can call the WaitForDebugEvent function.
#REPLACE DEBUG_ONLY_THIS_PROCESS            |CI$00000002    // If this flag is set, the calling process is treated as a debugger, and the new process is a process being debugged. No child processes of the new process are debugged.
                                                            // The system notifies the debugger of all debug events that occur in the process being debugged.
#REPLACE DETACHED_PROCESS                   |CI$00000008    // Not supported.
#REPLACE INHERIT_CALLER_PRIORITY            |CI$00020000    // If this flag is set, the new process inherits the priority of the creator process.
#REPLACE REALTIME_PRIORITY_CLASS            |CI000000100
#REPLACE HIGH_PRIORITY_CLASS                |CI$00000080
#REPLACE IDLE_PRIORITY_CLASS                |CI$00000040

// Readmode Constants
#REPLACE OF_READ                            |CI$00000000
#REPLACE OF_WRITE                           |CI$00000001
#REPLACE OF_READWRITE                       |CI$00000002

// Attribute Constants
#REPLACE FILE_ATTRIBUTE_READONLY            |CI$00000001
#REPLACE FILE_ATTRIBUTE_HIDDEN              |CI$00000002
#REPLACE FILE_ATTRIBUTE_SYSTEM              |CI$00000004
#REPLACE FILE_ATTRIBUTE_DIRECTORY           |CI$00000010
#REPLACE FILE_ATTRIBUTE_ARCHIVE             |CI$00000020
#REPLACE FILE_ATTRIBUTE_NORMAL              |CI$00000080
#REPLACE FILE_ATTRIBUTE_TEMPORARY           |CI$00000100
#REPLACE FILE_ATTRIBUTE_ENCRYPTED           |CI$00000040  //  NT, Windows 2000 only
#REPLACE FILE_ATTRIBUTE_OFFLINE             |CI$00001000  //  Windows 2000 only
#REPLACE FILE_ATTRIBUTE_NOT_CONTENT_INDEXED |CI$00002000  //  NT, Windows 2000 only

// Sharemode Constants
#REPLACE OF_SHARE_COMPAT                    |CI$00000000
#REPLACE OF_SHARE_EXCLUSIVE                 |CI$00000010
#REPLACE OF_SHARE_DENY_WRITE                |CI$00000020
#REPLACE OF_SHARE_DENY_READ                 |CI$00000030
#REPLACE OF_SHARE_DENY_NONE                 |CI$00000040
#REPLACE OF_PARSE                           |CI$00000100
#REPLACE OF_DELETE                          |CI$00000200
#REPLACE OF_VERIFY                          |CI$00000400
#REPLACE OF_CANCEL                          |CI$00000800
#REPLACE OF_CREATE                          |CI$00001000
#REPLACE OF_PROMPT                          |CI$00002000
#REPLACE OF_EXIST                           |CI$00004000
#REPLACE OF_REOPEN                          |CI$00008000

// Accessmode Constants
#REPLACE FILE_SHARE_DELETE                  |CI$00000000
#REPLACE FILE_SHARE_READ                    |CI$00000001
#REPLACE FILE_SHARE_WRITE                   |CI$00000002
#REPLACE FILE_BEGIN                         |CI0
#REPLACE FILE_CURRENT                       |CI1
#REPLACE FILE_END                           |CI2
#REPLACE OPEN_EXISTING                      |CI3
#REPLACE OPEN_ALWAYS                        |CI4
#REPLACE TRUNCATE_EXISTING                  |CI5
#REPLACE GENERIC_READ                       |CI$80000000
#REPLACE GENERIC_WRITE                      |CI$40000000

// file handle
#REPLACE INVALID_HANDLE_VALUE               |CI-00000001

// dwFlags Constants
#REPLACE WC_NO_BEST_FIT_CHARS               |CI$00000400
#REPLACE WC_COMPOSITECHECK                  |CI$00000200
#REPLACE WC_DISCARDNS                       |CI$00000010
#REPLACE WC_SEPCHARS                        |CI$00000020
#REPLACE WC_DEFAULTCHAR                     |CI$00000040
#REPLACE WC_ERR_INVALID_CHARS               |CI$00000080 // Vista onwards only

// Codepage Constants
#REPLACE CP_ACP                             |CI0
#REPLACE CP_OEMCP                           |CI1
#REPLACE CP_MACCP                           |CI2
#REPLACE CP_UTF7                            |CI65000
#REPLACE CP_UTF8                            |CI65001

// Running Process Constants
#REPLACE PROCESS_QUERY_INFORMATION_X        |CI$00001024
#REPLACE PROCESS_QUERY_INFORMATION          |CI$00000400
#REPLACE PROCESS_VM_READ_X                  |CI$00000016
#REPLACE PROCESS_VM_READ                    |CI$00000010
#REPLACE STANDARD_RIGHTS_REQUIRED           |CI$FFFF0000


// GUID Constants
#REPLACE GUID_STRING_LENGTH                 |CI$00000050       // GUID Length

// FormatMessage
#REPLACE FORMAT_MESSAGE_ALLOCATE_BUFFER     |CI$00000100
#REPLACE FORMAT_MESSAGE_FROM_SYSTEM         |CI$00001000
#REPLACE LANG_NEUTRAL                       |CI$00000000
#REPLACE SUBLANG_DEFAULT                    |CI$00000001
#REPLACE ERROR_BAD_USERNAME                 |CI$00002202

// InternetCanonicalizeUrl
#REPLACE ICU_NO_ENCODE                      |CI$20000000        // Don't convert unsafe characters to escape sequence
#REPLACE ICU_DECODE                         |CI$10000000        // Convert %XX escape sequences to characters
#REPLACE ICU_NO_META                        |CI$08000000        // Don't convert .. etc. meta path sequences
#REPLACE ICU_ENCODE_SPACES_ONLY             |CI$04000000        // Encode spaces only
#REPLACE ICU_BROWSER_MODE                   |CI$02000000        // Special encode/decode rules for browser

// advapi32 Crypt* constants
#REPLACE PROV_RSA_FULL                      |CI00000001
#REPLACE PROV_RSA_SIG                       |CI00000002
#REPLACE PROV_DSS                           |CI00000003
#REPLACE PROV_FORTEZZA                      |CI00000004
#REPLACE PROV_MS_EXCHANGE                   |CI00000005
#REPLACE PROV_SSL                           |CI00000006
#REPLACE PROV_RSA_SCHANNEL                  |CI00000012
#REPLACE PROV_DSS_DH                        |CI00000013
#REPLACE PROV_EC_ECDSA_SIG                  |CI00000014
#REPLACE PROV_EC_ECNRA_SIG                  |CI00000015
#REPLACE PROV_EC_ECDSA_FULL                 |CI00000016
#REPLACE PROV_EC_ECNRA_FULL                 |CI00000017
#REPLACE PROV_DH_SCHANNEL                   |CI00000018
#REPLACE PROV_SPYRUS_LYNKS                  |CI00000020
#REPLACE PROV_RNG                           |CI00000021
#REPLACE PROV_INTEL_SEC                     |CI00000022
#REPLACE PROV_REPLACE_OWF                   |CI00000023
#REPLACE PROV_RSA_AES                       |CI00000024

#REPLACE CRYPT_VERIFYCONTEXT                |CI00000000         // Supposedly |CI$F0000000
#REPLACE CRYPT_NEWKEYSET                    |CI00000008
#REPLACE CRYPT_DELETEKEYSET                 |CI00000016
#REPLACE CRYPT_MACHINE_KEYSET               |CI00000032
#REPLACE CRYPT_SILENT                       |CI00000064
#REPLACE CRYPT_EXPORTABLE                   |CI00000001
#REPLACE CRYPT_USER_PROTECTED               |CI00000002
#REPLACE CRYPT_CREATE_SALT                  |CI00000004
#REPLACE CRYPT_UPDATE_KEY                   |CI00000008

#REPLACE CALG_3DES                          |CI$00006603    // Triple DES encryption algorithm.
#REPLACE CALG_3DES_112                      |CI$00006609    // Two-key triple DES encryption with effective key length equal to 112 bits.
#REPLACE CALG_AES                           |CI$00006611    // Advanced Encryption Standard (AES). This algorithm is supported by the Microsoft AES Cryptographic Provider.
#REPLACE CALG_AES_128                       |CI$0000660E    // 128 bit AES. This algorithm is supported by the Microsoft AES Cryptographic Provider.
#REPLACE CALG_AES_192                       |CI$0000660F    // 192 bit AES. This algorithm is supported by the Microsoft AES Cryptographic Provider.
#REPLACE CALG_AES_256                       |CI$00006610    // 256 bit AES. This algorithm is supported by the Microsoft AES Cryptographic Provider.
#REPLACE CALG_AGREEDKEY_ANY                 |CI$0000AA03    // Temporary algorithm identifier for handles of Diffie-Hellman-agreed keys.
#REPLACE CALG_CYLINK_MEK                    |CI$0000660C    // An algorithm to create a 40-bit DES key that has parity bits and zeroed key bits to make its key length 64 bits. This algorithm is supported by the Microsoft Base Cryptographic Provider.
#REPLACE CALG_DES                           |CI$00006601    // DES encryption algorithm.
#REPLACE CALG_DESX                          |CI$00006604    // DESX encryption algorithm.
#REPLACE CALG_DH_EPHEM                      |CI$0000AA02    // Diffie-Hellman ephemeral key exchange algorithm.
#REPLACE CALG_DH_SF                         |CI$0000AA01    // Diffie-Hellman store and forward key exchange algorithm.
#REPLACE CALG_DSS_SIGN                      |CI$00002200    // DSA public key signature algorithm.
#REPLACE CALG_ECDH                          |CI$0000AA05    // Elliptic curve Diffie-Hellman key exchange algorithm.
#REPLACE CALG_ECDSA                         |CI$00002203    // Elliptic curve digital signature algorithm.
#REPLACE CALG_ECMQV                         |CI$0000A001    // Elliptic curve Menezes, Qu, and Vanstone (MQV) key exchange algorithm. This algorithm is not supported.
#REPLACE CALG_HASH_REPLACE_OWF              |CI$0000800B    // One way function hashing algorithm.
#REPLACE CALG_HUGHES_MD5                    |CI$0000A003    // Hughes MD5 hashing algorithm.
#REPLACE CALG_HMAC                          |CI$00008009    // HMAC keyed hash algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
#REPLACE CALG_KEA_KEYX                      |CI$0000AA04    // KEA key exchange algorithm (FORTEZZA). This algorithm is not supported.
#REPLACE CALG_MAC                           |CI$00008005    // MAC keyed hash algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
#REPLACE CALG_MD2                           |CI$00008001    // MD2 hashing algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
#REPLACE CALG_MD4                           |CI$00008002    // MD4 hashing algorithm.
#REPLACE CALG_MD5                           |CI$00008003    // MD5 hashing algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
#REPLACE CALG_NO_SIGN                       |CI$00002000    // No signature algorithm.
#REPLACE CALG_OID_INFO_CNG_ONLY             |CI$FFFFFFFF    // The algorithm is only implemented in CNG. The macro, IS_SPECIAL_OID_INFO_ALGID, can be used to determine whether a cryptography algorithm is only supported by using the CNG functions.
#REPLACE CALG_OID_INFO_PARAMETERS           |CI$FFFFFFFE    // The algorithm is defined in the encoded parameters. The algorithm is only supported by using CNG. The macro, IS_SPECIAL_OID_INFO_ALGID, can be used to determine whether a cryptography algorithm is only supported by using the CNG functions.
#REPLACE CALG_PCT1_MASTER                   |CI$00004C04    // Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
#REPLACE CALG_RC2                           |CI$00006602    // RC2 block encryption algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
#REPLACE CALG_RC4                           |CI$00006801    // RC4 stream encryption algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
#REPLACE CALG_RC5                           |CI$0000660D    // RC5 block encryption algorithm.
#REPLACE CALG_RSA_KEYX                      |CI$0000A400    // RSA public key exchange algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
#REPLACE CALG_RSA_SIGN                      |CI$00002400    // RSA public key signature algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
#REPLACE CALG_SCHANNEL_ENC_KEY              |CI$00004C07    // Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
#REPLACE CALG_SCHANNEL_MAC_KEY              |CI$00004C03    // Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
#REPLACE CALG_SCHANNEL_MASTER_HASH          |CI$00004C02    // Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
#REPLACE CALG_SEAL                          |CI$00006802    // SEAL encryption algorithm. This algorithm is not supported.
#REPLACE CALG_SHA                           |CI$00008004    // SHA hashing algorithm. This algorithm is supported by the Microsoft Base Cryptographic Provider.
#REPLACE CALG_SHA1                          |CI$00008004    // Same as CALG_SHA. This algorithm is supported by the Microsoft Base Cryptographic Provider.
#REPLACE CALG_SHA_256                       |CI$0000800C    // 256 bit SHA hashing algorithm. This algorithm is supported by Microsoft Enhanced RSA and AES Cryptographic Provider..
#REPLACE CALG_SHA_384                       |CI$0000800D    // 384 bit SHA hashing algorithm. This algorithm is supported by Microsoft Enhanced RSA and AES Cryptographic Provider.
#REPLACE CALG_SHA_512                       |CI$0000800E    // 512 bit SHA hashing algorithm. This algorithm is supported by Microsoft Enhanced RSA and AES Cryptographic Provider.
#REPLACE CALG_SKIPJACK                      |CI$0000660A    // Skipjack block encryption algorithm (FORTEZZA). This algorithm is not supported.
#REPLACE CALG_SSL2_MASTER                   |CI$00004C05    // Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
#REPLACE CALG_SSL3_MASTER                   |CI$00004C01    // Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
#REPLACE CALG_SSL3_SHAMD5                   |CI$00008008    // Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
#REPLACE CALG_TEK                           |CI$0000660B    // TEK (FORTEZZA). This algorithm is not supported.
#REPLACE CALG_TLS1_MASTER                   |CI$00004C06    // Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.
#REPLACE CALG_TLS1PRF                       |CI$0000800A    // Used by the Schannel.dll operations system. This ALG_ID should not be used by applications.

#REPLACE HP_ALGID                           |CI00000001
#REPLACE HP_HASHVAL                         |CI00000002
#REPLACE HP_HASHSIZE                        |CI00000004

#REPLACE MS_ENH_RSA_AES_PROV_XP             "Microsoft Enhanced RSA and AES Cryptographic Provider (Prototype)"
#REPLACE MS_ENH_RSA_AES_PROV                "Microsoft Enhanced RSA and AES Cryptographic Provider"
#REPLACE MS_DEF_PROV                        "Microsoft Base Cryptographic Provider v1.0"

#REPLACE PP_ADMIN_PIN                       |CI$0000001F  // Returns the administrator personal identification number (PIN) in the pbData parameter as a LPSTR.
#REPLACE PP_CERTCHAIN                       |CI$00000009  // Returns the certificate chain associated with the hProv handle. The returned certificate chain is 
                                                          // X509_ASN_ENCODING encoded.
#REPLACE PP_CONTAINER                       |CI$00000006  // The name of the current key container as a null-terminated CHAR string. 
                                                          // This string is exactly the same as the one passed in the pszContainer 
                                                          // parameter of the CryptAcquireContext function to specify the key 
                                                          // container to use. The pszContainer parameter can be read to determine 
                                                          // the name of the default key container.
#REPLACE PP_ENUMALGS                        |CI$00000001  // A PROV_ENUMALGS structure that contains information about one algorithm supported by the CSP being queried.
#REPLACE PP_ENUMALGS_EX                     |CI$00000016  // A PROV_ENUMALGS_EX structure that contains information about one algorithm supported by the CSP being queried. 
                                                          // The structure returned contains more information about the algorithm than the structure returned for PP_ENUMALGS.
#REPLACE PP_ENUMCONTAINERS                  |CI$00000002  // The name of one of the key containers maintained by the CSP in the form of a null-terminated CHAR string. 
#REPLACE PP_ENUMEX_SIGNING_PROT             |CI$00000028  // Indicates that the current CSP supports the dwProtocols member of the PROV_ENUMALGS_EX structure. 
                                                          // If this function succeeds, the CSP supports the dwProtocols member of the PROV_ENUMALGS_EX structure. 
                                                          // If this function fails with an NTE_BAD_TYPE error code, the CSP does not support the dwProtocols member.
#REPLACE PP_IMPTYPE                         |CI$00000003  // A DWORD value that indicates how the CSP is implemented. For a table of possible values, see Remarks.
#REPLACE PP_KEYEXCHANGE_PIN                 |CI$00000020  // Specifies that the key exchange PIN is contained in pbData. The PIN is represented as a null-terminated ASCII string.
#REPLACE PP_KEYSET_SEC_DESCR                |CI$00000008  // Retrieves the security descriptor for the key storage container. The pbData parameter is the address of a 
                                                          // SECURITY_DESCRIPTOR structure that receives the security descriptor for the key storage container. 
                                                          // The security descriptor is returned in self-relative format.
#REPLACE PP_NAME                            |CI$00000004  // The name of the CSP in the form of a null-terminated CHAR string. This string is identical to the one passed in 
                                                          // the pszProvider parameter of the CryptAcquireContext function to specify that the current CSP be used.

// Encryption key formats
#REPLACE PUBLICKEYBLOB                      |CI$00000006
#REPLACE PRIVATEKEYBLOB                     |CI$00000007
#REPLACE PLAINTEXTKEYBLOB                   |CI$00000008

// Key params for advapi32 cryptography dwParam
#REPLACE KP_IV                              |CI1         // Initialization vector
#REPLACE KP_SALT                            |CI2         // Salt value
#REPLACE KP_PADDING                         |CI3         // Padding values
#REPLACE KP_MODE                            |CI4         // Mode of the cipher
#REPLACE KP_MODE_BITS                       |CI5         // Number of bits to feedback
#REPLACE KP_PERMISSIONS                     |CI6         // Key permissions DWORD
#REPLACE KP_ALGID                           |CI7         // Key algorithm
#REPLACE KP_BLOCKLEN                        |CI8         // Block size of the cipher
#REPLACE KP_KEYLEN                          |CI9         // Length of key in bits
#REPLACE KP_SALT_EX                         |CI10        // Length of salt in bytes
#REPLACE KP_P                               |CI11        // DSS/Diffie-Hellman P value
#REPLACE KP_G                               |CI12        // DSS/Diffie-Hellman G value
#REPLACE KP_Q                               |CI13        // DSS Q value
#REPLACE KP_X                               |CI14        // Diffie-Hellman X value
#REPLACE KP_Y                               |CI15        // Y value
#REPLACE KP_RA                              |CI16        // Fortezza RA value
#REPLACE KP_RB                              |CI17        // Fortezza RB value
#REPLACE KP_INFO                            |CI18        // For putting information into an RSA envelope
#REPLACE KP_EFFECTIVE_KEYLEN                |CI19        // Setting and getting RC2 effective key length
#REPLACE KP_SCHANNEL_ALG                    |CI20        // for setting the Secure Channel algorithms
#REPLACE KP_CLIENT_RANDOM                   |CI21        // for setting the Secure Channel client random data
#REPLACE KP_SERVER_RANDOM                   |CI22        // for setting the Secure Channel server random data
#REPLACE KP_RP                              |CI23
#REPLACE KP_PRECOMP_MD5                     |CI24
#REPLACE KP_PRECOMP_SHA                     |CI25
#REPLACE KP_CERTIFICATE                     |CI26        // for setting Secure Channel certificate data (PCT1)
#REPLACE KP_CLEAR_KEY                       |CI27        // for setting Secure Channel clear key data (PCT1)
#REPLACE KP_PUB_EX_LEN                      |CI28
#REPLACE KP_PUB_EX_VAL                      |CI29
#REPLACE KP_KEYVAL                          |CI30
#REPLACE KP_ADMIN_PIN                       |CI31
#REPLACE KP_KEYEXCHANGE_PIN                 |CI32
#REPLACE KP_SIGNATURE_PIN                   |CI33
#REPLACE KP_PREHASH                         |CI34 
#REPLACE KP_OAEP_PARAMS                     |CI36        // for setting OAEP params on RSA keys
#REPLACE KP_CMS_KEY_INFO                    |CI37
#REPLACE KP_CMS_DH_KEY_INFO                 |CI38
#REPLACE KP_PUB_PARAMS                      |CI39        // for setting public parameters
#REPLACE KP_VERIFY_PARAMS                   |CI40        // for verifying DSA and DH parameters
#REPLACE KP_HIGHEST_VERSION                 |CI41        // for TLS protocol version setting 

// Key params for advapi32 cryptography KP_PADDING
#REPLACE PKCS5_PADDING                       |CI1         // PKCS 5 (sec 6.2) padding method
#REPLACE RANDOM_PADDING                      |CI2
#REPLACE ZERO_PADDING                        |CI3 

// Key params for advapi32 cryptography KP_MODE
#REPLACE CRYPT_MODE_CBC                      |CI1         // Cipher block chaining
#REPLACE CRYPT_MODE_ECB                      |CI2         // Electronic code book
#REPLACE CRYPT_MODE_OFB                      |CI3         // Output feedback mode
#REPLACE CRYPT_MODE_CFB                      |CI4         // Cipher feedback mode
#REPLACE CRYPT_MODE_CTS                      |CI5         // Ciphertext stealing mode 

// Params for crypt32 hashing
#REPLACE CRYPT_STRING_BASE64                |CI$00000001
#REPLACE CRYPT_STRING_HEX                   |CI$00000004
#REPLACE CRYPT_STRING_HEXASCII              |CI$00000005
#REPLACE CRYPT_STRING_HEXADDR               |CI$0000000A
#REPLACE CRYPT_STRING_HEXASCIIADDR          |CI$0000000B
#REPLACE CRYPT_STRING_HEXRAW                |CI$0000000C

// Used by out GetTimeZoneInformation / GetSystemTime
#REPLACE TIME_ZONE_ID_UNKNOWN               |CI$00000000
#REPLACE TIME_ZONE_ID_STANDARD              |CI$00000001
#REPLACE TIME_ZONE_ID_DAYLIGHT              |CI$00000002
#REPLACE TIME_ZONE_ID_INVALID               |CI$FFFFFFFF

//-------------------------------------------------------------------------
// Structs
//-------------------------------------------------------------------------

// Used by convert_date_format to convert two dword values representing file mod time into string
type _FILETIME
    field FILETIME.dwLowDateTime as dword
    field FILETIME.dwHighDateTime as dword
end_type

// Used by convert_date_format to convert two dword values representing system time into string
type _SYSTEMTIME
    field SYSTEMTIME.wYear as word
    field SYSTEMTIME.wMonth as word
    field SYSTEMTIME.wDayOfWeek as word
    field SYSTEMTIME.wDay as word
    field SYSTEMTIME.wHour as word
    field SYSTEMTIME.wMinute as word
    field SYSTEMTIME.wSecond as word
    field SYSTEMTIME.wMilliseconds as word
end_type

type _SYSTEMTIME2
    field SYSTEMTIME2.wYear as char 2
    field SYSTEMTIME2.wMonth as char 2
    field SYSTEMTIME2.wDayOfWeek as char 2
    field SYSTEMTIME2.wDay as char 2
    field SYSTEMTIME2.wHour as char 2
    field SYSTEMTIME2.wMinute as char 2
    field SYSTEMTIME2.wSecond as char 2
    field SYSTEMTIME2.wMilliseconds as char 2
end_type

// Used by list_directory to read data out of string returned by kernel32
type _WIN32_FIND_DATA
    field WIN32_FIND_DATA.dwFileAttributes as dword
    field WIN32_FIND_DATA.ftCreationLowDateTime as dword
    field WIN32_FIND_DATA.ftCreationHighDateTime as dword
    field WIN32_FIND_DATA.ftLastAccessLowDateTime as dword
    field WIN32_FIND_DATA.ftLastAccessHighDateTime As dword
    field WIN32_FIND_DATA.ftLastWriteLowDateTime as dword
    field WIN32_FIND_DATA.ftLastWriteHighDateTime as dword
    field WIN32_FIND_DATA.nFileSizeHigh as dword
    field WIN32_FIND_DATA.nFileSizeLow as dword
    field WIN32_FIND_DATA.dwReserved0 as dword
    field WIN32_FIND_DATA.dwReserved1 as dword
    field WIN32_FIND_DATA.cFileName as char 260
    field WIN32_FIND_DATA.cAlternateFileName as char 14
end_type
// used by fileopp to pass details into SHFileOperation
type _SHFILEOPSTRUCT
    field SHFileOpStruct.hWnd as dword // Handle of dialog box to display status info - think this is for vb c# etc
    field SHFileOpStruct.wFunc as dword // Operation to perform
    field SHFileOpStruct.pFrom as pointer // char // A string specifying one or more source file names. Multiple names must be null-separated. The list of names must be double null-terminated
    field SHFileOpStruct.pTo as pointer // char // Same as pFrom except for the destination
    field SHFileOpStruct.fFlags as integer // Flags that control the file operation
    field SHFileOpStruct.fAnyOperationsAborted as dword // TRUE if an operation was aborted before it was completed
    field SHFileOpStruct.hNameMappings as dword // Only used with certain flags
    field SHFileOpStruct.lpszProgressTitle as dword // Title for a progress dialog box
end_type

// Used by getComputerName to read computer name out into
type _SIZEGETCOMPUTERNAME
    field SIZEGETCOMPUTERNAME.dwSize as dWord
end_type

// Used by folderbrowse to send parameters to and read data out from SHBrowseFolder
type _BROWSEINFO
    field BROWSEINFO.hWndOwner as handle
    field BROWSEINFO.pIDLRoot as pointer
    field BROWSEINFO.pszDisplayName as pointer
    field BROWSEINFO.lpszTitle as pointer
    field BROWSEINFO.ulFlags as dword
    field BROWSEINFO.lpfnCallback as pointer
    field BROWSEINFO.lParam as dword
    field BROWSEINFO.iImage as dword
end_type
// Used by create_proc to read created process details out of kernel32
type _PROCESS_INFORMATION
    field PROCESS_INFORMATION.hProcess as handle
    field PROCESS_INFORMATION.hThread as handle
    field PROCESS_INFORMATION.dwProcessId as dword
    field PROCESS_INFORMATION.dwThreadId as dword
end_type
// Used by create_proc to read create process details into of kernel32
type _STARTUPINFO
    field STARTUPINFO.cb as integer
    field STARTUPINFO.lpReserved as pointer // to string
    field STARTUPINFO.lpDesktop as pointer // to string
    field STARTUPINFO.lpTitle as pointer // to string
    field STARTUPINFO.dwX as dword
    field STARTUPINFO.dwY as dword
    field STARTUPINFO.dwXSize as dword
    field STARTUPINFO.dwYSize as dword
    field STARTUPINFO.dwXCountChars as dword
    field STARTUPINFO.dwYCountChars as dword
    field STARTUPINFO.dwFillAttribute as dword
    field STARTUPINFO.dwFlags as dword
    field STARTUPINFO.wShowWindow as integer
    field STARTUPINFO.cbReserved2 as integer
    field STARTUPINFO.lpReserved2 as pointer
    field STARTUPINFO.hStdInput as handle
    field STARTUPINFO.hStdOutput as handle
    field STARTUPINFO.hStdError as handle
end_type
// Used to receive an array from get_procs
type _PROCESSARRAY
    field PROCESSARRAY.arrayItem as dword
end_type

// Used by ReadFile
type _STRUCTBYTESREAD
    field STRUCTBYTESREAD.integer0 as dword
end_type
// Used by get_procs
type _STRUCTBYTESBACK
    field STRUCTBYTESBACK.integer0 as dword
end_type

// Used by create_guid (http:// msdn.microsoft.com/en-us/library/aa373931(VS.85).aspx)
type _GUID
    field GUID.data1 as dword
    field GUID.data2 as word
    field GUID.data3 as word
    field GUID.data4 as char 8
end_type

type _DISKDATA1
    field DISKDATA1.sectorsPerCluster as dword
end_type
type _DISKDATA2
    field DISKDATA2.bytesPerSector as dword
end_type
type _DISKDATA3
    field DISKDATA3.numberOfFreeClusters as dword
end_type
type _DISKDATA4
    field DISKDATA4.totalNumberOfClusters as dword
end_type

// Used by GetProcessMemoryInfo
type _PROCESS_MEMORY_COUNTERS
    field PROCESS_MEMORY_COUNTERS.cb as dword               // The size of the structure, in bytes.
    field PROCESS_MEMORY_COUNTERS.PageFaultCount as dword           // The number of page faults.
    field PROCESS_MEMORY_COUNTERS.PeakWorkingSetSize as dword       // The peak working set size, in bytes.
    field PROCESS_MEMORY_COUNTERS.WorkingSetSize as dword           // The current working set size, in bytes.
    field PROCESS_MEMORY_COUNTERS.QuotaPeakPagedPoolUsage as dword      // The peak paged pool usage, in bytes.
    field PROCESS_MEMORY_COUNTERS.QuotaPagedPoolUsage as dword      // The current paged pool usage, in bytes.
    field PROCESS_MEMORY_COUNTERS.QuotaPeakNonPagedPoolUsage as dword   // The peak nonpaged pool usage, in bytes.
    field PROCESS_MEMORY_COUNTERS.QuotaNonPagedPoolUsage as dword       // The current nonpaged pool usage, in bytes.
    field PROCESS_MEMORY_COUNTERS.PagefileUsage as dword            // The current space allocated for the pagefile, in bytes. Those pages may or may not be in memory.
    field PROCESS_MEMORY_COUNTERS.PeakPagefileUsage as dword        // The peak space allocated for the pagefile, in bytes.
end_type

// Used by hashing algorithms based on advapi32
type _HCRYPTHASH
    field HCRYPTHASH.value as dword
end_type

// Used by encryption algorithms based on advapi32
type _HCRYPTKEY
    field HCRYPTKEY.value as dword
end_type

// Used by encryption algorithms to import / export keys
type _PLAINTEXTKEYBLOB 
    field PLAINTEXTKEYBLOB.BLOBHEADER AS char 8 //64 bit
    field PLAINTEXTKEYBLOB.dwKeySize AS dword
    field PLAINTEXTKEYBLOB.rgbKeyData AS char 512
end_type

// Used by encryption algorithms to import / export keys
type _BLOBHEADER
    field BLOBHEADER.bType AS byte
    field BLOBHEADER.bVersion AS byte
    field BLOBHEADER.Reserved AS word
    field BLOBHEADER.ALG_ID AS byte //UInt
end_type

// Used for numeric (int,dword,ptr etc) address referencing
type _DW_TYPE
    field DW_TYPE.value as dword
end_type

// Used by GetVersionEx
type _OSVERSIONINFO
  field OSVERSIONINFO.dwOSVersionInfoSize as dword
  field OSVERSIONINFO.dwMajorVersion as dword
  field OSVERSIONINFO.dwMinorVersion as dword
  field OSVERSIONINFO.dwBuildNumber as dword
  field OSVERSIONINFO.dwPlatformId as dword
  field OSVERSIONINFO.szCSDVersion as char 128
end_type

// TimeZoneInfo
type _TIME_ZONE_INFORMATION
    field TIME_ZONE_INFORMATION.Bias as dword // long
    field TIME_ZONE_INFORMATION.StandardName as char 64 // wchar array
    field TIME_ZONE_INFORMATION.StandardDate as char 16 // SYSTEMTIME (structure but not actually SYSTEMTIME)
    field TIME_ZONE_INFORMATION.StandardBias as dword // long
    field TIME_ZONE_INFORMATION.DaylightName as char 64 // wchar array (structure but not actually SYSTEMTIME)
    field TIME_ZONE_INFORMATION.DaylightDate as char 16 // SYSTEMTIME
    field TIME_ZONE_INFORMATION.DaylightBias as dword // long
end_type

//-------------------------------------------------------------------------
// Global arrays used to store results from legacy functions
//-------------------------------------------------------------------------            

// Global arrays for temporary use by win32 functions
object Win32API_result1 is an array             
end_object
object Win32API_result2 is an array
end_object
object Win32API_result3 is an array
end_object
object Win32API_result4 is an array
end_object
object Win32API_result5 is an array
end_object

// Used for buffering text files via win32
object Win32API_buffer is an array
end_object

// Used by list_directory for sorting
object Win32API_sort is an array
end_object
object Win32API_sort1 is an array
end_object
object Win32API_sort2 is an array
end_object
object Win32API_sort3 is an array
end_object
object Win32API_sort4 is an array
end_object
object Win32API_sort5 is an array
end_object
object Win32API_sort6 is an array
end_object

//-------------------------------------------------------------------------
// Global variables
//-------------------------------------------------------------------------
integer g_sConsoleTitleIsSet
move "" to g_sConsoleTitleIsSet
