//-------------------------------------------------------------------------
// hash.inc
//      This file contains some DataFlex 3.2 Console Mode functions
//      to provide limited hashing algorithms.  More advanced hashing
//      can be found in win32.inc.
//
// This file is to be included in df32func.mk
//
// Copyright (c) 2006-2015, glyn@8kb.co.uk
// 
// df32func/hash.inc
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------

// Produces an integer hash value for indexing into a DataFlex array.
//
// This is a rather unaccomplished hash, but is specifically designed to produce
// positive hashed values with a relatively low integer range (< 10,000,000)
// in order to give a decent distribution and still fit into a DataFlex 3.2 
// array object.
// It can without doubt be improved.
//
// Note that whilst this hash may appear to preserve ascii order to some 
// degree this is an artifact rather than by design and cannot be relied on.
function hash_for_df_arrays global string argv returns integer
    local integer l_iHash
    local string l_sTmp
    
    move 0 to l_iHash
    move (lowercase(trim(argv))) to l_sTmp
    
    if (l_sTmp <> "") begin
        // Start at the first character of the string and produce distributed starting points for integer hash values
        if (mid(l_sTmp,1,1)) in "1234567890" move (((ascii(mid(l_sTmp,1,1)))-47)*200000) to l_iHash                 //index at 1-10
        if (mid(l_sTmp,1,1)) in "abcdefghijklmnopqrstuvwxyz" move (((ascii(mid(l_sTmp,1,1)))-86)*200000) to l_iHash //index at 11-36        
        if not (mid(l_sTmp,1,1)) in "1234567890abcdefghijklmnopqrstuvwxyz" move (37*200000) to l_iHash              //index at 37+

        // Take the sum of the second, last and middle chars (upper and lowercase) in the string and add to the hash
        if (length(argv) > 1) calc (l_iHash + (ascii(mid(argv,1,2)))) to l_iHash
        if (length(argv) > 2) calc (l_iHash + (ascii(mid(argv,1,(length(argv)))))) to l_iHash
        if (length(argv) > 4) calc (l_iHash + (ascii(mid(argv,1,(length(argv)/2))))) to l_iHash

        // If the string is longer than 9 chars add the second last val * 100 to the hash
        if (length(argv) > 9) calc (l_iHash + (100*(ascii(mid(argv,1,(length(argv)-2)))))) to l_iHash

        // Add the length of the string onto the hash if shorter than 255 chars, else add 256.
        if (length(argv) > 255) calc (l_iHash + 256) to l_iHash
        else calc (l_iHash + length(argv)) to l_iHash
    end
    else begin
        move 0 to l_iHash
    end
    
    function_return l_iHash
end_function

// Chop up an integer hash that is out of the optimal DataFlex 3.2 array
// size range and make it fit. This reduces the uniqueness of the hash but
// unfortuantely is a necessary evil when using out of range hashes.
function reduce_hash global integer argv returns integer
    local integer l_iHash
    
    // Make sure positive
    if (argv < 0) calc (argv*-1) to l_iHash
    else move argv to l_iHash
    
    // Unashamedly chop out the middle range
    if (length(string(l_iHash)) > 7) begin
        move (integer(left(string(l_iHash),3)+right(string(l_iHash), 3))) to l_iHash
    end
        
    function_return l_iHash
end_function

// Djb2 hashing from Dan Bernstein
//
// The main info is here http://fr.wikipedia.org/wiki/Table_de_hachage#Fonction_de_Hachage
//
// http://en.wikipedia.org/wiki/Daniel_J._Bernstein
// http://cr.yp.to/djb.html
// http://www.cse.yorku.ca/~oz/hash.html
function hash_djb2 global string argv returns integer
    local integer l_iHash l_i
    
    move 5381 to l_iHash
    
    for l_i from 1 to (length(argv))
        move ((LShift(l_iHash,5) + l_iHash) + (ascii(mid(argv,1,L_i)))) to l_iHash
    loop
    
    function_return l_iHash
end_function

// This is the SDBM hashing algorithm as used in berkeley DB
//
// http://cpansearch.perl.org/src/RGARCIA/perl-5.10.0/ext/SDBM_File/sdbm/README
// http://en.wikipedia.org/wiki/Dbm
// http://en.wikipedia.org/wiki/Sleepycat_Software
// http://en.wikipedia.org/wiki/Berkeley_DB
function hash_sdbm global string argv returns integer
    local integer l_iHash l_i
    
    move 0 to l_iHash
    
    for l_i from 1 to (length(argv))
        move ((ascii(mid(argv,1,L_i))) + (LShift(l_iHash,6)) + (LShift(l_iHash,16)) - l_iHash) to l_iHash
    loop
    
    function_return l_iHash
end_function

// Lazy hash, efficient for small fairly unique strings.
function hash_lazy global string argv returns integer
    local integer l_iHash l_i
    
    move 0 to l_iHash
    
    for l_i from 1 to (length(argv))
        move (l_iHash + (ascii(mid(argv,1,L_i)))) to l_iHash
    loop
    
    function_return l_iHash
end_function
