//-------------------------------------------------------------------------
// win32.inc
//      This file contains DataFlex functions to provide wrappers around "Win32"
//      API calls.  See win32.h for external function definitions.
//
// This file is to be included when using Win32 capabilities in df32func.mk
//
// Copyright (c) 2006-2015, glyn@8kb.co.uk
// 
// df32func/win32.inc
//-------------------------------------------------------------------------

#IFDEF __win32_h__
#ELSE
    #INCLUDE win32.h
#ENDIF

//-------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------

// Takes both the high-order doubleword and low-order doubleword representing the date&time 2X32bit numbers
// and returns as a string
function convert_date_format global dword dwLowDateTime dword dwHighDateTime returns string
    local string sftTime sSystemTime sFormattedTime sFormattedDate sLocalFileTime
    local pointer lpsftTime lpsSystemTime lpsFormattedTime lpsFormattedDate lpsLocalFileTime
    local integer iSuccess iLenCcTime iDataLength iLenCcDate

    zerotype _FILETIME to sftTime
    put dwLowDateTime to sftTime at FILETIME.dwLowDateTime
    put dwHighDateTime to sftTime at FILETIME.dwHighDateTime
    getaddress of sftTime to lpsftTime

    zeroType _FILETIME to sLocalFileTime
    getaddress of sLocalFileTime to lpsLocalFileTime

    move (FileTimeToLocalFileTime(lpsftTime,lpsLocalFileTime)) to iSuccess
    if (iSuccess <> 0) begin
        zerotype _SYSTEMTIME to sSystemTime
        getaddress of sSystemTime to lpsSystemTime

        move (FileTimeToSystemTime(lpsLocalFileTime,lpsSystemTime)) to iSuccess
        if (iSuccess <> 0) begin
            zerostring 255 to sFormattedTime
            getaddress of sFormattedTime to lpsFormattedTime
            move (length(sFormattedTime)) to iLenCcTime
            move (GetTimeFormat("LOCALE_USER_DEFAULT", 0, lpsSystemTime, 0, lpsFormattedTime, iLenCcTime)) to iDataLength
            
            zerostring 255 To sFormattedDate
            getaddress of sFormattedDate To lpsFormattedDate
            move (length(sFormattedDate)) to iLenCcDate
            move (GetDateFormat("LOCALE_USER_DEFAULT", 0, lpsSystemTime, 0, lpsFormattedDate, iLenCcDate)) to iDataLength
            function_return (cstring (sFormattedDate) * cstring (sFormattedTime)) // return with terminating null char removed
        end
    end
end_function

// List directory takes a directory path and returns a file count-1
// file listing information including size is put into 5 global arrays
//
// Returns an integer file_count-1, where count represents the number of files found in the directory.
// I.e. If no files are found it will return -1, if one file is found it will return 0
// If files are found, attributes of the files can be found in the following global arrays:
//     
//     Win32API_result1 - File Name
//     Win32API_result2 - File Size
//     Win32API_result3 - Modified Date
//     Win32API_result4 - Access Date
//     Win32API_result5 - Creation Date
// 
function list_directory global string argv returns string
    local string sPathName sWin32FindData sLine sLast sComment sFileName sModifiedDate sAccessDate sCreationDate k sLastFile
    local integer l_01iResult iFileSize iFileCount
    local pointer pT5 pT6
    local handle hFile
    local dword dwFileSizeHigh dwFileSizeLow dwHighDateTime dwLowDateTime

    send delete_data to Win32API_result1
    send delete_data to Win32API_result2
    send delete_data to Win32API_result3
    send delete_data to Win32API_result4
    send delete_data to Win32API_result5

    zerotype _WIN32_FIND_DATA to sWin32FindData
    getaddress of sWin32FindData to pT5
    move argv to sPathName
    getaddress of sPathName to pT6
    move (FindFirstFile(pT6, pT5)) to hFile
    // if (hFile = -1) showln "Invalid file handle!"

    move -1 to iFileCount
    repeat 
        // FileName
        getbuff_String from sWin32FindData at WIN32_FIND_DATA.cFileName to sFileName
        if ((sFileName <> ".") and (sFileName <> "..") and (sFileName <> "")) begin
            increment iFileCount

            // FileSize
            getbuff from sWin32FindData at WIN32_FIND_DATA.nFileSizeHigh to dwFileSizeHigh
            getbuff from sWin32FindData at WIN32_FIND_DATA.nFileSizeLow to dwFileSizeLow
            moveint ((dwFileSizeHigh*MaxDword)+dwFileSizeLow) to iFileSize

            // File Modified Time
            getbuff from sWin32FindData at WIN32_FIND_DATA.ftLastWriteLowDateTime to dwLowDateTime
            getbuff from sWin32FindData at WIN32_FIND_DATA.ftLastWriteHighDateTime to dwHighDateTime
            move (convert_date_format(dwLowDateTime, dwHighDateTime)) to sModifiedDate
   
            // File Accessed Time
            getbuff from sWin32FindData at WIN32_FIND_DATA.ftLastAccessLowDateTime to dwLowDateTime
            getbuff from sWin32FindData at WIN32_FIND_DATA.ftLastAccessHighDateTime to dwHighDateTime
            move (convert_date_format(dwLowDateTime, dwHighDateTime)) to sAccessDate
   
            // File Creation Time
            getbuff from sWin32FindData at WIN32_FIND_DATA.ftCreationLowDateTime to dwLowDateTime
            getbuff from sWin32FindData at WIN32_FIND_DATA.ftCreationHighDateTime to dwHighDateTime
            move (convert_date_format(dwLowDateTime, dwHighDateTime)) to sCreationDate

            set array_value of (Win32API_result1(current_object)) item iFileCount to sFileName
            set array_value of (Win32API_result2(current_object)) item iFileCount to iFileSize
            set array_value of (Win32API_result3(current_object)) item iFileCount to sModifiedDate
            set array_value of (Win32API_result4(current_object)) item iFileCount to sAccessDate
            set array_value of (Win32API_result5(current_object)) item iFileCount to sCreationDate
        end
        zerotype _WIN32_FIND_DATA to sWin32FindData
        move (FindNextFile(hFile, pT5)) to l_01iResult
    until (l_01iResult = 0)
    move (FindClose(hFile)) to l_01iResult

    function_return iFileCount
end_function

// Sort a directory listing
// argv = array to sort by 1-6, argv2 = array size
// Returns the the data array size
function sort_results global integer argv integer argv2 returns integer
    local integer doneSort l_i l_j l_h l_tmpInt
    local number l_tmpNum
    local string l_tmpStr l_tmpStr2
    local date l_tmpDate
    
    send delete_data to Win32API_sort
    send delete_data to Win32API_sort1
    send delete_data to Win32API_sort2
    send delete_data to Win32API_sort3
    send delete_data to Win32API_sort4
    send delete_data to Win32API_sort5
    send delete_data to Win32API_sort6

    move 0 to doneSort
    if ((argv < 1) or (argv > 5)) goto sorted

    for l_i from 0 to argv2
        if (argv = 1) get string_value of (Win32API_result1(current_object)) item l_i to l_tmpStr
        if (argv = 2) get integer_value of (Win32API_result2(current_object)) item l_i to l_tmpInt
        if (argv = 3) get string_value of (Win32API_result3(current_object)) item l_i to l_tmpStr
        if (argv = 4) get string_value of (Win32API_result4(current_object)) item l_i to l_tmpStr
        if (argv = 5) get string_value of (Win32API_result5(current_object)) item l_i to l_tmpStr

        if (argv = 1) begin
            if ((trim(l_tmpStr)) = "") move "NULL" to l_tmpStr
            set array_value of (Win32API_sort(current_object)) item l_i to (string(lowercase(l_tmpStr)))
        end
        if (argv = 2) set array_value of (Win32API_sort(current_object)) item l_i to l_tmpInt
        if (argv > 2) begin
            calc ((((((integer(mid(l_tmpStr,2,12)))*60)+(integer(mid(l_tmpStr,2,15))))+(((date(mid(l_tmpStr,10,1)))-693975)*1440))*60)+(integer(mid(l_tmpStr,2,18)))) to l_tmpNum
            set array_value of (Win32API_sort(current_object)) item l_i to l_tmpNum
            set array_value of (Win32API_sort6(current_object)) item l_i to l_tmpNum
        end
    loop

    send sort_items to Win32API_sort ascending

    for l_i from 0 to argv2
        get string_value of (Win32API_sort(current_object)) item l_i to l_tmpStr
        if ((trim(uppercase(l_tmpStr))) <> "NULL") begin
            for l_j from 0 to argv2
                if (argv = 1) get string_value of (Win32API_result1(current_object)) item l_j to l_tmpStr2
                if (argv = 2) get string_value of (Win32API_result2(current_object)) item l_j to l_tmpStr2
                if (argv > 2) get string_value of (Win32API_sort6(current_object)) item l_j to l_tmpStr2

                if (((trim(l_tmpStr2)) <> "NULL") and ((trim(l_tmpStr2)) <> "")) begin
                    if ((trim(lowercase(l_tmpStr))) = (trim(lowercase(l_tmpStr2)))) begin
                        get string_value of (Win32API_result1(current_object)) item l_j to l_tmpStr
                        set array_value of (Win32API_sort1(current_object)) item l_i to l_tmpStr
                        get integer_value of (Win32API_result2(current_object)) item l_j to l_tmpInt
                        set array_value of (Win32API_sort2(current_object)) item l_i to l_tmpInt
                        get string_value of (Win32API_result3(current_object)) item l_j to l_tmpStr
                        set array_value of (Win32API_sort3(current_object)) item l_i to l_tmpStr
                        get string_value of (Win32API_result4(current_object)) item l_j to l_tmpStr
                        set array_value of (Win32API_sort4(current_object)) item l_i to l_tmpStr
                        get string_value of (Win32API_result5(current_object)) item l_j to l_tmpStr
                        set array_value of (Win32API_sort5(current_object)) item l_i to l_tmpStr

                        if (argv = 1) set array_value of (Win32API_result1(current_object)) item l_j to "NULL"
                        if (argv = 2) set array_value of (Win32API_result2(current_object)) item l_j to "NULL"
                        if (argv > 2) set array_value of (Win32API_sort6(current_object)) item l_j to "NULL"

                        move argv2 to l_j
                    end
                end
            loop
        end
    loop
    for l_i from 0 to argv2
        get string_value of (Win32API_sort1(current_object)) item l_i to l_tmpStr
        set array_value of (Win32API_result1(current_object)) item l_i to l_tmpStr
        get string_value of (Win32API_sort2(current_object)) item l_i to l_tmpInt
        set array_value of (Win32API_result2(current_object)) item l_i to l_tmpInt
        get string_value of (Win32API_sort3(current_object)) item l_i to l_tmpStr
        set array_value of (Win32API_result3(current_object)) item l_i to l_tmpStr
        get string_value of (Win32API_sort4(current_object)) item l_i to l_tmpStr
        set array_value of (Win32API_result4(current_object)) item l_i to l_tmpStr
        get string_value of (Win32API_sort5(current_object)) item l_i to l_tmpStr
        set array_value of (Win32API_result5(current_object)) item l_i to l_tmpStr
    loop

    send delete_data to Win32API_sort
    send delete_data to Win32API_sort1
    send delete_data to Win32API_sort2
    send delete_data to Win32API_sort3
    send delete_data to Win32API_sort4
    send delete_data to Win32API_sort5

    sorted:
    function_return doneSort
