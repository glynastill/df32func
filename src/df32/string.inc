//-------------------------------------------------------------------------
// string.inc
//      This file contains some DataFlex 3.2 Console Mode functions
//      and classes to provide extended string manipulation capabilities.
//
// This file is to be included in df32func.mk
//
// Copyright (c) 2006-2009, glyn@8kb.co.uk
// 
// df32func/string.inc
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------

// Convert a string to titlecase
function titlecase global string argv returns string
    local string l_01tmpStr
    local integer l_01tmpInt

    move "" to l_01tmpStr
    lp_maketitle01:
    append l_01tmpStr (uppercase(left(argv,1)))
    pos " " in argv to l_01tmpInt
    if l_01tmpInt eq 0 begin
        length (trim(argv)) to l_01tmpInt
        append l_01tmpStr (lowercase(right(argv,(l_01tmpInt-1))))
        goto lp_exittitle01
    end
    append l_01tmpStr (lowercase(mid(argv,(l_01tmpInt-1),2)))        
    trim (mid(argv,(length(argv)),(l_01tmpInt+1))) to argv        
    goto lp_maketitle01
    lp_exittitle01:

    function_return l_01tmpStr
end_function

//replace all characters in argv2 found in argv with srting in argv3 - see replaces
function replaceall global string argv string argv2 string argv3 returns string
    local integer l_01tmpInt
    
    move (trim(argv)) to argv
    for l_01tmpInt from 1 to (length(argv))
        move (replaces((mid(argv,1,l_01tmpInt)),argv2,argv3)) to argv2
    loop
    
    function_return argv2
end_function

// Pad a string with zeros to the left
function zeropad global string argv integer argv2 returns string
    local string l_01tmpStr
    
    move "" to l_01tmpStr
    move (repeat("0",(argv2-(length(trim(argv)))))) to l_01tmpStr
    append l_01tmpStr (trim(argv))
    
    function_return l_01tmpStr
end_function

// Padd a string with spaces to the left
function leftpad global string argv integer argv2 returns string
    local string l_01tmpStr
    
    move "" to l_01tmpStr
    move (repeat(" ",(argv2-(length(trim(argv)))))) to l_01tmpStr
    append l_01tmpStr (trim(argv))
    
    function_return l_01tmpStr
end_function

// Returns the string in reverse
function reverse global string argv returns string
    local string l_sReturn
    local integer l_i l_iLen
    
    move (length(argv)) to l_iLen
    move "" to l_sReturn
    
    for l_i from 0 to l_iLen
        append l_sReturn (mid(argv,1,l_iLen-l_i))
    loop
    
    function_return l_sReturn
end_function

