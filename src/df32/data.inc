//-------------------------------------------------------------------------
// data.inc
//      This file contains some DataFlex 3.2 Console Mode classes
//      to provide some useful data structures.
//
// This file is to be included in df32func.mk
//
// Copyright (c) 2006-2015, glyn@8kb.co.uk
// 
// df32func/data.inc
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Classes
//-------------------------------------------------------------------------

// Linked list class - impliments a linked list type structure in an array, unlike
// traditional linked lists the data is actually the prev/next links (array indexes)
//
// Prev and next addresses are stored as a string "prev,next" rather than using XORing;
// this is so we can start traversal in either direction from a particular address
// without having to also know the prev or next address.
//
// Get methods:
//    probe_state                   - Returns a summary of the linked list state
//    first_link                    - Returns the first link in the list
//    last_link                     - Returns the last link in the list
//    link_count                    - Returns the total number of links
//    next_link <current_link>      - Returns the next link after the link id passed in
//    prev_link <current_link>      - Returns the previous link after the link id passed in
//
// Set methods: (All of the following methods are intended to be private)
//    next_link <current_link>      - Sets the next link after the link id passed in
//    prev_link <current_link>      - Sets the previous link after the link id passed in
//    seek_link                     - Seeks out the position in the list for a new link    
//
// Send message methods:
//    insert_link                   - Insert a new item into the linked list
//    remove_link                   - Remove an item from the linked list
//
//
// Example usage:
//    
//    string buf
//    integer max min count i
//    
//    object test is a linkedlist
//    end_object
//    
//    // Create some links
//    for i from 10 to 15
//        send insert_link to test (i*100)
//    loop
//    for i from 1 to 5
//        send insert_link to test (i*100)
//    loop
//    
//    send insert_link to test 750
//
//    // Remove a link
//    send remove_link to test 300
//    
//    // Access the list
//    get probe_state of test to buf
//    get first_link of test to min
//    get last_link of test to max
//    get link_count of test to count
//    
//    showln "There are " count " items in the linked list"
//    showln buf
//    
//    show "Traverse forwards: "
//    move min to i
//    while (i <> -1) 
//        show i "->"   
//        get next_link of test item i to i
//    loop
//    showln "END"
//    
//    show "Traverse backwards: "
//    move max to i
//    while (i <> -1) 
//        show i "->"   
//        get prev_link of test item i to i
//    loop
//    showln "END"

class linkedlist is an array
    procedure construct_object integer argc
        object mTokens is a StringTokenizer
        end_object
    
        forward send construct_object
        
        property integer c_iMinAddr
        property integer c_iMaxAddr
        property integer c_iCount
        property number c_nDist
        
        set c_iMinAddr to -1
        set c_iMaxAddr to -1
        set c_iCount to 0
        set c_nDist to 1
    end_procedure
    
    procedure delete_data
        set c_iMinAddr to -1
        set c_iMaxAddr to -1
        set c_iCount to 0
        set c_nDist to 1
        forward send delete_data
    end_procedure    
    
    function probe_state returns string
        local integer l_iMinAddr l_iMaxAddr l_iCount 
        local number l_nDist
        
        get c_iMaxAddr to l_iMaxAddr
        get c_iMinAddr to l_iMinAddr
        get c_iCount to l_iCount
        get c_nDist to l_nDist
        
        function_return ("Address range: "+string(l_iMinAddr)+"<->"+string(l_iMaxAddr)+" Items: "+string(l_iCount)+" Dist: "+string(l_nDist))
    end_function
    
    function last_link returns integer
        local integer l_iMaxAddr
        
        get c_iMaxAddr to l_iMaxAddr
        
        function_return l_iMaxAddr
    end_function
    
    function first_link returns integer
        local integer l_iMinAddr
        
        get c_iMinAddr to l_iMinAddr
        
        function_return l_iMinAddr
    end_function    
    
    function link_count returns integer
        local integer l_iCount
        
        get c_iCount to l_iCount
        
        function_return l_iCount
    end_function      
    
    function next_link integer l_iAddr returns integer
        local string l_sBuf
        local integer l_iNext
        
        forward get string_value item l_iAddr to l_sBuf
        if (l_sBuf = "") custom_error ERROR_CODE_INVALID_ADDRESS$ ERROR_MSG_INVALID_ADDRESS l_iAddr
        move (integer(right(l_sBuf,(length(l_sBuf)-pos(",",l_sBuf))))) to l_iNext
        
        function_return l_iNext
    end_function
    
    function prev_link integer l_iAddr returns integer
        local string l_sBuf
        local integer l_iPrev
        
        forward get string_value item l_iAddr to l_sBuf
        if (l_sBuf = "") custom_error ERROR_CODE_INVALID_ADDRESS$ ERROR_MSG_INVALID_ADDRESS l_iAddr
        move (integer(left(l_sBuf,pos(",",l_sBuf)-1))) to l_iPrev        
        
        function_return l_iPrev
    end_function 
    
    procedure set next_link integer l_iAddr integer l_iNext
        local string l_sBuf
        local integer l_iPrev
        
        forward get string_value item l_iAddr to l_sBuf
        move (integer(left(l_sBuf,pos(",",l_sBuf)-1))) to l_iPrev
        forward set array_value item l_iAddr to (string(l_iPrev)+","+string(l_iNext))
        
    end_procedure

    procedure set prev_link integer l_iAddr integer l_iPrev
        local string l_sBuf
        local integer l_iNext
        
        forward get string_value item l_iAddr to l_sBuf
        move (integer(right(l_sBuf,(length(l_sBuf)-pos(",",l_sBuf))))) to l_iNext
        forward set array_value item l_iAddr to (string(l_iPrev)+","+string(l_iNext))
        
    end_procedure    
    
    function seek_link integer l_iAddr returns string
        local integer l_iOn l_iNext l_iPrev l_iMinAddr l_iMaxAddr
        local string l_sBuf
        local number l_nDist
        
        get c_iMaxAddr to l_iMaxAddr
        get c_iMinAddr to l_iMinAddr
        get c_nDist to l_nDist


        if (show_debug_lines) begin        
            showln "DEBUG: Addr " l_iAddr
            showln "DEBUG: Range " l_iMinAddr " <-> " l_iMaxAddr
            showln "DEBUG: Dist " l_nDist
        end
        
        move l_iMinAddr to l_iPrev
        move l_iMaxAddr to l_iNext 
        
        if (l_iAddr > l_iMaxAddr) move l_iMaxAddr to l_iOn
        else move l_iMinAddr to l_iOn
        
        if (l_iOn > -1) begin
            while (l_iOn < l_iAddr)
                forward get string_value item l_iOn to l_sBuf
            if (l_sBuf = "") break
                else begin
                    move (integer(left(l_sBuf,pos(",",l_sBuf)-1))) to l_iPrev
                    move (integer(right(l_sBuf,(length(l_sBuf)-pos(",",l_sBuf))))) to l_iNext
                end    
                if ((l_iNext = -1) or (l_iNext > l_iAddr)) break             
                move l_iNext to l_iOn
            loop
        end
        
        if (l_iPrev > l_iAddr) begin 
                move l_iPrev to l_iNext
                move -1 to l_iOn
            move -1 to l_iPrev
        end            
        
        function_return (string(l_iPrev)+","+string(l_iOn)+","+string(l_iNext))
    end_function
    
    procedure insert_link integer l_iAddr
        local integer l_iMinAddr l_iMaxAddr l_iPrev l_iNext l_iOn l_iCount
        local string l_sBuf
        local number l_nDist
        
        if (l_iAddr < 0) custom_error ERROR_CODE_INVALID_ADDRESS$ ERROR_MSG_INVALID_ADDRESS l_iAddr
        else begin
            forward get string_value item l_iAddr to l_sBuf
            if (l_sBuf <> "") custom_error ERROR_CODE_ADDRESS_TAKEN$ ERROR_MSG_ADDRESS_TAKEN l_iAddr
            else begin
                get c_iMaxAddr to l_iMaxAddr
            get c_iMinAddr to l_iMinAddr
            get seek_link item l_iAddr to l_sBuf

                send delete_data to (mTokens(current_object))
                send set_string to (mTokens(current_object)) l_sBuf ","
                get integer_value of (mTokens(current_object)) item 0 to l_iPrev
                get integer_value of (mTokens(current_object)) item 1 to l_iOn
                get integer_value of (mTokens(current_object)) item 2 to l_iNext
                
                if (show_debug_lines) begin
                    showln "DEBUG: Insert address: " l_iAddr " Seek data '" l_sBuf "'"
                end
                
                if (l_iOn <> -1) set next_link item l_iOn to l_iAddr  
                forward set array_value item l_iAddr to (string(l_iOn)+","+string(l_iNext))
                if (l_iNext <> -1) set prev_link item l_iNext to l_iAddr
                
                if (l_iAddr > l_iMaxAddr) begin 
                    move l_iAddr to l_iMaxAddr
                    set c_iMaxAddr to l_iMaxAddr
                end
            if ((l_iAddr < l_iMinAddr) or (l_iMinAddr = -1)) begin
                move l_iAddr to l_iMinAddr
                set c_iMinAddr to l_iMinAddr
            end
            get c_iCount to l_iCount
            get c_nDist to l_nDist
            increment l_iCount
            set c_iCount to l_iCount
            set c_nDist to (number(l_iCount)/(l_iMaxAddr-l_iMinAddr))
            end
        end
    end_procedure
    
    procedure remove_link integer l_iAddr
        local integer l_iMinAddr l_iMaxAddr l_iPrev l_iNext l_iOn l_iCount
        local string l_sBuf
        local number l_nDist
        
        if (l_iAddr < 0) custom_error ERROR_CODE_INVALID_ADDRESS$ ERROR_MSG_INVALID_ADDRESS l_iAddr
        else begin
            get c_iMaxAddr to l_iMaxAddr
            get c_iMinAddr to l_iMinAddr
            forward get string_value item l_iAddr to l_sBuf
            
            move (integer(left(l_sBuf,pos(",",l_sBuf)-1))) to l_iPrev
            move (integer(right(l_sBuf,(length(l_sBuf)-pos(",",l_sBuf))))) to l_iNext
            
            if (show_debug_lines) begin
                showln "DEBUG: Remove address: " l_iAddr " Link data '" l_sBuf "'"
            end
            
            if (l_iPrev <> -1) set next_link item l_iPrev to l_iNext
            if (l_iNext <> -1) set prev_link item l_iNext to l_iPrev
            forward set array_value item l_iAddr to ""
            
            if (l_iMaxAddr = l_iAddr) set c_iMaxAddr to l_iPrev            
            if (l_iMinAddr = l_iAddr) set c_iMinAddr to l_iNext            
            
            get c_iCount to l_iCount
            get c_nDist to l_nDist
            decrement l_iCount
            set c_iCount to l_iCount
            set c_nDist to (number(l_iCount)/(l_iMaxAddr-l_iMinAddr))                        
        end        
    end_procedure

end_class