end_function

// This function allows basic file operations delete, move, copy, rename
// Useful where DataFlex internal functionssuch as erasefile or copyfile are flakey.
//    
//     fileOpp(<operation type>,<source file>,<dest file>)
//     <operation name> can be any of "COPY", "DELETE", "MOVE" or "RENAME"
//
// Example usage:
//
//     fileOpp("delete","C:\FileTo.delete","")
//     fileOpp("move","C:\Source.file","C:\Destination.file")
//
function fileopp global string argv string argv2 string argv3 returns integer
    local string sFileOp
    local pointer lpFileOp lpArgv2 lpArgv3
    local integer l_iResult

    move 0 to l_iResult
    move (trim(uppercase(argv))) to argv
    move (trim(argv2)) to argv2
    move (trim(argv3)) to argv3

    if (((argv = "COPY") or (argv = "PRINT") or (argv = "DELETE") or (argv = "MOVE") or (argv = "RENAME")) and (argv2 <> "")) begin
        zerotype _SHFILEOPSTRUCT to sFileOp
        getaddress of sFileOp to lpFileOp

        case begin
            case (argv = "COPY") put FO_COPY to sFileOp at SHFILEOPSTRUCT.wFunc
            case break
            case (argv = "PRINT") put FO_COPY to sFileOp at SHFILEOPSTRUCT.wFunc
            case break
            case (argv = "DELETE") put FO_DELETE to sFileOp at SHFILEOPSTRUCT.wFunc
            case break
            case (argv = "MOVE") put FO_MOVE to sFileOp at SHFILEOPSTRUCT.wFunc
            case break
            case (argv = "RENAME") put FO_RENAME to sFileOp at SHFILEOPSTRUCT.wFunc
            case break
        case end

        move (argv2+character(0)+character(0)) to argv2
        move (argv3+character(0)+character(0)) to argv3
        getAddress of argv2 to lpArgv2
        put lpArgv2 to sFileOp at SHFILEOPSTRUCT.pFrom
        
        if (argv <> "DELETE") begin
            getAddress Of argv3 to lpArgv3
            put lpArgv3 to sFileOp at SHFILEOPSTRUCT.pTo
        end  

        case begin
            case (argv = "PRINT") put (FOF_NOCONFIRMATION+FOF_NOCONFIRMMKDIR+FOF_SILENT+FOF_NOERRORUI+FOF_NOCOPYSECURITYATTRIBS) to sFileOp at SHFILEOPSTRUCT.fFlags
            case break
            case else put (FOF_NOCONFIRMATION+FOF_NOCONFIRMMKDIR+FOF_SILENT+FOF_NOERRORUI) to sFileOp at SHFILEOPSTRUCT.fFlags
            case break
        case end
            
        // put ? to sFileOp at SHFILEOPSTRUCT.hWnd
        // put ? to sFileOp at SHFILEOPSTRUCT.fAnyOperationsAborted
        // put ? to sFileOp at SHFILEOPSTRUCT.hNameMappings
        // put ? to sFileOp at SHFILEOPSTRUCT.lpszProgressTitle

        move (SHFileOperation(lpFileOp)) to l_iResult
    end

    function_return l_iResult
end_function

// Get temp dir on local machine from windows registry
function get_local_temp global integer argv returns string
    local string lpBuffer
    local integer l_01iResult
    local pointer tmpPtr
    
    move (pad(lpBuffer,255)) to lpBuffer // this is a hack to allocate a specific size to a local string
    getaddress of lpBuffer to tmpPtr
    move (GetTempPath(255,tmpPtr)) to l_01iResult

    function_return lpbuffer
end_function

// Get system dir on local machine from windows registry
function get_local_system global integer argv returns string
    local string sBuffer
    local pointer lpBuffer l_iResult uSize

    move 255 to uSize
    move (repeat(character(0),(uSize+1))) to sBuffer    
    getAddress of sBuffer to lpBuffer
    move (GetSystemDirectory(lpBuffer,uSize)) to l_iResult      
    
    function_return (left(sBuffer,l_iResult))
end_function

// Function to open close cd tray dll - 0 to open 1 to close
function cd_tray global integer argv returns integer
    local integer l_iResult
    local string l_sReturn l_sCmd   
    local pointer l_pCmd l_pReturn
    
    zerostring 127 to l_sCmd    
    getaddress of l_sCmd to l_pCmd
    getaddress of l_sReturn to l_pReturn

    if (argv = 0) move "set CDAudio door open" to l_sCmd
    if (argv = 1) move "set CDAudio door closed" to l_sCmd
    move (mciSendString(l_pCmd,l_pReturn,127,0)) to l_iResult
    
    function_return l_sReturn
end_function

// This function will force dataflex to exit with the error code in iReturnCode
function exit_process global integer iReturnCode returns integer
  local integer iVoid
  
  move (ExitProcessEx(iReturnCode)) To iVoid
  
  function_return iVoid
end_function

// Grab the process ID of dfruncon
function get_process_id global integer argv returns integer
    local integer iRVal
    
    move (GetPID()) TO iRVal
    
    function_return (Low(iRVal))
end_function

// Grab the computername
function get_computer global integer argv returns string
    local string  strName lsSize
    local pointer lpNameAddr lpSize
    local integer l_01iResult

    move (repeat(character(0),255)) to strName
    getAddress of strName to lpNameAddr
    move (repeat(character(0),_SIZEGETCOMPUTERNAME_SIZE)) to lsSize
    put 16 to lsSize at SIZEGETCOMPUTERNAME.dwSize
    getAddress of lsSize to lpSize
    move (GetComputername(lpNameAddr, lpSize )) to l_01iResult
    
    if (l_01iResult) function_return (cstring(strName)) // return with terminating null char removed
    else function_return "Unknown"
end_function
    
// Grab the windows username
function get_user_name global integer argv returns string
    local string strName
    local pointer lpNameAddr
    local integer l_01iResult

    move (repeat(character(0),255)) to strName
    getAddress of strName to lpNameAddr
    move (WNetGetUser(0, lpNameAddr, DWORDtoBytes(255))) to l_01iResult
    if (l_01iResult = 0) function_return (uppercase(cstring(strName))) // return with terminating null char removed
    else function_return "Unknown"
end_function

// Use a standard windows folder browser, takes the title of the browser, returns the file path
function folder_browse global string argv returns String
    local string sFolder sBrowseInfo sTitle
    local pointer lpItemIdList lpsFolder lpsBrowseInfo lpsTitle
    integer iFolderSelected l_iRetval

    zeroType _BROWSEINFO to sBrowseInfo

    if (argv<>"") begin
        move argv to sTitle
        getAddress of sTitle to lpsTitle
        put lpsTitle to sBrowseInfo at BROWSEINFO.lpszTitle
    end

    put BIF_RETURNONLYFSDIRS+BIF_EDITBOX+BIF_BROWSEINCLUDEFILES to sBrowseInfo At BROWSEINFO.ulFlags
    // put (window_handle(focus(desktop))) to sBrowseInfo At BROWSEINFO.hWndOwner ??
    move (repeat(character(0),260)) to sFolder // set the size of sFolder to 128 by nulling in 8 chars
    
    getAddress Of sBrowseInfo to lpsBrowseInfo
    getAddress Of sFolder to lpsFolder

    move (SHBrowseForFolder(lpsBrowseInfo)) to lpItemIdList // select folder
    move (SHGetPathFromIDList(lpItemIdList, lpsFolder)) to iFolderSelected // get folder name
    move (CoTaskMemFree(lpItemIdList)) to l_iRetval // release memory used by ItemIdList

    if (iFolderSelected = 0)  move "" to sFolder
    
    function_return (cString(sFolder)) // return with terminating null char removed
end_function

// Function to convert a long filename into a windows short filename
function get_short_path global string argv returns string
    local string sShortPath sLongPath
    local pointer lpszShortPath lpszLongPath
    local integer cchBuffer l_iResult
    
    move (trim(argv)) to sLongPath
    move 255 to cchBuffer
    move (repeat(character(0),(cchBuffer+1))) to sShortPath
    getaddress of sLongPath to lpszLongPath
    getaddress of sShortPath to lpszShortPath
    move (GetShortPathName(lpszLongPath,lpszShortPath,cchBuffer)) to l_iResult
    
    function_return (left(sShortPath,l_iResult))
end_function

// Set of function to disable close widgets of shell
function disable_close global integer argv returns integer
    local number Ret
    local handle hWnd hMenu

    if (g_sConsoleTitleIsSet <> "DataFlex") begin
        // change the window title so we can find the window
        move (SetConsoleTitle("DataFlex")) to strmark 
        move "DataFlex" to g_sConsoleTitleIsSet
        //  Give SetConsoleTitle a chance to take effect
        sleep 1
    end
    
    // find the window
    move (FindWindow(0, "DataFlex")) to hWnd
    // grab the menu
    move (GetSystemMenu(hWnd, 0)) to hMenu
    // disable the X
    if (argv = 0) move (EnableMenuItem(hMenu, SC_CLOSE, (MF_BYCOMMAND ior MF_GRAYED))) to ret
    // enable the X    
    if (argv = 1) move (EnableMenuItem(hMenu, SC_CLOSE, (MF_BYCOMMAND ior MF_ENABLED))) to ret
    
    function_return 0
end_function

// This function will run any external application directly from dataflex
// argv = application to run (command name/path) argv2 = any parameters to pass to the program argv3 = directory to run from
function shell_exec global string argv string argv2 string argv3 returns integer
    local handle windowHandle
    local pointer lpOperation lpFile lpParameters lpDirectory
    local integer nShowCmd l_iResult
    local string sOperation sFile sParameters sDirectory

    if ((trim(argv)) <> "") begin
        move 0 to windowHandle
        move "open" to sOperation
        move argv to sFile
        if ((trim(argv2)) <> "") move argv2 to sParameters
        else move "" to sParameters 
        if ((trim(argv3)) <> "") move argv3 to sDirectory
        else move "" to sDirectory
        move "" to sDirectory

        getAddress of sOperation to lpOperation
        getAddress of sFile to lpFile
        getAddress of sParameters to lpParameters
        getAddress of sDirectory to lpDirectory

        move (ShellExecute(windowHandle,lpOperation,lpFile,lpParameters,lpDirectory,SW_SHOWMAXIMIZED)) to l_iResult
    end
