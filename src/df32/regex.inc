//-------------------------------------------------------------------------
// regex.inc
//      This file contains DataFlex functions to provide basic regex 
//      functionality based on the GNU POSIX regex library, and accessed
//      via Win32 API calls to df32func.dll.  
//      See df32func.h for external function definitions.
//
// This file is to be included when using Win32 capabilities in df32func.mk
//
// Copyright (c) 2006-2015, glyn@8kb.co.uk
// 
// df32func/regex.inc
//-------------------------------------------------------------------------

#IFDEF __df32func_h__
#ELSE
    #INCLUDE df32func.h
#ENDIF

//-------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------

// All the regex function accept a set of flags, this can be one or more of:
//      g = Perform match against each substring rather than just the first (greedy)
//      n = Perform newline-sensitive matching
//      i = Perform cases insensitive matching

//Purely check if a regex expression produces match in the input string
// Returns 1 on match, 0 on no match
//    E.g
//    move (regexp_match('the quick brown fox jumps over the lazy dog.', 'fox', 'g'))
function regexp_match global string str string pattern string flags returns integer
    local integer l_iReturn
    local pointer l_pStr l_pPattern l_pFlags
    
    getaddress of str to l_pStr
    getaddress of pattern to l_pPattern
    getaddress of flags to l_pFlags
    
    move (RegexpMatch(l_pStr, l_pPattern, l_pFlags, ERRORS_TO_STDERR)) to l_iReturn
    
    function_return l_iReturn
end_function   

//Return a string containing all regex matches in the input string
//    E.g
//    move (regexp_matches('the quick brown fox jumps over the la\{zy d"og.', 'fox|(the)|brown|(la\\\{zy)|(d"og)', 'g')) to myString
function regexp_matches global string str string pattern string flags returns string
    local integer l_iReturn
    local pointer l_pStr l_pPattern l_pFlags l_pOut
    local string l_sOut l_sReturn
    
    move "" to l_sReturn
    getaddress of str to l_pStr
    getaddress of pattern to l_pPattern
    getaddress of flags to l_pFlags
    zerostring MAX_DFREGEX_BUFFER to l_sOut
    getaddress of l_sOut to l_pOut
    
    move (RegexpMatches(l_pStr, l_pPattern, l_pFlags, l_pOut, MAX_DFREGEX_BUFFER, ERRORS_TO_STDERR)) to l_iReturn
    
    if (l_iReturn = 0);
        move (cstring(l_sOut)) To l_sReturn
    else begin
        if (l_iReturn = -1);
            custom_error ERROR_CODE_REGEX_BUFFER_OVERFLOW$ ERROR_MSG_REGEX_BUFFER_OVERFLOW MAX_DFREGEX_BUFFER           
        if (l_iReturn = -2);
                        custom_error ERROR_CODE_REGEX_COMPILE_FAILURE$ ERROR_MSG_REGEX_COMPILE_FAILURE
        move "" to l_sReturn
    end
    
    function_return l_sReturn
end_function 

//Perform a replacement on the input string all matches with the given pattern
//    E.g.
//    move (regexp_replace('22 quick brown foxes jump over the 44 lazy dogs.', '([0-9]*).* (foxes) .* ([0-9]*) .* (dogs).*', 'SELECT build_data(\1,\2), build_data(\3,\4);', 'g')) to myString
function regexp_replace global string str string pattern string replacement string flags returns string
    local integer l_iReturn
    local pointer l_pStr l_pPattern l_pFlags l_pReplacement l_pOut
    local string l_sOut l_sReturn
    
    move "" to l_sReturn
    getaddress of str to l_pStr
    getaddress of pattern to l_pPattern
    getaddress of flags to l_pFlags
    getaddress of replacement to l_pReplacement
    zerostring MAX_DFREGEX_BUFFER to l_sOut
    getaddress of l_sOut to l_pOut
    
    move (RegexpReplace(l_pStr, l_pPattern, l_pReplacement, l_pFlags, l_pOut, MAX_DFREGEX_BUFFER, ERRORS_TO_STDERR)) to l_iReturn
    
    if (l_iReturn = 0);
        move (cstring(l_sOut)) To l_sReturn
    else begin
        if (l_iReturn = -1);
            custom_error ERROR_CODE_REGEX_BUFFER_OVERFLOW$ ERROR_MSG_REGEX_BUFFER_OVERFLOW MAX_DFREGEX_BUFFER           
        if (l_iReturn = -2);
                        custom_error ERROR_CODE_REGEX_COMPILE_FAILURE$ ERROR_MSG_REGEX_COMPILE_FAILURE
        move "" to l_sReturn
    end
            
    function_return l_sReturn
