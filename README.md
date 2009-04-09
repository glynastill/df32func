df32func
========
Some useful helper functions to speed up development when working with DataFlex
3.2 console mode.

Requirements
------------
MinGW or compatible GNU C compiler: http://www.mingw.org/
DataFlex 3.2 Console Mode or greater: http://www.dataaccess.com/

Installation
------------
Ensure you have both DF32 and MinGW environments set up correctly and have both 
bin directories are in your path (i.e. locations of "dfcomp" and "make")

Open a DOS prompt and cd into df32func directory.

To build the dynamic link library and the dataflex precompiled package:

	build all

To build just the dataflex precompiled package:

	build df

Or build each independently

The dynamic link library:

	cd src/c
	make clean
	make

The dataflex precompiled package:

	cd src/df32
	dfcomp df32func.mk -p
	
Alternatively include the dataflex includes as required directly in dataflex
source code.