end_function

// This function will run the console application stated in argv1
// argv2 = set to 1 to run the process in a new window
// argv3 = set to 1 to leave the new process running and continue without killing it
// argv4  = The time to live before killing the process - set to zero to wait until finished
// Note - Setting argv3 to 1 will result in build up of open handles for finished processes 
//        if term_proc is not used to terminate the process.
// It is possible to have multiple processes running in one window by
// setting argv2 = 0 and argv3 = 1, but handling how they behave on the screen 
// requires some careful fiddling.
function create_proc global string argv integer argv2 integer argv3 integer argv4 returns string
    local pointer lpProcessInformation lpStartupInformation
    local integer l_iResult
    local pointer lpApplicationName lpCommandLine lpProcessAttributes lpThreadAttributes lpEnvironment lpCurrentDirectory
    local integer bInheritHandles iProcessAttributes iThreadAttributes iEnvironment 
    local dword dwCreationFlags dwMilliseconds
    local string sProcessInformation sStartupInformation sApplicationName sCommandLine sCurrentDirectory l_sExit l_sTmp
    local handle hProcess hThread
    
    zeroType _PROCESS_INFORMATION to sProcessInformation
    zeroType _STARTUPINFO to sStartupInformation

    move STRINGNULL to l_sExit
    move STRINGNULL to sApplicationName
    move argv to sCommandLine
    move HEXNULL to iProcessAttributes
    move HEXNULL to iThreadAttributes
    move HEXTRUE to bInheritHandles
    move HEXNULL to iEnvironment
    move STRINGNULL to sCurrentDirectory
    if (argv2 = 0) move NORMAL_PRIORITY_CLASS to dwCreationFlags
    if (argv2 = 1) move (CREATE_NEW_CONSOLE+NORMAL_PRIORITY_CLASS) to dwCreationFlags
    
    getaddress of sApplicationName to lpApplicationName
    getaddress of sCommandLine to lpCommandLine
    getaddress of iProcessAttributes to lpProcessAttributes
    getaddress of iThreadAttributes to lpThreadAttributes
    getaddress of iEnvironment to lpEnvironment
    getaddress of sCurrentDirectory to lpCurrentDirectory
    getaddress of sProcessInformation to lpProcessInformation
    getaddress of sStartupInformation to lpStartupInformation
    
    put (length(sStartupInformation)) to sStartupInformation at STARTUPINFO.cb
    
    move (CreateProcess(lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,dwCreationFlags,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInformation,lpProcessInformation)) to l_iResult
    
    getbuff from sProcessInformation at PROCESS_INFORMATION.hProcess to hProcess
    getbuff from sProcessInformation at PROCESS_INFORMATION.hThread to hThread

    if (argv3 <> 1) begin
        if (argv4 = 0) move INFINITE to dwMilliseconds
        if (argv4 <> 0) move argv4 to dwMilliseconds
        move (WaitForSingleObject(hProcess,dwMilliseconds)) to l_iResult
        move (TerminateProcess(hProcess,HEXNULL)) to l_iResult
        move (CloseHandle(hThread)) to l_iResult
        move (CloseHandle(hProcess)) to l_iResult
    end
    if (argv3 = 1) begin                
        move hProcess to l_sExit
        append l_sExit "|" hThread
    end
    
    function_return l_sExit
end_function

// This will terminate a process started in create_proc with argv3 set to 1
// move the string returned by create_proc to argv
// set argv2 to 0 if you want to wait for the process to finish before terminating
// set argv2 to 1 if you want to terminate the process without waiting for it to finish
function term_proc global string argv integer argv2 returns integer
    local integer l_iSuccess
    local integer dwMilliseconds l_iResult
    local handle hProcess hThread
    
    move 0 to l_iSuccess
    move (trim(argv)) to argv
    if ((argv contains "|") and ((length(argv)) >= 3)) begin    
        move (left(argv,(pos("|",argv)-1))) to hProcess
        move (mid(argv,(length(argv)-pos("|",argv)),(pos("|",argv)+1))) to hThread
        move INFINITE to dwMilliseconds
        if (argv2 = 0) move (WaitForSingleObject(hProcess,dwMilliseconds)) to l_iResult
        move (TerminateProcess(hProcess,HEXNULL)) to l_iResult
        move (CloseHandle(hThread)) to l_iResult
        move (CloseHandle(hProcess)) to l_iResult
    end
    
    function_return l_iSuccess
end_function

// Check if a file is locked by a windows process
// Returns 1 if the file is locked.
function is_locked global string argv returns integer
    local integer l_iResult l_iDllErr l_iThrow
    local handle l_hFile    
    move 0 to l_iResult
    move -1 to l_hFile
    move (trim(argv)) to argv   
    if (argv <> "") begin
        move (lOpen(argv,(OF_READ+OF_SHARE_EXCLUSIVE))) to l_hFile
        move (GetLastError()) to l_iDllErr
        if ((l_hFile = -1) and (l_iDllErr = 32)) move 1 to l_iResult
        if (l_hFile <> -1) begin
            move (lClose(l_hFile)) to l_iThrow
        end
    end
    function_return l_iResult
end_function

// Check if a file exists. Returns 1 if the file exists.
function does_exist global string argv returns integer
    local integer l_iResult l_iDllErr l_iThrow
    local handle l_hFile    
    move 0 to l_iResult
    move -1 to l_hFile
    move (trim(argv)) to argv   
    if (argv <> "") begin
        move 1 to l_iResult
        move (lOpen(argv,(OF_READ+OF_SHARE_DENY_NONE))) to l_hFile
        move (GetLastError()) to l_iDllErr
        if ((l_hFile = -1) and (l_iDllErr = 2)) move 0 to l_iResult
        if (l_hFile <> -1) begin
            move (lClose(l_hFile)) to l_iThrow
        end
    end
    function_return l_iResult
end_function

// Read a text file line by line into the buffer array "Win32API_buffer"
// Returns an integer i-1 where i is the count of array elements/lines.
//
// Ref: http:// msdn2.microsoft.com/en-us/library/aa365467.aspx
function buffer_text_file global string argv string argv2 returns integer
    local string l_sBuf l_sBufL l_structBytesRead l_sLine // String l_structBytesRead used with struct to overcome problem of df not being able to getaddress of integers
    local handle l_hFileHandle l_hFile
    local pointer l_pFileName l_pBuf l_pBytesRead
    local integer l_iFileSize l_iThrow l_iBytesRead l_iBytesToRead l_i l_iDllErr l_iLines
    
    send delete_data to Win32API_buffer
    move -1 to l_iLines
    
    move (trim(argv)) to argv
    move (trim(argv2)) to argv2
    move -1 to l_hFileHandle
    move 0 to l_iBytesRead
    move 1 to l_iBytesToRead
    move "" to l_sLine
    
    if (argv <> "") begin
        getaddress of argv to l_pFileName
        move (CreateFile(l_pFileName,GENERIC_READ,FILE_SHARE_READ,HexNull,OPEN_EXISTING,0,HexNull)) to l_hFile
        move (GetFileSize(l_hFile,0)) to l_iFileSize
        for l_i from 1 to l_iFileSize
            // move (SetFilePointer(l_hFile,l_i,0,FILE_CURRENT)) to l_iThrow
            zerostring 1 to l_sBuf
            getaddress of l_sBuf to l_pBuf
            zerotype _STRUCTBYTESREAD to l_structBytesRead
            getaddress of l_structBytesRead to l_pBytesRead
            move (ReadFile(l_hFile,l_pBuf,l_iBytesToRead,l_pBytesRead,HexNull)) to l_iThrow
            getbuff from l_structBytesRead at STRUCTBYTESREAD.integer0 to l_iBytesRead
            if ((ascii(l_sBuf) = 10) or (ascii(l_sBuf) = 13) or ((argv2 <> "") and (argv2 = l_sBuf))) begin
                if (ascii(l_sBufL) <> 13) begin
                    increment l_iLines
                    set array_value of (Win32API_buffer(current_object)) item l_iLines to l_sLine
                    move "" to l_sLine
                end
                
            end
            if ((ascii(l_sBuf) <> 10) and (ascii(l_sBuf) <> 13) and ((argv2 = "") or (argv2 <> l_sBuf))) append l_sLine l_sBuf
            move l_sBuf to l_sBufL
        end 
        move (CloseHandle(l_hFile)) to l_iThrow
    end
    function_return l_iLines
end_function

// Return file size in bytes from win32
function file_size_bytes global string argv returns integer
    local integer l_iFileSize l_iThrow
    local pointer l_pFileName
    local handle l_hFile
    
    move -1 to l_iFileSize
    
    if (argv <> "") begin
        getaddress of argv to l_pFileName
        move (CreateFile(l_pFileName,GENERIC_READ,FILE_SHARE_READ,HexNull,OPEN_EXISTING,0,HexNull)) to l_hFile
        move (GetFileSize(l_hFile,0)) to l_iFileSize
        move (CloseHandle(l_hFile)) to l_iThrow
    end
    
    function_return l_iFileSize
end_function

// Attempt to convert a string from unicode to ASCII/cp850 via WideCharToMultiByte
// http:// msdn2.microsoft.com/en-us/library/ms776420.aspx
function to_ascii global string argv returns string
    local string l_sAscii l_sUnicode
    local pointer l_pAscii l_pUnicode
    local integer l_iCharsNeeded l_iThrow
    move (trim(argv)) to l_sUnicode
    
    if (l_sUnicode <> "") begin
        zerostring (length(l_sUnicode)) to l_sAscii
        getAddress of l_sAscii to l_pAscii
        getAddress of l_sUnicode to l_pUnicode
        
        // set the length of cchWideChar to -1 and function assumes null termination and calculates length itsself
        move (WideCharToMultiByte(CP_OEMCP,0,l_pUnicode,-1,0,0,0,0)) to l_iCharsNeeded              
        move (WideCharToMultiByte(CP_OEMCP,0,l_pUnicode,-1,l_pAscii,l_iCharsNeeded,0,0)) to l_iThrow
    end
    function_return (cstring(l_sAscii))
end_function

