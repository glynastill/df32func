//-------------------------------------------------------------------------
// date.inc
//      This file contains some DataFlex 3.2 Console Mode functions
//      to provide extended date manipulation capabilities.
//
// This file is to be included in df32func.mk
//
// Copyright (c) 2006-2015, glyn@8kb.co.uk
// 
// df32func/date.inc
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------------

// Returns day of the week as an integer score 1 = monday 7 = sunday
function get_day_score global date argv returns integer
    local integer l_01tmpInt
    
    calc (mod(argv,7)) to l_01tmpInt
    if (l_01tmpInt < 0) calc (l_01tmpInt+7) to l_01tmpInt
    
    case begin
        case (l_01tmpInt = 3) move 1 to l_01tmpInt
        case break
        case (l_01tmpInt = 4) move 2 to l_01tmpInt
        case break
        case (l_01tmpInt = 5) move 3 to l_01tmpInt
        case break
        case (l_01tmpInt = 6) move 4 to l_01tmpInt
        case break
        case (l_01tmpInt = 0) move 5 to l_01tmpInt
        case break
        case (l_01tmpInt = 1) move 6 to l_01tmpInt
        case break
        case (l_01tmpInt = 2) move 7 to l_01tmpInt
        case break
    case end
    
    function_return l_01tmpInt
end_function

// Returns the current day as a string from the date
function get_day global date argv returns string
    local integer l_iDay
    local string l_sDay

    move (get_day_score(argv)) to l_iDay
    
    case begin
        case (l_iDay = 1) move "Monday" to l_sDay
        case break
        case (l_iDay = 2) move "Tuesday" to l_sDay
        case break
        case (l_iDay = 3) move "Wednesday" to l_sDay
        case break
        case (l_iDay = 4) move "Thursday" to l_sDay
        case break
        case (l_iDay = 5) move "Friday" to l_sDay
        case break
        case (l_iDay = 6) move "Saturday" to l_sDay
        case break
        case (l_iDay = 7) move "Sunday" to l_sDay
        case break
   case end

    function_return l_sDay
end_function

// Returns the current month as a string from the date
function get_month global date argv returns string
    local integer l_iMonth
    local string l_sMonth

    calc ((integer(mid(argv, 2, 4)))-1) to l_iMonth
    case begin
        case (l_iMonth = 0) move "January" to l_sMonth
        case (l_iMonth = 1) move "February" to l_sMonth
        case (l_iMonth = 2) move "March" to l_sMonth
        case (l_iMonth = 3) move "April" to l_sMonth
        case (l_iMonth = 4) move "May" to l_sMonth
        case (l_iMonth = 5) move "June" to l_sMonth
        case (l_iMonth = 6) move "July" to l_sMonth
        case (l_iMonth = 7) move "August" to l_sMonth
        case (l_iMonth = 8) move "September" to l_sMonth
        case (l_iMonth = 9) move "October" to l_sMonth
        case (l_iMonth = 10) move "November" to l_sMonth
        case (l_iMonth = 11) move "December"  to l_sMonth
   case end

    function_return l_sMonth
end_function

// Returns the current day of the month
function get_day_of global date argv returns string
    local string l_01tmpStr
    local integer l_01tmpInt

    move (mid(argv, 2, 1)) to l_01tmpInt
    move "th" to l_01tmpStr
    if (mid(argv, 1, 1)) ne 1 begin
        if (mid(argv, 1, 2)) eq 1 move "st" to l_01tmpStr
        if (mid(argv, 1, 2)) eq 2 move "nd" to l_01tmpStr
        if (mid(argv, 1, 2)) eq 3 move "rd" to l_01tmpStr
    end

    function_return l_01tmpStr
end_function

// Returns a string representing the full date e.g. "Friday 31st July 2009"
function fulldate global date argv returns string
    local string l_01tmpStr l_02tmpStr 
    local integer l_01tmpInt l_01arrayindex

    move (get_day(argv)) to l_02tmpStr
    move (mid(argv, 2, 1)) to l_01tmpInt
    move "th" to l_01tmpStr
    if (mid(argv, 1, 1)) ne 1 begin
        if (mid(argv, 1, 2)) eq 1 move "st" to l_01tmpStr
        if (mid(argv, 1, 2)) eq 2 move "nd" to l_01tmpStr
        if (mid(argv, 1, 2)) eq 3 move "rd" to l_01tmpStr
    end
    append l_02tmpStr " " l_01tmpInt (trim(l_01tmpStr)) " "
    move (get_month(argv)) to l_01tmpStr
    append l_02tmpStr (trim(l_01tmpStr)) " " (trim(mid(argv, 4, 7)))

    function_return l_02tmpStr
end_function

// Returns a string representing the date in short form e.g. "31 July 2009"
function shortdate global date argv returns string
    local string l_01tmpStr

    move "" to l_01tmpStr
    append l_01tmpStr (mid(argv,2,1)) " " (get_month(argv)) " " (mid(argv,4,7))
    
    function_return l_01tmpStr
