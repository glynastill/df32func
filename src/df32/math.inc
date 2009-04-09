//-------------------------------------------------------------------------
// math.inc
//      This file contains some DataFlex 3.2 Console Mode functions
//      to provide extended mathematical capabilities.
//
// This file is to be included in df32func.mk
//
// Copyright (c) 2006-2009, glyn@8kb.co.uk
// 
// df32func/math.inc
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------

// Round a number to n decimal places. Returns a string.
// argv = number argv2 = decimal places
function decround global number argv integer argv2 returns string
    local number l_nInVal l_nDcnVal l_nDcnDecVal l_nDcnAppPoints
    local integer l_iDcnPoints
    local string l_sReturn

    move argv to l_nInVal
    move argv2 to l_iDcnPoints
  
    move (number("0."+repeat(0,l_iDcnPoints)+"5")) to l_nDcnDecVal
    if (l_nInVal < 0) calc (l_nDcnDecVal*-1) to l_nDcnDecVal
    calc (l_nInVal+l_nDcnDecVal) to l_nDcnVal   

    move (l_iDcnPoints-(length(l_nDcnVal)-pos(".", l_nDcnVal))) to l_nDcnAppPoints
    if (l_iDcnPoints >= 1) move (left(l_nDcnVal,((pos(".",l_nDcnVal))+l_iDcnPoints))) to l_sReturn
    else move (left(l_nDcnVal,((pos(".",l_nDcnVal))+l_iDcnPoints)-1)) to l_sReturn
    if (not (l_sReturn contains ".") and l_iDcnPoints <> 0) append l_sReturn "." (repeat("0",l_iDcnPoints))
    else if (l_nDcnAppPoints > 0) append l_sReturn (repeat("0",l_nDcnAppPoints))
    if (number(l_sReturn) = 0) move (replaces("-",l_sReturn,"")) to l_sReturn

    function_return l_sReturn

end_function

// Performs a right binary shift on a variable.
// Relies on the embedded binary arithmetic of ior and iand
function rshift global integer argv integer shift_by returns integer
    function_return (integer(argv/(2^shift_by)) iand 255)
end_function

// Performs a left binary shift on a variable.
// Relies on the embedded binary arithmetic of ior and iand
function lshift global integer argv integer shift_by returns integer
    function_return (integer(argv*(2^shift_by)) iand 255) 
end_function

// Ternary operator function like perls boolean ? "true var" : "false var"
// Relies on TERNARY command (macro.inc) - beware all evaluations passed to 
// the function are actually run, so only usefull for strings really in argv2/3
function ternary global string argv string argv2 string argv3 returns string
    local string l_sReturn
    local integer tmp

    if ((argv = "0") or (argv = "1")) begin
            ternary (integer(argv)) argv2 argv3 l_sReturn

    end
    else if ("@t@f@false@true@" contains ("@"+lowercase(argv)+"@")) begin
            ternary argv argv2 argv3 l_sReturn
    end
    else begin
            custom_error ERROR_CODE_INVALID_BOOLEAN$ ERROR_MSG_INVALID_BOOLEAN ERROR_DETAIL_INVALID_BOOLEAN argv
    end
    function_return l_sReturn
end_function