// Attempt to convert a string from ASCII to unicode via MultiByteToWideChar
function to_unicode global string argv returns string
    local string l_sAscii l_sUnicode
    local pointer l_pAscii l_pUnicode
    local integer l_iCharsNeeded l_iThrow
    move (trim(argv)) to l_sAscii
    
    if (l_sAscii <> "") begin
        zerostring (length(l_sAscii)*2) to l_sUnicode
        getAddress of l_sUnicode to l_pUnicode
        getAddress of l_sAscii to l_pAscii
        
        // set the length of cchWideChar to -1 and function assumes null termination and calculates length itsself
        move (MultiByteToWideChar(CP_ACP,0,l_pAscii,-1,0,0,0,0)) to l_iCharsNeeded              
        move (MultiByteToWideChar(CP_ACP,0,l_pAscii,-1,l_pUnicode,l_iCharsNeeded,0,0)) to l_iThrow
    end
    function_return (cstring(l_sUnicode))  
end_function

// Attempt to convert a string from ascii to UTF8 via WideCharToMultiByte
function to_utf8 global string argv returns string
    local string l_sUTF8 l_sUnicode
    local pointer l_pUTF8 l_pUnicode
    local integer l_iCharsNeeded l_iThrow
    move (trim(argv)) to l_sUnicode
    
    if (l_sUnicode <> "") begin
        zerostring (length(l_sUnicode)) to l_sUTF8
        getAddress of l_sUTF8 to l_pUTF8
        getAddress of l_sUnicode to l_pUnicode
        
        // set the length of cchWideChar to -1 and function assumes null termination and calculates lenght itsself
        move (WideCharToMultiByte(CP_UTF8,0,l_pUTF8,-1,0,0,0,0)) to l_iCharsNeeded              
        move (WideCharToMultiByte(CP_UTF8,0,l_pUTF8,-1,l_pUnicode,l_iCharsNeeded,0,0)) to l_iThrow
    end
    
    function_return (cstring(l_sUTF8))
end_function

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms647473%28v=vs.85%29.aspx
// Note security considerations, as this function doesn't enforce string lengths
function ansi_to_oem global string argv returns string
    local string l_sOem l_sAnsi
    local pointer l_pOem l_pAnsi
    local integer l_iResult
    
    if (length(argv) <> 0) begin
        move argv to l_sAnsi
        getaddress of l_sAnsi to l_pAnsi
        zerostring (length(l_sAnsi)+1) to l_sOem
        getaddress of l_sOem to l_pOem
        move (CharToOem(l_pAnsi, l_pOem)) to l_iResult
    end
    else;
        move argv to l_sOem
        
    function_return (cstring(l_sOem))
end_function

// https://msdn.microsoft.com/en-us/library/windows/desktop/ms647493%28v=vs.85%29.aspx
// Note security considerations, as this function doesn't enforce string lengths
function oem_to_ansi global string argv returns string
    local string l_sOem l_sAnsi
    local pointer l_pOem l_pAnsi
    local integer l_iResult
    
    if (length(argv) <> 0) begin
        move argv to l_sOem
        getaddress of l_sOem to l_pOem
        zerostring (length(l_sOem)+1) to l_sAnsi
        getaddress of l_sAnsi to l_pAnsi
        move (CharToOem(l_pOem, l_pAnsi)) to l_iResult
    end
    else;
        move argv to l_sAnsi
        
    function_return (cstring(l_sAnsi))
end_function

// Get running processes on the system
// http:// msdn2.microsoft.com/en-us/library/ms682629.aspx
// in progress - currently churns out list of process id's to screen
function get_procs global integer argv returns integer
    local string l_sProcesses l_sStructBytesBack l_sBuf l_sModules
    local integer l_iThrow l_iBytes l_iBytes2 l_iBytesBack l_iBytesBack2 l_i l_j l_iPid l_iMid
    local pointer l_pProcesses l_pBytesBack l_pBytesBack2 l_pModules
    local handle l_hProcess
    
    move (1024*10) to l_iBytes  
    zerostring l_iBytes to l_sProcesses
    move 0 to l_iBytesBack
    
    getAddress of l_sProcesses to l_pProcesses  
    zerotype _STRUCTBYTESBACK to l_sStructBytesBack
    getaddress of l_sStructBytesBack to l_pBytesBack
    
    move (EnumProcesses(l_pProcesses,l_iBytes,l_pBytesBack)) to l_iThrow

    getbuff from l_sStructBytesBack at STRUCTBYTESBACK.integer0 to l_iBytesBack
    
    if (mod(l_iBytesBack,4) = 0) begin
        for l_i from 1 to (l_iBytesBack/4)
            move (left(l_sProcesses,4)) to l_sBuf
            move (mid(l_sProcesses,(length(l_sProcesses)-4),5)) to l_sProcesses
            getbuff from l_sBuf at PROCESSARRAY.arrayItem to l_iPid     
            move (OpenProcess((PROCESS_VM_READ ior PROCESS_QUERY_INFORMATION),HEXNULL,l_iPid)) to l_hProcess
            
            move 1024 to l_iBytes2
            zerostring l_iBytes2 to l_sModules
            getAddress of l_sModules to l_pModules
            zerotype _STRUCTBYTESBACK to l_sStructBytesBack
            getaddress of l_sStructBytesBack to l_pBytesBack2
            
            move (EnumProcessModules(l_hProcess,l_pModules,l_iBytes2,l_pBytesBack2)) to l_iThrow
            getbuff from l_sStructBytesBack at STRUCTBYTESBACK.integer0 to l_iBytesBack2
            
            // Err here            
            showln l_i  " PROC=" l_iPid " BYTES=" l_iBytesBack2 " H=" l_hProcess " LERR=" (GetLastError())
            
            // showln l_iBytesBack2 " " l_hProcess
            if (mod(l_iBytesBack2,4) = 0) begin
                for l_j from 1 to (l_iBytesBack2/4)
                    move (left(l_sModules,4)) to l_sBuf
                    move (mid(l_sModules,(length(l_sModules)-4),5)) to l_sModules
                    getbuff from l_sBuf at PROCESSARRAY.arrayItem to l_iMid

                loop
            end
            move (CloseHandle(l_hProcess)) to l_iThrow
            
        loop
    end
        
    showln l_iThrow
    showln "BYTES " l_iBytesBack
    
    function_return 0
end_function

// Returns the current system time via the GetSystemTime call
// Takes an integer value; 
//     1 - displays individual segments comma separated
//     0 - displays a formatted date time
function time_data global integer argv returns string
    local string sTimeData sResult sFormattedTime sFormattedDate
    local pointer pTimeData pFormattedTime pFormattedDate
    local integer iThrow iYear iMonth iDayOfWeek iDay iHour iMinute iSecond iMilliSeconds iLenCcTime iLenCcDate iDataLength
    
    zeroType _SYSTEMTIME to sTimeData
    getAddress of sTimeData to pTimeData
    move (GetSystemTime(pTimeData)) to iThrow
    
    // just return the structure comma separated
    if (argv = 1) begin
        getBuff from sTimeData at SYSTEMTIME.wYear to iYear
        getBuff from sTimeData at SYSTEMTIME.wMonth to iMonth
        getBuff from sTimeData at SYSTEMTIME.wDayOfWeek to iDayOfWeek
        getBuff from sTimeData at SYSTEMTIME.wDay to iDay
        getBuff from sTimeData at SYSTEMTIME.wHour to iHour
        getBuff from sTimeData at SYSTEMTIME.wMinute to iMinute
        getBuff from sTimeData at SYSTEMTIME.wSecond to iSecond
        getBuff from sTimeData at SYSTEMTIME.wMilliSeconds to iMilliSeconds
        
        move "" to sResult
        append sResult iYear "," iMonth "," iDay "," iHour "," iMinute "," iSecond "," iMilliSeconds
    end 
    // give formatted date_time
    if (argv = 0) begin 
        zerostring 255 to sFormattedTime
        getaddress of sFormattedTime to pFormattedTime
        move (length(sFormattedTime)) to iLenCcTime
    
        move (GetTimeFormat("LOCALE_USER_DEFAULT", 0, pTimeData, 0, pFormattedTime, iLenCcTime)) to iDataLength

        zerostring 255 To sFormattedDate
        getaddress of sFormattedDate To pFormattedDate
        move (length(sFormattedDate)) to iLenCcDate
    
        move (GetDateFormat("LOCALE_USER_DEFAULT", 0, pTimeData, 0, pFormattedDate, iLenCcDate)) to iDataLength
        move (cstring(sFormattedDate)) to sResult
        append sResult " " (cstring(sFormattedTime)) // terminating null char removed    
    end
    function_return sResult
end_function

// Insert zeros into the correct places to make a field x wide (similar to zeropad)
function fill_0 global integer iValue integer iSize returns string
    local string sReturn

    move iValue to sReturn
    while (length(sReturn) < iSize)
        insert '0' in sReturn at 1
    end

    function_return sReturn
end_function

// Checks the runtime date format and if it's not adding on the epoch add it
function check_date_error global string sDate returns date
    local integer iDate iY1k
    local Date    dDate

    move sDate to iDate
    move 693975 to iY1k

    if (iDate < iY1k) Calc (iDate + iY1k) to iDate
    move iDate to dDate

    function_return dDate
end_function

