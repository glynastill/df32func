//-------------------------------------------------------------------------
// tap.inc
//      This file contains some DataFlex 3.2 Console Mode classes
//      to provide some test anything protocol functionality.
//    See: http://testanything.org/
//
// This file is to be included in df32func.mk
//
// Copyright (c) 2006-2015, glyn@8kb.co.uk
// 
// df32func/tap.inc
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Classes
//-------------------------------------------------------------------------

// TAP class - impliments the vet basic of the Test Anything Protocol  
//
// Get methods:
//    plan                           - Gets the "plan" or expected number of tests
//    tests                          - Gets the number of tests executed so far
//
// Set methods: (All of the following methods are intended to be private)
//    plan <number of tests>           - Sets the "plan" or expected number of tests
//
// Send message methods:
//    ok <boolean>                   - Fundamental test, to check binary outcome of an expression
//    is <v1> <v2> <msg>               - Test values are equivaent
//    isnt <v1> <v2> <msg>             - Test values are not equivaent
//    cmp_ok <val1> <val2> <op> <msg>  - Test values are not equivaent
//    finish                           - Complete the set of tests (also alias "done_testing")
//
// Notes
//    If a plan has been set, and the program aborts without calling finish, finish is called
//    automatically, and results will be output.  Piping test output to a file or creating a
//    "wrapper" around the program with a simple "chain wait" allows test results to always be
//    seen.
//
// Example usage:  
//
//    object myTest is a TAP
//    end_object
//
//    set plan of myTest to 8
//
//    send ok to myTest (1=1) "One is equal to one"
//    send ok to myTest (2=1) "Two is equal to one"
//    send ok to myTest (3=3) "Three is equal to three"
//    send is to myTest "pie" 100 "Pie is numeric"
//    send isnt to myTest "pie" "pie" "Both should be pie"
//    send cmp_ok to myTest "pie" "pie" "=" "Pie equals pie"
//    send cmp_ok to myTest 1 2 "=" "One equals two"
//    send cmp_ok to myTest 1 2 ">" "One is greater than two"
//    send cmp_ok to myTest "pankcake" "cake" "~~" "Pankace contains cake"
//
//    send finish to myTest
//
class TAP is an array
    procedure construct_object integer argc
        forward send construct_object
        property integer c_iPlan public argc        
        property integer c_iTest
        set c_iPlan to -1
        set c_iTest to 0        
    end_procedure
    
    procedure set plan integer argv
        set c_iPlan to argv
    end_procedure
    
    function plan
        local integer l_iPlan
        get c_iPlan to l_iPlan
        function_return l_iPlan
    end_procedure    

    function tests
        local integer l_iTest
        get c_iTest to l_iTest
        function_return l_iTest
    end_procedure 
    
    procedure is string argv string argv2 string argv3
        local integer l_iTest
        local string l_sTestResult
        
        get c_iTest to l_iTest
        increment l_iTest
               
        move (ternary((argv = argv2),"1","0")+string(l_iTest)+" - "+argv3) to l_sTestResult
                
        forward set array_value item l_iTest to l_sTestResult 
        set c_iTest to l_iTest     
    end_procedure
    
    procedure isnt string argv string argv2 string argv3
        local integer l_iTest
        local string l_sTestResult
        
        get c_iTest to l_iTest
        increment l_iTest
               
        move (ternary((argv <> argv2),"1","0")+string(l_iTest)+" - "+argv3) to l_sTestResult
                
        forward set array_value item l_iTest to l_sTestResult 
        set c_iTest to l_iTest     
    end_procedure 
    
    procedure cmp_ok string argv string argv2 string argv3 string argv4
        local integer l_iTest
        local string l_sTestResult
        
        get c_iTest to l_iTest
        increment l_iTest

        case begin
            case ((argv3 = "=") or (argv3 = "eq") or (argv3 = "==")) move (ternary((argv = argv2),"1","0")+string(l_iTest)+" - "+argv4) to l_sTestResult
            case break
            case ((argv3 = "<>") or (argv3 = "ne") or (argv3 = "!=") or (argv3 = "!")) move (ternary((argv <> argv2),"1","0")+string(l_iTest)+" - "+argv4) to l_sTestResult
            case break
            case ((argv3 = ">") or (argv3 = "gt")) move (ternary((argv > argv2),"1","0")+string(l_iTest)+" - "+argv4) to l_sTestResult
            case break          
            case ((argv3 = ">=") or (argv3 = "ge")) move (ternary((argv >= argv2),"1","0")+string(l_iTest)+" - "+argv4) to l_sTestResult
            case break          
            case ((argv3 = "<") or (argv3 = "lt")) move (ternary((argv < argv2),"1","0")+string(l_iTest)+" - "+argv4) to l_sTestResult
            case break                      
            case ((argv3 = "<=") or (argv3 = "le")) move (ternary((argv <= argv2),"1","0")+string(l_iTest)+" - "+argv4) to l_sTestResult
            case break                                  
            case ((argv3 = "~") or (argv3 = "~~") or (argv3 = "contains")) move (ternary(((argv contains argv2) > 0),"1","0")+string(l_iTest)+" - "+argv4) to l_sTestResult
            case break  
            case else custom_error ERROR_CODE_COMPARISON_OPERATOR$ ERROR_MSG_COMPARISON_OPERATOR argv3
        case end
        
        forward set array_value item l_iTest to l_sTestResult 
        set c_iTest to l_iTest     
    end_procedure      
    
    procedure ok integer argv string argv2
        local integer l_iTest
        local string l_sTestResult
        
        get c_iTest to l_iTest
        increment l_iTest
        
        if ((argv <= 1) and (argv >= 0)) begin
            move (string(argv)+string(l_iTest)+" - "+argv2) to l_sTestResult
        end
        else begin
            custom_error ERROR_CODE_INVALID_BOOLEAN$ ERROR_MSG_INVALID_BOOLEAN ERROR_DETAIL_INVALID_BOOLEAN argv
        end
                
        forward set array_value item l_iTest to l_sTestResult 
        set c_iTest to l_iTest      
    end_procedure
    
    procedure finish
        local integer l_iPlan l_iTest l_i l_iStatus
        local string l_sBuf l_sMsg
        
        get c_iPlan to l_iPlan
        get c_iTest to l_iTest

        
        if (l_iPlan <> -1) showln "1.." l_iPlan
        for l_i from 1 to l_iTest
            forward get string_value item l_i to l_sBuf
            move (left(l_sBuf,1)) to l_iStatus
            move (right(l_sBuf,length(l_sBuf)-1)) to l_sMsg
            
            if (l_iStatus = 1) begin
                showln "ok " l_sMsg
            end
            else begin
                showln "not ok " l_sMsg
            end
        loop        
        
        if (l_iPlan <> -1) begin                
            set c_iPlan to -1
        end
        else showln "1.." l_iTest
        
        forward send delete_data
        set c_iTest to 0
    end_procedure
    
    procedure done_testing
        send finish
    end_procedure
    
    procedure destroy_object
        local integer l_iPlan
        get c_iPlan to l_iPlan
        if (l_iPlan <> -1) send finish
        //forward send destroy_object
        send destroy_object
    end_procedure
    
