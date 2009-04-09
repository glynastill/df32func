//-------------------------------------------------------------------------
// tstamp.inc
//      This file contains some DataFlex 3.2 Console Mode functions
//      to provide extended timestamp and timezone manipulation capabilities.
//      Depends on functions in both win32.inc and string.inc/
//
// This file is to be included in df32func.mk
//
// Copyright (c) 2006-2009, glyn@8kb.co.uk
// 
// df32func/tstamp.inc
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Global tokenizer used for splitting timezon data
//-------------------------------------------------------------------------
object tzTok is a StringTokenizer
end_object

//-------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------

// Convert a textual timestamp to a posix number
// Also see posixtime and posixtime_reverse in date.inc
function timestemp_to_posix global string inTs returns number
        local number l_posix l_hr l_min l_sec l_msec
        local date l_date

        // leap seconds not coded        
        if ((length(inTs) < 10) or (mid(inTs, 1, 3) <> "/") or (mid(inTs, 1, 6)  <> "/") or (mid(inTs, 1, 11)  <> " ")) begin
            custom_error ERROR_CODE_INVALID_TIMESTAMP$ ERROR_MSG_INVALID_TIMESTAMP ERROR_DETAIL_INVALID_TIMESTAMP "DD/MM/YYYY"
        end
        if (((length(inTs) >= 16) and (mid(inTs, 1, 14)  <> ":")) or ((length(inTs) >= 19) and (mid(inTs, 1, 17)  <> ":"));
            or ((length(inTs) >= 21) and (mid(inTs, 1, 20)  <> "."))) begin
            custom_error ERROR_CODE_INVALID_TIMESTAMP$ ERROR_MSG_INVALID_TIMESTAMP ERROR_DETAIL_INVALID_TIMESTAMP "DD/MM/YYYY HH:mm:SS.mss"
        end
        
        move (left(inTs,(pos(" ",inTs)-1))) to l_date
        if (length(inTs) >= 13) move (mid(inTs, 2, 12)) to l_hr
        else move 0 to l_hr
        if (length(inTs) >= 16) move (mid(inTs, 2, 15)) to l_min
        else move 0 to l_min
        if (length(inTs) >= 19) move (mid(inTs, 2, 18)) to l_sec
        else move 0 to l_sec
        if (length(inTs) >= 21) move (mid(inTs, 3, 21)) to l_msec
        else move 0 to l_msec
        
        if ((l_hr > 23) or (l_min > 59) or (l_sec > 59) or (l_msec > 999);
            or (l_hr < 0) or (l_min < 0) or (l_sec < 0) or (l_msec < 0)) begin
            custom_error ERROR_CODE_INVALID_TIMESTAMP$ ERROR_MSG_INVALID_TIMESTAMP
        end
        
        calc (((integer(l_date))-(integer(date("01/01/1970"))))*86400) to l_posix
        calc (((((l_hr*60)+l_min)*60)+l_sec)+(l_msec/1000)+l_posix) to l_posix
        
        function_return l_posix
end_function

// Convert a posix number to a textual timestamp
function posix_to_timestamp global number argv returns string
        local date l_date
        local number l_subt 
        local integer l_hr l_min l_sec l_msec
        local string l_posix_reverse
        
        // leap seconds not coded
        calc ((argv/86400)+(integer(date("01/01/1970")))) to l_date
        calc (argv-(((integer(l_date))-(integer(date("01/01/1970"))))*86400)) to l_subt
        calc (l_subt/3600) to l_hr
        calc ((l_subt-(l_hr*3600))/60) to l_min
        calc (l_subt-(((l_hr*60)+l_min)*60)) to l_sec
        calc ((l_subt-((((l_hr*60)+l_min)*60)+l_sec))*1000) to l_msec
        
        if ((l_hr > 23) or (l_min > 59) or (l_sec > 59) or (l_msec > 999);
            or (l_hr < 0) or (l_min < 0) or (l_sec < 0) or (l_msec < 0)) begin
            custom_error ERROR_CODE_INVALID_POSIX_NUMBER$ ERROR_MSG_INVALID_POSIX_NUMBER argv
        end        
                
    move "" to l_posix_reverse
    move (string(l_date)+" "+zeropad(l_hr,2)+":"+zeropad(l_min,2)+":"+zeropad(l_sec,2)+"."+zeropad(l_msec,3)) to l_posix_reverse
           
        function_return l_posix_reverse