// Get the mod time of a file
// This is the core of the replacement / leap year fix of GET_FILE_MOD_TIME
// Usage:
// get_time(<file>, <mode>)
//     1 = created time
//     2 = accessed time
//     3 = modified time
// 
function get_time global string sFileName integer iMode returns string
    local string  sCreated sLastAccess sLastChanged sSystemTime sLocalTime sDate sTime sDateSep
    Local pointer pCreated pLastAccess pLastChanged pSystemTime pLocalTime pFileName
    Local handle  hCheckFile
    Local integer iResult iVal iDateSep iDateFormat iDate4State

    move "" to sTime
    move "" to sDate
    
    getaddress of sFileName to pFileName
    move (CreateFile(pFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING , 0, 0)) to hCheckFile
    
    if (hCheckFile <> INVALID_HANDLE_VALUE) begin
        zerotype _FILETIME to sCreated
        zerotype _FILETIME to sLastAccess
        zerotype _FILETIME to sLastChanged
        zerotype _FILETIME to sLocalTime
        getAddress of sCreated to pCreated
        getAddress of sLastAccess to pLastAccess
        getAddress of sLastChanged to pLastChanged
        getAddress of sLocalTime to pLocalTime

        move (GetFileTime (hCheckFile, pCreated, pLastAccess, pLastChanged)) to iResult 
          
        if (iResult) begin
            if (iMode = 1) move (FileTimeToLocalFileTime(pCreated,pLocalTime)) to iResult
            else if (iMode = 2) move (FileTimeToLocalFileTime(pLastAccess,  pLocalTime)) to iResult
            else if (iMode = 3) move (FileTimeToLocalFileTime(pLastChanged, pLocalTime)) to iResult
            
            zerotype _SYSTEMTIME2 to sSystemTime
            getAddress of sSystemTime to pSystemTime

            move (FileTimeToSystemTime(pLocalTime, pSystemTime)) to iResult

            get_attribute DF_DATE_SEPARATOR to iDateSep
            move (character(iDateSep))   to sDateSep
            get_attribute DF_DATE_FORMAT to iDateFormat

            if (iDateFormat = DF_DATE_USA) begin
                getbuff from sSystemTime at SYSTEMTIME2.wMonth to iVal
                append sDate (fill_0(iVal,2))
                getbuff from sSystemTime at SYSTEMTIME2.wDay to iVal
                append sDate sDateSep (fill_0(iVal,2))
                getbuff from sSystemTime at SYSTEMTIME2.wYear to iVal
                append sDate sDateSep (fill_0(iVal,4))
            end
            else if iDateFormat eq DF_DATE_EUROPEAN begin
                getbuff from sSystemTime at SYSTEMTIME2.wDay to iVal
                append sDate (fill_0(iVal,2))
                getbuff from sSystemTime at SYSTEMTIME2.wMonth to iVal
                append sDate sDateSep (fill_0(iVal,2))
                getbuff from sSystemTime at SYSTEMTIME2.wYear to iVal
                append sDate sDateSep (fill_0(iVal,4))
            end
            else if iDateFormat eq DF_DATE_MILITARY begin
                getbuff from sSystemTime at SYSTEMTIME2.wYear to iVal
                append sDate (fill_0(iVal,4))
                getbuff from sSystemTime at SYSTEMTIME2.wMonth to iVal
                append sDate sDateSep (fill_0(iVal,2))
                getbuff from sSystemTime at SYSTEMTIME2.wDay to iVal
                append sDate sDateSep (fill_0(iVal,2))
            end

            getbuff from sSystemTime at SYSTEMTIME2.wHour to iVal
            append sTime (fill_0(iVal,2))
            getbuff from sSystemTime at SYSTEMTIME2.wMinute to iVal
            append sTime ":" (fill_0(iVal,2))
            getbuff from sSystemTime at SYSTEMTIME2.wSecond to iVal
            append sTime ":" (fill_0(iVal,2))

            append sDate sTime
        end
        move (CloseHandle (hCheckFile)) to iResult
    end

    function_return sDate
end_function

// Create a guid GUID (Microsoft)
function create_guid global returns string        
    local integer l_iThrow
    local pointer l_ptGUID l_ptGUIDString
    local string l_stGUID l_stGUIDString l_sResult
   
    zerotype _GUID to l_stGUID
    getaddress of l_stGUID to l_ptGUID

    zerostring GUID_STRING_LENGTH to l_stGUIDString
    getaddress of l_stGUIDString to l_ptGUIDString
    
    if (CoCreateGuid(l_ptGUID) = 0) begin
        // If successfully created put it in a string
        move (StringFromGUID2(l_ptGUID, l_ptGUIDString, GUID_STRING_LENGTH)) to l_iThrow
        move (cstring(to_ascii(l_stGUIDString))) to l_sResult
    end
    
    function_return l_sResult
end_function

// Get textual description of a win32 error returned by GetLastError()
function get_last_error_detail global integer iError returns string
    local integer l_iThrow
    local string l_sBuf 
    local pointer l_pBuf
    
    zerostring 200 to l_sBuf
    getaddress of l_sBuf to l_pBuf
    
    move (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, iError, LANG_NEUTRAL, l_pBuf, 200, 0)) to l_iThrow
    
    function_return (string(iError)+": "+l_sBuf)
end_function

// Get system disk info
// argv1 = disk mount point i.e. c:\
// argv2 = "" or "TOTAL", where TOTAL returns free and blank displays used.
function disk_info global string argv string argv2 returns number
    local string l_sSectorsPerCluster l_sBytesPerSector l_sNumberOfFreeClusters l_sTotalNumberOfClusters
    local pointer l_pSectorsPerCluster l_pBytesPerSector l_pNumberOfFreeClusters l_pTotalNumberOfClusters
    local number l_iSectorsPerCluster l_iBytesPerSector l_iNumberOfFreeClusters l_iTotalNumberOfClusters l_iSpace
    
    move 0 to l_iSpace

    if (argv <> "") begin
        zerotype _DISKDATA1 to l_sSectorsPerCluster
        zerotype _DISKDATA2 to l_sBytesPerSector 
        zerotype _DISKDATA3 to l_sNumberOfFreeClusters 
        zerotype _DISKDATA4 to l_sTotalNumberOfClusters

        getaddress of l_sSectorsPerCluster to l_pSectorsPerCluster
        getaddress of l_sBytesPerSector to l_pBytesPerSector
        getaddress of l_sNumberOfFreeClusters to l_pNumberOfFreeClusters
        getaddress of l_sTotalNumberOfClusters to l_pTotalNumberOfClusters

        showln (GetDiskFreeSpace(argv, l_pSectorsPerCluster, l_pBytesPerSector, l_pNumberOfFreeClusters, l_pTotalNumberOfClusters))

        getbuff from l_sSectorsPerCluster at DISKDATA1.sectorsPerCluster to l_iSectorsPerCluster
        getbuff from l_sBytesPerSector at DISKDATA2.bytesPerSector to l_iBytesPerSector
        getbuff from l_sNumberOfFreeClusters at DISKDATA3.numberOfFreeClusters to l_iNumberOfFreeClusters
        getbuff from l_sTotalNumberOfClusters at DISKDATA4.totalNumberOfClusters to l_iTotalNumberOfClusters

        // showln l_iSectorsPerCluster
        // showln l_iBytesPerSector 
        // showln l_iNumberOfFreeClusters
        // showln l_iTotalNumberOfClusters
        
        if (argv2 = "TOTAL") calc (l_iSectorsPerCluster*l_iBytesPerSector*l_iTotalNumberOfClusters) to l_iSpace
        else calc (l_iSectorsPerCluster*l_iBytesPerSector*l_iNumberOfFreeClusters) to l_iSpace
    end
    
    function_return l_iSpace
end_function

// Get system memory usage
function get_mem_usage global returns integer
    local integer l_iThrow l_iPid l_iMem
    local string l_sProcessMemoryCounters
    local pointer l_lpProcessMemoryCounters
    local handle l_hProcess

    zeroType _PROCESS_MEMORY_COUNTERS to l_sProcessMemoryCounters
    getaddress of l_sProcessMemoryCounters to l_lpProcessMemoryCounters

    put (length(l_sProcessMemoryCounters)) to l_sProcessMemoryCounters at PROCESS_MEMORY_COUNTERS.cb

    move (get_process_id(0)) to l_iPid
    move (OpenProcess((PROCESS_VM_READ ior PROCESS_QUERY_INFORMATION),HEXNULL,l_iPid)) to l_hProcess

    move (GetProcessMemoryInfo(l_hProcess, l_lpProcessMemoryCounters, length(l_sProcessMemoryCounters))) to l_iThrow
    getbuff from l_sProcessMemoryCounters at PROCESS_MEMORY_COUNTERS.WorkingSetSize to l_iMem
    
    // showln l_hProcess " " l_iThrow
    // showln (GetLastError())
    // showln (get_last_error_detail(GetLastError()))

    function_return l_iMem
end_function

// Uses Microsofts InternetCanonicalizeUrl functionality
// http:// msdn.microsoft.com/en-us/library/windows/desktop/aa384342%28v=vs.85%29.aspx
// https:// groups.google.com/forum/#!topic/microsoft.public.vb.winapi/0RY8jPsx_14
function urldecode global string argv returns string
    local string l_szUrl l_szBuffer l_szStructBytesRead l_sResult
    local pointer l_lpszUrl l_lpszBuffer l_lpdwBufferLength 
    local integer l_iwBufferLength l_iResult l_iDllErr

    move argv to l_szUrl
    move argv to l_sResult
    
    if (length(l_szUrl) > 0) begin
        zerostring ((length(l_szUrl))+1) to l_szBuffer
        getaddress of l_szUrl to l_lpszUrl
        getaddress of l_szBuffer to l_lpszBuffer
        
        zerotype _STRUCTBYTESREAD to l_szStructBytesRead
        put ((length(l_szBuffer))*4) to l_szStructBytesRead at STRUCTBYTESREAD.integer0   // allow 4 bytes per char to generously allow for any length changes (should be 2)
        getaddress of l_szStructBytesRead to l_lpdwBufferLength

        move (InternetCanonicalizeUrl(l_lpszUrl, l_lpszBuffer, l_lpdwBufferLength, ICU_NO_ENCODE+ICU_DECODE)) to l_iResult

        getbuff from l_szStructBytesRead at STRUCTBYTESREAD.integer0 to l_iwBufferLength

        if (l_iResult <> 1) begin
            move (GetLastError()) to l_iDllErr
            custom_error ERROR_CODE_URLDECODE$ ERROR_MSG_URLDECODE ERROR_DETAIL_GETLASTERROR (get_last_error_detail(l_iDllErr))
        end
        move (cstring(l_szBuffer)) to l_sResult     
    end        
    function_return l_sResult        
end_function

// Uses Microsofts InternetCanonicalizeUrl functionality
// Only encodes parts before ? and #
function urlencode global string argv returns string
    local string l_szUrl l_szBuffer l_szStructBytesRead l_sResult
    local pointer l_lpszUrl l_lpszBuffer l_lpdwBufferLength 
    local integer l_iwBufferLength l_iResult l_iDllErr

    move argv to l_szUrl
    move argv to l_sResult
    if (length(l_szUrl) > 0) begin
        zerostring ((length(l_szUrl))+1) to l_szBuffer
        getaddress of l_szUrl to l_lpszUrl
        getaddress of l_szBuffer to l_lpszBuffer
        
        zerotype _STRUCTBYTESREAD to l_szStructBytesRead
        put ((length(l_szBuffer))*4) to l_szStructBytesRead at STRUCTBYTESREAD.integer0   // allow 4 bytes per char to generously allow for any length changes (should be 2)
        getaddress of l_szStructBytesRead to l_lpdwBufferLength

        move (InternetCanonicalizeUrl(l_lpszUrl, l_lpszBuffer, l_lpdwBufferLength, ICU_BROWSER_MODE)) to l_iResult

        getbuff from l_szStructBytesRead at STRUCTBYTESREAD.integer0 to l_iwBufferLength

        if (l_iResult <> 1) begin
            move (GetLastError()) to l_iDllErr
            custom_error ERROR_CODE_URLENCODE$ ERROR_MSG_URLENCODE ERROR_DETAIL_GETLASTERROR (get_last_error_detail(l_iDllErr))
        end
        move (cstring(l_szBuffer)) to l_sResult     
    end        
    function_return l_sResult        
