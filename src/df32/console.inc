//-------------------------------------------------------------------------
// console.inc
//      This file contains some DataFlex 3.2 Console Mode functions
//      to manipulate console window.
//
// This file is to be included in df32func.mk
//
// Copyright (c) 2006-2015, glyn@8kb.co.uk
// 
// df32func/console.inc
//-------------------------------------------------------------------------

#IFDEF __console_h__
#ELSE
    #INCLUDE console.h
#ENDIF

//-------------------------------------------------------------------------
// Global objects
//-------------------------------------------------------------------------            

// Global StringTokenizer used to store "font blocks" for large text
object font_split is a StringTokenizer
end_object

//-------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------

// Set the display mode to argv lines
function set_mode global integer argv returns integer
    local string l_01tmpStr

    if (sysconf(sysconf_os_name)) eq "Win32 Console Mode" move "c:\windows\system32\mode con: cols=80 lines=" to l_01tmpStr
    if (sysconf(sysconf_os_name)) ne "Win32 Console Mode" move "c:\windows\command\mode con: cols=80 lines=" to l_01tmpStr

    append l_01tmpStr argv
    runprogram wait l_01tmpStr

    function_return 0
end_function

// Display a text file to screen
function screen_display global string argv returns integer
    local string l_sBuf l_k l_sTmp
    local integer l_i l_iLineAt l_iTotalLines l_iTotalWidth l_iWidthAt

    // Add color markup here.    
    move 1 to l_iWidthAt
    move 0 to l_iLineAt
    move 0 to l_iTotalLines
    move 0 to l_iTotalWidth

    direct_input channel DEFAULT_FILE_CHANNEL argv   
    while not (seqeof)
        readln channel DEFAULT_FILE_CHANNEL l_sBuf
        move (rtrim(l_sBuf)) to l_sbuf
        move (replaces("{YELLOW}",l_sBuf,"")) to l_sBuf
        move (replaces("{WHITE}",l_sBuf,"")) to l_sBuf
        move (replaces("{RED}",l_sBuf,"")) to l_sBuf
        move (replaces("{GREEN}",l_sBuf,"")) to l_sBuf
        move (replaces("{BLUE}",l_sBuf,"")) to l_sBuf
        move (replaces("{PURPLE}",l_sBuf,"")) to l_sBuf
        move (replaces("{GREY}",l_sBuf,"")) to l_sBuf
        move (replaces("{BLACK}",l_sBuf,"")) to l_sBuf
        if (length(l_sBuf) > l_iTotalWidth) move ((length(l_sBuf))+1) to l_iTotalWidth
        increment l_iTotalLines
    loop
    close_input channel DEFAULT_FILE_CHANNEL 

    while not (key.escape)
        direct_input channel DEFAULT_FILE_CHANNEL argv
            for l_i from 1 to l_iLineAt
                readln channel DEFAULT_FILE_CHANNEL 
            loop
            for l_i from 1 to 22
                readln channel DEFAULT_FILE_CHANNEL l_sBuf
                gotoxy (l_i-1) 0

                    if (uppercase(l_sBuf) contains "{YELLOW}") begin
                        move (replaces("{YELLOW}",l_sBuf,"")) to l_sBuf
                        screenmode 1
                    end
                    if (uppercase(l_sBuf) contains "{WHITE}") begin
                        move (replaces("{WHITE}",l_sBuf,"")) to l_sBuf
                        screenmode 2
                    end
                    if (uppercase(l_sBuf) contains "{RED}") begin
                        move (replaces("{RED}",l_sBuf,"")) to l_sBuf
                        screenmode 28
                    end
                    if (uppercase(l_sBuf) contains "{GREEN}") begin
                        move (replaces("{GREEN}",l_sBuf,"")) to l_sBuf
                        screenmode 26
                    end
                    if (uppercase(l_sBuf) contains "{BLUE}") begin
                        move (replaces("{BLUE}",l_sBuf,"")) to l_sBuf
                        screenmode 19
                    end
                    if (uppercase(l_sBuf) contains "{PURPLE}") begin
                        move (replaces("{PURPLE}",l_sBuf,"")) to l_sBuf
                        screenmode 29
                    end
                    if (uppercase(l_sBuf) contains "{GREY}") begin
                        move (replaces("{GREY}",l_sBuf,"")) to l_sBuf
                        screenmode 24
                    end
                    if (uppercase(l_sBuf) contains "{BLACK}") begin
                        move (replaces("{BLACK}",l_sBuf,"")) to l_sBuf
                        screenmode 16
                    end

                show (pad(mid(l_sBuf,80,l_iWidthAt),80))
                screenmode 1
            loop
        close_input channel DEFAULT_FILE_CHANNEL        
    
        move "lines " to l_sTmp
        append l_sTmp (l_iLineAt+1) "-" (l_iLineAt+22) " of " (l_iTotalLines)
        if (l_iTotalLines < (l_iLineAt+22)) begin
            move "lines " to l_sTmp
            append l_sTmp (l_iLineAt+1) "-" (l_iTotalLines) " of " (l_iTotalLines)
        end
        gotoxy 23 0
        screenmode 112
        show (center(l_sTmp,80)) 
        screenmode 1
        
        gotoxy 26 0
        inkey l_k
        if (key.up) begin
            if (l_iLineAt <> 0) decrement l_iLineAt
        end
        if (key.down) begin
            if ((l_iLineAt+22) < l_iTotalLines) increment l_iLineAt
        end
        if ((ascii(l_k)) = 34) begin
            calc (l_iLineAt-22) to l_iLineAt
            if (l_iLineAt < 0) move 0 to l_iLineAt
        end
        if ((ascii(l_k)) = 35) begin
            calc (l_iLineAt+22) to l_iLineAt
            if (l_iLineAt > (l_iTotalLines-22)) begin
                move (l_iTotalLines-22) to l_iLineAt
            end
        end
        if ((ascii(l_k)) = 44) begin
            move 0 to l_iLineAt
        end
        if ((ascii(l_k)) = 45) begin
            calc (l_iTotalLines-22) to l_iLineAt
        end
        if ((ascii(l_k)) = 4) begin
            if (l_iWidthAt <> 1) decrement l_iWidthAt
        end
        if ((ascii(l_k)) = 5) begin
            if ((l_iWidthAt+80) <= l_iTotalWidth) increment l_iWidthAt
        end
    loop