end_function

// Adjust supplied timestamp by supplied milliseconds
function timestamp_adjust global string inTs number inMSeconds returns string
    local string retTimestamp
    local number nPosix
    
    move (timestemp_to_posix(inTs)) to nPosix
    move (posix_to_timestamp(nPosix+(inMSeconds/1000))) to retTimestamp
    
    function_return retTimestamp
end_function

// Limited as we'd need to know the actual timestamp referred to to do months, years etc.
// Supports millisecond, second, minute, hour, day, week
function interval_to_posix global string argv returns number
    local number l_value l_return
    local integer l_i
    local string l_unit l_interval  l_numerics
    
    move (trim(argv)) to l_interval
    move 0 to l_return
    
    if (length(l_interval) <> 0) begin
        move 1 to l_i
        while (l_i < length(l_interval))
            move 0 to l_value
            move "" to l_numerics
            move "" to l_unit
            while ((ascii(mid(l_interval, 1, l_i)) > 47) and (ascii(mid(l_interval, 1, l_i)) < 58))
                append l_numerics (mid(l_interval, 1, l_i))
                increment l_i
            loop
            while (ascii(mid(l_interval, 1, l_i)) = 32)
                increment l_i
            loop

            while ((ascii(mid(l_interval, 1, l_i)) > 64) and (ascii(mid(l_interval, 1, l_i)) < 91);
                or (ascii(mid(l_interval, 1, l_i)) > 96) and (ascii(mid(l_interval, 1, l_i)) < 123))
                append  l_unit (mid(l_interval, 1, l_i))
                increment l_i
            loop

            move (lowercase(trim(l_unit))) to l_unit        
            case begin
                case ((mid(l_unit,11,1) = "millisecond") or (mid(l_unit,2,1) = "ms")) move (l_numerics/number(1000)) to l_value
                case break
                case ((mid(l_unit,6,1) = "second") or (mid(l_unit,1,1) = "s")) move l_numerics to l_value
                case break
                case ((mid(l_unit,6,1) = "minute") or (mid(l_unit,1,1) = "m")) move (l_numerics*60) to l_value
                case break          
                case ((mid(l_unit,4,1) = "hour") or (mid(l_unit,1,1) = "h")) move (l_numerics*3600) to l_value                  
                case break                      
                case ((mid(l_unit,3,1) = "day") or (mid(l_unit,1,1) = "d")) move (l_numerics*86400) to l_value
                case break                                  
                case ((mid(l_unit,4,1) = "week") or (mid(l_unit,1,1) = "w")) move (l_numerics*604800) to l_value
                case break                                  
            case end

            move (l_return+l_value) to l_return
        loop
    end
    
    function_return l_return
end_function