// Standard escaping via C standard.
//
// For PostgreSQL when no_backslash_quote is defined single quotes are 
// escaped per SQL standard by doubling '' rather than \' because in 
// some encodings multibyte characters have a last byte numerically 
// equivalent to ASCII escaped by backslash "\".
// This should not be required if client encoding is LATIN1
// and safe_encoding is set.
function addslashes global string argv returns string
    local string l_sReturn
    
    move (replaces("\",argv,"\\")) to l_sReturn
    #IFDEF no_backslash_quote
        move (replaces("'",l_sReturn,"''")) to l_sReturn
    #ELSE
        move (replaces("'",l_sReturn,"\'")) to l_sReturn
    #ENDIF
    move (replaces('"',l_sReturn,'\"')) to l_sReturn
    
    function_return l_sReturn
end_function

// Standard escaping for quoted CSV standard
function quotecsv global string argv returns string
    local string l_sReturn
    
    move (replaces('"',argv,'""')) to l_sReturn
    
    function_return l_sReturn
end_function

// Sanitize an integer for use in an sql statement. 
// This doesn't do anything other than make sure the value fits in the integer
function sanitize_int global integer l_iInput integer bound returns integer
    local integer l_iReturn
    
    move (integer(l_iInput)) to l_iReturn
    
    function_return l_iReturn   
end_function

// Sanitize an number for use in an sql statement. 
// This doesn't do anything other than make sure the value fits in the number
function sanitize_num global number l_nInput returns integer
    local number l_nReturn
    
    move (number(l_nInput)) to l_nReturn
    
    function_return l_nReturn   
end_function

// Sanitize a string. 3 Modes
// SQL - this just does addslashes 
// SYSTEM - removes all characters that could be problematic at the console
// PARANOID - removes all non alphanumeric characters and any aparent sql it is worried about.
function sanitize_str global string l_sInput string l_sLevel returns string
    local string l_sReturn
    local integer l_i l_iCode l_iBadScore
    
    move (uppercase(trim(l_sLevel))) to l_sLevel
    if ((l_sLevel <> "SQL") and (l_sLevel <> "SYSTEM") and (l_sLevel <> "PARANOID")) move "PARANOID" to l_sLevel
    move "" to l_sReturn
    
    if (length(l_sInput) <> 0) begin
        if (l_sLevel = "SQL") move (addslashes(l_sInput)) to l_sReturn
        
        if (l_sLevel = "SYSTEM") begin
            move (replaceall("!=()<>/\|`'^~%$#;&",l_sInput,"")) to l_sReturn
            move (replaces(character(10),l_sReturn,"")) to l_sReturn
            move (replaces(character(13),l_sReturn,"")) to l_sReturn
            move (replaces('"',l_sReturn,'')) to l_sReturn
        end
        if (l_sLevel = "PARANOID") begin
            for l_i from 1 to (length(l_sInput))
                move (ascii(mid(l_sInput,1,l_i))) to l_iCode
                if (((l_iCode >= 48) and (l_iCode <= 57)) or ((l_iCode >= 65) and (l_iCode <=90)) or ((l_iCode >= 97) and (l_iCode <=122)) or (l_iCode = 32)) begin
                    append l_sReturn (mid(l_sInput,1,l_i))
                end
            loop
            
            move 0 to l_iBadScore
            if ((uppercase(l_sReturn) contains "DROP ") or (uppercase(l_sReturn) contains "CREATE ") or (uppercase(l_sReturn) contains "ALTER ") or (uppercase(l_sReturn) contains "TRUNCATE ") or (uppercase(l_sReturn) contains "COPY ")) begin
                increment l_iBadScore
                if (uppercase(l_sReturn) contains " TABLE ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " INDEX ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " DATABASE ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " GROUP ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " FUNCTION ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " RULE ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " AGGREGATE ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " TYPE ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " TRIGGER ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " OPERATOR ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " USER ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " SEQUENCE ") increment l_iBadScore
            end
            else if ((uppercase(l_sReturn) contains "GRANT ") or (uppercase(l_sReturn) contains "REVOKE ")) begin
                increment l_iBadScore
                if ((uppercase(l_sReturn) contains " ON ") and (uppercase(l_sReturn) contains " TO ")) increment l_iBadScore
                if ((uppercase(l_sReturn) contains " ON ") and (uppercase(l_sReturn) contains " FROM ")) increment l_iBadScore
                if (uppercase(l_sReturn) contains " ALL ") increment l_iBadScore
                if (uppercase(l_sReturn) contains " ALL ") increment l_iBadScore
                
            end
            else if ((uppercase(l_sReturn) contains "UPDATE ") or (uppercase(l_sReturn) contains "DELETE ")) begin
                increment l_iBadScore
                if (not (uppercase(l_sReturn)) contains " WHERE") increment l_iBadScore
            end
            if (l_iBadScore > 1) begin
                move "" to l_sReturn
            end
        end
        
    end
    else begin
        move l_sInput to l_sReturn
    end
    
    function_return l_sReturn   
end_function

// Return none blank of two strings
function nbstring global string argv string argv2 returns string
    if (argv <> "") function_return argv
    else if (argv2 <> "") function_return argv2
    else function_return ""
end_function

// Do transformation of xml based on xsl stylesheet
// E.g. msxsl("\\somehost\bin\msxsl.exe", "c:\test.xml","c:\test.xsl","","c:\test.html")
function msxsl global string engine string source string stylesheet string params string outfile returns string
    local string l_sRemoteEngine l_sFile l_sOpts l_sReturn
    local string l_iThrow l_iFileSize
    
    move (trim(engine)) to engine
    move (trim(source)) to source
    move (trim(stylesheet)) to stylesheet
    move (trim(params)) to params
    move (trim(outfile)) to outfile
    move "" to l_sReturn 
    
    // Attempt to keep a copy of the executable locally regardless of location
    if not (g_bMsxslPresent) begin
        if (does_exist(engine) = 1) begin
            if (does_exist(g_sMsxslEngine) = 0) begin
                move (fileopp("copy",engine,g_sMsxslEngine)) to l_iThrow
                if (l_iThrow <> 0) move engine to g_sMsxslEngine
            end
            indicate g_bMsxslPresent true
        end
        else indicate g_bMsxslPresent false
    end
    
    if (g_bMsxslPresent) begin
        if (outfile = "") move (trim(cstring(get_local_temp(0)))+"msxsl."+(create_guid())) to l_sFile
        else move outfile to l_sFile
    
        if ((source <> "") and (does_exist(source) = 1) and (stylesheet <> "") and (does_exist(stylesheet) = 1)) begin
            move (source+" "+stylesheet+" -o "+l_sFile) to l_sOpts
            if (params <> "") append l_sOpts " " params
            move (create_proc(g_sMsxslEngine+" "+l_sOpts,0,0,0)) to l_iThrow

            if (outfile = "") begin
                move (file_size_bytes(l_sFile)) to l_iFileSize
                direct_input channel default_file_channel l_sFile
                    read_block channel default_file_channel l_sReturn l_iFileSize
                close_input channel default_file_channel
                move (fileopp("delete",l_sFile,"")) to l_iThrow
            end
            else move outfile to l_sReturn
        end
    end
    
    function_return l_sReturn
end_function

//-------------------------------------------------------------------------
// Classes
//-------------------------------------------------------------------------

// String tokenizer class
//
// Send message methods:
//    set_string
//
// Set methods:
//    token_value 
//
// Get methods:
//    token_value 
//    token_count
//    next_token
//    token_ptr
//
// Example usage:
//
//    object myToken is a StringTokenizer
//    end_object
//
//    send set_string to (myToken(current_object)) tmp ","
//
//    get token_count of (myToken(current_object)) to x
//
//    for i from 0 to x
//        get token_value of (myToken(current_object)) item i to buf
//        showln buf
//    loop
//
//    repeat
//        get next_token of (myToken(current_object)) to buf
//        showln buf
//        get token_ptr of (myToken(current_object)) to i
//    until (i = -1)

class StringTokenizer is an array
    procedure construct_object integer argc
            forward send construct_object
            property integer c_iTokens public argc
            property integer c_iTokenOn
    end_procedure

    procedure set_string string inString string inSep
        local integer l_iTokens l_iPos l_iPad
        local string l_01tmpStr l_02tmpStr

        move -1 to l_iTokens
        move (trim(inString)) to l_01tmpStr
        move (length(inSep)) to l_iPad
        move 2 to l_iPos
        
        while (l_01tmpStr <> "") 
            if (inSep <> "") move (pos(inSep,l_01tmpStr)) to l_iPos
                
            move (left(l_01tmpStr, (l_iPos-1))) to l_02tmpStr
            if (l_01tmpStr = l_02tmpStr) move "" to l_01tmpStr
            else move (right(l_01tmpStr,length(l_01tmpStr)-(l_iPos+l_iPad-1))) to l_01tmpStr

            increment l_iTokens
            forward set array_value item l_iTokens to l_02tmpStr
        end 
    
        set c_iTokenOn to 0
        set c_iTokens to l_iTokens
    end_procedure

    procedure set token_value integer itemx string val
        forward set array_value item itemx to val
    end_procedure

    function token_value integer itemx returns string
        local string l_sBuf                
        forward get string_value item itemx to l_sBuf
        function_return l_sBuf
    end_function 

    function next_token returns string
        local string l_sBuf
        local string l_iTokenOn l_iTokens
        
        get c_iTokenOn to l_iTokenOn                
        get c_iTokens to l_iTokens
        forward get string_value item l_iTokenOn to l_sBuf
        
        if (l_iTokenOn < l_iTokens) set c_iTokenOn to (l_iTokenOn+1)
        else set c_iTokenOn to -1
        function_return l_sBuf
    end_function 

    function token_ptr returns integer
        local integer l_iTokenOn
        get c_iTokenOn to l_iTokenOn
        function_return l_iTokenOn
    end_function
    
    function token_count returns integer
        local integer l_iTokens
        get c_iTokens to l_iTokens
        function_return l_iTokens
    end_function
end_class