// Hashtable class - unlike other similar "hash" data structures which usually
// store key/value pairs; the hashtable class was only initially intended to 
// store a key and return a hash index.  Typical use would usually have been 
// to then store the value in an array at the hash index.  (See the "hash" class
// below for a key/value orientated class)
// It is however possible to use insert_at_hash and find_at_hash to store a different
// value at a particular key hash, this is however an afterthought and colisions are 
// possible.
//
// Send message methods:
//    delete_data       - Clear the hashtable
//    hash_algorithm    - Set the hash algorithm, any of ("hash_for_df_arrays", 
//                        "hash_reduced_djb2", "hash_reduced_lazy", "hash_reduced_sdbm")
//                        with "hash_for_df_arrays" being the default.
//    remove_hash       - Removes an item from the hashtable.
//
// Set methods:
//    item_ptr          - Set the index of the current item_ptr (next_hash will return the next item from this)
//
// Get methods:
//    item_count
//    insert_hash       - Insert a value (where the key is equal to the value) and return hash index
//    insert_at_hash    - Insert a value at the hash index of a key and return hash index
//    find_hash         - Check if a value is stored in the hash (where the key is equal to the value) and return it's hash index
//    find_at_hash      - Check if a value is stored in the hash at the hash index of a key and return it's hash index
//    value_at_index    - Return the value stored at a particular hash index
//    string_at_index   - Same as above, but specifically retun in string context
//    next_hash         - Get the next value stored in the hash
//    item_ptr          - Get the index of the last value returned by next_hash
//
// Example usage:
//
//    object myHashtable is a hashTable
//    end_object
//
//    integer i ix
//    string k
//
//    clearscreen
//    get insert_hash of (myHashtable(current_object)) item "ABC" to i
//    get insert_hash of (myHashtable(current_object)) item "HELLO" to ix
//    get insert_hash of (myHashtable(current_object)) item "ZZZ" to i
//    send remove_hash to (myHashtable(current_object)) ix
//    get item_count of (myHashtable(current_object)) to i  
//
//    get find_hash of (myHashtable(current_object)) item "HELLO" to i
//    showln i
//
//    set item_ptr of (myHashTable(current_object)) to 0
//    move 0 to i
//    while (i  <> -1)
//        get next_hash of (myHashtable(current_object)) to k
//        get item_ptr of (myHashtable(current_object)) to i  
//
//        showln "*** " i " " k
//        inkey k
//    loop
//
class hashtable is an array
    procedure construct_object integer argc
        forward send construct_object
        property integer c_iMaxHash public argc
        property integer c_iMinHash
        property integer c_iItems
        property integer c_iItemP
        property string c_sHashAlgorithm
        set c_sHashAlgorithm to "hash_for_df_arrays"
        set c_iMinHash to 99999999
    end_procedure
        
    procedure delete_data
        set c_iMinHash to 0
        set c_iMaxHash to 0
        set c_iItems to 0
        set c_iItemP to 0
        forward send delete_data
    end_procedure
        
    procedure hash_algorithm string l_sType
        local integer l_iItems
        
        get c_iItems to l_iItems
        
        // Allow algorithm change only when empty.
        if (l_iItems = 0) begin
            if (trim(lowercase(l_sType)) = "hash_for_df_arrays") set c_sHashAlgorithm to "hash_for_df_arrays"
            if (trim(lowercase(l_sType)) = "hash_reduced_djb2") set c_sHashAlgorithm to "hash_reduced_djb2"
            if (trim(lowercase(l_sType)) = "hash_reduced_sdbm") set c_sHashAlgorithm to "hash_reduced_sdbm"
            if (trim(lowercase(l_sType)) = "hash_reduced_lazy") set c_sHashAlgorithm to "hash_reduced_lazy"
        end
    end_procedure
        
    procedure set item_ptr integer l_iItemP
        set c_iItemP to l_iItemp
    end_procedure
        
    function item_ptr returns integer
        local integer l_iItemP
        get c_iItemP to l_iItemP
        function_return l_iItemP
    end_function
        
    procedure remove_hash integer l_iHash
        local string l_sNext
        local integer l_iItems
        
        get c_iItems to l_iItems
        if (l_iItems > 0) begin
            forward get array_value item (l_iHash+1) to l_sNext
            if (trim(l_sNext) <> "") forward set array_value item l_iHash to "<!!#ITEM REMOVED#!!>"
            if (trim(l_sNext) = "") forward set array_value item l_iHash to ""
            set c_iItems to (l_iItems-1)
        end
    end_procedure
        
    function item_count returns integer
            local integer l_iItems
            get c_iItems to l_iItems
            function_return l_iItems
    end_procedure
        
    function write_hash string l_sHash string l_sValue returns integer
        local integer l_iHash l_iMinHash l_iMaxHash l_iItems l_iReuse
        local string l_sTmp l_sHashAlgorithm l_sStorage

        if (l_sValue = "") begin
            move l_sHash to l_sStorage
        end
        else begin
            move l_sValue to l_sStorage
        end

        // Get our object properties
        get c_iMaxHash to l_iMaxHash
        get c_iMinHash to l_iMinHash
        get c_iItems to l_iItems
        get c_sHashAlgorithm to l_sHashAlgorithm
        
        // Generate an initial hash
        move 0 to l_iHash
        
        case begin
            case (l_sHashAlgorithm = "hash_reduced_djb2") move (reduce_hash(hash_djb2(l_sHash))) to l_iHash
            case break
            case (l_sHashAlgorithm = "hash_reduced_sdbm") move (reduce_hash(hash_sdbm(l_sHash))) to l_iHash
            case break
            case (l_sHashAlgorithm = "hash_reduced_lazy") move (reduce_hash(hash_lazy(l_sHash))) to l_iHash
            case break
            case else move (hash_for_df_arrays(l_sHash)) to l_iHash
            case break
        case end
        
        // Then we check in the array to see if out hash is available or equal
        // if not we bucket the value by moveing along into the next available slot
        move 0 to l_iReuse
        next_bucket01:
        forward get string_value item l_iHash to l_sTmp
        if ((l_sTmp <> "") and (l_sTmp <> l_sStorage)) begin
            // If we come across a removed item we may want to reuse the hash space
            if ((l_sTmp = "<!!#ITEM REMOVED#!!>") and (l_iReuse = 0)) move l_iHash to l_iReuse 
            calc (l_iHash+1) to l_iHash
            goto next_bucket01
        end
            
        // If this is a new object and we have a slot to reuse then do so here
        if ((l_iReuse <> 0) and (l_sTmp = "")) move l_iReuse to l_iHash 
        if (l_iHash > l_iMaxHash) move l_iHash to l_iMaxHash
        if (l_iHash < l_iMinHash) move l_iHash to l_iMinHash
        forward set array_value item l_iHash to l_sStorage
        
        set c_iMaxHash to l_iMaxHash
        set c_iMinHash to l_iMinHash
        set c_iItems to (l_iItems+1)
        
        function_return l_iHash
    end_function

    function insert_hash string l_sHash returns integer
        local integer l_iHash
        get write_hash item l_sHash item "" to l_iHash
        function_return l_iHash
    end_procedure
        
    function insert_at_hash string l_sHash string l_sValue returns integer
        local integer l_iHash
        get write_hash item l_sHash item l_sValue to l_iHash
        function_return l_iHash
    end_procedure
        
    function read_hash string l_sHash string l_sValue returns integer
        local integer l_iHash l_iMinHash l_iMaxHash l_iItems
        local string l_sTmp l_sHashAlgorithm l_sStorage

        if (l_sValue = "") begin
            move l_sHash to l_sStorage
        end
        else begin
            move l_sValue to l_sStorage
        end

        // Get our object properties
        get c_iMaxHash to l_iMaxHash
        get c_iMinHash to l_iMinHash
        get c_iItems to l_iItems
        get c_sHashAlgorithm to l_sHashAlgorithm
        
        // Generate an initial hash
        move 0 to l_iHash
        
        case begin
            case (l_sHashAlgorithm = "hash_reduced_djb2") move (reduce_hash(hash_djb2(l_sHash))) to l_iHash
            case break
            case (l_sHashAlgorithm = "hash_reduced_sdbm") move (reduce_hash(hash_sdbm(l_sHash))) to l_iHash
            case break
            case (l_sHashAlgorithm = "hash_reduced_lazy") move (reduce_hash(hash_lazy(l_sHash))) to l_iHash
            case break
            case else move (hash_for_df_arrays(l_sHash)) to l_iHash
            case break
        case end

        // Then we check in the array to see if our hash is available or equal
        // if not we bucket the value by moveing along into the next available slot
        next_bucket02:
        forward get string_value item l_iHash to l_sTmp
        
        if ((l_sTmp <> "") and (l_sTmp <> l_sStorage)) begin
                calc (l_iHash+1) to l_iHash
                goto next_bucket02
        end
        if (l_sTmp <> l_sStorage) move -1 to l_iHash
        
        function_return l_iHash
    end_function
    
    function find_hash string l_sHash returns integer
        local integer l_iHash
        get read_hash item l_sHash item "" to l_iHash
        function_return l_iHash
    end_function
    
    function find_at_hash string l_sHash string l_sValue returns integer
        local integer l_iHash
        get read_hash item l_sHash item l_sValue to l_iHash
        function_return l_iHash
    end_function

    function value_at_index integer l_iHash returns string
        local string l_sBuf

        forward get array_value item l_iHash to l_sBuf
        
        function_return l_sBuf
    end_function
    
    function string_at_index integer l_iHash returns string
        local string l_sBuf

        forward get string_value item l_iHash to l_sBuf
        
        function_return l_sBuf
    end_function    
    
    function next_hash returns string
        local string l_sBuf
        local integer l_iItemP l_iMaxHash l_iMinHash
        
        get c_iItemP to l_iItemP
        get c_iMaxHash to l_iMaxHash
        get c_iMinHash to l_iMinHash
        
        if (l_iMinHash > l_iItemP) move (l_iMinHash-1) to l_iItemP
        
        move "" to l_sBuf
        while ((l_sBuf = "") and (l_iItemP <= l_iMaxHash))
            increment l_iItemP 
            forward get array_value item l_iItemP to l_sBuf
            if (trim(l_sBuf) = "<!!#ITEM REMOVED#!!>") move "" to l_sBuf            
        loop
        
        if (l_iItemP > l_iMaxHash) move -1 to l_iItemP
        set c_iItemP to l_iItemP
    
        function_return l_sBuf
    end_function 
    
end_class