// Adjust supplied timestamp by supplied interval
// Supports microsecond, millisecond, second, minute, hour, day, week, month, year, decade, century, millennium
// Negative sinage is indicated by "ago", words without preceeding units are treated as noise. E.g:
// timestamp_adjust_interval("28/02/2009 09:00:00.000", "1century 2years 1month 1day 1hour 1minute 1second and 500 milliseconds ago")
// Note that actual timestamps will be bound by posix and dataflex timestamp implementations.
function timestamp_adjust_interval global string inTs string inInterval returns string
    local string retTimestamp tmpTimestamp l_unit l_interval  l_numerics
    local number nPosix l_value l_return l_year l_mon l_day
    local integer l_i l_iThrow l_iSign
    
    if ((length(inTs) < 10) or (mid(inTs, 1, 3) <> "/") or (mid(inTs, 1, 6)  <> "/")) begin
        custom_error ERROR_CODE_INVALID_TIMESTAMP$ ERROR_MSG_INVALID_TIMESTAMP ERROR_DETAIL_INVALID_TIMESTAMP "DD/MM/YYYY"
    end
    
    move (mid(inTs, 2, 1)) to l_day
    move (mid(inTs, 2, 4)) to l_mon
    move (mid(inTs, 4, 7)) to l_year
    
    if (lowercase(inInterval) contains "ago");
        move -1 to l_iSign
    else;
        move 1 to l_iSign
    
    move (trim(inInterval)) to l_interval
    move 0 to l_return
    
    if (length(l_interval) <> 0) begin
        move 1 to l_i
        while (l_i < length(l_interval))
            move 0 to l_value
            move "" to l_numerics
            move "" to l_unit
            while ((ascii(mid(l_interval, 1, l_i)) > 47) and (ascii(mid(l_interval, 1, l_i)) < 58))
                append l_numerics (mid(l_interval, 1, l_i))
                increment l_i
            loop
            while (ascii(mid(l_interval, 1, l_i)) = 32)
                increment l_i
            loop
            while ((ascii(mid(l_interval, 1, l_i)) > 64) and (ascii(mid(l_interval, 1, l_i)) < 91);
                or (ascii(mid(l_interval, 1, l_i)) > 96) and (ascii(mid(l_interval, 1, l_i)) < 123))
                append  l_unit (mid(l_interval, 1, l_i))
                increment l_i
            loop
            move (lowercase(trim(l_unit))) to l_unit        
            case begin
                case ((mid(l_unit,10,1) = "millennium") or (mid(l_unit,6,1) = "millen")) move (l_year+(l_numerics*1000*l_iSign)) to l_year
                case break
                case ((mid(l_unit,7,1) = "century") or (mid(l_unit,1,1) = "c")) move (l_year+(l_numerics*100*l_iSign)) to l_year
                case break
                case ((mid(l_unit,6,1) = "decade") or (mid(l_unit,3,1) = "dec")) move (l_year+(l_numerics*10*l_iSign)) to l_year
                case break
                case ((mid(l_unit,4,1) = "year") or (mid(l_unit,1,1) = "y")) move (l_year+(l_numerics*l_iSign)) to l_year
                case break
                case ((mid(l_unit,5,1) = "month") or (mid(l_unit,3,1) = "mon")) move (l_mon+(l_numerics*l_iSign)) to l_mon
                case break
                case ((mid(l_unit,11,1) = "millisecond") or (mid(l_unit,2,1) = "ms")) move (l_numerics/number(1000)) to l_value
                case break
                case ((mid(l_unit,6,1) = "second") or (mid(l_unit,1,1) = "s")) move l_numerics to l_value
                case break
                case ((mid(l_unit,6,1) = "minute") or (mid(l_unit,3,1) = "min")) move (l_numerics*60) to l_value
                case break          
                case ((mid(l_unit,4,1) = "hour") or (mid(l_unit,1,1) = "h")) move (l_numerics*3600) to l_value                  
                case break                      
                case ((mid(l_unit,3,1) = "day") or (mid(l_unit,1,1) = "d")) move (l_numerics*86400) to l_value
                case break                                  
                case ((mid(l_unit,4,1) = "week") or (mid(l_unit,1,1) = "w")) move (l_numerics*604800) to l_value
                case break                                  
            case end
            move (l_return+l_value) to l_return
        loop
    end 
    
    move (timestemp_to_posix(zeropad(l_day,2)+"/"+zeropad(l_mon,2)+"/"+zeropad(l_year,4)+mid(inTs,length(inTs)-10,11))) to nPosix
    move (posix_to_timestamp(nPosix+(l_return*l_iSign))) to retTimestamp
    
    function_return retTimestamp
end_function

