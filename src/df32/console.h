//-------------------------------------------------------------------------
// console.h
//      This file contains definitions "font maps" used to draw large chars
//
// This file is to be included when using bigText in df32func.mk
//
// Copyright (c) 2006-2009, glyn@8kb.co.uk
// 
// df32func/console.h
//-------------------------------------------------------------------------

Define __console_h__

//-------------------------------------------------------------------------
// Global objects
//-------------------------------------------------------------------------            

// Global array used to store "font map" for large text
object font_array is an array
end_object

set array_value of (font_array(current_object)) item (ascii("0")) to "1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("1")) to "0,0,0,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0"
set array_value of (font_array(current_object)) item (ascii("2")) to "1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("3")) to "1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("4")) to "1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1"
set array_value of (font_array(current_object)) item (ascii("5")) to "1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("6")) to "1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("7")) to "1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0"
set array_value of (font_array(current_object)) item (ascii("8")) to "1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("9")) to "1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("A")) to "0,1,1,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1"
set array_value of (font_array(current_object)) item (ascii("B")) to "1,1,1,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0"
set array_value of (font_array(current_object)) item (ascii("C")) to "0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("D")) to "1,1,1,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,0"
set array_value of (font_array(current_object)) item (ascii("E")) to "1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("F")) to "1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0"
set array_value of (font_array(current_object)) item (ascii("G")) to "1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("H")) to "1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1"
set array_value of (font_array(current_object)) item (ascii("I")) to "1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("J")) to "0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("K")) to "1,1,1,0,0,1,1,1,1,1,1,0,1,1,1,0,1,1,1,1,1,1,0,0,1,1,1,0,1,1,1,0,1,1,1,0,0,1,1,1"
set array_value of (font_array(current_object)) item (ascii("L")) to "1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("M")) to "1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,0,1,1"
set array_value of (font_array(current_object)) item (ascii("N")) to "1,1,1,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,1,1,0,1,1,1,1,0,0,1,1,1,1,1,1,0,0,0,1,1,1"
set array_value of (font_array(current_object)) item (ascii("O")) to "1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("P")) to "1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0"
set array_value of (font_array(current_object)) item (ascii("Q")) to "1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,0,0,1,0,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,0"
set array_value of (font_array(current_object)) item (ascii("R")) to "1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1,1,1,0,0,1,1,1"
set array_value of (font_array(current_object)) item (ascii("S")) to "0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0"
set array_value of (font_array(current_object)) item (ascii("T")) to "1,1,1,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0"
set array_value of (font_array(current_object)) item (ascii("U")) to "1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("V")) to "1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0"
set array_value of (font_array(current_object)) item (ascii("W")) to "1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,1,1,0,1,1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("X")) to "1,1,0,0,0,0,1,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,1"
set array_value of (font_array(current_object)) item (ascii("Y")) to "1,1,0,0,0,0,1,1,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0"
set array_value of (font_array(current_object)) item (ascii("Z")) to "1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1"
set array_value of (font_array(current_object)) item (ascii("%")) to "1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1"
set array_value of (font_array(current_object)) item (ascii("ø")) to "0,0,0,1,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
set array_value of (font_array(current_object)) item (ascii(".")) to "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0"
set array_value of (font_array(current_object)) item (ascii("-")) to "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
set array_value of (font_array(current_object)) item (ascii("+")) to "0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0"