end_function

// Returns the current time and or date in different formats
function now global string argv returns string
    local string l_01tmpStr l_02tmpStr
    local date l_01nowDate
    local number l_01nowTime l_02nowTime l_03nowTime l_01nowSecs
    sysdate l_01nowDate
    calc ((number(strlen))+((number(strmark))/100)) to l_01nowTime
    move "" to l_01tmpStr
    if ((trim(lowercase(argv))) = "time") append l_01tmpStr (string(decround(l_01nowTime,2)))
    if (((trim(lowercase(argv))) = "longtime") or ((trim(lowercase(argv))) = "timestamp")) begin
        sysdate l_01nowDate l_01nowTime l_02nowTime l_03nowTime
        move "" to l_01tmpStr
        if ((trim(lowercase(argv))) = "timestamp") append l_01tmpStr l_01nowDate " "
        if (l_01nowTime < 10) append l_01tmpStr 0
        append l_01tmpStr l_01nowTime ":"
        if (l_02nowTime < 10) append l_01tmpStr 0
        append l_01tmpStr l_02nowTime ":"
        if (l_03nowTime < 10) append l_01tmpStr 0
        append l_01tmpStr l_03nowTime
    end
    if ((trim(lowercase(argv))) = "seconds") begin
        sysdate l_01nowDate l_01nowTime l_02nowTime l_03nowTime
        move (string((l_01nowTime*3600)+(l_02nowTime*60)+l_03nowTime)) to l_01tmpStr 
    end
    if ((trim(lowercase(argv))) = "date") append l_01tmpStr l_01nowDate
    if ((trim(lowercase(argv))) = "shorttimestamp") append l_01tmpStr l_01nowDate "," (string(decround(l_01nowTime,2)))
    if ((trim(lowercase(argv))) = "longtimestamp") begin
        append l_01tmpStr (fulldate(l_01nowDate)) " at "
        if (l_01nowTime > 12) append l_01tmpStr (string(decround(l_01nowTime-12,2))) "PM"
        if (l_01nowTime <= 12) append l_01tmpStr (string(decround(l_01nowTime,2))) "AM"
    end
    
    function_return l_01tmpStr
end_function

// Return a date and time in the RSS standard format
function rssdate global date argv string argv2 returns string
    local string l_01tmpStr

    move "" to l_01tmpStr
    append l_01tmpStr (left((get_day(argv)),3)) ", " (mid(argv,2,1)) " " (left((get_month(argv)),3)) " " (mid(argv,4,7)) " " (trim(argv2)) " GMT"
    move (uppercase(l_01tmpStr)) to l_01tmpStr

    function_return l_01tmpStr
end_function

// Retuns a date in the format CCYYMMDD
function cymd global date argv returns string
    local string l_01tmpStr
    
    move (mid(argv,4,7)) to l_01tmpStr
    append l_01tmpStr (mid(argv,2,4)) (mid(argv,2,1))
    
    function_return l_01tmpStr
end_function

// Retuns a date in the format CCYY-MM-DD
function sqldate global date argv returns string
    local string l_01tmpStr
    move "" to l_01tmpStr
    
    if (argv <> "") begin
        move (mid(argv,4,7)) to l_01tmpStr
        append l_01tmpStr "-" (mid(argv,2,4)) "-" (mid(argv,2,1))
    end
    function_return l_01tmpStr
end_function

// Retuns a timestamp in the format CCYY-MM-DD HH:mm:SS
function sqltimestamp global date argv integer argv2 integer argv3 integer argv4 returns string
    local string l_01tmpStr
    move "" to l_01tmpStr
            
    if ((argv <> "") and ((argv2 >= 0) and (argv2 <= 23)) and ((argv3 >= 0) and (argv3 <= 59)) and ((argv4 >= 0) and (argv4 <= 59))) begin
        move (mid(argv,4,7)) to l_01tmpStr
        append l_01tmpStr "-" (mid(argv,2,4)) "-" (mid(argv,2,1)) " "  (zeropad(argv2,2)) ":" (zeropad(argv3,2)) ":" (zeropad(argv4,2))
    end
    
    function_return l_01tmpStr
end_function

// Retuns a timestamp in the format DD/MM/CCYY HH:mm:SS
function dftimestamp global date argv integer argv2 integer argv3 integer argv4 returns string
    local string l_01tmpStr
    move "" to l_01tmpStr
            
    if ((argv <> "") and ((argv2 >= 0) and (argv2 <= 23)) and ((argv3 >= 0) and (argv3 <= 59)) and ((argv4 >= 0) and (argv4 <= 59))) begin
        move "" to l_01tmpStr
        append l_01tmpStr argv " "  (zeropad(argv2,2)) ":" (zeropad(argv3,2)) ":" (zeropad(argv4,2))
    end
    
    function_return l_01tmpStr