// This takes the day of the week and month occourance values from a SYSTEMTIME 
// in a _TIME_ZONE_INFORMATION and works out the correct day of the month
// See description under "DaylightDate" here https://msdn.microsoft.com/en-us/library/ms724253.aspx
function get_day_of_month_for_daylight_savings global integer inYear integer inMonth integer inDayOfWeek integer inDay returns integer
    local integer l_dom l_i
    local date l_date
    
    if (inDayOfWeek = 0) move 7 to inDayOfWeek
    
    move ("01/"+zeropad(inMonth,2)+"/"+zeropad(inYear,4)) to l_date
    
    while (get_day_score(l_date) <> inDayOfWeek)
        increment l_date
    loop
    
    if (integer(mid(l_date, 2, 4)) = inMonth);
        move (mid(l_date, 2, 1)) to l_dom           
    
    for l_i from 1 to (inDay-1)
        calc (l_date+7) to l_date
        
    if (integer(mid(l_date, 2, 4)) = inMonth);
        move (mid(l_date, 2, 1)) to l_dom           
    loop
    
    function_return l_dom
end_function

// Get local system time and starts to pull values to adjust to UTC (needs finishing)
function systemtime_utc global returns string
    local string sSystemTime sTimeZoneInformation sStdName sDlName sFormattedTime sFormattedDate sTs
    local integer iSuccess iBias iStdBias iDlBias iBiasNow iLenCcTime iLenCcDate iDataLength
    local pointer lpSystemTime lpTimeZoneInformation lpsFormattedTime lpsFormattedDate

    // Get the current system local time
    zerotype _SYSTEMTIME to sSystemTime
    getaddress of sSystemTime to lpSystemTime

    move (GetSystemTime(lpSystemTime)) to iSuccess

    if (iSuccess <> 0) begin
        // Get the current system timezone information
        zerotype _TIME_ZONE_INFORMATION to sTimeZoneInformation
        getaddress of sTimeZoneInformation to lpTimeZoneInformation

        move (GetTimeZoneInformation(lpTimeZoneInformation)) to iSuccess
            
        if (iSuccess = TIME_ZONE_ID_INVALID) begin
            custom_error ERROR_CODE_INVALID_SYSTEM_TIMEZONE$ ERROR_MSG_INVALID_SYSTEM_TIMEZONE
        end
        else begin
        getbuff from sTimeZoneInformation at TIME_ZONE_INFORMATION.Bias to iBias
        move (cstring(to_ascii(mid(sTimeZoneInformation, 64, 5)))) to sStdName //getbuff from sTimeZoneInformation at TIME_ZONE_INFORMATION.StandardName to sStdName //UTF-16/wchar                      
        getbuff from sTimeZoneInformation at TIME_ZONE_INFORMATION.StandardBias to iStdBias
        move (cstring(to_ascii(mid(sTimeZoneInformation, 64, 89)))) to sDlName //getbuff from sTimeZoneInformation at TIME_ZONE_INFORMATION.DaylightName to sDlName //UTF-16/wcharsDlName 
        getbuff from sTimeZoneInformation at TIME_ZONE_INFORMATION.DaylightBias to iDlBias

        zerostring 255 to sFormattedTime
        getaddress of sFormattedTime to lpsFormattedTime
        move (length(sFormattedTime)) to iLenCcTime
        move (GetTimeFormat("LOCALE_USER_DEFAULT", 0, lpSystemTime, 0, lpsFormattedTime, iLenCcTime)) to iDataLength                

        zerostring 255 To sFormattedDate
        getaddress of sFormattedDate To lpsFormattedDate
        move (length(sFormattedDate)) to iLenCcDate
        move (GetDateFormat("LOCALE_USER_DEFAULT", 0, lpSystemTime, 0, lpsFormattedDate, iLenCcDate)) to iDataLength

        // Work out bias and apply to get UTC
        if (iSuccess = TIME_ZONE_ID_DAYLIGHT) begin
            move (iBias+iDlBias) to iBiasNow
        end
        else begin
            move (iBias+iStdBias) to iBiasNow
        end
        
        move (cstring(sFormattedDate) * cstring(sFormattedTime)) to sTs
        move (timestamp_adjust(sTs, iBiasNow*60000)) to sTs
    end
    end

    function_return sTs
end_function