end_function

// Functions to pull windows os version string
function get_os_version global returns string
    local string l_sOsInfo l_sVersion l_sReturn
    local pointer l_pOsInfo
    local integer l_iResult l_iMajor l_iMinor l_iBuild l_iPlatform
    
    move "" to l_sVersion
    
    zerotype _OSVERSIONINFO to  l_sOsInfo
    put (length(l_sOsInfo)) to l_sOsInfo at OSVERSIONINFO.dwOSVersionInfoSize
    getaddress of l_sOsInfo to  l_pOsInfo
    
    move (GetVersionEx(l_pOsInfo)) to l_iResult
    
    if (l_iResult = 1) begin
        getbuff from l_sOsInfo at OSVERSIONINFO.dwMajorVersion to l_iMajor
        getbuff from l_sOsInfo at OSVERSIONINFO.dwMinorVersion to l_iMinor
        getbuff from l_sOsInfo at OSVERSIONINFO.dwBuildNumber to l_iBuild
        getbuff from l_sOsInfo at OSVERSIONINFO.dwPlatformId to l_iPlatform
        // getbuff from l_sOsInfo at OSVERSIONINFO.szCSDVersion to l_sVersion    !??
        move (cstring(right(l_sOsInfo,128))) to l_sVersion
    end
    
    move ("Windows Version: "+string(l_iMajor)+"."+string(l_iMinor)+" Build: "+string(l_iBuild)+" Platform: "+string(l_iPlatform)+" CSDVersion: "+l_sVersion) to l_sReturn
    
    function_return l_sReturn 
end_function

// Functions to pull windows os version as a numeric value
function get_os_version_numeric global returns number
    local string l_sOsInfo l_sVersion
    local pointer l_pOsInfo
    local integer l_iResult l_iMajor l_iMinor l_iBuild l_iPlatform
    
    move "" to l_sVersion
    
    zerotype _OSVERSIONINFO to  l_sOsInfo
    put (length(l_sOsInfo)) to l_sOsInfo at OSVERSIONINFO.dwOSVersionInfoSize
    getaddress of l_sOsInfo to  l_pOsInfo
    
    move (GetVersionEx(l_pOsInfo)) to l_iResult
    
    if (l_iResult = 1) begin
        getbuff from l_sOsInfo at OSVERSIONINFO.dwMajorVersion to l_iMajor
        getbuff from l_sOsInfo at OSVERSIONINFO.dwMinorVersion to l_iMinor
    end
    
    function_return (number(l_iMajor)+(number(l_iMinor)/10))
end_function

// Converts binary to hex or base64 strings and vice versa
function binary_to_string_to_binary global string argv string argv2 string argv3 returns string
    local string l_sData l_sDataDecoded l_sDataSizeDecoded l_sReturn
    local pointer l_pData l_pDataDecoded l_pDataSizeDecoded
    local integer l_iResult l_iDataSize l_iDataSizeDecoded l_iOffset
    
    move argv to l_sData
    move (length(l_sData)) to l_iDataSize
    getaddress of l_sData to l_pData
     
    zerostring ((length(l_sData)*4)+1) to l_sDataDecoded
    getaddress of l_sDataDecoded to l_pDataDecoded
            
    zerotype _DW_TYPE to l_sDataSizeDecoded
    put (length(l_sDataDecoded)) to l_sDataSizeDecoded at DW_TYPE.value
    getaddress of l_sDataSizeDecoded to l_pDataSizeDecoded      
    
    case begin
        case (argv2 = "HEX") begin
            if (argv3 = 0) move (CryptBinaryToString(l_pData, l_iDataSize, CRYPT_STRING_HEX, l_pDataDecoded, l_pDataSizeDecoded)) to l_iResult
            if (argv3 = 1) move (CryptStringToBinary(l_pData, l_iDataSize, CRYPT_STRING_HEX, l_pDataDecoded, l_pDataSizeDecoded, HEXNULL, HEXNULL)) to l_iResult
        end
        case break
        case (argv2 = "BASE64") begin
            if (argv3 = 0) move (CryptBinaryToString(l_pData, l_iDataSize, CRYPT_STRING_BASE64, l_pDataDecoded, l_pDataSizeDecoded)) to l_iResult
            if (argv3 = 1) move (CryptStringToBinary(l_pData, l_iDataSize, CRYPT_STRING_BASE64, l_pDataDecoded, l_pDataSizeDecoded, HEXNULL, HEXNULL)) to l_iResult
        end
        case break
        case else custom_error ERROR_CODE_UNKNOWN_FORMAT$ ERROR_MSG_UNKNOWN_FORMAT argv2
    case end

    getbuff from l_sDataSizeDecoded at DW_TYPE.value to l_iDataSizeDecoded

    if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
        showln "ERROR: " (ternary(argv3 = 0, "CryptBinaryToString", "CryptStringToBinary")) " "  l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
        showln "DATA = " l_sDataDecoded
        showln "SIZE = " l_iDataSizeDecoded
    end
    else begin
        if (argv3 = 0) move (replaces(character(9),replaces(character(10),replaces(character(13),replaces(character(32),cstring(mid(l_sDataDecoded,l_pDataSizeDecoded-l_iOffset,1+l_iOffset)),""),""),""),"")) to l_sReturn              
        else move (cstring(mid(l_sDataDecoded,l_pDataSizeDecoded-l_iOffset,1+l_iOffset))) to l_sReturn
    end
    
    function_return l_sReturn
end_function

// Convert binary data to hex or base64 
function binary_to_string global string argv string argv2 returns string
    function_return (binary_to_string_to_binary(argv, argv2, 0))
end_function
// Convert hex or base64 strings to binary data
function string_to_binary global string argv string argv2 returns string
    function_return (binary_to_string_to_binary(argv, argv2, 1))
end_function

// List out cryptographic providers on ms windows
function ms_adv_listproviders global returns integer
    local integer l_i l_iResult l_iType
    local string l_sType l_sName l_sNameSize
    local pointer l_pType l_pName l_pNameSize

    move -1 to l_i
    repeat
        increment l_i
        
        zerotype _DW_TYPE to l_sType
        getaddress of l_sType to l_pType
        
        zerostring 255 to l_sName
        getaddress of l_sName to l_pName
        
        zerotype _DW_TYPE to l_sNameSize
        put length(l_sName) to l_sNameSize at DW_TYPE.value
        getaddress of l_sNameSize to l_pNameSize
    
        move (CryptEnumProviders(l_i, HEXNULL, 0, l_pType, l_pName, l_pNameSize)) to l_iResult

        if ((l_iResult <> 1) and (GetLastError() <> 0) and (GetLastError() <> 259)) begin
            showln "ERROR: " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
        end
        getbuff from l_sType at DW_TYPE.value to l_iType
        
        if (l_iResult = 1) showln l_i ") " l_iType " - '" (cstring(l_sName)) "'"
    until (l_iResult <> 1)

end_function

//-------------------------------------------------------------------------
// Classes
//-------------------------------------------------------------------------

