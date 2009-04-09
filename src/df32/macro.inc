//-------------------------------------------------------------------------
// macro.inc
//      This file contains definitions of DataFlex 3.2 Console Mode macro functions
//      to provide some extended command functionality.
//
// This file is to be included in df32func.mk
//
// Copyright (c) 2006-2009, glyn@8kb.co.uk
// 
// df32func/macro.inc
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Macro commands
//-------------------------------------------------------------------------


// Assertions
// ASSERT <true/false evaluation> <assert message> -- provides a similar functionality to c assert.  
// 
//     if enable_dfassert is defined then evaluations are run and errors are raised
//     if enable_dfassert is undefined then assertions are effectively removed at compile time
//    
//     A failed assertion will raise an error
//
// Example usage:   
//    
//     Define enable_dfassert
//     ASSERT (1=2) "TEST ASSERT"
//
#COMMAND ASSERT R
    #IFDEF enable_dfassert
        #IF (!0=2)
            #IFTYPE !1 "I"
                if (!1 = 0);
                    error 999999 ("RAISED ASSERTION: '"+string(!2)+"'")
            #ENDIF
        #ENDIF
    #ENDIF
#ENDCOMMAND

// Custom errors
//
// Used to produce multiline error messages as the error command is 
// limited to 40 chars (the original length of ERROR_DESCR in FLEXERRS.DAT)
//
// Will replace "??" in any message with a variable
//
// Example usage:
//
//     custom_error <error_code> <error_message>
//     custom_error <error_code> <error_message> <error_message replacement>
//     custom_error <error_code> <error_message> <error_detail> <error_detail replacement>
//
#COMMAND CUSTOM_ERROR R
    #IF (!0>1)
        #IFTYPE !1 "I"
            #IFTYPE !2 "S"
                #IF (!0=2)
                    error !1 !2
                #ELSE
                    #IF (!0=3)
                        error !1 (replace("??", !2, string(!3)))
                    #ELSE
                        #IF (!0=4)
                            error !1 !2
                            #IFTYPE !3 "S"
                                error !1 ("Detail: "+(replace("??", !3, string(!4))))
                            #ENDIF
                        #ENDIF
                    #ENDIF
                #ENDIF
            #ENDIF
        #ENDIF
    #ENDIF        
#ENDCOMMAND

// Ternary command (an operator would be nice)
// Behaves similar to perl/c ternary operators E.g. "(evaluation) ? true_var : false_var"
//
// Example usage:
//
//     ternary mytable.myboolean "YES" "NO" my_string
//     ternary (1=1) "YES" "NO" my_string
//
// Note that this command form functions just like perl/c and if something like:
// "(evaluation) ? true_evaluation : false_evaluation" is written then only the
// secondary OR tertiary evaluation will execute dependant on the result of 
// the primary evaluation.
//
// There is also a "ternary" function (math.inc) which can be used in place of an operator, 
// however beware that the function works differently; both the secondary AND tertiary
// evaluations happen before primary evaluation is tested, rather than being exclusive
// (as they should really be) this is a limitation of passing via the function. Thus 
// the function is only really useful for variables.
//
#COMMAND TERNARY R
    #IF (!0>3)
        #IFTYPE !1 "I"
            if (!1 = 1) move !2 to !4
            else move !3 to !4
        #ELSE
            #IFTYPE !1 "S"
                if ((lowercase(!1) eq 't') or (lowercase(!1) eq 'true')) move !2 to !4
                else move !3 to !4
            #ELSE
                move !3 to !4
            #ENDIF
        #ENDIF
    #ENDIF
#ENDCOMMAND
