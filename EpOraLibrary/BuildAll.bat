if not exist "C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv.exe" goto :VS9Compile

"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpOraLibrary80.sln /build "Debug" /project "EpOraLibrary\EpOraLibrary80.vcproj" /projectconfig "Debug"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpOraLibrary80.sln /build "Debug DLL" /project "EpOraLibrary\EpOraLibrary80.vcproj" /projectconfig "Debug DLL"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpOraLibrary80.sln /build "Debug DLL Unicode" /project "EpOraLibrary\EpOraLibrary80.vcproj" /projectconfig "Debug DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpOraLibrary80.sln /build "Debug Unicode" /project "EpOraLibrary\EpOraLibrary80.vcproj" /projectconfig "Debug Unicode"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpOraLibrary80.sln /build "Release" /project "EpOraLibrary\EpOraLibrary80.vcproj" /projectconfig "Release"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpOraLibrary80.sln /build "Release DLL" /project "EpOraLibrary\EpOraLibrary80.vcproj" /projectconfig "Release DLL"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpOraLibrary80.sln /build "Release DLL Unicode" /project "EpOraLibrary\EpOraLibrary80.vcproj" /projectconfig "Release DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 8\Common7\IDE\devenv" EpOraLibrary80.sln /build "Release Unicode" /project "EpOraLibrary\EpOraLibrary80.vcproj" /projectconfig "Release Unicode"

:VS9Compile

if not exist "C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv.exe" goto :VS10Compile

"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpOraLibrary90.sln /build "Debug" /project "EpOraLibrary\EpOraLibrary90.vcproj" /projectconfig "Debug"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpOraLibrary90.sln /build "Debug DLL" /project "EpOraLibrary\EpOraLibrary90.vcproj" /projectconfig "Debug DLL"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpOraLibrary90.sln /build "Debug DLL Unicode" /project "EpOraLibrary\EpOraLibrary90.vcproj" /projectconfig "Debug DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpOraLibrary90.sln /build "Debug Unicode" /project "EpOraLibrary\EpOraLibrary90.vcproj" /projectconfig "Debug Unicode"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpOraLibrary90.sln /build "Release" /project "EpOraLibrary\EpOraLibrary90.vcproj" /projectconfig "Release"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpOraLibrary90.sln /build "Release DLL" /project "EpOraLibrary\EpOraLibrary90.vcproj" /projectconfig "Release DLL"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpOraLibrary90.sln /build "Release DLL Unicode" /project "EpOraLibrary\EpOraLibrary90.vcproj" /projectconfig "Release DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 9.0\Common7\IDE\devenv" EpOraLibrary90.sln /build "Release Unicode" /project "EpOraLibrary\EpOraLibrary90.vcproj" /projectconfig "Release Unicode"

:VS10Compile

if not exist "C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv.exe" goto :Done

"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpOraLibrary100.sln /build "Debug" /project "EpOraLibrary\EpOraLibrary100.vcxproj" /projectconfig "Debug"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpOraLibrary100.sln /build "Debug DLL" /project "EpOraLibrary\EpOraLibrary100.vcxproj" /projectconfig "Debug DLL"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpOraLibrary100.sln /build "Debug DLL Unicode" /project "EpOraLibrary\EpOraLibrary100.vcxproj" /projectconfig "Debug DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpOraLibrary100.sln /build "Debug Unicode" /project "EpOraLibrary\EpOraLibrary100.vcxproj" /projectconfig "Debug Unicode"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpOraLibrary100.sln /build "Release" /project "EpOraLibrary\EpOraLibrary100.vcxproj" /projectconfig "Release"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpOraLibrary100.sln /build "Release DLL" /project "EpOraLibrary\EpOraLibrary100.vcxproj" /projectconfig "Release DLL"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpOraLibrary100.sln /build "Release DLL Unicode" /project "EpOraLibrary\EpOraLibrary100.vcxproj" /projectconfig "Release DLL Unicode"
"C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE\devenv" EpOraLibrary100.sln /build "Release Unicode" /project "EpOraLibrary\EpOraLibrary100.vcxproj" /projectconfig "Release Unicode"

:Done