// Object to provide basic implimentations of some popular hash algorithms and encryption
// provided by the Microsoft Cryptographic Provider
//
// Send message methods:
//
//    aquire_context                - Create the context of the Microsoft CSP
//    release_context               - Release the context of the Microsoft CSP
//    import_key <key> <ealg>       - Incomplete/WIP
//    derive_key <psw> <halg> <ealg>- Derives an encryption key provider when supplied
//                                    with a password, a hash algorithm and the required 
//                                    encryption.
//    modify_key_iv <iv>            - Set the initilization vector of the key provider
//    modify_key_mode <mode>        - Set the key provider mode E.g. CBC, ECB etc
//    destroy_key                   - Dispose of the current key provider
//
// Get methods:
//    hash_data <data> <halg>       - Returns a hash of the passed data in the specified 
//                                    algorithm
//    export_key                    - Returns the current encryption key
//    generate_random_key_iv        - Generates and sets a random initilization vector 
//                                    for the key provider
//    encrypt <data>                - Encrypt data
//    decrypt <data>                - Decrypt data
//
// Example usage:
//    
//    object test is an msAdvCrypt
//    end_object
//    string data buf
//    
//    // Generate a hash
//    send aquire_context to test
//    get hash_data of test "MYTEXT" "SHA1" to data
//    send release_context to test
//    showln "HASHED: " (binary_to_string(data,"HEX"))
//    
//    // Encrypt some data
//    send aquire_context to test
//    send derive_key to test "MYPASSWORD" "SHA256" "AES_256"
//    send modify_key_mode to test "CBC"
//    get generate_random_key_iv of test to buf
//    move buf to data
//    get encrypt of test "MYDATA" to buf
//    append data buf
//    send destroy_key to test
//    send release_context to test
//    showln "ENCRYPTED: " (binary_to_string(data,"HEX"))
//    
//    // Decrypt some data
//    send aquire_context to test
//    send derive_key to test "MYPASSWORD" "SHA256" "AES_256"
//    send modify_key_mode to test "CBC"
//    send modify_key_iv to test (mid(data,16,1))
//    get decrypt of test (mid(data,length(data)-16,17)) to data
//    send destroy_key to test
//    send release_context to test
//    showln "DECRYPTED: " data
//    
class msAdvCrypt is an array
    procedure construct_object string argc 
        forward send construct_object argc
        
        property handle c_hProv 
        property handle c_hHash 
        property handle c_hKey
        property string c_sAlg
    end_procedure

    procedure aquire_context
        local integer l_iResult
        local handle l_hProv
        local string l_shProv
        local pointer l_phProv
        
        zerotype _DW_TYPE to l_shProv
        getaddress of l_shProv to l_phProv
        
        if (get_os_version_numeric() < 5.2) begin
            move (CryptAcquireContext(l_phProv, HEXNULL, MS_ENH_RSA_AES_PROV_XP, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) to l_iResult
            if (GetLastError() = -2146893802) begin
                move (CryptAcquireContext(l_phProv, HEXNULL, MS_ENH_RSA_AES_PROV_XP, PROV_RSA_AES, CRYPT_NEWKEYSET)) to l_iResult
            end
        end
        else begin
            move (CryptAcquireContext(l_phProv, HEXNULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) to l_iResult
            if (GetLastError() = -2146893802) begin
                move (CryptAcquireContext(l_phProv, HEXNULL, MS_ENH_RSA_AES_PROV, PROV_RSA_AES, CRYPT_NEWKEYSET)) to l_iResult
            end
        end
        
        if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
            showln "ERROR: CryptAcquireContext " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
        end
        else begin
            getbuff from l_shProv at DW_TYPE.value to l_hProv
            set c_hProv to l_hProv
        end        
    end_procedure
    
    function make_hash string in_data string in_hashalgorithm returns string
        local integer l_iResult l_iHashSize
        local string l_shHash l_sHash l_sRawString l_sHashSize
        local handle l_hProv l_hHash
        local pointer l_phHash l_pHash l_pRawString l_pHashSize
        
        get c_hProv to l_hProv
        
        if (l_hProv = 0) begin
            custom_error ERROR_CODE_NO_CONTEXT$ ERROR_MSG_NO_CONTEXT
        end
        else begin
            move in_data to l_sRawString
            getaddress of l_sRawString to l_pRawString
            
            zerotype _HCRYPTHASH to l_shHash
            getaddress of l_shHash to l_phHash        
    
            case begin
                case (in_hashalgorithm = "MD5") begin
                    move (CryptCreateHash(l_hProv, CALG_MD5, 0, 0, l_phHash)) to l_iResult
                    zerostring (128/8) to l_sHash
                end
                case break  
                case ((in_hashalgorithm = "SHA1") or (in_HASHalgorithm = "SHA")) begin
                    move (CryptCreateHash(l_hProv, CALG_SHA1, 0, 0, l_phHash)) to l_iResult
                    zerostring (160/8) to l_sHash
                end
                case break
                case (in_hashalgorithm = "SHA256") begin
                    move (CryptCreateHash(l_hProv, CALG_SHA_256, 0, 0, l_phHash)) to l_iResult
                    zerostring (256/8) to l_sHash           
                end
                case break
                case (in_hashalgorithm = "SHA384") begin
                    move (CryptCreateHash(l_hProv, CALG_SHA_384, 0, 0, l_phHash)) to l_iResult
                    zerostring (384/8) to l_sHash           
                end
                case break
                case (in_hashalgorithm = "SHA512") begin
                    move (CryptCreateHash(l_hProv, CALG_SHA_512, 0, 0, l_phHash)) to l_iResult
                    zerostring (512/8) to l_sHash                    
                end
                case break  
                case else begin
                    custom_error ERROR_CODE_UNKNOWN_ALGORITHM$ ERROR_MSG_UNKNOWN_ALGORITHM in_hashalgorithm
                end
            case end        
        
            if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
                showln "ERROR: CryptCreateHash " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
            end
        
            getbuff from l_shHash at HCRYPTHASH.value to l_hHash
            getaddress of l_sHash to l_pHash
        
            move (CryptHashData(l_hHash, l_pRawString, (length(l_sRawString)), 0)) to l_iResult
        
            if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
                showln "ERROR: Crypthash_data " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
            end
        
            zerotype _DW_TYPE to l_sHashSize
            put (length(l_sHash)) to l_sHashSize at DW_TYPE.value
            getaddress of l_sHashSize to l_pHashSize
    
            move (CryptGetHashParam(l_hHash, HP_HASHVAL, l_pHash, l_pHashSize, 0)) to l_iResult
        
            if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
                showln "ERROR: CryptGetHashParam " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
            end    
        
            getbuff from l_sHashSize at DW_TYPE.value to l_iHashSize

            if (l_iHashSize <> length(l_sHash)) begin
                showln "WARNING: Binary data does not match expected hash size:"
                showln "DATA = " l_sHash
                showln "SIZE = " l_iHashSize " / " (length(l_sHash))
            end        
        end
    
        set c_hHash to l_hHash        
   
        function_return (mid(l_sHash,l_iHashSize,1))
    end_function 
        
    procedure destroy_hash
        local integer l_iResult
        local handle l_hHash
        
        get c_hHash to l_hHash

        if (l_hHash <> 0) begin
            move (CryptDestroyHash(l_hHash)) to l_iResult
            if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
                showln "ERROR: Cryptdestroy_hash " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
            end        
            else set c_hHash to 0
        end        
    end_procedure

    function hash_data string in_data string in_hashalgorithm returns string
        local integer l_iResult
        local string l_sHash
        
        get make_hash in_data in_hashalgorithm to l_sHash
        send destroy_hash
        
        function_return (cstring(l_sHash))
    end_function 
    
    //WIP
    procedure import_key string in_key string in_algorithm
        local integer l_iResult
        local string l_sBlobHeader l_sPlainTextKeyBlob l_shKey
        local handle l_hProv l_hKey
        local pointer l_pPlainTextKeyBlob l_phKey
        
        get c_hProv to l_hProv   
        
        zerotype _BLOBHEADER to l_sBlobHeader 
        put PLAINTEXTKEYBLOB to l_sBlobHeader at BLOBHEADER.bType
        put 2 to l_sBlobHeader at BLOBHEADER.bVersion
        put 0 to l_sBlobHeader at BLOBHEADER.Reserved
        
        case begin        
            case (in_algorithm = "DES") put CALG_DES to l_sBlobHeader at BLOBHEADER.ALG_ID
            case break 
            case (in_algorithm = "3DES") put CALG_3DES to l_sBlobHeader at BLOBHEADER.ALG_ID
            case break 
            case (in_algorithm = "3DES_112") put CALG_3DES_112 to l_sBlobHeader at BLOBHEADER.ALG_ID
            case break 
            case (in_algorithm = "AES") put CALG_AES to l_sBlobHeader at BLOBHEADER.ALG_ID
            case break 
            case (in_algorithm = "AES_128") put CALG_AES_128 to l_sBlobHeader at BLOBHEADER.ALG_ID
            case break 
            case (in_algorithm = "AES_192") put CALG_AES_192 to l_sBlobHeader at BLOBHEADER.ALG_ID
            case break 
            case (in_algorithm = "AES_256") put CALG_AES_256 to l_sBlobHeader at BLOBHEADER.ALG_ID
            case break 
            case (in_algorithm = "RC2") put CALG_RC2 to l_sBlobHeader at BLOBHEADER.ALG_ID
            case break 
            case (in_algorithm = "RC4") put CALG_RC4 to l_sBlobHeader at BLOBHEADER.ALG_ID        
            case break 
            case else custom_error ERROR_CODE_UNKNOWN_ALGORITHM$ ERROR_MSG_UNKNOWN_ALGORITHM in_algorithm
        case end
        
        zerotype _PLAINTEXTKEYBLOB to l_sPlainTextKeyBlob
        put l_sBlobHeader to l_sPlainTextKeyBlob at PLAINTEXTKEYBLOB.BLOBHEADER
        put (length(in_key)) to l_sPlainTextKeyBlob at PLAINTEXTKEYBLOB.dwKeySize
        put_string in_key to l_sPlainTextKeyBlob at PLAINTEXTKEYBLOB.rgbKeyData       
        
        getaddress of l_sPlainTextKeyBlob to l_pPlainTextKeyBlob
        
        zerotype _HCRYPTKEY to l_shKey
        getaddress of l_shKey to l_phKey
        
        move (CryptImportKey(l_hProv, l_pPlainTextKeyBlob, length(l_sPlainTextKeyBlob), 0, CRYPT_EXPORTABLE, l_phKey)) to l_iResult
        
        if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
            showln "ERROR: CryptImportKey " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
        end
            
        getbuff from l_shKey at HCRYPTKEY.value to l_hKey
        
    end_procedure
    
    procedure derive_key string in_data string in_hashalgorithm string in_algorithm
        local integer l_iResult
        local handle l_hProv l_hHash l_hKey
        local string l_sKey l_shKey
        local pointer l_phKey
        
        get c_hProv to l_hProv        
        get make_hash in_data in_hashalgorithm to l_sKey        
        get c_hHash to l_hHash

        if (l_hHash <> 0) begin
            zerotype _HCRYPTKEY to l_shKey
            getaddress of l_shKey to l_phKey

            // The default cipher mode to be used depends on the underlying CSP and the algorithm that's being used, but it's generally CBC mode
            case begin
                case (in_algorithm = "DES") move (CryptDeriveKey(l_hProv, CALG_DES, l_hHash, CRYPT_EXPORTABLE, l_phKey)) to l_iResult
                case break 
                case (in_algorithm = "3DES") move (CryptDeriveKey(l_hProv, CALG_3DES, l_hHash, CRYPT_EXPORTABLE, l_phKey)) to l_iResult
                case break 
                case (in_algorithm = "3DES_112") move (CryptDeriveKey(l_hProv, CALG_3DES_112, l_hHash, CRYPT_EXPORTABLE, l_phKey)) to l_iResult
                case break 
                case (in_algorithm = "AES") move (CryptDeriveKey(l_hProv, CALG_AES, l_hHash, CRYPT_EXPORTABLE, l_phKey)) to l_iResult
                case break 
                case (in_algorithm = "AES_128") move (CryptDeriveKey(l_hProv, CALG_AES_128, l_hHash, CRYPT_EXPORTABLE, l_phKey)) to l_iResult
                case break 
                case (in_algorithm = "AES_192") move (CryptDeriveKey(l_hProv, CALG_AES_192, l_hHash, CRYPT_EXPORTABLE, l_phKey)) to l_iResult
                case break 
                case (in_algorithm = "AES_256") move (CryptDeriveKey(l_hProv, CALG_AES_256, l_hHash, CRYPT_EXPORTABLE, l_phKey)) to l_iResult
                case break 
                case (in_algorithm = "RC2") move (CryptDeriveKey(l_hProv, CALG_RC2, l_hHash, CRYPT_EXPORTABLE, l_phKey)) to l_iResult
                case break 
                case (in_algorithm = "RC4") move (CryptDeriveKey(l_hProv, CALG_RC4, l_hHash, CRYPT_EXPORTABLE, l_phKey)) to l_iResult
                case break 
                case else custom_error ERROR_CODE_UNKNOWN_ALGORITHM$ ERROR_MSG_UNKNOWN_ALGORITHM in_algorithm
            case end
            
            if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
                showln "ERROR: CryptDeriveKey " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
            end
            else begin        
                getbuff from l_shKey at HCRYPTKEY.value to l_hKey
                set c_sAlg to in_algorithm
            end

            set c_hKey to l_hKey
        end
    end_procedure
       
    procedure modify_key_iv string in_iv
        local integer l_iResult l_iBlockSize
        local handle l_hKey
        local string l_sIV l_sAlg
        local pointer l_pIV
        
        get c_hKey to l_hKey
        get c_sAlg to l_sAlg
        
        // Set expected block size in bytes
        case begin
            case (l_sAlg contains "DES") calc (64/8) to l_iBlockSize
            case break         
            case (l_sAlg contains "AES") calc (128/8) to l_iBlockSize
            case break 
            case (l_sAlg = "RC2") calc (64/8) to l_iBlockSize
            case break 
            case else custom_error ERROR_CODE_INCOMPATIBLE_ALGORITHM$ ERROR_MSG_INCOMPATIBLE_ALGORITHM l_sAlg
        case end
        
        if (length(in_iv) <> l_iBlockSize) custom_error ERROR_CODE_INVALID_BLOCKSIZE$ ERROR_MSG_INVALID_BLOCKSIZE (l_sAlg+"="+string(l_iBlockSize)+" NOT "+string(length(in_iv)))
        
        move in_iv to l_sIV
        getaddress of l_sIV to l_pIV
        
        move (CryptSetKeyParam(l_hKey, KP_IV, l_pIV, 0)) to l_iResult
        
        if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
            showln "ERROR: CryptSetKeyParam " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
        end        
        
    end_procedure
    
    function generate_random_key_iv returns string
        local integer l_i l_iBlockSize
        local string l_sIV l_sAlg        
     
        get c_sAlg to l_sAlg
        move "" to l_sIV
        
        if ((l_sAlg contains "DES") or (l_sAlg = "RC2")) calc (64/8) to l_iBlockSize
    if (l_sAlg contains "AES") calc (128/8) to l_iBlockSize
        
        for l_i from 1 to l_iBlockSize
            append l_sIV (character(48+random(47)))
        loop
        
        send modify_key_iv l_sIV
        
        function_return l_sIV
    end_function
    
    procedure modify_key_mode string in_mode
        local integer l_iResult
        local handle l_hKey
        local string l_sMode l_sbData
        local pointer l_pbData
        
        get c_hKey to l_hKey

        case begin
            case (in_mode contains "CBC") move CRYPT_MODE_CBC to l_sMode
            case break
            case (in_mode contains "ECB") move CRYPT_MODE_ECB to l_sMode
            case break
            case (in_mode contains "OFB") move CRYPT_MODE_OFB to l_sMode
            case break
            case (in_mode contains "CFB") move CRYPT_MODE_CFB to l_sMode
            case break
            case (in_mode contains "CTS") move CRYPT_MODE_CTS to l_sMode
            case break            
            case else custom_error ERROR_CODE_UNRECOGNISED_MODE$ ERROR_MSG_UNRECOGNISED_MODE l_sMode        
        case end
        
        zerotype _DW_TYPE to l_sbData
        put l_sMode to l_sbData at DW_TYPE.value
        getaddress of l_sbData to l_pbData       
        
        move (CryptSetKeyParam(l_hKey, KP_MODE, l_pbData, 0)) to l_iResult
        
        if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
            showln "ERROR: CryptSetKeyParam " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
        end        
    
    end_procedure
    
    function export_key returns string
        local integer l_iResult
        local string l_sData l_sDataSize
        local handle l_hKey
        local pointer l_pData l_pDataSize
        local integer l_iKeyBlobSize l_iDataSize
        
        get c_hKey to l_hKey
        
        if (l_hKey <> 0) begin
            zerotype _PLAINTEXTKEYBLOB to l_sData
            getaddress of l_sData to l_pData
        
            zerotype _DW_TYPE to l_sDataSize
            put (length(l_sData)) to l_sDataSize at DW_TYPE.value
            getaddress of l_sDataSize to l_pDataSize
        
            move (CryptExportKey(l_hKey, 0, PLAINTEXTKEYBLOB, 0, l_pData, l_pDataSize)) to l_iResult
            if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
                showln "ERROR: CryptExportKey " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
            end 
        
            getbuff from l_sDataSize at DW_TYPE.value to l_iKeyBlobSize
            getbuff from l_sData at PLAINTEXTKEYBLOB.dwKeySize to l_iDataSize
            move (mid(l_sData,l_iDataSize,13)) to l_sData
            
            if (show_debug_lines) begin
                showln "DEBUG: Key blob Size = " l_iKeyBlobSize        
            end
       end
       function_return l_sData
    end_function
    
    function encrypt_decrypt string in_data integer in_decrypt returns string
        local integer l_iResult l_iDataSize
        local string l_sData l_sDataSize
        local pointer l_pData l_pDataSize 
        local handle l_hKey
        
        move in_data to l_sData    
        get c_hKey to l_hKey
        
        zerotype _DW_TYPE to l_sDataSize
        put (length(l_sData)) to l_sDataSize at DW_TYPE.value
        getaddress of l_sDataSize to l_pDataSize                        
        
        move (l_sData+repeat(' ',length(l_sData)*2)) to l_sData
        getaddress of l_sData to l_pData        
        
        if (in_decrypt = 0) move (CryptEncrypt(l_hKey, HEXNULL, HEXTRUE, 0, l_pData, l_pDataSize, length(l_sData))) to l_iResult
        else move (CryptDecrypt(l_hKey, HEXNULL, HEXTRUE, 0, l_pData, l_pDataSize)) to l_iResult             

        if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
            showln "ERROR: " (ternary(in_decrypt = 0, "CryptEncrypt", "CryptDecrypt")) " " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
        end
        
        getbuff from l_sDataSize at DW_TYPE.value to l_iDataSize
        move (cstring(mid(l_sData,l_iDataSize,1))) to l_sData
        
        function_return l_sData
    end_function
    
    function encrypt string in_data returns string
        local string l_sData
        
        get encrypt_decrypt in_data 0 to l_sData
        function_return l_sData
    end_function
    
    function decrypt string in_data returns string
        local string l_sData
        
        get encrypt_decrypt in_data 1 to l_sData
        function_return l_sData
    end_function
    
    procedure destroy_key
        local integer l_iResult
        local handle l_hKey l_hHash
        
        get c_hKey to l_hKey
        
        if (l_hKey <> 0) begin
            move (CryptDestroyKey(l_hKey)) to l_iResult     
            if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
                showln "ERROR: CryptDestroyKey " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))                      
            end  
            else begin
                set c_hKey to 0
                set c_sAlg to ""
            end
        end
        
        get c_hHash to l_hHash
        if (l_hHash <> 0) send destroy_hash        
        
    end_procedure
    
    procedure release_context
        local integer l_iResult
        local handle l_hProv  
        
        get c_hProv to l_hProv
    
        if (l_hProv <> 0) begin    
            move (CryptReleaseContext(l_hProv, 0)) to l_iResult
            if ((l_iResult <> 1) and (GetLastError() <> 0)) begin
                showln "ERROR: Cryptrelease_context " l_iResult " - " (GetLastError()) " - " (get_last_error_detail(GetLastError()))
            end    
            else set c_hProv to 0
        end
        
    end_procedure
    
    procedure destory_object
        local handle l_hProv l_hHash l_hKey
        
        get c_hKey to l_hKey
        if (l_hKey <> 0) send destroy_key
        
        get c_hHash to l_hHash
        if (l_hHash <> 0) send destroy_hash
        
        get c_hProv to l_hProv
        if (l_hProv <> 0) send release_context
             
        forward send destory_object
    end_procedure
        