// Returns UTC BIAS in minutes
function get_bias global string inDestTz integer inYear integer inMon integer inDay integer inHr integer inMin integer inSec integer inIsUTC returns integer
    local string sTimeZone sResult sBuf sDaylightDate sDaylightTime sStandardDate sStandardTime StdTs DltTs inTs
    local integer iSuccess iBiasNow iBias iStdBias iDaylightBias iIsDaylightSaving iThrow iTmp
    local integer iEYear iEMon iEDay iEDOW iEHr iEMin iESec iBYear iBMon iBDay iBDOW iBHr iBMin iBSec iEDOM iBDOM
    local number nTO nBO nEO StdPo DltPo inPo
    local pointer lpTimeZone lpResult

    ASSERT ((inIsUTC >= 0) and (inIsUTC <= 1)) "Unsupported mode, inIsUTC must be either 1 or 0"
            
    move 0 to iIsDaylightSaving
    move (trim(inDestTz)) to sTimeZone    
    
    if (sTimeZone <> "") begin
        if (show_debug_lines = 1);
            showln "Timezone:          " sTimeZone
    
        getaddress of sTimeZone to lpTimeZone
                
        zerostring 128 to sResult
        getaddress of sResult to lpResult
                
        move (GetTzi(lpTimeZone, lpResult)) to iSuccess
                    
        if (iSuccess = -1) begin
        
            send set_string to (tzTok(current_object)) sResult ","
            get token_value of (tzTok(current_object)) item 0 to iBias
            get token_value of (tzTok(current_object)) item 1 to iStdBias
            get token_value of (tzTok(current_object)) item 2 to iDaylightBias
            get token_value of (tzTok(current_object)) item 3 to sStandardDate
            get token_value of (tzTok(current_object)) item 4 to sStandardTime
            get token_value of (tzTok(current_object)) item 5 to sDaylightDate
            get token_value of (tzTok(current_object)) item 6 to sDaylightTime
            
            // Once all of our data has been retrieved we need to determine daylight saving time
            //showln  sDaylightDate " " sDaylightTime " = " sStandardDate " " sStandardTime

            send set_string to (tzTok(current_object)) sDaylightDate "/"            
            get token_value of (tzTok(current_object)) item 0 to iBYear
            get token_value of (tzTok(current_object)) item 1 to iBMon
            get token_value of (tzTok(current_object)) item 2 to iBDay
            get token_value of (tzTok(current_object)) item 3 to iBDOW
            
            send set_string to (tzTok(current_object)) sStandardDate "/"            
            get token_value of (tzTok(current_object)) item 0 to iEYear
            get token_value of (tzTok(current_object)) item 1 to iEMon
            get token_value of (tzTok(current_object)) item 2 to iEDay
            get token_value of (tzTok(current_object)) item 3 to iEDOW
            
            ASSERT ((iBMon <> 0) and (iEMon <> 0)) "Daylight saving not supported"
            
            if (show_debug_lines = 1) begin
                if ((iBMon = 0) or (iEMon = 0));
                    showln "Supports Dls:      no" 
                else;
                    showln "Supports Dls:      yes" 
            end         
                                            
            if ((iBMon <> 0) and (iEMon <> 0)) begin
                if ((iBYear = 0) or (iBYear = inYear)) begin
                
                    move (get_day_of_month_for_daylight_savings((iBYear max inYear),iBMon,iBDOW,iBday)) to iBDOM
                    move (get_day_of_month_for_daylight_savings((iEYear max inYear),iEMon,iEDOW,iEday)) to iEDOM              
                    
                    send set_string to (tzTok(current_object)) sDaylightTime ":"
                    get token_value of (tzTok(current_object)) item 0 to iBHr
                    get token_value of (tzTok(current_object)) item 1 to iBMin
                    get token_value of (tzTok(current_object)) item 2 to iBSec

                    send set_string to (tzTok(current_object)) sStandardTime ":"
                    get token_value of (tzTok(current_object)) item 0 to iEHr
                    get token_value of (tzTok(current_object)) item 1 to iEMin
                    get token_value of (tzTok(current_object)) item 2 to iESec

                    move (zeropad(iBDOM,2)+"/"+zeropad(iBMon,2)+"/"+string(iEYear max inYear)+" "+zeropad(iBHr,2)+":"+zeropad(iBMin,2)+":"+zeropad(iBSec,2)) to DltTs
                    move (zeropad(iEDOM,2)+"/"+zeropad(iEMon,2)+"/"+string(iEYear max inYear)+" "+zeropad(iEHr,2)+":"+zeropad(iEMin,2)+":"+zeropad(iESec,2)) to StdTs
                    move (zeropad(inDay,2)+"/"+zeropad(inMon,2)+"/"+string(inYear)+" "+zeropad(inHr,2)+":"+zeropad(inMin,2)+":"+zeropad(inSec,2)) to inTs
                    
                    move (timestemp_to_posix(DltTs)) to DltPo
                    move (timestemp_to_posix(StdTs)) to StdPo
                    move (timestemp_to_posix(inTs)+(iBias*-60*inIsUTC)) to inPo
                    
                    if (show_debug_lines = 1) begin
                        showln "InTimestamp:       " inTs
                        showln "CmpTimestamp:      " (posix_to_timestamp(inPo))
                        showln "DaylightTimestamp: " DltTs " Offset="  (iBias+iDaylightBias)                    
                        showln "StandardTimestamp: " StdTs " Offset="  (iBias+iStdBias)
                    end
                    
                    if (DltPo < StdPo) begin
                        if ((inPo >= DltPo) and (inPo < StdPo)) move 1 to iIsDaylightSaving
                    end
                    else begin
                        if not ((inPo >= StdPo) and (inPo < DltPo)) move 1 to iIsDaylightSaving
                    end
                end
                
            end
            
            // If the clocks go forward at 1.00am then the 1.00am-1.59am doesn't exist in localtime; 
            // conversins back and forth between nonexistent times will look wrong, but should never occour
            // If the clocks go back at 1.00am then 1.00am effectively occours twice 
            // More info: http://www.timeanddate.com/time/dst/transition.html
            
            // Now we can figure out the actual bias
            if (iIsDaylightSaving = 1) begin
                move (iBias+iDaylightBias) to iBiasNow
                if (show_debug_lines = 1) begin
                    showln "Zone currently in daylight saving"
                end
            end
            else begin
                move (iBias+iStdBias) to iBiasNow
            end
            if (show_debug_lines = 1) begin
                showln "Bias:              " iBiasNow
            end
            
            send destroy_object to tzTok
        end
        else begin
            custom_error ERROR_CODE_INVALID_TIMESTAMP$ ERROR_MSG_INVALID_TIMESTAMP
        end        
    end

    function_return iBiasNow