end_function

// Returns date or time as a string out of a dateTime struct
function sqldatetime global string argv string argv2 returns string
    local string l_sResult
    
    move (trim(argv)) to argv
    move (trim(uppercase(argv2))) to argv2
    
    if (argv2 = "DATE") begin
        move (mid(argv,2,9)) to l_sResult
        append l_sResult "/" (mid(argv,2,6)) "/" (mid(argv,4,1))
    end
    if (argv2 = "TIME") begin
        move (right(argv,12)) to l_sresult
    end
    
    function_return l_sResult
end_function

// Returns date or timestamp as a string out of a dateTime struct
function sqltimestampformat global string argv string argv2 returns string
    local string l_sResult
    
    move (trim(argv)) to argv
    move (trim(uppercase(argv2))) to argv2
    
    if (argv2 = "DATE") begin
        move (mid(argv,2,9)) to l_sResult
        append l_sResult "/" (mid(argv,2,6)) "/" (mid(argv,4,1))
    end
    if (argv2 = "TIME") begin
        move (right(argv,length(argv)-pos(" ", argv))) to l_sresult
    end
    
    function_return l_sResult
end_function

// Returns last day of month as a date
function last_day_of_month global integer month integer year returns date
    local date l_dDate
    if (month = 12) begin
        calc (year+1) to year
        move 1 to month
    end
    else begin
        calc (month+1) to month
    end
    
    move (('01/'+string(month)+'/'+string(year))-1) to l_dDate
    
    function_return l_dDate
end_function

// Returns a posix timestamp when supplied with a date and numeric values
// for hour, minutes ,seconds and milliseconds: (date,hr,min,sec,millisec)
//
// Also see timestamp_to_posix and posix_to_timestamp in timestamp.inc
function posixtime global date argv number argv2 number argv3 number argv4 number argv5 returns number
    local number l_posix
    
    // Leap seconds not coded
    calc (((integer(argv))-(integer(date("01/01/1970"))))*86400) to l_posix
    calc (((((argv2*60)+argv3)*60)+argv4)+(argv5/1000)+l_posix) to l_posix
    
    function_return l_posix
end_function

// Returns a comma separated string representing a timestamp (date,hr,min,sec,millisec)
// when supplied with a posix time value.
function posixtime_reverse global number argv returns string
    local date l_date
    local number l_subt 
    local integer l_hr l_min l_sec l_msec
    local string l_posix_reverse
    
    // Leap seconds not coded
    calc ((argv/86400)+(integer(date("01/01/1970")))) to l_date
    calc (argv-(((integer(l_date))-(integer(date("01/01/1970"))))*86400)) to l_subt
    calc (l_subt/3600) to l_hr
    calc ((l_subt-(l_hr*3600))/60) to l_min
    calc (l_subt-(((l_hr*60)+l_min)*60)) to l_sec
    calc ((l_subt-((((l_hr*60)+l_min)*60)+l_sec))*1000) to l_msec
            
    move l_date to l_posix_reverse
    append l_posix_reverse "," l_hr "," l_min "," l_sec "," l_msec
   
    function_return l_posix_reverse
end_function

// Returns 0 until the timeout in seconds value passed is exceeded
// when it returns 1.
//
// Relies on the value of global integer "g_nRuntime_entry";
// the only way to use more than once is to reset that global to 0
// by calling runtime_timeout_reset.
function runtime_timeout global integer argv returns integer
    local integer l_iTimeout
    local number l_nHour l_nMinute l_nSec l_nRuntime_now
    local date l_dToday
    
    sysdate l_dToday l_nHour l_nMinute l_nSec
    move 0 to l_iTimeout
    
    if (argv <> 0) begin
        if (g_nRuntime_entry = 0) begin
            move (posixTime(l_dToday,l_nHour,l_nMinute,l_nSec,0)) to g_nRuntime_entry
        end
    
        move (posixTime(l_dToday,l_nHour,l_nMinute,l_nSec,0)) to l_nRuntime_now
        
        if ((l_nRuntime_now - g_nRuntime_entry) > argv) begin
            move 1 to l_iTimeout
        end
    end
    
    function_return l_iTimeout
end_function 

// Resets the value of global integer "g_nRuntime_entry" as used by
// runtime_timeout
function runtime_timeout_reset global returns integer
    move 0 to g_nRuntime_entry
    function_return 0
end_function 

// Returns the time left in seconsd until runtime_timeout is exceeded.
function runtime_timeout_left global returns integer
    local number l_nHour l_nMinute l_nSec l_nRuntime_now
    local date l_dToday

    sysdate l_dToday l_nHour l_nMinute l_nSec
    move (posixTime(l_dToday,l_nHour,l_nMinute,l_nSec,0)) to l_nRuntime_now     
    
    function_return (l_nRuntime_now - g_nRuntime_entry)
end_function 