end_function

// Draw a large character in a dos window
function draw_bigchar global string argv integer posx integer posy returns integer
    local integer l_onCount l_i l_iChars
    local string l_c
    move 0 to l_onCount
    move (uppercase(argv)) to argv
    
    send delete_data to (font_split(current_object))
    send set_string to (font_split(current_object)) argv ","
    get token_count of (font_split(current_object)) to l_iChars
    
    for l_i from 0 to (l_iChars+1)
        get token_value of (font_split(current_object)) item l_i to l_c
        if ((integer(l_c)) = 1) increment l_onCount
        if (l_i < 8) begin
            gotoxy posx (posy+l_i)
            if ((integer(l_c)) = 0) show (character(32))
            if ((integer(l_c)) = 1) show (character(219))
        end
        if ((l_i >= 8) and (l_i < 16)) begin
            gotoxy (posx+1) (posy+l_i-8)
            if ((integer(l_c)) = 0) show (character(32))
            if ((integer(l_c)) = 1) show (character(219))
        end
        if ((l_i >= 16) and (l_i < 24)) begin
            gotoxy (posx+2) (posy+l_i-16)
            if ((integer(l_c)) = 0) show (character(32))
            if ((integer(l_c)) = 1) show (character(219))
        end
        if ((l_i >= 24) and (l_i < 32)) begin
            gotoxy (posx+3) (posy+l_i-24)
            if ((integer(l_c)) = 0) show (character(32))
            if ((integer(l_c)) = 1) show (character(219))
        end
        if ((l_i >= 32) and (l_i <= 40)) begin
            gotoxy (posx+4) (posy+l_i-32)
            if ((integer(l_c)) = 0) show (character(32))
            if ((integer(l_c)) = 1) show (character(219))
        end
    loop
    
    function_return l_onCount
end_function

// Draw large text in a dos window
function big_text global string argv integer posx integer posy returns integer
    local integer l_iResult l_i
    local string l_tmpStr

    for l_i from 1 to  (length(argv))
        move (mid(argv,1,l_i)) to l_tmpStr
        get array_value of (font_array(current_object)) item (ascii(l_tmpStr)) to l_tmpStr
        move (draw_bigchar(l_tmpStr,posx,(posy+((l_i-1)*9)))) to l_iResult
    loop
    function_return l_i
end_function