end_function

// Returns UTC time converted from the current time in the passed timezone 
function get_utc_time_from_timezone_time global string inDestTz string inTs returns string
    local integer l_bias
    local string l_ts
    local integer inGmtYear inGmtMon inGmtDay inGmtHr inGmtMin inGmtSec 

    if ((length(inTs) < 10) or (mid(inTs, 1, 3) <> "/") or (mid(inTs, 1, 6)  <> "/") or (mid(inTs, 1, 11)  <> " ")) begin
        custom_error ERROR_CODE_INVALID_TIMESTAMP$ ERROR_MSG_INVALID_TIMESTAMP ERROR_DETAIL_INVALID_TIMESTAMP "DD/MM/YYYY"
    end
    if (((length(inTs) >= 16) and (mid(inTs, 1, 14)  <> ":")) or ((length(inTs) >= 19) and (mid(inTs, 1, 17)  <> ":"));
        or ((length(inTs) >= 21) and (mid(inTs, 1, 20)  <> "."))) begin
        custom_error ERROR_CODE_INVALID_TIMESTAMP$ ERROR_MSG_INVALID_TIMESTAMP ERROR_DETAIL_INVALID_TIMESTAMP "DD/MM/YYYY HH:mm:SS.mss"
    end
            
    move (mid(inTs, 2, 1)) to inGmtDay
    move (mid(inTs, 2, 4)) to inGmtMon
    move (mid(inTs, 4, 7)) to inGmtyear
    
    if (length(inTs) >= 13) move (mid(inTs, 2, 12)) to inGmtHr
    else move 0 to inGmtHr
    if (length(inTs) >= 16) move (mid(inTs, 2, 15)) to inGmtMin
    else move 0 to inGmtMin
    if (length(inTs) >= 19) move (mid(inTs, 2, 18)) to inGmtSec
    else move 0 to inGmtSec
    
    move (get_bias(inDestTz, inGmtYear, inGmtMon, inGmtDay, inGmtHr, inGmtMin, inGmtSec, 0)) to l_bias
    move (zeropad(inGmtDay,2)+"/"+zeropad(inGmtMon,2)+"/"+zeropad(inGmtYear,4)+" "+zeropad(inGmtHr,2)+":"+zeropad(inGmtMin,2)+":"+zeropad(inGmtSec,2)) to l_ts
    
    move (timestamp_adjust(l_ts, l_bias*60000)) to l_ts
    
    function_return l_ts