end_class

//-------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------

// Used for procedural invocations of hashing and encrypting
object msAdvCrypt_global_obj is an msAdvCrypt
end_object

// Procedural one-shot use of msAdvCrypt hashing
function msAdvCrypt_hash global string in_data string in_hash returns string
    local string l_sReturn
   
    send aquire_context to msAdvCrypt_global_obj
    get make_hash of msAdvCrypt_global_obj in_data in_hash to l_sReturn
    send destroy_hash to msAdvCrypt_global_obj
    send release_context to msAdvCrypt_global_obj
    
    function_return l_sReturn
end_function

function sha512_hex global string in_data returns string
    function_return (binary_to_string(msAdvCrypt_hash(in_data, "SHA512"),'HEX'))
end_function

function sha512_base64 global string in_data returns string
    function_return (binary_to_string(msAdvCrypt_hash(in_data, "SHA512"),'BASE64'))
end_function

function sha384_hex global string in_data returns string
    function_return (binary_to_string(msAdvCrypt_hash(in_data, "SHA384"),'HEX'))
end_function

function sha384_base64 global string in_data returns string
    function_return (binary_to_string(msAdvCrypt_hash(in_data, "SHA384"),'BASE64'))
end_function

function sha256_hex global string in_data returns string
    function_return (binary_to_string(msAdvCrypt_hash(in_data, "SHA256"),'HEX'))
end_function

function sha256_base64 global string in_data returns string
    function_return (binary_to_string(msAdvCrypt_hash(in_data, "SHA256"),'BASE64'))
end_function

function sha1_hex global string in_data returns string
    function_return (binary_to_string(msAdvCrypt_hash(in_data, "SHA1"),'HEX'))
end_function

function sha1_base64 global string in_data returns string
    function_return (binary_to_string(msAdvCrypt_hash(in_data, "SHA1"),'BASE64'))
end_function

function md5_hex global string in_data returns string
    function_return (binary_to_string(msAdvCrypt_hash(in_data, "MD5"),'HEX'))
end_function

function md5_base64 global string in_data returns string
    function_return (binary_to_string(msAdvCrypt_hash(in_data, "MD5"),'BASE64'))
end_function

// Procedural one-shot use of msAdvCrypt AES256 encryption (HEX)
function aes256_hex_enc global string in_data string in_key returns string
    local string l_sReturn l_sBuf
   
    send aquire_context to msAdvCrypt_global_obj
    
    send derive_key to msAdvCrypt_global_obj in_key "SHA256" "AES_256"
    send modify_key_mode to msAdvCrypt_global_obj "CBC"
    
    get generate_random_key_iv of msAdvCrypt_global_obj to l_sBuf    
    move l_sBuf to l_sReturn
    
    get encrypt of msAdvCrypt_global_obj in_data to l_sBuf
    append l_sReturn l_sBuf
    
    send destroy_key to msAdvCrypt_global_obj    
    send release_context to msAdvCrypt_global_obj
    
    function_return (binary_to_string(l_sReturn,"HEX"))
end_function

// Procedural one-shot use of msAdvCrypt AES256 decryption (HEX)
function aes256_hex_dec global string in_data string in_key returns string
    local string l_sReturn l_sBuf
   
    move (string_to_binary(in_data,"HEX")) to l_sBuf
   
    send aquire_context to msAdvCrypt_global_obj    
    send derive_key to msAdvCrypt_global_obj in_key "SHA256" "AES_256"
    send modify_key_mode to msAdvCrypt_global_obj "CBC"
    
    send modify_key_iv to msAdvCrypt_global_obj (mid(l_sBuf,16,1))
    
    get decrypt of msAdvCrypt_global_obj (mid(l_sBuf,length(l_sBuf)-16,17)) to l_sReturn
    
    send destroy_key to msAdvCrypt_global_obj    
    send release_context to msAdvCrypt_global_obj
    
    function_return l_sReturn
end_function
