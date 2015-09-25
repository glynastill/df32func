df32func
========
Some useful helper functions to speed up development when working with DataFlex
3.2 console mode.

Requirements
------------
MinGW or compatible GNU C compiler: 
	http://www.mingw.org/
DataFlex 3.2 Console Mode or greater: 
	http://www.dataaccess.com/
For regex functionality requires the GNU posix regex libraries:
	http://sourceforge.net/projects/mingw/files/Other/UserContributed/regex/mingw-regex-2.5.1/mingw-libgnurx-2.5.1-src.tar.gz/download

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

Once everything is built, copy the dataflex .pki and .flp files to the location where
your dataflex setup expects to find it's pkg and flx files. Copy both df32func.dll and
libgnurx-0.dll to a location in your path.