// Hash class - more akin to similar "hash" data structures in other languages
// which stores key/value pairs.
// This is a quick 5 minute implementation, it depends on the hashtable object 
// above for it's hashing.
//
// Send message methods:
//
//    truncate          - Clear the hash
//    remove_key        - Removes a key/value pair from the hash
//
// Set methods:
//    value_at_key      - Gets the value stored for a particular key
//
// Get methods:
//    insert_key        - Inserts a key/value pair into the hash
//    value_at_key      - Retrieves the value stored for a particular key
//    item_count        - Retrieves the count of items in the hash
//
//    Example usage:
//    
//    
//    string key iv buf
//    integer i
//    
//    object test is a hash
//    end_object
//    
//    send insert_key to test "INDEX 1" "VALUE 1"
//    send insert_key to test "INDEX 2" "VALUE 2"
//    send insert_key to test "INDEX 3" "VALUE 3"
//    
//    get first_key of test to key
//    showln "KEY " key
//    get value_at_key of test item key to buf
//    showln "VALUE " buf
//    
//    while (key <> "") 
//        get next_key of test key to key
//        if (key <> "") begin
//            showln "KEY " key
//            get value_at_key of test item key to buf
//            showln "VALUE " buf
//        end
//    loop
//    
//    set value_at_key of test item "INDEX 2" item "VALUE TWO"
//    showln "SET VALUE AT 'INDEX 2'"
//    
//    get value_at_key of test item "INDEX 1" to buf
//    showln "LOOKUP 'INDEX 1' = " buf
//    get value_at_key of test item "INDEX 2" to buf
//    showln "LOOKUP 'INDEX 2' = " buf
//    
//    get item_count of test to i
//    showln "ITEM COUNT " i
//    
//    send remove_key to test "INDEX 3"
//    showln "REMOVE AT KEY: 'INDEX 3'"
//    
//    get item_count of test to i
//    showln "ITEM COUNT " i
//    
//    get value_at_key of test item "INDEX 3" to buf
//    showln "LOOKUP 'INDEX 3' =" buf
//    
//    send truncate to test
//    showln "TRUNCATED"
//    
//    get value_at_key of test item "INDEX 1" to buf
//    showln "LOOKUP 'INDEX 1' = " buf
//    
//    get item_count of test to i
//    showln "EMPTY COUNT " i
//
class hash is an array
    procedure construct_object integer argc
        object keystore is a hashtable
        end_object
        
        object linkstore is a linkedlist
        end_object
        
        forward send construct_object
    end_procedure

    procedure truncate
        send delete_data to (keystore(current_object))
        send delete_data to (linkstore(current_object))
        
        forward send delete_data
    end_procedure    
    
    procedure remove_key string l_sKey
        local integer l_iIndex
        get find_hash of (keystore(current_object)) item l_sKey to l_iIndex
        if (l_iIndex <> -1) begin
            send remove_hash to (keystore(current_object)) l_iIndex
            send remove_link to (linkstore(current_object)) l_iIndex
            forward set array_value item l_iIndex to ""
        end
    end_procedure
    
    procedure set value_at_key string l_sKey string l_sValue
        local integer l_iIndex
        
        get find_hash of (keystore(current_object)) item l_sKey to l_iIndex
        forward set array_value item l_iIndex to l_sValue
        
    end_procedure
    
    procedure insert_key string l_sKey string l_sValue
        local integer l_iIndex
        
        get insert_hash of (keystore(current_object)) item l_sKey to l_iIndex
        send insert_link to (linkstore(current_object)) l_iIndex
        forward set array_value item l_iIndex to l_sValue
        
    end_procedure
    
    function value_at_key string l_sKey returns string
        local integer l_iIndex
        local string l_sValue
        
        get find_hash of (keystore(current_object)) item l_sKey to l_iIndex
        forward get string_value item l_iIndex to l_sValue
        
        function_return l_sValue
    end_function
    
    function first_key returns string
        local integer l_iIndex
        local string l_sKey
        
        get first_link of (linkstore(current_object)) to l_iIndex
        get string_at_index of (keystore(current_object)) item l_iIndex to l_sKey

        function_return l_sKey
    end_function    
    
    function next_key string l_sKey returns string
        local integer l_iIndex
        
        get find_hash of (keystore(current_object)) item l_sKey to l_iIndex
        get next_link of (linkstore(current_object)) item l_iIndex to l_iIndex
        get string_at_index of (keystore(current_object)) item l_iIndex to l_sKey

        function_return l_sKey
    end_function
    
    function item_count returns integer
        local integer l_iResult
        get item_count of (keystore(current_object)) to l_iResult
        
        function_return l_iResult
    end_function
end_class

// Matrix class - Provides an indexed two-dimensional array / matrix class
//
// This class is sensitive to ascii char 1, which is used for delimiting values 
// within the second dimension.  The implimentation is rather unrefined and 
// stores delimited values within an array, thus the wider the matrix the 
// slower any adding removing or sorting will be. 
//
// This is designed purely as a convenience to provide something matrix like within 
// the limitations of DataFlex 3.2 Console Mode; be sure to keep this in mind before 
// using.
//
// To allow quick lookup of linked data, a hash index may be created on one column 
// of a matrix at a time.
// A hash index may be added both before and after populating the matrix with your 
// data.
//
// As with a hash table the hasing algorithm may be set for the matrix, but to take
// affect this must be called before the hash_on_column message. (see hash.inc 
// function for available algorithms)
//        
// The hash_is_unique and remove_hash_is_unique messages allow enforcement of unique
// values in the hashed column.  If the hash_is_unique message is sent after the 
// creation of a hash index on a matrix already populated with data the constraint will 
// only apply to new data.
//    
// The hash may also be removed from the column (freeing up any memory used, which can
// be fairly large) at any time, this allows for removing the hash from one column and 
// re-assigning it to another or changing hash algorithm on the same data.
//  
// Lookups on the hash index are performed with the matrix_index_from_value, matrix_index_count_from_value
//
// Send message methods:
//    delete_data                   - Clear the matrix
//    [obsolete] matrix_sort        - Y pos to sort on, ASC OR DESC
//    sort_items                    - Y pos to sort on, ASC OR DESC (auto detects)
//    sort_items_ascii              - Y pos to sort on, ASC OR DESC (ascii)
//    sort_items_num                - Y pos to sort on, ASC OR DESC (numeric)
//    matrix_delete                 - X and Y pos to delete 
//    delete_item                   - X position to delete (this reshuffles the matrix; avoid using)
//    type_store                    - Store column type against column (speeds up sort_items at expense of numeric inserts) DEFAULT
//    remove_type_store             - Do not store column type against column (speeds up numeric inserts at expense of sort_items (but not sort_items_ascii or num))
//    hash_on_column_algorithm      - Hash algorithm to use
//    hash_on_column                - Y pos of column to hash
//    remove_hash_on_column         - Remove the hash from the column
//    hash_is_unique                - Add a unique constraint on the hash
//    remove_hash_is_unique         - Remove a unique constraint from the hash
//    matrix_index_lookup_clear     - Clear the lookup buffer
//    matrix_append_csv             - Append some data in CSV format to the array E.g. ('My Name,"My,\"address\""')
//    matrix_copy_csv_in            - Copy csv data from specified file into matrix
//    matrix_copy_csv_in_header     - Copy csv data with header from specified file into matrix
//    matrix_copy_csv_out           - Copy csv data from matrix into specified file
//
// Set methods:
//    matrix_value                  - Set a value at X, Y
//    matrix_string    
//
// Get methods:
//    matrix_value                  - Get a value at X, Y
//    matrix_string                 - Get an string value at X, Y
//    matrix_integer                - Get an integer value at X, Y
//    matrix_numeric                - Get an numeric value at X, Y
//    matrix_real                   - Get an real value at X, Y
//    matrix_hash_from_value        - Get the hash index value used for an indexed column value
//    matrix_indextable_from_value  - Get list of matrix x pos indexes for a particular hashed value
//    matrix_index_lookup_clear     - Clear the buffer for an indexed lookup
//    matrix_index_count_from_value - Get a count of rows with a particular value
//    matrix_index_from_value       - Get the next X pos (row) with indexed value. Returns -1 when nothing left to find.
//    item_count                    - Get count of rows in matrix
//    item_width                    - Get count of columns in matrix
//    
// Example usage:
//
//    object test is a matrix
//    end_object
//
//    set matrix_value of (test(current_object)) item 0 item 1 to "1"    - x then y pos to Value
//    get matrix_value of (test(current_object)) item 0 item 1 to tmpStr - x then y pos to Value
//    send matrix_append_csv to test ('My Name,"My,\"address\""')        - Append CSV data to the end of the matrix
//    send matrix_copy_csv_in to (test(current_object)) "f:\data.csv"       - Copy data from csv file into matrix
//    [obsolete] send matrix_sort to (test(current_object)) 1 ASC        - y pos to sort by, ASCENDING/DESCENDING
//    send sort_items to (test(current_object)) 1                        - y pos to sort by, ASCENDING/DESCENDING (auto)
//    send sort_items_ascii to (test(current_object)) 1                  - y pos to sort by, ASCENDING/DESCENDING (ascii)
//    send sort_items_num to (test(current_object)) 1                    - y pos to sort by, ASCENDING/DESCENDING (numeric)
//    send matrix_delete to (test(current_object)) 1 1           - x then y pos to delete 
//    send matrix_delete_row to (test(current_object)) 1             - x essentially blanks record out, no reshuffle
//    send delete_item to (test(current_object)) 1              - x pos (not v efficient), reshuffles
//
// Hash indexed columns usage:
//
//    send hash_on_column_algorithm to (test(current_object)) "hash_reduced_lazy"
//    send hash_on_column to (test(current_object)) 0
//    send remove_hash_on_column to (test(current_object)) 
//    send hash_is_unique to (test(current_object)) 
//    
//    send matrix_index_lookup_clear to (test(current_object)) 
//    get matrix_index_count_from_value of (test(current_object)) item "1" to count
//    get matrix_index_from_value of (test(current_object)) item "1" to x_pos   
//    get matrix_indextable_from_value of (test(current_object)) item "1" to tmpStr 
//    get matrix_hash_from_value of (test(current_object)) item "1" to tmpInt   
//    get item_count of (test(current_object) to tmpInt
//    get item_width of (test(current_object) to tmpInt

