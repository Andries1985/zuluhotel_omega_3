use math;


function Log(base, answer)

	var x := 1.000;
	var logcount := 0.000;
	var diff := 1.000;
	var success := 0;
	var count;

	While (success == 0)

		x := x * base;

		If (Abs(answer - x) < 0.001)
			logcount := logcount + 1;
			success := 1;
		else
			If (x > answer)
				diff := diff / 2;
				logcount := logcount - diff;
				x := Pow(base, logcount);
			else
				logcount := logcount + diff;
			endif
		endif
	
		Count := Count + 1;

		If (Count > 200) 
			success := 1;
		endif

	endwhile

	return logcount;

endfunction


function Exp( x , y )
	return Pow( x , y );
endfunction


function Trunc(number, places)

	var base := Pow(10, places);

	number := number * base;
	number := Cint(number);
	number := CDbl(number);
	number := number / base;

	return number;

endfunction


function Fac(factorial);

	var x;
	While (factorial > 0)
		x := x * factorial;
		factorial := factorial - 1;
	endwhile
	return x;

endfunction


function YRoot(x, y)

	
	var i := 0.000;
	var n := 0.000;
	var rootx := 2.000;
	var a := CInt(Sqrt(x));

	if (a < 4)
		a := 4;
	endif

	var success := 0;
	
	
	While (success == 0)		

		n := Pow(rootx, y);
		i := x - n;
		i := i / (a * rootx);
		i := Trunc(i, 3);
		rootx := rootx + i;
		if (i == 0)
			success := 1;
			if ( Pow(rootx, y) > x )
				rootx := rootx - 0.001;
			endif
			if ( Pow((rootx + 0.001), y) == x)
				rootx := rootx + 0.001;
			endif
		endif
	endwhile

	return rootx;

endfunction


function Mean(ArrayX)

	var n, i:=0.000;

	foreach n in ArrayX
		i := i + ArrayX[n];
	endforeach

	n := CDbl(i / Len(ArrayX) );

	return n;

endfunction


function StdDev(ArrayX)

	var m, n, i := 0.000;
	var ArrayMean := Mean(ArrayX);
	var ArrayLen := Len(ArrayX);

	foreach m in ArrayX
		n := ArrayX[m] - ArrayMean;
		n := Pow(n, 2);
		i := i + n;
	endforeach

	i := i / (ArrayLen - 1);
	n := Sqrt (i);

	return n;

endfunction


function ZTest(ArrayX, mu, sigma)

	var i, n;
	var z:= 0.000;
	var ArrayMean := Mean(ArrayX);
	var ArrayLen := Len(ArrayX);

	i := ArrayMean - mu;
	n := sigma / Sqrt(ArrayLen);

	z := i/n;

	return z;

endfunction


function TTest(ArrayX, mu)

	var i, n;
	var t:= 0.000;
	var ArrayMean := Mean(ArrayX);
	var ArrayLen := Len(ArrayX);
	var s := StdDev(ArrayX);

	i := ArrayMean - mu;
	n := s / Sqrt(ArrayLen);

	t := i/n;

	return t;

endfunction

function BitIn(ArrayX)

	var i := 8;
	var n;
	var loop := 1;
	var bitvalue := 0;

	While (i >= 0)

		n := 8 - i;
		if (ArrayX[i] == 1)
			bitvalue := bitvalue + Pow( 2, n );
		endif
		i := i - 1;

	endwhile

	return bitvalue;

endfunction


function BitOut(x)

	x := Cint(x);
	var loop := 1;
	var bitvalue := 0;
	var ArrayX := {0,0,0,0,0,0,0,0};
	var n;

	while (loop <= 8)

		n := 8 - loop;
		bitvalue := Pow(2, n);
		if ( (x - bitvalue) >= 0)
			ArrayX[loop] := 1;
			x := x - bitvalue;
		endif
		loop := loop + 1;

	endwhile

	return ArrayX;

endfunction


function BitAnd(x, y)

	var ArrayX := BitOut(x);
	var ArrayY := BitOut(y);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)
	
		n := loop - 1;
		if ( (ArrayX[loop] == 1) AND (ArrayY[loop] == 1) )
			z := z + Pow(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction


function BitOr(x, y)

	var ArrayX := BitOut(x);
	var ArrayY := BitOut(y);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)

		n := loop - 1;
		if ( (ArrayX[loop] == 1) OR (ArrayY[loop] == 1) )
			z := z + Pow(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction


function BitXor(x, y)

	var ArrayX := BitOut(x);
	var ArrayY := BitOut(y);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop] - ArrayY[loop] <> 0)
			z := z + Pow(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction


function BitEqv(x, y)

	var ArrayX := BitOut(x);
	var ArrayY := BitOut(y);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop] - ArrayY[loop] == 0)
			z := z + Pow(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction


function BitImp(x, y)

	var ArrayX := BitOut(x);
	var ArrayY := BitOut(y);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop] - ArrayY[loop] < 1)
			z := z + Pow(2, n);
		endif
		loop := loop + 1;

	endwhile

	return z;

endfunction


function BitNot(x)

	var ArrayX := BitOut(x);
	var z := 0;
	var loop := 1;
	var n;

	while (loop <= 8)

		n := loop - 1;
		if (ArrayX[loop] == 0)
			ArrayX[loop] := 1;
			z := z + Pow(2, n);
		else
			ArrayX[loop] := 0;
		endif

		loop := loop + 1;

	endwhile

	return z;

endfunction


function FlagSet(flag, bit);

	bit := 9 - bit;
	
	var FlagArray := BitOut(flag);
	FlagArray[bit] := 1;

	bit := BitIn(FlagArray);

	return bit;

endfunction


function FlagReset(flag, bit);

	var x := 1;
	var y := 0;
	var n := 0;
	
	bit := 9 - bit;
	
	var FlagArray := BitOut(flag);
	FlagArray[bit] := 0;

	While (x <= 8)

		if (FlagArray[x] == 1)
			n := x - 1;
			y := y + Pow(2, n);
		endif

		x := x + 1;

	endwhile

	return y;

endfunction


function FlagCheck(flag, bit);

	var FlagArray := BitOut(flag);
	bit := 9 - bit;

	if (FlagArray[bit] == 1)
		return 1;
	else
		return 0;
	endif

endfunction