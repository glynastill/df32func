//-------------------------------------------------------------------------
// encode.inc
//      This file contains some DataFlex 3.2 Console Mode functions
//      to provide base64 and rc4 encoding/decoding.  More advanced 
//      encoding and encryption can be found in win32.inc.
//
// This file is to be included in df32func.mk
//
// Copyright (c) ????, ????@????.?? (unknown origin)
// 
// df32func/encode.inc
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------

// Check if a string is base64
function is_base64 global integer c returns integer
    if (c >= ascii('A') and c <= ascii('Z')) function_return true
    else if (c >= ascii('a') and c <= ascii('z')) function_return true
    else if (c >= ascii('0') and c <= ascii('9')) function_return true
    else if (c = ascii('+')) function_return true
    else if (c = ascii('/')) function_return true
    else if (c = ascii('=')) function_return true
    else function_return false
end_function

//encode integer to hex ascii code
function int_encode64 global integer uc returns integer
    if (uc < 26) function_return (ascii('A')+uc)
    else if (uc < 52) function_return (ascii('a')+(uc-26))
    else if (uc < 62) function_return (ascii('0')+(uc-52))
    else if (uc = 62) function_return (ascii('+'))
    else function_return (ascii('/'))
end_function

//decode hex to integer ascii code
function int_decode64 global integer c returns integer
    if (c >= ascii('A') and c <= ascii('Z')) function_return (c - ascii('A'))
    else if (c >= ascii('a') and c <= ascii('z')) function_return (c - ascii('a') + 26)
    else if (c >= ascii('0') and c <= ascii('9')) function_return (c - ascii('0') + 52)
    else if (c = ascii('+')) function_return 62
    else function_return 63
end_function

// encode to base64
function encode64 global string sText returns string
    local integer iLen iPos iChar
    local integer by1 by2 by3 
    local integer by4 by5 by6 by7
    local string  sRet
    
    move (length(sText)) to iLen
    move 1 to iPos
    
    if iPos Lt 1 function_return 
    
    while (iPos <= iLen)            
       move 0 to by1
       move 0 to by2
       move 0 to by3       
       move 0 to by4
       move 0 to by5
       move 0 to by6
       move 0 to by7
       
       if (iPos+0 <= iLen) move (ascii(mid(sText,1,iPos+0))) to by1 
       if (iPos+1 <= iLen) move (ascii(mid(sText,1,iPos+1))) to by2
       if (iPos+2 <= iLen) move (ascii(mid(sText,1,iPos+2))) to by3       
       move (rshift(by1,2)) to by4
       move (lshift((by1 iand 3),4)  ior rshift(by2,4)) to by5  
       move (lshift((by2 iand 15),2) ior rshift(by3,6)) to by6 
       move (by3 iand 63) to by7
       
       append sRet (character(int_encode64(by4)))
       append sRet (character(int_encode64(by5)))
       
       if (iPos+1<=iLen) append sRet (character(int_encode64(by6)))
       else append sRet "=" 
       
       if (iPos+2<=iLen) append sRet (character(int_encode64(by7)))  
       else append sRet "="
       
       move (iPos+3) to iPos
    end
    function_return sRet
end_function