class matrix is an array
    procedure construct_object integer argc
        object mTokens is a StringTokenizer
        end_object
        object mTokens2 is a StringTokenizer
        end_object
        
        forward send construct_object
        property integer c_iWidth public argc
        property integer c_iHashOn
        property integer c_iLastIndexTableHash
        property integer c_iLastIndexTablePos
        property integer c_iEnforceUnique
        property integer c_iMaintainTypes
        property string c_sHashAlgorithm
        property string c_sTypes
        
        set c_sTypes to ""
        set c_sHashAlgorithm to ""
        set c_iHashOn to -1
        set c_iLastIndexTableHash to -1
        set c_iLastIndexTablePos to -1
        set c_iEnforceUnique to 0
        set c_iMaintainTypes to 1
    end_procedure    
    
    // Pull the value of a column from the string representation
    function column_value integer itemy string row
        local string l_sResult
        local integer l_i
        
        move row to l_sResult
        
        for l_i from 0 to (itemy-1)
            move (right(l_sResult,length(l_sResult)-pos(character(1),l_sResult))) to l_sResult
        loop
        move (left(l_sResult,pos(character(1),l_sResult)-1)) to l_sResult
        
        function_return l_sResult
    end_function    
    
    procedure hash_on_column_algorithm string hashalg
        if ((hashalg = "hash_reduced_djb2") or (hashalg = "hash_reduced_sdbm") or (hashalg = "hash_reduced_lazy") or (hashalg = "hash_for_df_arrays") or (hashalg = "")) begin
            set c_sHashAlgorithm to hashalg
        end     
    end_procedure

    procedure type_store
        set c_iMaintainTypes to 1
    end_procedure
    
    procedure remove_type_store
        set c_iMaintainTypes to 0
        set c_sTypes to ""
    end_procedure    
    
    procedure hash_is_unique
        set c_iEnforceUnique to 1
    end_procedure
    
    procedure remove_hash_is_unique
        set c_iEnforceUnique to 0
    end_procedure   
    
    procedure hash_on_column integer l_iColumn
        local integer l_iMax l_iHashOn l_i l_iHash l_iEnforceUnique l_iHashError
        local string l_sBuf l_sTmp l_sHashAlgorithm 
        
        forward get item_count to l_iMax
        get c_iHashOn to l_iHashOn
        
        // Allow adding hash only when no hash already set
        if ((l_iHashOn = -1) and (l_iColumn >= 0)) begin
                            
            object mHash_array is an array
            end_object              
                
            object mHash_table is a hashTable
            end_object
                
            get c_sHashAlgorithm to l_sHashAlgorithm
            get c_iEnforceUnique to l_iEnforceUnique
            
            if (l_sHashAlgorithm <> "") begin
                send hash_algorithm to (mHash_table(current_object)) l_sHashAlgorithm
            end
            
            if (l_iMax <> 0) begin
                // Hash the current matrix if not empty
                move (l_iMax-1) to l_iMax       
                
                for l_i from 0 to l_iMax
                    forward get array_value item l_i to l_sBuf

                    get column_value item l_iColumn item l_sBuf to l_sTmp
                    
                    get insert_hash of (mHash_table(current_object)) item l_sTmp to l_iHash
                    get string_value of (mHash_array(current_object)) item l_iHash to l_sTmp
                        
                    if ((l_iEnforceUnique = 1) and (length(replace("|",l_sTmp,"")) <> 0)) begin
                        custom_error ERROR_CODE_DUPLICATE_HASH_KEY$ ERROR_MSG_DUPLICATE_HASH_KEY ERROR_DETAIL_DUPLICATE_HASH_KEY l_iColumn
                        move 1 to l_iHashError
                        break
                    end
                    else if not (l_sTmp contains ("|"+string(l_i)+"|")) begin
                        if (length(l_sTmp) = 0) move "|" to l_sTmp
                        append l_sTmp (string(l_i)+"|")
                        set array_value of (mHash_array(current_object)) item l_iHash to l_sTmp
                    end
                loop
            end
                
            if (l_iHashError = 0) begin
                set c_iHashOn to l_iColumn
            end
            else begin
                send destroy_object to (mHash_array(current_object))
                send destroy_object to (mHash_table(current_object))                    
            end
        end
    end_procedure
        
    procedure remove_hash_on_column
        local integer l_iHashOn
        
        get c_iHashOn to l_iHashOn
        
        if (l_iHashOn <> -1) begin          
            set c_iHashOn to -1
            set c_iLastIndexTableHash to -1
            set c_iLastIndexTablePos to -1
            send destroy_object to (mHash_array(current_object))
            send destroy_object to (mHash_table(current_object))
        end
    end_procedure
        
    procedure set matrix_value integer itemx integer itemy string val
        local string l_sBuf l_sTmp l_sOldVal l_sTypes
        local integer l_i l_iWidth l_iHashOn l_iHash l_iEnforceUnique l_iHashError l_iMaintainTypes

        move 0 to l_iHashError
        get c_iWidth to l_iWidth
        get c_iHashOn to l_iHashOn        
        
        forward get array_value item itemx to l_sBuf
        
        //Maintain (guess of) types of columns
        get c_iMaintainTypes to l_iMaintainTypes
        if (l_iMaintainTypes = 1) begin
            get c_sTypes to l_sTypes
            //All columns start off as numeric
            while (length(l_sTypes) < 1+itemy)
                append l_sTypes "1"
            loop
            //If we encounter a non-numeric value when we have defined numeric switch the type
            if ((mid(l_sTypes,1,1+itemy) = "1") and not (is_number(val))) begin
                move (overstrike("0",l_sTypes,1+itemy)) to l_sTypes
                set c_sTypes to l_sTypes
            end        
        end
        
        // Maintain width of matrix                
        if (itemy > l_iWidth) begin
                set c_iWidth to itemy
                move itemy to l_iWidth
        end

        // Delimiter is ascii char 1 (start of heading/console interrupt)
        // so any values containing ascii 1 will, of course break the matrix
        send delete_data to (mTokens(current_object))
        send set_string to (mTokens(current_object)) l_sBuf (character(1))
        
        if (l_iHashOn = itemy) begin
            get token_value of (mTokens(current_object)) item itemy to l_sOldVal
        end
        if (val = "") set token_value of (mTokens(current_object)) item itemy to (character(3))
        else set token_value of (mTokens(current_object)) item itemy to (replaces(character(1),(replaces(character(3),val,"")),""))
    
        move "" to l_sBuf
        for l_i from 0 to l_iWidth                        
            get token_value of (mTokens(current_object)) item l_i to l_sTmp                        
            if (length(l_sTmp) = 0) move (character(3)) to l_sTmp        
            if (length(l_sBuf) <> 0) append l_sBuf (character(1))
            append l_sBuf l_sTmp
        loop
                
        move (replaces(character(3),l_sBuf,"")) to l_sBuf
                    
        // Insert/update in the value to the hash
        if ((l_iHashOn = itemy) and (l_sOldVal <> val)) begin       
            get c_iEnforceUnique to l_iEnforceUnique
            get insert_hash of (mHash_table(current_object)) item val to l_iHash
            get string_value of (mHash_array(current_object)) item l_iHash to l_sTmp
            
            if ((l_iEnforceUnique = 1) and (length(replace("|",l_sTmp,"")) <> 0)) begin
                custom_error ERROR_CODE_DUPLICATE_HASH_KEY$ ERROR_MSG_DUPLICATE_HASH_KEY ERROR_DETAIL_DUPLICATE_HASH_KEY itemy
                move 1 to l_iHashError
            end
            else if not (l_sTmp contains ("|"+string(itemx)+"|")) begin
                if (length(l_sTmp) = 0) move "|" to l_sTmp
                append l_sTmp (string(itemx)+"|")
                set array_value of (mHash_array(current_object)) item l_iHash to l_sTmp
            end
 
            // Remove old hash (if any) when insert succeeds
            if (l_iHashError = 0) begin
                get find_hash of (mHash_table(current_object)) item l_sOldVal to l_iHash
                if (l_iHash <> 0) begin
                    get string_value of (mHash_array(current_object)) item l_iHash to l_sTmp                
                    if (l_sTmp contains ("|"+string(itemx)+"|")) begin
                        move (replace(("|"+string(itemx)+"|"),l_sTmp,"")) to l_sTmp
                        if (l_sTmp = "") begin
                            send remove_hash to (mHash_table(current_object)) l_iHash
                        end
                        else begin
                            if (left(l_sTmp,1) <> "|") move ("|"+l_sTmp) to l_sTmp
                            else append l_sTmp "|"
                        end
                        set array_value of (mHash_array(current_object)) item l_iHash to l_sTmp
                    end
                end
            end
        end
                
        if (l_iHashError = 0) begin
            forward set array_value item itemx to l_sBuf
        end
    end_procedure
    
    procedure matrix_append_csv string row        
        local integer l_iMax l_iValues l_i l_iHashOn l_iWidth l_iOffset l_iCount l_iMaintainTypes
        local string l_sChar l_sBuf l_sTypes
            
        get c_iHashOn to l_iHashOn
        forward get item_count to l_iMax
        
        // If the csv data contains quoted data we currenlty have to
        // pass each column on to matrix_value
        if ((l_iHashOn <> -1) or (row contains '"')) begin 
            send delete_data to (mTokens2(current_object))
            send set_string_csv to (mTokens2(current_object)) row
            get token_count of (mTokens2(current_object)) to l_iValues
        
            for l_i from 0 to l_iValues                 
                get token_value of (mTokens2(current_object)) item l_i to l_sBuf
                indicate err false
                set matrix_value item l_iMax item l_i to l_sBuf         
                if (err) forward send delete_item l_iMax
                if (err) break
            loop
        end
        // Otherwise we take a shortcut and set the array row in one
        else begin
            // Maintain width of matrix
            get c_iWidth to l_iWidth
            
            //Maintain (guess of) types of columns
            get c_iMaintainTypes to l_iMaintainTypes            
            if (l_iMaintainTypes = 1) begin
                get c_sTypes to l_sTypes
                move 0 to l_iOffset
            end      
            else;
                move (pos(',', row)) to l_iOffset
            
            move 0 to l_iCount
            move "" to l_sBuf
            forward set array_value item l_iMax to (replaces(',', row, character(1)))
            for l_i from l_iOffset to (length(row))
                move (mid(row,1,l_i))  to l_sChar
                if (l_sChar = ',') begin
                    increment l_iCount
                    
                    if (l_iMaintainTypes = 1) begin
                        //All columns start off as numeric
                        while (length(l_sTypes) < l_iCount)
                            append l_sTypes "1"
                        loop
                        //If we encounter a non-numeric value when we have defined numeric switch the type
                        if ((mid(l_sTypes,1,l_iCount) = "1") and not (is_number(l_sBuf))) begin
                            move (overstrike("0",l_sTypes,l_iCount)) to l_sTypes
                        end                             
                        move "" to l_sBuf
                    end
                end
                else if (l_iMaintainTypes = 1);
                    append l_sBuf l_sChar
            loop            
            if (l_iCount > l_iWidth);
                set c_iWidth to l_iCount
            if (l_iMaintainTypes = 1) begin
                if ((mid(l_sTypes,1,l_iCount) = "1") and not (is_number(l_sBuf))) begin
                    move (overstrike("0",l_sTypes,l_iCount)) to l_sTypes
                end        
                set c_sTypes to l_sTypes
            end   
        end

    end_procedure

    procedure matrix_copy_csv_worker string fname integer offset
        local string l_sBuf 
        local integer l_i
        
        move 0 to l_i
        if (does_exist(fname)) begin
            direct_input channel DEFAULT_FILE_CHANNEL fname
                while not (seqeof)
                    readln channel DEFAULT_FILE_CHANNEL l_sBuf
                    increment l_i
                    if (l_i <= offset) break begin
                    if (seqeof) break
                    if (trim(l_sBuf) <> "") begin
                        send matrix_append_csv l_sBuf
                    end
                loop
            close_input channel DEFAULT_FILE_CHANNEL
        end
        else;
            custom_error ERROR_CODE_FILE_NOT_FOUND$ ERROR_MSG_FILE_NOT_FOUND ERROR_DETAIL_FILE_NOT_FOUND fname
                    
    end_procedure 
    
    procedure matrix_copy_csv_in string fname
        send matrix_copy_csv_worker fname 0
    end_procedure 

    procedure matrix_copy_csv_in_header string fname
        send matrix_copy_csv_worker fname 1
    end_procedure 
        
    procedure matrix_copy_csv_out string fname 
        local integer l_iMax l_i l_j l_iValues
        local string l_sBuf
        
        forward get item_count to l_iMax
        
        direct_output channel DEFAULT_FILE_CHANNEL fname
            for l_i from 0 to l_iMax
                forward get string_value item l_i to l_sBuf
                if (l_sBuf <> "") begin
                    if ((l_sBuf contains '"') or (l_sBuf contains ',')) begin
                        send delete_data to (mTokens2(current_object))
                        send set_string to (mTokens2(current_object)) l_sBuf (character(1))
                        get token_count of (mTokens2(current_object)) to l_iValues
        
                        for l_j from 0 to l_iValues                 
                            get token_value of (mTokens2(current_object)) item l_j to l_sBuf
                            if (l_j <> 0);
                                write channel DEFAULT_FILE_CHANNEL ','
                            if (l_sBuf contains '"');
                                write channel DEFAULT_FILE_CHANNEL ('"'+(replaces('"', l_sBuf, '\"'))+'"')
                            else;
                                write channel DEFAULT_FILE_CHANNEL l_sBuf
                        loop
                        writeln channel DEFAULT_FILE_CHANNEL ""
                    end
                    else;
                        writeln channel DEFAULT_FILE_CHANNEL (replaces(character(1), l_sBuf, ','))
                end
            loop
        close_output channel DEFAULT_FILE_CHANNEL
    end_procedure 
        
    function matrix_value integer itemx integer itemy returns string
        local string l_sBuf l_sTmp

        forward get array_value item itemx to l_sBuf
        get column_value item itemy item l_sBuf to l_sTmp
        
        function_return l_sTmp
    end_function        
    
    function matrix_string integer itemx integer itemy returns string
        local string l_sTmp

        get matrix_value item itemx item itemy to l_sTmp
        
        function_return l_sTmp
    end_function        
    
    function matrix_integer integer itemx integer itemy returns integer
        local integer l_iTmp
        
        get matrix_value item itemx item itemy to l_iTmp
        
        function_return l_iTmp
    end_function
    
    function matrix_number integer itemx integer itemy returns number
        local number l_nTmp
        
        get matrix_value item itemx item itemy to l_nTmp
        
        function_return l_nTmp
    end_function
    
    function matrix_real integer itemx integer itemy returns real
        local real l_rTmp
        
        get matrix_value item itemx item itemy to l_rTmp
        
        function_return l_rTmp
    end_function
    
    function matrix_hash_from_value string val returns integer
        local integer l_iHash l_iHashOn
        
        get c_iHashOn to l_iHashOn
                
        if (l_iHashOn <> -1) begin
            get find_hash of (mHash_table(current_object)) item val to l_iHash
        end

        function_return l_iHash
    end_function
    
    function matrix_indextable_from_value string val returns string
        local integer l_iHashOn l_iHash
        local string l_sIndexTable

        get c_iHashOn to l_iHashOn
                
        if (l_iHashOn <> -1) begin
            get find_hash of (mHash_table(current_object)) item val to l_iHash
            get string_value of (mHash_array(current_object)) item l_iHash to l_sIndexTable
        end

        function_return l_sIndexTable
    end_function
        
    procedure matrix_index_lookup_clear 
        local integer l_iHashOn
        
        get c_iHashOn to l_iHashOn
    
        if (l_iHashOn <> -1) begin
            set c_iLastIndexTableHash to -1
            set c_iLastIndexTablePos to -1
        end
    end_procedure
        
    function matrix_index_from_value string val returns integer
        local integer l_iHashOn l_iHash l_iLastIndexTableHash l_iLastIndexTablePos l_iIndex l_iIndexValues
        local string l_sIndexTable

        get c_iHashOn to l_iHashOn
        move -1 to l_iIndex
        move 0 to l_iLastIndexTablePos
            
        if (l_iHashOn <> -1) begin
            get find_hash of (mHash_table(current_object)) item val to l_iHash
            get string_value of (mHash_array(current_object)) item l_iHash to l_sIndexTable

            get c_iLastIndexTableHash to l_iLastIndexTableHash          
            
            if (l_iHash = l_iLastIndexTableHash) begin
                get c_iLastIndexTablePos to l_iLastIndexTablePos
            end
            increment l_iLastIndexTablePos
            
            send delete_data to (mTokens(current_object))
            send set_string to (mTokens(current_object)) l_sIndexTable "|"
            get token_count of (mTokens(current_object)) to l_iIndexValues
            if (l_iLastIndexTablePos <= l_iIndexValues) begin
                get token_value of (mTokens(current_object)) item l_iLastIndexTablePos to l_iIndex
                set c_iLastIndexTableHash to l_iHash
                set c_iLastIndexTablePos to l_iLastIndexTablePos
            end
            else begin
                move -1 to l_iIndex
                set c_iLastIndexTableHash to -1
                set c_iLastIndexTablePos to -1
            end
        end

        function_return l_iIndex
    end_function
    
    function matrix_index_count_from_value string val returns integer
        local integer l_iHashOn l_iHash l_iIndexValues l_i
        local string l_sIndexTable
    
        get c_iHashOn to l_iHashOn
    
        if (l_iHashOn <> -1) begin
            get find_hash of (mHash_table(current_object)) item val to l_iHash
            get string_value of (mHash_array(current_object)) item l_iHash to l_sIndexTable
    
            move 0 to l_iIndexValues
            for l_i from 1 to (length(l_sIndexTable))
                if (mid(l_sIndexTable,1,l_i) = '|');
                    increment l_iIndexValues
            loop
        end
    
        function_return (l_iIndexValues-1)
    end_function
        
    procedure set item_count integer newVal
        forward set item_count to newVal
    end_procedure
    
    function item_width returns integer
        local integer l_iWidth
        get c_iWidth to l_iWidth
        function_return l_iWidth
    end_function
        
    procedure matrix_delete integer itemx integer itemy
        local string l_sBuf l_sTmp l_sOldVal
        local integer l_i l_iWidth l_iHashOn l_iHash

        get c_iWidth to l_iWidth
        get c_iHashOn to l_iHashOn
    
        forward get array_value item itemx to l_sBuf
            
        send delete_data to (mTokens(current_object))
        send set_string to (mTokens(current_object)) l_sBuf (character(1))
        
        if (l_iHashOn = itemy) begin                
            get token_value of (mTokens(current_object)) item itemy to l_sOldVal
        end
        set token_value of (mTokens(current_object)) item itemy to (character(3))

        move "" to l_sBuf
        for l_i from 0 to l_iWidth
            get token_value of (mTokens(current_object)) item l_i to l_sTmp
            if (length(l_sTmp) = 0) move (character(3)) to l_sTmp
            if (length(l_sBuf) <> 0) append l_sBuf (character(1))
            append l_sBuf l_sTmp
        loop
        move (replaces(character(3),l_sBuf,"")) to l_sBuf

        forward set array_value item itemx to l_sBuf
        
        // Delete the value in the hash
        if (l_iHashOn = itemy) begin                
            get find_hash of (mHash_table(current_object)) item l_sOldVal to l_iHash
            if (l_iHash <> 0) begin                     
        get string_value of (mHash_array(current_object)) item l_iHash to l_sTmp                    
        if (l_sTmp contains ("|"+string(itemx)+"|")) begin
            move (replace(("|"+string(itemx)+"|"),l_sTmp,"")) to l_sTmp
            if (l_sTmp = "") begin
                send remove_hash to (mHash_table(current_object)) l_iHash
            end
            else begin
                if (left(l_sTmp,1) <> "|") move ("|"+l_sTmp) to l_sTmp
                else append l_sTmp "|"
            end
            set array_value of (mHash_array(current_object)) item l_iHash to l_sTmp
        end                             
            end
        end
    end_procedure
        
    // Inefficient.
    procedure delete_item integer itemx                
        local string l_sBuf l_sOldVal l_sTmp l_sIndexTable
        local integer l_iHashOn l_iHash l_i l_j l_iItems l_iIndexValues l_iIndex
        
        get c_iHashOn to l_iHashOn
        // Delete the value in the hash
        if (l_iHashOn <> -1) begin
            forward get array_value item itemx to l_sBuf
            send delete_data to (mTokens(current_object))
            send set_string to (mTokens(current_object)) l_sBuf (character(1))
            get token_value of (mTokens(current_object)) item l_iHashOn to l_sOldVal
            get find_hash of (mHash_table(current_object)) item l_sOldVal to l_iHash
            if (l_iHash <> 0) begin                     
                get string_value of (mHash_array(current_object)) item l_iHash to l_sTmp
                if (l_sTmp contains ("|"+string(itemx)+"|")) begin
                    move (replace(("|"+string(itemx)+"|"),l_sTmp,"")) to l_sTmp
                    if (l_sTmp = "") begin
                        send remove_hash to (mHash_table(current_object)) l_iHash
                    end
                    else begin
                        if (left(l_sTmp,1) <> "|") move ("|"+l_sTmp) to l_sTmp
                        else append l_sTmp "|"
                    end
                    set array_value of (mHash_array(current_object)) item l_iHash to l_sTmp
                end                     
            end
                
            forward get item_count to l_iItems
        
            for l_i from (itemx+1) to l_iItems
        
                forward get array_value item l_i to l_sBuf
                send delete_data to (mTokens(current_object))
                send set_string to (mTokens(current_object)) l_sBuf (character(1))
                get token_value of (mTokens(current_object)) item l_iHashOn to l_sOldVal
                get find_hash of (mHash_table(current_object)) item l_sOldVal to l_iHash
                
                if (l_iHash <> 0) begin
                    get string_value of (mHash_array(current_object)) item l_iHash to l_sIndexTable
                    
                    send delete_data to (mTokens(current_object))
                    send set_string to (mTokens(current_object)) l_sIndexTable "|"
                    get token_count of (mTokens(current_object)) to l_iIndexValues
                    move "|" to l_sIndexTable
                    for l_j from 1 to l_iIndexValues
                        get token_value of (mTokens(current_object)) item l_j to l_iIndex
                        if (l_iIndex = l_i) calc (l_iIndex-1) to l_iIndex
                        append l_sIndexTable (string(l_iIndex)+"|")
                    loop
                    
                    set array_value of (mHash_array(current_object)) item l_iHash to l_sIndexTable
                end
            loop
            
        end
            
        forward send delete_item to current_object itemx
    end_procedure

    // The routine below relies on the internal dataflex sort, doing
    // what is essentially a nested loop join on the result and rebuilding
    // the original matrix.  It's pretty awful and is only left here for
    // reference.  Behaviour isn't quite quadratic, a feeble guess is
    // something like O( (2N + Nlog(n) + N^1.8) :-(
    procedure matrix_sort integer itemy string order
        local string l_sBuf l_sTmp l_sTmp2 l_sHash
        local integer l_iX l_i l_j l_iMax l_iPoolMax l_iWidth l_iThrow l_iNumCount l_iHashOn l_iHash
        
        move (trim(uppercase(order))) to order
        if ((left(order,3) <> "ASC") and (left(order,4) <> "DESC")) move "ASCENDING" to order
        
        object mSort_array is an array
        end_object
        object mClone_array is an array
        end_object
        
        get c_iHashOn to l_iHashOn
        get c_iWidth to l_iWidth
        forward get item_count to l_iMax
        
        send delete_data to (mSort_array(current_object))
        send delete_data to (mClone_array(current_object))
        
        if (l_iHashOn <> -1) begin
            //Zero the hash
            send delete_data to (mHash_array(current_object))
        end
        
        move (l_iMax-1) to l_iMax       
        
        for l_i from 0 to l_iMax
            forward get array_value item l_i to l_sBuf
            
            send delete_data to (mTokens(current_object))
            send set_string to (mTokens(current_object)) l_sBuf (character(1))
            get token_value of (mTokens(current_object)) item itemy to l_sTmp
        
            move 0 to l_iNumCount 
            for l_j from 1 to (length(l_sTmp))
                if not (((ascii(mid(l_sTmp,1,l_j))) >= 48) and ((ascii(mid(l_sTmp,1,l_j))) <= 57) or ((ascii(mid(l_sTmp,1,l_j))) = 46)) break
                increment l_iNumCount
            loop
            if ((length(l_sTmp) > 0) and (length(l_sTmp) = l_iNumCount)) begin
                set array_value of (mSort_array(current_object)) item l_i to (number(l_sTmp))
            end
            else begin
                if (length(l_sTmp) = 0) move (character(2)) to l_sTmp
                set array_value of (mSort_array(current_object)) item l_i to l_sTmp
            end
        loop
        
        //Rely on dataflex sort
        if (left(order,3) = "ASC") send sort_items to (mSort_array(current_object)) ascending
        if (left(order,4) = "DESC") send sort_items to (mSort_array(current_object)) descending

        move l_iMax to l_iPoolMax

        // Nested loop join, sort of. Not good :-(
        for l_i from 0 to l_iMax
            get array_value of (mSort_array(current_object)) item l_i to l_sTmp
            if (l_sTmp = character(2)) move "" to l_sTmp
            
            for l_j from 0 to l_iPoolMax
                // Ideally we'd change the next 3 lines for a lookup table instead
                forward get array_value item l_j to l_sBuf
                
                send delete_data to (mTokens(current_object))
                send set_string to (mTokens(current_object)) l_sBuf (character(1))
                get token_value of (mTokens(current_object)) item itemy to l_sTmp2
                
                if (l_sTmp = l_sTmp2) begin
                    set array_value of (mClone_array(current_object)) item l_i to l_sBuf                    
                    
                    // On successful find shrink the sort pool here by moving max to l_j and decrementing max                 
                    forward get array_value item l_iPoolMax to l_sBuf
                    forward set array_value item l_j to l_sBuf
                    forward send delete_item to current_object l_iPoolMax
                    decrement l_iPoolMax                    
                        
                    // Rebuild hash
                    if (l_iHashOn <> -1) begin                  
                        get token_value of (mTokens(current_object)) item l_iHashOn to l_sHash
                        get find_hash of (mHash_table(current_object)) item l_sHash to l_iHash
                        get string_value of (mHash_array(current_object)) item l_iHash to l_sTmp
                        if not (l_sTmp contains ("|"+string(l_i)+"|")) begin
                            if (length(l_sTmp) = 0) move "|" to l_sTmp
                            append l_sTmp (string(l_i)+"|")
                            set array_value of (mHash_array(current_object)) item l_iHash to l_sTmp
                        end
                    end                    
                    goto dirty_speedup_jump
                end
            loop
            dirty_speedup_jump:
        loop
        send delete_data to (mSort_array(current_object))
                
        for l_i from 0 to l_iMax
            get array_value of (mClone_array(current_object)) item l_i to l_sBuf
            forward set array_value item l_i to l_sBuf                      
        loop
        
        send destroy_object to (mSort_array(current_object))  // Use "send request_destroy_object" to destroy object and all children.
        send destroy_object to (mClone_array(current_object))
    end_procedure

    
    // Recursive partition for quicksort.
    // Dataflex arrays track the type of each row and perform a sort acordingly
    // but we have no easy way of knowing.  So perform compare based on what a
    // value looks "like" unless told otherwise.    
    // Index from (lo_in), index to (hi_in), numeric/string mode (1=integer, 0=string), invert (1=descending, 0=ascending)
    procedure partition integer lo_in integer hi_in integer mode integer itemy integer invert
        local integer pivot lo_idx hi_idx t
        local string pivot_val lo_row hi_row lo_val hi_val      
        
        if ((hi_in-lo_in) > 0) begin
            move lo_in to lo_idx
            move hi_in to hi_idx       
            move ((lo_in+hi_in)/2) to pivot
                        
            while ((lo_idx <= pivot) AND (hi_idx >= pivot))
                        
                forward get array_value item pivot to pivot_val
                get column_value item itemy item pivot_val to pivot_val
                
                forward get array_value item lo_idx to lo_row
                get column_value item itemy item lo_row to lo_val

                forward get array_value item hi_idx to hi_row
                get column_value item itemy item hi_row to hi_val

                
                if (invert) begin
                    while ( ( ((mode) and (number(lo_val) > number(pivot_val))) or (not (mode) and (lo_val > pivot_val))) and (lo_idx <= pivot))
                        increment lo_idx
                        forward get array_value item lo_idx to lo_row
                        get column_value item itemy item lo_row to lo_val
                    loop
                    while ( ( ((mode) and (number(hi_val) < number(pivot_val))) or (not (mode) and (hi_val < pivot_val))) and (hi_idx >= pivot))
                        decrement hi_idx
                        forward get array_value item hi_idx to hi_row
                        get column_value item itemy item hi_row to hi_val
                    loop
                end
                else begin
                    while ( ( ((mode) and (number(lo_val) < number(pivot_val))) or (not (mode) and (lo_val < pivot_val))) and (lo_idx <= pivot))
                        increment lo_idx
                        forward get array_value item lo_idx to lo_row
                        get column_value item itemy item lo_row to lo_val
                    loop
                    while ( ( ((mode) and (number(hi_val) > number(pivot_val))) or (not (mode) and (hi_val > pivot_val))) and (hi_idx >= pivot))
                        decrement hi_idx
                        forward get array_value item hi_idx to hi_row
                        get column_value item itemy item hi_row to hi_val
                    loop
                end
                
                forward set array_value item lo_idx to hi_row
                forward set array_value item hi_idx to lo_row
                
                increment lo_idx
                decrement hi_idx
                
                if ((lo_idx-1) = pivot) begin
                    increment hi_idx
                    move hi_idx to pivot
                end
                else if ((hi_idx+1) = pivot) begin
                    decrement lo_idx
                    move lo_idx to pivot
                end
                
            loop
    
            if ((pivot-lo_in) > 1);
                send partition lo_in (pivot-1) mode itemy invert
            if ((hi_in-pivot) > 1);
                send partition (pivot+1) hi_in mode itemy invert
        end        
    end_procedure   
    
    // Perform a quick sort on a particular column (y) in the martix
    // This is done in native dataflex, so no match for compiled C
    procedure quick_sort integer itemy string order integer mode
        local integer l_i l_j l_iHashOn l_iMax l_iInvert l_iMaintainTypes
        local string l_sBuf l_sTypes
        
        if (uppercase(left(trim(order),4)) = "DESC") move 1 to l_iInvert
        else move 0 to l_iInvert    
        
        get item_count to l_iMax
        
        // If we've not been told string/numeric, try and work out here.
        if (mode = -1) begin
            // If  we've been maintaining type information use it
            get c_iMaintainTypes to l_iMaintainTypes
            if (l_iMaintainTypes = 1) begin
                get c_sTypes to l_sTypes
                move (integer(mid(l_sTypes,1,1+itemy))) to mode             
            end
            // Else loop until we can make a decision
            else begin
                for l_i from 0 to (l_iMax-1)
                    forward get array_value item l_i to l_sBuf
                    get column_value item itemy item l_sBuf to l_sBuf
                    move (is_number(l_sBuf)) to mode
                    if (mode = 0) break
                loop
            end
        end
        
        // Remove the current hash index if there is one        
        get c_iHashOn to l_iHashOn
        if (l_iHashOn <> -1);
            send remove_hash_on_column  
        
        // Do the quick-sort
        send partition 0 (l_iMax-1) mode itemy l_iInvert
        
        // Recreate any the hash if there was one
        if (l_iHashOn <> -1);
            send hash_on_column l_iHashOn

    end_procedure
    
    //Wrapper for sort_items
    procedure sort_items integer itemy string order
        send quick_sort itemy order -1
    end_procedure
    
    //Wrapper for sort_items
    procedure sort_items_ascii integer itemy string order
        send quick_sort itemy order 0
    end_procedure    
    
    //Wrapper for sort_items
    procedure sort_items_num integer itemy string order
        send quick_sort itemy order 1
    end_procedure        
    