end_class

// TAP_harness class - overrides finish method of TAP class to provide a 
//                     basic harness
//
// Inherits all methods from TAP
//
// Set methods: (extra methods)
//    timing_on                          - Turns on test timing
//    timing_off                         - Turns off test timing
//    notices_on                         - Turns on test notices
//    notices_off                        - Turns off test notices
//
class TAP_harness is a TAP
    procedure construct_object integer argc
        forward send construct_object argc
        property integer c_iNotices
        property integer c_iTiming
        property integer c_nStart
        set c_iNotices to 0
        set c_iTiming to 0      
    end_procedure

    function get_timer_seconds returns number
        local date l_dDate
        local number l_nHr l_nMin l_nSec
        
        sysdate l_dDate l_nHr l_nMin l_nSec
        function_return ((integer(l_dDate)-integer(date("01/01/1970"))*86400)+(((l_nHr*60)+l_nMin)*60)+l_nSec)
    end_procedure
    
    procedure notices_on 
        set c_iNotices to 1
    end_procedure
    
    procedure notices_off
        set c_iNotices to 0
    end_procedure   
    
    procedure timing_on 
        local number l_iSecs
        set c_iTiming to 1
        get get_timer_seconds to l_iSecs
        set c_nStart to l_iSecs
    end_procedure

    procedure timing_off
        set c_iTiming to 0
    end_procedure
    
    procedure finish
        local integer l_iPlan l_iTest l_i l_iStatus l_iFailed l_iNotices l_iTiming
        local string l_sBuf l_sMsg l_sFailed
        local number l_nStart l_nSecs
        
        forward get c_iPlan to l_iPlan
        forward get c_iTest to l_iTest
        get c_iNotices to l_iNotices
        get c_iTiming to l_iTiming
        
        move 0 to l_iFailed
        move "" to l_sFailed
        
        if (l_iPlan <> -1) showln "1.." l_iPlan
        for l_i from 1 to l_iTest
            forward get string_value item l_i to l_sBuf
            move (left(l_sBuf,1)) to l_iStatus
            move (right(l_sBuf,length(l_sBuf)-1)) to l_sMsg
            
            if (l_iStatus = 1) begin
                showln "ok " l_sMsg
            end
            else begin
                showln "not ok " l_sMsg
                if (l_iFailed > 0) append l_sFailed ", "
                append l_sFailed l_i
                increment l_iFailed
            end
        loop        
        
        if (l_iPlan <> -1) begin
            if (l_iNotices) begin
                if (l_iTest < l_iPlan);
                    showln "Notice: Only ran " l_iTest " of " l_iPlan " tests"
                if (l_iTest > l_iPlan);
                    showln "Notice: Ran " l_iTest " tests, but only expected " l_iPlan
            end
                        
            if (l_iFailed > 0) begin
                showln "FAILED test" (ternary((l_iFailed > 1),"s "," ")) l_sFailed
                showln "Failed " l_iFailed "/" l_iTest " tests, " (decround(1-(number(l_iFailed)/number(l_iTest))*100,2)) "% ok"
            end
                
            forward set c_iPlan to -1
        end
        else showln "1.." l_iTest
        
        if (l_iTiming) begin
            get get_timer_seconds to l_nSecs
            get c_nStart to l_nStart
            showln "Timing: " (l_nSecs-l_nStart) " seconds"
        end
    end_procedure

end_class
