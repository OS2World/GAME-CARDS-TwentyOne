@echo off
rem =========================================================================
rem Compile script for TwentyOne (Open Watcom C on OS/2 / ArcaOS)
rem Run from the project root:  compile-wat.cmd
rem =========================================================================

rem Auto-detect Watcom installation (OS/2 typical paths)
if "%WATCOM%"=="" (
    if exist C:\WATCOM\binp\wcc386.exe (
        set WATCOM=C:\WATCOM
    ) else if exist C:\WATCOM\bin\wcc386.exe (
        set WATCOM=C:\WATCOM
    ) else if exist C:\WATCOM2\binp\wcc386.exe (
        set WATCOM=C:\WATCOM2
    ) else if exist D:\WATCOM\binp\wcc386.exe (
        set WATCOM=D:\WATCOM
    ) else (
        echo ERROR: Watcom not found. Set WATCOM environment variable.
        exit 1
    )
)

rem Watcom OS/2 tools are installed under binp (Open Watcom on OS/2)
if exist %WATCOM%\binp\wcc386.exe (
    set PATH=%WATCOM%\binp;%PATH%
) else (
    set PATH=%WATCOM%\bin;%PATH%
)

rem Auto-detect OS/2 Toolkit
if "%OS2TK%"=="" (
    if exist C:\OS2TK45\h\os2.h (
        set OS2TK=C:\OS2TK45
    ) else if exist C:\OS2TK\h\os2.h (
        set OS2TK=C:\OS2TK
    ) else (
        echo WARNING: OS2TK not set, defaulting to C:\OS2TK45
        set OS2TK=C:\OS2TK45
    )
)

rem Set up environment for Open Watcom on OS/2
set PATH=%WATCOM%\binp;%WATCOM%\binw;%PATH%
set BEGINLIBPATH=%WATCOM%\binp\dll
set INCLUDE=%WATCOM%\h;%WATCOM%\h\os2;%OS2TK%\h;src;%INCLUDE%
set LIB=%WATCOM%\lib386;%WATCOM%\lib386\os2;%OS2TK%\lib;%LIB%

if "%WIPFC%"=="" set WIPFC=%WATCOM%\wipfc

rem Log file
set LOGFILE=compile-wat.log

rem Start logging - write header
echo ========================================== > %LOGFILE%
echo TwentyOne Build Log >> %LOGFILE%
echo Date: %DATE% Time: %TIME% >> %LOGFILE%
echo WATCOM=%WATCOM% >> %LOGFILE%
echo OS2TK=%OS2TK% >> %LOGFILE%
echo ========================================== >> %LOGFILE%
echo. >> %LOGFILE%

rem Also show on screen
echo Building TwentyOne with Open Watcom...
echo WATCOM=%WATCOM%
echo OS2TK=%OS2TK%
echo Log: %LOGFILE%
echo.

rem Clean first
echo [CLEAN] Running wmake clean...
wmake -f makefile.wat clean >> %LOGFILE% 2>&1

set FAILED=0
echo [BUILD] Running wmake all...
wmake -f makefile.wat all >> %LOGFILE% 2>&1
if errorlevel 1 set FAILED=1

rem Check result
if "%FAILED%"=="1" goto failed
if exist bin\TwentyOne.exe (
    echo.
    echo BUILD OK
    echo BUILD OK >> %LOGFILE%
    exit 0
)
:failed
echo.
echo BUILD FAILED
echo BUILD FAILED >> %LOGFILE%
exit 1