end_class

// Rss 2.0 data class - RFC-822 dates used
//
// Send message methods:
//     init_rss                  - Initialise a new rss20 instance
//     init_img                  - Initialise the image to be used in the feed
//     add_item                  - Add an item to the feed
//     write_rss                 - Write the feed out to disk
//
// Set methods:
//    set_ttl                    - Set the TTL/refresh rate of the feed
//    set_contacts               - Set admin contacts
//    
// Get methods:
//
// Example usage:
//
//    object test is an rss20
//    end_object
//
//    move "" to link
//    move "" to url
//
//    move "Google Maps" to title
//    move ("http:/"+"/www.google.com/maps") to link
//    move "Try out google maps" to desc
//    send init_rss to (test(current_object)) title link desc
//
//    move ("http:/"+"/www.google.com/images/srpr/logo11w.png") to url
//    move 19 to x
//    move 41 to y
//    send init_img to (test(current_object)) url x y
//
//    send set_ttl to (test(current_object)) 30
//    send set_contacts to (test(current_object)) "maps@google.com" "search@google.com"
//
//    for i from 1 to 15
//        move "Test item " to title
//        append title i
//        move ("http:/"+"/www.google.com") to link
//        move "Test description " to desc
//        append desc i
//        move "NONE" to cat
//
//        send add_item to (test(current_object)) title link desc cat (rssdate((now("date")),(now("longtime"))))
//    loop
//    send write_rss to (test(current_object)) "c:\google_maps.rss"

