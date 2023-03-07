@echo off
%comspec% /k "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
echo Building...
scons platform=windows bits=64
echo Done building
PAUSE