end_function 

// Parse an output string from regexp_matches to get the result count
//    E.g
//    move (regexp_matches_count(myRegexMatchesOutput)) to myInt
function regexp_matches_count global string argv returns integer
    local integer l_iCount l_i
    local string l_sChar l_sLast
    
    move "" to l_sChar
    move "" to l_sLast
    for l_i from 0 to (length(argv))
        move (mid(argv,1,l_i)) to l_sChar
        if ((l_sChar = '{') and (l_sLast <> '\')) increment l_iCount
        move l_sChar to l_sLast
    loop
    
    function_return l_iCount
end_function

// Parse an output string from regexp_matches to get the result at an index
// This can then be treated as csv data.
//    E.g
//    move (regexp_matches_item(myRegexMatchesOutput,muInt)) to myString
//    object mt is a StringTokenizer
//    end_object
//    move (regexp_matches_count(myString)) to myCount
//    for i from 1 to myCount
//        move (regexp_matches_item(myString,i)) to bufString
//        send delete_data to mt    
//        send set_string_csv to mt bufString
//        get token_count of mt to myTokenCount
//        for j from 0 to myTokenCount
//            get token_value of mt item j to bufString
//           showln i "-" j ")" buf
//        loop
//  loop
function regexp_matches_item global string argv integer argv2 returns string
    local integer l_iCount l_i
    local string l_sChar l_sLast l_sReturn
    
    move "" to l_sChar
    move "" to l_sLast
    move "" to l_sReturn
    for l_i from 0 to (length(argv))
        move (mid(argv,1,l_i)) to l_sChar
        move (mid(argv,1,l_i-1)) to l_sLast
        if ((l_sChar = '{') and (l_sLast <> '\')) increment l_iCount        
        if ((l_sChar = '}') and (l_sLast <> '\') and (l_iCount = argv2)) break
        if (((l_sChar = '{') and (l_sLast <> '\')) or (l_iCount < argv2)) break begin
        
        append l_sReturn l_sChar
    loop
    
    function_return l_sReturn
end_function

// Parse an output string from regexp_matches to get the result at an index
// stripping out all escaping.
//    E.g
//    move (regexp_matches_item_stripped(myRegexMatchesOutput,muInt)) to myString
function regexp_matches_item_stripped global string argv integer argv2 returns string
    local integer l_iCount l_i l_iOpen l_iQuot
    local string l_sChar l_sLast l_sNext l_sBuf
    
    move 0 to l_iCount
    move 0 to l_iOpen
    move 0 to l_iQuot
    move "" to l_sLast
    for l_i from 0 to (length(argv))
        move (mid(argv,1,l_i)) to l_sChar
        move (mid(argv,1,l_i-1)) to l_sLast
        
        if ((l_sChar = '{') and (l_sLast <> '\')) increment l_iCount        
        if (l_iCount <> argv2) break begin

        move (mid(argv,1,l_i+1)) to l_sNext
        
        if ((l_sChar = '{') and not (l_iQuot)) begin
            move 1 to l_iOpen
            move "" to l_sBuf   
        end     
        else if ((l_sChar = '}') and not (l_iQuot)) begin
            move 0 to l_iOpen
        end 
        else if ((l_sChar = '"') and (l_sLast <> '\')) begin
            if (l_iQuot) move 0 to l_iQuot
            else move 1 to l_iQuot
        end
        if ((l_sChar = ',') and not (l_iOpen)) break begin
        if (((l_sChar = '{') or (l_sChar = '}')) and not (l_iQuot)) break begin
        if ((l_sChar = '"') and (l_sLast <> '\')) break begin
        if ((l_iQuot) and (l_sChar = '\') and ((l_sNext = '"') or (l_sNext = '\'))) break begin
        
        append l_sBuf l_sChar       
    loop
    
    function_return l_sBuf
end_function
