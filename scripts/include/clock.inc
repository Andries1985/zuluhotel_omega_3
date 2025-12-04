use uo;
use cfgfile;

function GetTime()

    var cfg := ReadConfigFile( "::days" );
    var elem := FindConfigElem( cfg , "WholeDay" );
    var length := GetConfigInt( elem , "Length" );
    var day_length := (60 * length);

//Print("DayLength: " + day_length);

    var x := readgameclock();

//Print("Read Game Clock: " + x);

    var bigpart := x / day_length;

//Print("Bigpart: " + bigpart);

    var smallpart := bigpart * day_length;
    x := (x - smallpart);

//Print("Time of Day: " + x);
//Print("The Day is " + day_length/60 + " minutes long.");

return ( x );

endfunction

function GetTransition()

    var cfg := ReadConfigFile( "::days" );
    var elem := FindConfigElem( cfg , "Transition" );
    var length := GetConfigInt( elem , "Length" );

    return length;
endfunction

function GetDayLength()

    var cfg := ReadConfigFile( "::days" );
    var elem := FindConfigElem( cfg , "WholeDay" );
    var length := GetConfigInt( elem , "Length" );

    return length;
endfunction

// Returns time in the format "hour:minutes", where hour = 00-23
function GetTimeofDay()

    var time_sec := GetTime();
    var length := (GetDayLength() * 60) * 1.0 ;

    var temp := length / time_sec;
    var raw_time := 24.0 / temp;

    var hours := cint(raw_time);
    var remainder := raw_time - hours;
    var minutes := cint( remainder * 60 );

    if( len(cstr(minutes)) == 1 )
	minutes := "0" + cstr(minutes);
    endif

    if( len(cstr(hours)) == 1 )
	hours := "0" + cstr(hours);
    endif

    return (hours+":"+minutes);

endfunction

// Returns a number of the hour of the day (1-24);
function GetHourOfDay()
	var time  := GetTimeOfDay();
	var colon := find(time, ":", 1);
	return CInt(time[1, colon]);
endfunction

///////////////////////////////////////////////////////////
// Purpose: 		Returns true if the hour is within
//			the specified range (inclusive).
//
// Params:  hr		Hour to check for.
//	    hrstart	Start hr.
//	    hrend	Ending hr.
//
// Returns: 1		Hour is within range.
//	    0		Hour is outside range
///////////////////////////////////////////////////////////
function HourWithinRange(hr, hrstart, hrend)
	hr	:= CInt(hr);
	hrstart	:= CInt(hrstart);
	hrend	:= CInt(hrend);

	if (hrstart > hrend)
		if (hr >= hrstart or hr < hrend)
			return 1;
		endif
	else
		if (hr >= hrstart and hr <= hrend)
			return 1;
		endif
	endif

	return 0;
endfunction

///////////////////////////////////////////////////////////
// Purpose: 			Return the number of hours before
//				the starting hour.
//
// Params:  current_hour	Current hour
//	    starting_hour	Starting hour
//
// Returns: delay		Number of hours before the starting hour
///////////////////////////////////////////////////////////
function GetNumberOfHoursBeforeStartingHour( current_hour , starting_hour )

	var delay := starting_hour - current_hour;
	if( delay < 0 )
		delay := delay + 24;
	endif
	
	return delay;
	
endfunction

///////////////////////////////////////////////////////////
// Purpose: 			Return the number of seconds before
//				the starting hour.
//
// Params:  current_hour	Current hour
//	    starting_hour	Starting hour
//
// Returns: delay		Number of seconds before the starting hour
///////////////////////////////////////////////////////////
function GetNumberOfSecondsBeforeStartingHour( current_hour , starting_hour )

	return Cint( GetNumberOfHoursBeforeStartingHour( current_hour , starting_hour ) * 60 * 60 );
	
endfunction