class rss20 is a matrix
    procedure construct_object string argc 
        forward send construct_object argc
        property string c_rssTitle
        property string c_rssLink
        property string c_rssDesc
        
        property string c_imgTitle
        property string c_imgUrl
        property string c_imgLink
        property string c_imgDesc
        
        property string c_webMaster
        property string c_manEditor
        
        property integer c_imgx
        property integer c_imgy
        property integer c_ttl
        
        property integer c_itemCount
    end_procedure
    
    procedure init_rss string rssTitle string rssLink string rssDesc
        set c_rssTitle to rssTitle
        set c_rssLink to rssLink
        set c_rssDesc to rssDesc
        set c_itemCount to 0
    end_procedure
    
    procedure init_img string imgUrl integer imgx integer imgy
        local string imgTitle imgLink imgDesc
        get c_rssTitle to imgTitle
    get c_rssLink to imgLink
    get c_rssDesc to imgDesc
    
        set c_imgTitle to imgTitle
        set c_imgUrl to imgUrl
        set c_imgLink to imgLink
        set c_imgDesc to imgDesc
        set c_imgx to imgx
        set c_imgy to imgy
    end_procedure
    
    procedure set_ttl integer ttl
        if (ttl > 0) set c_ttl to ttl
    end_procedure
    
    procedure set_contacts string webMaster string manEditor
        if (webMaster <> "") set c_webMaster to webMaster
        if (manEditor <> "") set c_manEditor to manEditor
    end_procedure
    
    procedure add_item string itemTitle string itemLink string itemDesc string itemCat string pubDate string itemGuID
        local integer l_itemCount
        get c_itemCount to l_itemCount
    
        // The standard says we should not have more than 15 items, but ignore this.
        //if ((l_itemCount < 15) and (itemTitle <> "")) begin 
        if (itemTitle <> "") begin
            increment l_itemCount
            set c_itemCount to l_itemCount
            
            forward set matrix_value item l_itemCount item 0 to itemTitle
            forward set matrix_value item l_itemCount item 1 to itemLink
            forward set matrix_value item l_itemCount item 2 to itemDesc
            forward set matrix_value item l_itemCount item 3 to itemCat
            forward set matrix_value item l_itemCount item 4 to itemGuID
            if ((pubDate <> "") and (pubDate <> "NOW")) forward set matrix_value item l_itemCount item 6 to pubDate
        end
    end_procedure
    
    procedure write_rss string rssFileName
        local string l_rssTitle l_rssLink l_rssDesc l_imgTitle l_imgUrl l_imgLink l_itemTitle l_itemLink l_itemDesc l_itemCat l_sConflict l_property l_manEditor l_webMaster l_pubDate l_itemGuID l_itemCc
        local integer l_imgx l_imgy l_itemcount l_i l_j l_iConflict l_iTtl
        
        get c_rssTitle to l_rssTitle
        get c_rssLink to l_rssLink
        get c_rssDesc to l_rssDesc
    
        get c_imgTitle to l_imgTitle
        get c_imgUrl to l_imgUrl
        get c_imgLink to l_imgLink
        get c_manEditor to l_manEditor
        get c_webMaster to l_webMaster
        
        get c_imgx to l_imgx
        get c_imgy to l_imgy
        get c_itemCount to l_itemCount
        get c_ttl to l_iTtl
    
        direct_output channel DEFAULT_FILE_CHANNEL rssFileName
            writeln channel DEFAULT_FILE_CHANNEL '<?xml version="1.0" ?>'
            writeln channel DEFAULT_FILE_CHANNEL '<?xml-stylesheet type="text/xsl" href="rss.xsl" media="screen"?>'
            write channel DEFAULT_FILE_CHANNEL '<rss version="2.0" xmlns:dc="http:/' '/purl.org/dc/elements/1.1/" xmlns:sy="http:/'
            write channel DEFAULT_FILE_CHANNEL '/purl.org/rss/1.0/modules/syndication/" xmlns:admin="http:/' '/webns.net/mvcb/" xmlns:rdf="http:/'
            writeln channel DEFAULT_FILE_CHANNEL '/www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:content="http:/' '/purl.org/rss/1.0/modules/content/">'
            
            // skipHours skipDays cloud - all currently not used
            // Write out Channel
            writeln channel DEFAULT_FILE_CHANNEL '       <channel>'
            writeln channel DEFAULT_FILE_CHANNEL '               <title>' (trim(l_rssTitle)) '</title>'
            writeln channel DEFAULT_FILE_CHANNEL '               <link>' (trim(l_rssLink)) '</link>'
            writeln channel DEFAULT_FILE_CHANNEL '               <description>' (trim(l_rssDesc)) '</description>'
            writeln channel DEFAULT_FILE_CHANNEL '               <language>en-gb</language>'
            writeln channel DEFAULT_FILE_CHANNEL '               <generator>Df32func RSS Object Generator</generator>'
            writeln channel DEFAULT_FILE_CHANNEL '               <copyright>Copyright ' (trim(l_rssTitle)) ' (C) ' (now("date")) '</copyright>'
            writeln channel DEFAULT_FILE_CHANNEL '               <lastBuildDate>' (rssdate((now("date")),(now("longtime")))) '</lastBuildDate>'
            writeln channel DEFAULT_FILE_CHANNEL '               <pubDate>' (rssdate((now("date")),(now("longtime")))) '</pubDate>'
            
            if (l_manEditor <> "") writeln channel DEFAULT_FILE_CHANNEL '               <managingEditor>' l_manEditor '</managingEditor>'
            if (l_webMaster <> "") writeln channel DEFAULT_FILE_CHANNEL '               <webMaster>' l_webMaster '</webMaster>'
            if (l_iTtl <> 0) writeln channel DEFAULT_FILE_CHANNEL '               <ttl>' l_iTtl '</ttl>'           
            
            // Write out image
            if ((l_imgUrl <> "") and (l_imgx > 0) and (l_imgy > 0)) begin
                writeln channel DEFAULT_FILE_CHANNEL '               <image>'
                writeln channel DEFAULT_FILE_CHANNEL '                       <title>' (trim(l_imgTitle)) '</title>'
                writeln channel DEFAULT_FILE_CHANNEL '                       <url>' (trim(l_imgUrl)) '</url>'
                writeln channel DEFAULT_FILE_CHANNEL '                       <link>' (trim(l_imgLink)) '</link>'
                writeln channel DEFAULT_FILE_CHANNEL '                       <height>' l_imgx '</height>'
                writeln channel DEFAULT_FILE_CHANNEL '                       <width>' l_imgy '</width>'
                writeln channel DEFAULT_FILE_CHANNEL '                       <description>' (trim(l_rssDesc)) '</description>'
                writeln channel DEFAULT_FILE_CHANNEL '               </image>'
            end
            
            // Write out items
            for l_i from 1 to l_itemCount
                forward get matrix_value item l_i item 0 to l_itemTitle
                forward get matrix_value item l_i item 1 to l_itemLink
                forward get matrix_value item l_i item 2 to l_itemDesc
                forward get matrix_value item l_i item 3 to l_itemCat
                forward get matrix_value item l_i item 4 to l_itemGuID
                forward get matrix_value item l_i item 5 to l_itemCc
                forward get matrix_value item l_i item 6 to l_pubDate
                    

                // Escape html in the description
                move (replaces('"',l_itemDesc,"&quot;")) to l_itemDesc
                move (replaces('<',l_itemDesc,"&lt;")) to l_itemDesc
                move (replaces('>',l_itemDesc,"&gt;")) to l_itemDesc
                
                writeln channel DEFAULT_FILE_CHANNEL '               <item>'
                writeln channel DEFAULT_FILE_CHANNEL '                      <title>' l_itemTitle '</title>'
                writeln channel DEFAULT_FILE_CHANNEL '                      <link>' l_itemLink '</link>'
                writeln channel DEFAULT_FILE_CHANNEL '                      <description>' l_itemDesc '</description>'
                
                if (l_itemGuID = "") begin
                move 0 to l_iConflict
                for l_j from 1 to (l_i-1)
                    forward get matrix_value item l_j item 1 to l_sConflict
                    if (l_sConflict = l_itemLink) increment l_iConflict
                end
                    if (l_iConflict > 0) append l_iTemLink "#" l_iConflict
                end
                if (l_itemGuID <> "") append l_itemLink "#" l_itemGuID
                
                writeln channel DEFAULT_FILE_CHANNEL '                      <guid isPermaLink="false">' l_itemLink '</guid>'
                if ((l_pubDate = "") or (l_pubDate = "NOW")) writeln channel DEFAULT_FILE_CHANNEL '                      <pubDate>' (rssdate((now("date")),(now("longtime")))) '</pubDate>'
                else writeln channel DEFAULT_FILE_CHANNEL '                      <pubDate>' l_pubDate '</pubDate>'
                writeln channel DEFAULT_FILE_CHANNEL '                      <category>' l_itemCat '</category>'
                writeln channel DEFAULT_FILE_CHANNEL '               </item>'          
            loop
            
            // Write out file/channel close
            writeln channel DEFAULT_FILE_CHANNEL '       </channel>'
            writeln channel DEFAULT_FILE_CHANNEL '</rss>'  
        close_output channel DEFAULT_FILE_CHANNEL
        
    end_procedure        
    