end_function

//Returns current time in the passed timezone from the time passed as UTC
function get_timezone_time_from_utc_time global string inDestTz string inTs returns string
    local integer l_bias
    local string l_ts
    local integer inUtcYear inUtcMonth inUtcDay inUtcHr inUtcMin inUtcSec 

    if ((length(inTs) < 10) or (mid(inTs, 1, 3) <> "/") or (mid(inTs, 1, 6)  <> "/") or (mid(inTs, 1, 11)  <> " ")) begin
        custom_error ERROR_CODE_INVALID_TIMESTAMP$ ERROR_MSG_INVALID_TIMESTAMP ERROR_DETAIL_INVALID_TIMESTAMP "DD/MM/YYYY"
    end
    if (((length(inTs) >= 16) and (mid(inTs, 1, 14)  <> ":")) or ((length(inTs) >= 19) and (mid(inTs, 1, 17)  <> ":"));
        or ((length(inTs) >= 21) and (mid(inTs, 1, 20)  <> "."))) begin
        custom_error ERROR_CODE_INVALID_TIMESTAMP$ ERROR_MSG_INVALID_TIMESTAMP ERROR_DETAIL_INVALID_TIMESTAMP "DD/MM/YYYY HH:mm:SS.mss"
    end
            
    move (mid(inTs, 2, 1)) to inUtcDay
    move (mid(inTs, 2, 4)) to inUtcMonth
    move (mid(inTs, 4, 7)) to inUtcyear
    
    if (length(inTs) >= 13) move (mid(inTs, 2, 12)) to inUtcHr
    else move 0 to inUtcHr
    if (length(inTs) >= 16) move (mid(inTs, 2, 15)) to inUtcMin
    else move 0 to inUtcMin
    if (length(inTs) >= 19) move (mid(inTs, 2, 18)) to inUtcSec
    else move 0 to inUtcSec
            
    move (get_bias(inDestTz, inUtcYear, inUtcMonth, inUtcDay, inUtcHr, inUtcMin, inUtcSec, 1)) to l_bias
    move (zeropad(inUtcDay,2)+"/"+zeropad(inUtcMonth,2)+"/"+zeropad(inUtcYear,4)+" "+zeropad(inUtcHr,2)+":"+zeropad(inUtcMin,2)+":"+zeropad(inUtcSec,2)) to l_ts
    
    move (timestamp_adjust(l_ts, l_bias*-60000)) to l_ts
    
    function_return l_ts
end_function

//Convert a timestamp from one local zone to another
function get_timezone_time_from_timezone_time global string inSourceTz string inDestTz string inTs returns string
    local string l_ts
    
    move (get_timezone_time_from_utc_time(inDestTz, (get_utc_time_from_timezone_time(inSourceTz, inTs)))) to l_ts
    
    function_return l_ts
end_function

