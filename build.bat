@echo off

set LC1=%1
set LC1=%LC1:A=a%
set LC1=%LC1:B=b%
set LC1=%LC1:C=c%
set LC1=%LC1:D=d%
set LC1=%LC1:E=e%
set LC1=%LC1:F=f%
set LC1=%LC1:G=g%
set LC1=%LC1:H=h%
set LC1=%LC1:I=i%
set LC1=%LC1:J=j%
set LC1=%LC1:K=k%
set LC1=%LC1:L=l%
set LC1=%LC1:M=m%
set LC1=%LC1:N=n%
set LC1=%LC1:O=o%
set LC1=%LC1:P=p%
set LC1=%LC1:Q=q%
set LC1=%LC1:R=r%
set LC1=%LC1:S=s%
set LC1=%LC1:T=t%
set LC1=%LC1:U=u%
set LC1=%LC1:V=v%
set LC1=%LC1:W=w%
set LC1=%LC1:X=x%
set LC1=%LC1:Y=y%
set LC1=%LC1:Z=z%

if A%LC1%A == AdebugA goto do_debug
if A%LC1%A == AallA goto do_all
if A%LC1%A == AdfA goto do_df
if A%LC1%A == AcleanA goto do_clean
goto unknown

:do_clean
    @echo MAKE: Cleaning for df32func dll library
    cd src\c
    if %errorlevel% == 0 goto cclean
    @echo Can't cd to directory (Error level %errorlevel%)
    goto errors 
    :cclean
    make clean
    cd ..\..
    @echo DEL: Cleaning for df32func DataFlex precompiled package
    cd src\df32
    del df32func.flp df32func.pki
    cd ..\..
    goto do_exit
:do_all
    @echo MAKE: Compiling df32func dll library
    cd src\c
    if %errorlevel% == 0 goto cmake
    @echo Can't cd to directory (Error level %errorlevel%)
    goto errors 
    :cmake
    make clean
    make
    if %errorlevel% == 0 goto okayc
    @echo Errors occoured during compile for df32func.dll (Error level %errorlevel%)
    goto errors
    :okayc
    cd ..\..
:do_df	
    @echo DFCOMP: Compiling df32func DataFlex precompiled package
    cd src\df32
    if %errorlevel% == 0 goto dfmake
    @echo Can't cd to directory (Error level %errorlevel%)
    goto errors 
    :dfmake
    dfcomp df32func.mk -p
    if %errorlevel% == 0 goto okaydf
    @echo Errors occoured during compile for df32func.flp (Error level %errorlevel%)
    goto errors
    :okaydf   
    @echo df32func.inc last compiled on %date% at %time% > df32func.inc.autodoc
    @echo df32func DLL functions: >> df32func.inc.autodoc
    findstr /s /i /b "external_function" *.* >> df32func.inc.autodoc
    @echo df32func functions: >> df32func.inc.autodoc
    findstr /s /i /b "function " *.* >> df32func.inc.autodoc
    @echo df32func procedures: >> df32func.inc.autodoc
    findstr /s /i /b "procedure " *.* >> df32func.inc.autodoc    
    @echo df32func classes: >> df32func.inc.autodoc
    findstr /s /i /b "class " *.* >> df32func.inc.autodoc
    cd ..\..
    goto do_exit
:do_debug
    @echo DFCOMP: Compiling df32func DataFlex debug package df32fdbg.mk
    del df32fdbg.mk
    copy df32func.mk df32fdbg.mk
    dfcomp df32fdbg.mk -p -f
    goto do_exit

:unknown
@echo Unknown action: "%LC1%"
exit /B

:errors
@echo Compilation failed: see above for detail
exit /B

:do_exit
@echo Compilation completed successfully
exit /B