end_class


// File list - Returns the contents of the DataFlex filelist
//
// In order to retrieve file attributes including the file number the file needs to be opened.
//
// Send message methods:
//    init                  - Initialize the matrix by reading the filelist
//
// Set methods:
//    <na>
//
// Get methods:
//    item_count            - Return the count of filelist items
//    root_name             - Get the root name of the file
//    display_name          - Get the user friendly name of the file
//    system_name           - Get the DataFlex friendly name of the table / file
//    valid                 - Non-zero if the DataFlex FD file exists
//
// Example usage:
//
//    object test is a filelist
//    end_object

//    integer x i 
//    string buf1 buf2 buf3 buf4
//    send init to (test(current_object)) "c:\df32" "filelist.cfg"
//    get item_count of test to x
//    
//    for i from 0 to x
//        get root_name of (test(current_object)) item i to buf1
//        get display_name of (test(current_object)) item i to buf2
//        get system_name of (test(current_object)) item i to buf3
//        get valid of (test(current_object)) item i to buf4
//        showln buf1 " " buf2 " " buf3 " " buf4
//    loop
//

class filelist is a matrix
    procedure construct_object string argc 
            forward send construct_object argc
            property string c_filelistDirectory
            property string c_filelistName
            property integer c_itemCount
    end_procedure
    
    function item_count returns integer
        local integer l_iItems
        get c_itemCount to l_iItems
        function_return l_iItems
    end_function
    
    procedure init string filelistDirectory string filelistName
        local integer l_iFileNumber
        local string l_sRootName l_sUserDisplayName l_sFileName l_sHead l_sUrn
        
        move 0 to l_iFileNumber
        if (filelistName = "") begin
            move "filelist.cfg" to filelistName
        end
        
        set c_filelistDirectory to filelistDirectory
        set c_filelistName to filelistName
        
        direct_input channel DEFAULT_FILE_CHANNEL (filelistDirectory+filelistName)
            read_block l_sHead 256          
            while not (seqeof)          
                //Block of 128 split 41\33\54
                read_block channel DEFAULT_FILE_CHANNEL l_sRootName 41
                read_block channel DEFAULT_FILE_CHANNEL l_sUserDisplayName 33
                read_block channel DEFAULT_FILE_CHANNEL l_sFileName 54

                move filelistDirectory to l_sUrn
                append l_sUrn (trim(cstring(l_sFileName))) ".FD"

                if ((trim(cstring(l_sFileName))) <> "") begin
                    forward set matrix_value item l_iFileNumber item 0 to (trim(cstring(l_sRootName)))
                    forward set matrix_value item l_iFileNumber item 1 to (trim(cstring(l_sUserDisplayName)))
                    forward set matrix_value item l_iFileNumber item 2 to (trim(cstring(l_sFileName)))
                    if (does_exist(l_sUrn) = 1) begin
                        forward set matrix_value item l_iFileNumber item 3 to 1
                    end
                    else begin
                        forward set matrix_value item l_iFileNumber item 3 to 0
                    end
                    increment l_iFileNumber
                end
            loop        
        close_input channel DEFAULT_FILE_CHANNEL
    
        set c_itemCount to l_iFileNumber
    end_procedure
    
    function root_name integer itemx returns integer
        local string l_sBuf
        forward get matrix_value item itemx item 0 to l_sBuf
        function_return l_sBuf
    end_function
        
    function display_name integer itemx returns integer
        local string l_sBuf
        forward get matrix_value item itemx item 1 to l_sBuf
        function_return l_sBuf
    end_function

    function system_name integer itemx returns integer
        local string l_sBuf
        forward get matrix_value item itemx item 2 to l_sBuf
        function_return l_sBuf
    end_function
    
    function valid integer itemx returns integer
        local integer l_iTmp
        forward get matrix_value item itemx item 3 to l_iTmp
        function_return l_iTmp
    end_function
