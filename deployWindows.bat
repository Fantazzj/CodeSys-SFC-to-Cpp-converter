mkdir .\windows-deploy
del .\windows-deploy\*
move .\build-release\CodeSys-SFC-to-Cpp-Converter.exe .\windows-deploy
windeployqt --no-translations .\windows-deploy\
copy LICENSE .\windows-deploy
copy README.md .\windows-deploy