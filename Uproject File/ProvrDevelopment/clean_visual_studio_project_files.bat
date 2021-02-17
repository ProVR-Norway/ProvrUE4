@echo off
setlocal
echo ***************************************************************
echo ************** Clean Unreal Engine Project Files **************
echo ***************************************************************
echo.
:PROMPT
SET /P AREYOUSURE=Are you sure to delete all Binaries, Build, Intermediate, Saved and DerivedDataCache folders in your project and plugins (y/[N])?
IF /I "%AREYOUSURE%" NEQ "y" GOTO END
echo.
echo Cleaning your project, please wait...
FOR /d /r %%d IN ("Binaries","Build","Intermediate","Saved","DerivedDataCache") DO @IF EXIST "%%d" rd /s /q "%%d"
del *.sln
echo.
echo Your project cleaned perfectly, you can click "Generate Visual Studio project files" now.
pause
:END
endlocal