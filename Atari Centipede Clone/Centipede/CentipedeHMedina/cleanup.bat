REM Visual Studio project Clean Up
REM Andre Berthiaume, April 2013 (Improved from Ed Keenan's original)

REM Assign PNAME with the name of the .sln file
for %%* in (*.sln) do set PNAME=%%~n*

REM Delete all the unneeded stuff
rmdir /S /Q Debug
rmdir /S /Q ipch
rmdir /S /Q Release
del /F /S %PNAME%.sdf
del /F /S /A:H %PNAME%.suo
rmdir /S /Q %PNAME%\Debug
rmdir /S /Q %PNAME%\Release