end_class


//Class for reading unicode files when we know they have low ASCII only
//
// Example Usage:
//
//    object test is a UnicodeReader
//    end_object
//
//    local string asciiline
//    local integer error i count channelx
//
//    send open_file to (test(current_object)) 1 "c:\test_unicode.txt"
//    while not (seqeof)
//        get readline of (test(current_object)) 1 to asciiline
//        showln asciiline
//    loop
//    send close_file to (test(current_object)) 1

class UnicodeReader is an array
    procedure construct_object integer argc
        forward send construct_object
        property integer c_iSizeBytes public argc
        property integer c_iBytesOn                
        property integer c_iOpen
        property string c_sPeek
        set c_iOpen to 0
    end_procedure
        
    procedure open_file integer l_iChan string l_sFile
        local integer l_iSizeBytes l_iOpen
        local string l_sTmp l_sBom
        get c_iOpen to l_iOpen
            
        move (trim(l_sFile)) to l_sFile
        if ((l_sFile <> "") and (l_iOpen = 0)) begin    
            move (file_size_bytes(l_sFile)-2) to l_iSizeBytes
            direct_input channel l_iChan l_sFile
                read_block channel l_iChan l_sTmp 1
                if (ascii(l_sTmp) < 254) begin
                    set_channel_position l_iChan to 0
                end
                else begin
                    read_block channel l_iChan l_sTmp 1
                end
            
                set c_iSizeBytes to l_iSizeBytes
                set c_iBytesOn to 0
                set c_iOpen to 1
        end
    end_procedure
        
    procedure close_file integer l_iChan
        local integer l_iOpen
        
        get c_iOpen to l_iOpen
        if (l_iOpen = 0) begin  
            close_input channel l_iChan
        end
        set c_iOpen to 0
    end_procedure
    
    function readline global integer l_iChan returns string
        local string l_sReturn l_sTmp
        local integer l_iBytesOn l_iSizeBytes
        
        move "" to l_sTmp
        move "" to l_sReturn
        get c_iSizeBytes to l_iSizeBytes
        get c_iBytesOn to l_iBytesOn
        
        while ((l_sTmp <> character(13)) and (l_iBytesOn < l_iSizeBytes))
            read_block channel l_iChan l_sTmp 1
            increment l_iBytesOn
            if ((l_sTmp <> character(13)) and (l_sTmp <> character(10)) and (l_sTmp <> character(0))) begin
                move (l_sReturn+l_sTmp) to l_sReturn
            end
        loop
    
        function_return l_sReturn
    end_function
    
end_class

// ListDirectory class - provides a directory listing
//
// Send message methods:
//    delete_data           - Clear the listing
//    list_files            - Read the directory listing into the object
//    sort_files            - Sort the file list on a particular property
//
// Set methods:
//    <na>
//
// Get methods:
//    file_count            - Return the count of files in the list
//    filename              - Get the base name of a file in the list
//    filesize              - Get the size of a file in the list
//    file_created          - Get the created timestamp of the file
//    file_modified         - Get the modification timestamp of the file
//    file_accessed         - Get the last access timestamp of the file
//
// Example usage:
//
//    object test is a ListDirectory
//    end_object
//    
//    integer i x
//    string buf tmp
//    
//    send delete_data to test
//    send list_files to (test(current_object)) "c:\*"
//    get file_count of (test(current_object)) to x
//    send sort_files to test "file_accesed" "ASCENDING"
//    
//    for i from 0 to x
//        get filename of (test(current_object)) item i to tmp
//        get filesize of (test(current_object)) item i to buf
//        append tmp "," buf
//        move (pad(tmp,35)) to tmp
//        get file_created of (test(current_object)) item i to buf
//        append tmp "," buf
//        get file_modified of (test(current_object)) item i to buf
//        append tmp "," buf
//        get file_accessed of (test(current_object)) item i to buf
//        append tmp "," buf
//        showln tmp
//    loop    

class ListDirectory is a matrix
    procedure construct_object integer argc
        forward send construct_object argc
        property integer c_iFiles public argc
    end_procedure
    
    procedure delete_data
        set c_iFiles to 0
        forward send delete_data
    end_procedure
    
    procedure list_files string sPathName
        local string sWin32FindData sLine sLast sComment sFileName sModifiedDate sAccessDate sCreationDate k sLastFile
        local integer l_01iResult iFileSize l_iFiles
        local pointer pT5 pT6
        local handle hFile
        local dword dwFileSizeHigh dwFileSizeLow dwHighDateTime dwLowDateTime

        forward send delete_data
        
        zerotype _WIN32_FIND_DATA to sWin32FindData
        getaddress of sWin32FindData to pT5
        move (trim(sPathName)) to sPathName
        getaddress of sPathName to pT6
        move (FindFirstFile(pT6, pT5)) to hFile
        //if (hFile = -1) showln "Invalid file handle!"

        move -1 to l_iFiles
        repeat 
            // FileName
            getbuff_String from sWin32FindData at WIN32_FIND_DATA.cFileName to sFileName
            if ((sFileName <> ".") and (sFileName <> "..") and (sFileName <> "")) begin
                increment l_iFiles

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

                move (cstring(sFileName)) to sFileName
                forward set matrix_value item l_iFiles item 1 to sFileName
                forward set matrix_value item l_iFiles item 2 to iFileSize
                forward set matrix_value item l_iFiles item 3 to (integer(date(sModifiedDate)))
                forward set matrix_value item l_iFiles item 4 to (integer(date(sAccessDate)))
                forward set matrix_value item l_iFiles item 5 to (integer(date(sCreationDate)))
            end
            zerotype _WIN32_FIND_DATA to sWin32FindData
            move (FindNextFile(hFile, pT5)) to l_01iResult
        until (l_01iResult = 0)
        move (FindClose(hFile)) to l_01iResult

        set c_iFiles to l_iFiles
    end_procedure

    function filename integer itemx returns string
        local string l_sBuf
        move "" to l_sBuf
        forward get matrix_value item itemx item 1 to l_sBuf
        function_return l_sBuf
    end_function 
    
    function filesize integer itemx returns integer
            local integer l_iBuf                
            forward get matrix_value item itemx item 2 to l_iBuf
            function_return l_iBuf
    end_function
    
    function file_modified integer itemx returns date
        local integer l_iBuf                
        forward get matrix_value item itemx item 3 to l_iBuf
        function_return (date(l_iBuf))
    end_function
    
    function file_accessed integer itemx returns date
        local integer l_iBuf                
        forward get matrix_value item itemx item 4 to l_iBuf
        function_return (date(l_iBuf))
    end_function 
    
    function file_created integer itemx returns date
        local integer l_iBuf                
        forward get matrix_value item itemx item 5 to l_iBuf
        function_return (date(l_iBuf))
    end_function
    
    procedure sort_files string sField string sOrder
        local integer l_iSort
        if ((sOrder <> "ASCENDING") and (sOrder <> "DESCENDING")) move "ASCENDING" to sOrder
        move 1 to l_iSort
        if (sField = "filename") move 1 to l_iSort
        if (sField = "filesize") move 2 to l_iSort
        if (sField = "file_modified") move 3 to l_iSort
        if (sField = "file_accessed") move 4 to l_iSort
        if (sField = "file_created") move 5 to l_iSort
        forward send matrix_sort l_iSort sOrder
    end_procedure
    
    function file_count returns integer
        local integer l_iFiles
        get c_iFiles to l_iFiles
        function_return l_iFiles
    end_function
end_class

// ProcessList class - provides a listing of running processes
//
// Experimental; all aspects reading process info appear to fail, it can
// be useful however to check if a particular process pid is still running.
//
// Send message methods:
//    delete_data           - Clear the listing
//    init_processes        - Read the process list table
//
// Set methods:
//    <na>
//
// Get methods:
//    get_process_id        - Return the PID of a particular process
//    process_count         - Return count of processes in the list
//    process_handle        - BROKEN
//
// Example usage:
//
//    object test is an ProcessList
//    end_object
//    
//    integer i x id hx
//    
//    send init_processes to test
//    get process_count of (test(current_object)) to x
//    showln "Processes in list = "  x
//    
//    for i from 0 to x
//        get process_id of (test(current_object)) item i to id
//    loop
//
class ProcessList is an array
    procedure construct_object integer argc
            forward send construct_object
            property integer c_iProcesses public argc
    end_procedure
    
    procedure delete_data
        set c_iProcesses to 0
        forward send delete_data
    end_procedure
    
    procedure init_processes
    local string l_sProcesses l_sStructBytesBack l_sBuf l_sModules
    local integer l_iThrow l_iBytes l_iBytes2 l_iBytesBack l_iBytesBack2 l_i l_j l_iPid l_iMid l_iProcesses
    local pointer l_pProcesses l_pBytesBack l_pBytesBack2 l_pModules
    local handle l_hProcess

    move (1024*10) to l_iBytes  
    zerostring l_iBytes to l_sProcesses
    move 0 to l_iBytesBack
    move 0 to l_iProcesses
    forward send delete_data

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
            move (OpenProcess(PROCESS_QUERY_INFORMATION, 0,l_iPid)) to l_hProcess
            
            // Fails to open the process for more info here unfortunately
            //showln "LERR=" (get_last_error_detail(GetLastError())) " " l_ipid

            move 1024 to l_iBytes2
            zerostring l_iBytes2 to l_sModules
            getAddress of l_sModules to l_pModules
            zerotype _STRUCTBYTESBACK to l_sStructBytesBack
            getaddress of l_sStructBytesBack to l_pBytesBack2

            move (EnumProcessModules(l_hProcess,l_pModules,l_iBytes2,l_pBytesBack2)) to l_iThrow
            getbuff from l_sStructBytesBack at STRUCTBYTESBACK.integer0 to l_iBytesBack2
            
            increment l_iProcesses              
            forward set array_value item (l_i-1) to (string(l_iPid)+"|"+string(l_hProcess))

            if (mod(l_iBytesBack2,4) = 0) begin
                for l_j from 1 to (l_iBytesBack2/4)
                    move (left(l_sModules,4)) to l_sBuf
                    move (mid(l_sModules,(length(l_sModules)-4),5)) to l_sModules
                    getbuff from l_sBuf at PROCESSARRAY.arrayItem to l_iMid
                loop
            end
            move (CloseHandle(l_hProcess)) to l_iThrow
        loop
        
        set c_iTokenOn to 0
        set c_iProcesses to l_iProcesses
    end
    end_procedure

    function process_id integer itemx returns integer
        local string l_sBuf                
        forward get array_value item itemx to l_sBuf
        function_return (integer(left(l_sBuf,pos("|",l_sBuf)-1)))
    end_function 

    // There's not much point to this as we couldn't get the handle because OpenProcess failed.
    function process_handle integer itemx returns integer
        local string l_sBuf                
        forward get array_value item itemx to l_sBuf
        function_return (integer(right(l_sBuf,length(l_sBuf)-pos("|",l_sBuf))))
    end_function 
    
    function process_count returns integer
        local integer l_iProcesses
        get c_iProcesses to l_iProcesses
        function_return l_iProcesses
    end_function
end_class