// decode from base64
function decode64 global string sText returns string
    local integer iLen iPos iChar isOK
    local integer c1  c2  c3  c4 
    local integer cc1 cc2 cc3
    local integer by1 by2 by3 by4
    local string  sRet
    
    move (length(sText)) to iLen
    
    move 1 to isOK
    for iPos from 1 to iLen
       move (is_base64(ascii(mid(sText,1,iPos)))) to isOK 
       if isOK eq 0 Break
    loop

    if (isOK) begin
      move 1 to iPos
      while (iPos<=iLen)
        move (ascii('A')) to c1
        move (ascii('A')) to c2
        move (ascii('A')) to c3
        move (ascii('A')) to c4
       
        if (iPos+0<=iLen) move (ascii(mid(sText,1,iPos+0))) to c1 
        if (iPos+1<=iLen) move (ascii(mid(sText,1,iPos+1))) to c2
        if (iPos+2<=iLen) move (ascii(mid(sText,1,iPos+2))) to c3
        if (iPos+3<=iLen) move (ascii(mid(sText,1,iPos+3))) to c4
       
        move (int_decode64(c1)) to by1
        move (int_decode64(c2)) to by2
        move (int_decode64(c3)) to by3
        move (int_decode64(c4)) to by4
       
        append sRet (character(lshift(by1,2) ior rshift(by2,4)))
        if (c3<>ascii('=')) append sRet (character(lshift((by2 iand 15),4) ior rshift(by3,2)))
        if (c4<>ascii('=')) append sRet (character(lshift((by3 iand 3) ,6) ior by4))
                                                                          
        move (iPos+4) to iPos
      end
    end
    function_return sRet
end_function

procedure set rc4_array_value integer iIndex integer iVal
    move (iIndex+1)  to iIndex
    if iIndex Le 128 ;
         move (Overstrike(character(iVal),rc4SBXA,iIndex))     to rc4SBXA
    else move (Overstrike(character(iVal),rc4SBXB,iIndex-128)) to rc4SBXB
end_procedure

function rc4_array_value integer iIndex returns integer
    local integer iRet
    move (iIndex+1)  to iIndex
    if iIndex Le 128 ;
         move (ascii(mid(rc4SBXA,1,iIndex)))     to iRet
    else move (ascii(mid(rc4SBXB,1,iIndex-128))) to iRet
    function_return iRet
end_function

procedure create_rc4_key string key
    local integer a b keylen idx
    local integer ikey atmp btmp

    move (length(key)) to keylen
    
    // initialise key array
    move rc4SBXA to rc4SBXA_TMP
    move rc4SBXA to rc4SBXB_TMP
    
    move "" to rc4SBXA
    move "" to rc4SBXB
    
    for idx from 0 to 255
       set rc4_array_value idx to idx
    loop                   
    
    // encode key array
    for a from 0 to 255
       move (ascii(mid(key,1,mod(a,keylen)+1))) to ikey
       get rc4_array_value a to atmp
       move (mod((b+atmp+ikey),256)) to b

       get rc4_array_value b to btmp

       set rc4_array_value a to btmp
       set rc4_array_value b to atmp
    loop
    
end_procedure

function rc4 global string key string text returns string
    local integer ix iy temp offset origlen cipherlen
    local integer ixtmp iytmp ixytmp ixc
    local string sRet

    send create_rc4_key key  
    move (length(text)) to origlen
    
    move "" to sRet
    for offset from 1 to origlen
        move (mod((offset-1),256)) to ix
        get rc4_array_value ix to ixtmp
        move (mod((iy+ixtmp),256)) to iy
        //
        get rc4_array_value ix to ixtmp
        get rc4_array_value iy to iytmp
        //
        set rc4_array_value ix to iytmp
        set rc4_array_value iy to ixtmp
        //
        move (ascii(mid(text,1,offset))) to ixc
        //
        get rc4_array_value ix to ixtmp
        get rc4_array_value iy to iytmp
        get rc4_array_value (mod((ixtmp+iytmp),256)) to ixytmp
        move ((ixc ior ixytmp) - (ixc iand ixytmp)) to ixc //XOR
        //
        move (sRet+(character(ixc))) to sRet
    loop
    function_return sRet
end_function

function rc4encode global string key string text returns string
    function_return (rc4(key,text))
end_function

function rc4decode global string key string text returns string
    function_return (rc4(key,text))
end_function

function rc4encode_base64 global string key string text returns string
    function_return (encode64(rc4encode(key,text)))
end_function

function rc4decode_base64 global string key string text returns string
    function_return (rc4decode(key,decode64(text)))
end_function
