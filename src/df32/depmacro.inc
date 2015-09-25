//-------------------------------------------------------------------------
// depmacro.inc
//      This file contains definitions of DataFlex 3.2 Console Mode macro 
//      functions that rely on previously defined DataFlex functions.
//
// This file is to be included in df32func.mk
//
// Copyright (c) 2006-2015, glyn@8kb.co.uk
// 
// df32func/depmacro.inc
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Macro commands
//-------------------------------------------------------------------------

// Create replacement GET_FILE_MOD_TIME - this will replace (leap year) in 
// faulty builtin GET_FILE_MOD_TIME command.
// Console mode clients rely on GetTime (win32.inc).
#COMMAND IH_GET_FILE_MOD_TIME R "TO" R
    if (sysconf(sysconf_os_short_name) = 'WIN32CM') begin
        move (get_time(!1,3)) to g_sGetFileModTime
        move (Left(g_sGetFileModTime,10)) to !3

        #IF (!0 > 3)
            move (mid (g_sGetFileModTime,2,11)) to !4
        #ENDIF

        #IF (!0 > 4)
            move (mid (g_sGetFileModTime,2,14)) to !5
        #ENDIF

        #IF (!0 > 5)
            move (mid (g_sGetFileModTime,2,17)) to !6
        #ENDIF
    end // Console mode 
    else begin // DOS mode
        !A [] $592  !1 !3

        #IF (!0 > 3)
          !A [] $593  !4
        #ENDIF

        #IF (!0 > 4)
          !A [] $594  !5
        #ENDIF

        #IF (!0 > 5)
          !A [] $595  !6
        #ENDIF

        // Check& correct year for both modes
        move (check_date_error(!3)) to !3
    end  // DOS mode

#ENDCOMMAND

// Replace old get_file_mod_time with new
#REPLACE GET_FILE_MOD_TIME IH_GET_FILE_MOD_TIME
