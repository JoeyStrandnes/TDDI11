#include <libepc.h>


/*
1. Why “stopTime” is defined as “DWORD32”?
	Becuase it is used for "Now_Plus()" witch returnes the data as "DWORD32".

2. Why “finalCount” is defined as “QWORD64”?
	Becuase it is used for "CPU_Clock_Cycles()" witch returnes the data as "DWORD64".

3. What does “Now_Plus()” do?
	Returns number of of millisecconds since program started plus secconds. 

4. What does “CPU_Clock_Cycles()” do?
	Returns the clock cycles of the CPU (sysick). Used to set a relative start time reference. 

5. What does “x” (or similarly “xM”) represent?
	The elapsed time from start of the while loop to the end of the while loop. Duration is defined by "stopTime".

6. What does “Milliseconds()” do?
	Returns number of millisecconds since program started. 

7. Compare numbers printed by section 2 and sections 3 of the code 
7.1. Are they different?
	Yes. Differ with 36 cycles. 

7.2. Should they be similar?
	Section 2 uses Now_Plus(1) for stopTime while Section 3 uses millisecconds + 1000ms.
	They should be simmilar but not the same.

7.3. How do you explain that these two sections may report different values?
	Se above. 

7.4. Which approach (section 2 or 3) is more appropriate? Why?
	Millisecconds() is the better choice, less issues with rounding errors.

8. Section 4 prints two different numbers. What causes their difference?
	The hardcoded addition of "2345" combined with elapsed exection time between the measuremnts.

9. Consider an operation etc.
	Not an accurete test since the compiler will optimize the code for "paralell" exection (using multiple ALUs etc), loop unrolling. 
	incrementing a value + some overhead = 105 clock cycles. 

*/





int main(int argc, char *argv[])
{
	//+++++ Section 1. Initialization +++++

	ClearScreen(0x07);
	SetCursorPosition(1, 1);
	DWORD32 stopTime = 0;
	QWORD64 initialCount = 0;
	QWORD64 finalCount = 0;
	QWORD64 x = 0;
	DWORD32 xM = 0;

	//+++++ Section 2. +++++

	stopTime = Now_Plus(1);
	initialCount = CPU_Clock_Cycles();

	while( Now_Plus(0) < stopTime ){}

	finalCount = CPU_Clock_Cycles();
	x = finalCount - initialCount;
	xM = (DWORD32)(x / 1000000);
	
	PutString("x2 = ");
	PutUnsigned(xM, 10, 8);
	PutString(" x 10^6 \n");

	//+++++ Section 3. +++++

	stopTime = Milliseconds() + 1000;
	initialCount = CPU_Clock_Cycles();

	while( Milliseconds() < stopTime ){}

	finalCount = CPU_Clock_Cycles();
	x = finalCount - initialCount;
	xM= (DWORD32)(x / 1000000);
	
	SetCursorPosition(2, 1);
	PutString("x3 = ");
	PutUnsigned(xM, 10, 8);
	PutString(" x 10^6 \n");

	//+++++ Section 4. +++++

	stopTime = Milliseconds();
	SetCursorPosition(3, 1);
	PutUnsigned(stopTime, 10, 8);
	PutString("\n");

	stopTime = Milliseconds() + 2345;

	while( Milliseconds() < stopTime ){}
	SetCursorPosition(4, 1);
	stopTime = Milliseconds();
	PutUnsigned(stopTime, 10, 8);
	PutString("\r\n");	

	initialCount = CPU_Clock_Cycles();
	for(int i = 0; i < 100000 ; i++){

	
	}

	finalCount = CPU_Clock_Cycles();

	PutString("ADD = ");
	PutUnsigned(((finalCount - initialCount)/100000), 10, 8);
	PutString("\r\n");

	return 0;
}
