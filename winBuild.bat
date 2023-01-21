
d:
cd \git\GoldenCheetah

copy /Y D:\git\GoldenCheetah\gc-ci-libs\10_Precompiled_DLL\usbexpress_3.5.1\USBXpress\USBXpress_API\Host\x64\SiUSBXp.dll .\
copy /Y D:\git\GoldenCheetah\gc-ci-libs\10_Precompiled_DLL\libsamplerate64\lib\libsamplerate-0.dll .\
copy /Y D:\git\GoldenCheetah\gc-ci-libs\10_Precompiled_DLL\VLC\win64\lib\libvlc*.dll .\
xcopy /Y /s /i /e /q D:\git\GoldenCheetah\gc-ci-libs\10_Precompiled_DLL\VLC\win64\plugins .\plugins 
copy /Y D:\git\GoldenCheetah\gc-ci-libs\OpenSSL-v111-Win64\bin\lib*.dll .\
copy /Y D:\git\GoldenCheetah\gc-ci-libs\OpenSSL-v111-Win64\license.txt .\"OpenSSL License.txt"

:: mkdir C:\python37-x64\lib\site-packages
:: C:\python37-x64\python -m pip install --upgrade pip --no-warn-script-location
:: C:\python37-x64\python -m pip install -r src\Python\requirements.txt -t C:\python37-x64\lib\site-packages --use-pep517


set QTDIR=C:\Qt\5.15\msvc2019_64
set PATH=%QTDIR%\bin;%PATH%
qmake --version

echo "PATH="%PATH%


call c:\"Program Files (x86)"\"Microsoft Visual Studio"\2019\Community\VC\Auxiliary\Build\vcvarsall.bat amd64


echo ":23"
set PATH=%PATH%;C:\Program Files (x86)\NSIS;

set PATH=%PATH%;D:\git\vcpkg
vcpkg install gsl:x64-windows
copy /Y D:\git\vcpkg\installed\x64-windows\bin\gsl*.dll .\


copy /Y qwt\qwtconfig.pri.in qwt\qwtconfig.pri
copy /Y gc-ci-libs\gcconfig64-Release.appveyor.pri src\gcconfig.pri
powershell -Command "(gc src\gcconfig.pri) -replace 'GC_COMPONENTS=c:\\libs', 'GC_COMPONENTS=D:\\git\\GoldenCheetah\\gc-ci-libs' | Out-File -encoding ASCII src\gcconfig.pri"
powershell -Command "(gc src\gcconfig.pri) -replace 'c:\\libs\\', 'D:\\git\\GoldenCheetah\\gc-ci-libs\\' | Out-File -encoding ASCII src\gcconfig.pri"


set PATH=%PATH%;C:\Program Files\R\R-4.1.3\bin;
echo DEFINES+=GC_WANT_R >> src\gcconfig.pri

echo CONFIG += console >> src\gcconfig.pri
echo QMAKE_LFLAGS = -mwindows >> src\gcconfig.pri


"C:\python37-x64\python.exe" --version

cd .sdk\sip-4.19.8
"C:\python37-x64\python.exe" configure.py

:: jom clean
jom -j4

nmake install

cd \git\GoldenCheetah

echo DEFINES+=GC_WANT_PYTHON >> src\gcconfig.pri
echo PYTHONINCLUDES=-ICore -I\"c:\python37-x64\include\" >> src\gcconfig.pri
echo PYTHONLIBS=-L\"c:\python37-x64\libs\" -lpython37 >> src\gcconfig.pri
echo GSL_INCLUDES=D:\git\vcpkg\installed\x64-windows\include >> src\gcconfig.pri
echo GSL_LIBS=-LD:\git\vcpkg\installed\x64-windows\lib -lgsl -lgslcblas >> src\gcconfig.pri

echo CloudDB=active >> src\gcconfig.pri
:: echo DEFINES+=GC_WANT_ROBOT >> src\gcconfig.pri
echo DEFINES+=_MATH_DEFINES_DEFINED >> src\gcconfig.pri

:: not replaced: secrets

:: line 361

qmake.exe build.pro -r -spec win32-msvc

cd qwt\
:: jom clean
jom -j1
:: eventually "jom clean" before may be usefull

:: line 778

cd ..
:: jom clean
jom -j4

:: line 2487

copy /Y src\release\GoldenCheetah.exe .\

cd src\release

:: copy dependencies
windeployqt --release GoldenCheetah.exe
copy /Y D:\git\GoldenCheetah\gc-ci-libs\10_Precompiled_DLL\usbexpress_3.5.1\USBXpress\USBXpress_API\Host\x64\SiUSBXp.dll .\
copy /Y D:\git\GoldenCheetah\gc-ci-libs\10_Precompiled_DLL\libsamplerate64\lib\libsamplerate-0.dll .\
copy /Y D:\git\GoldenCheetah\gc-ci-libs\10_Precompiled_DLL\VLC\win64\lib\libvlc*.dll .\
xcopy /Y /s /i /e /q D:\git\GoldenCheetah\gc-ci-libs\10_Precompiled_DLL\VLC\win64\plugins .\plugins
copy /Y D:\git\GoldenCheetah\gc-ci-libs\OpenSSL-v111-Win64\bin\lib*.dll .\
xcopy /Y /s /i /e /q C:\python37-x64 .
copy /Y C:\python37-x64\DLLs\*.* .\
copy /Y D:\git\vcpkg\installed\x64-windows\bin\gsl*.dll .\
echo "python37.zip import site" > python37._pth
copy /Y ..\..\python37.zip .\

copy /Y D:\git\GoldenCheetah\gc-ci-libs\OpenSSL-v111-Win64\Licence.txt .\"OpenSSL License.txt"
copy /Y C:\python37-x64\LICENSE.txt .\"PYTHON LICENSE.txt"

copy ..\Resources\win32\ReadMe.txt .\
echo GoldenCheetah is licensed under the GNU General Public License v2 > license.txt
echo. >> license.txt
type ..\..\COPYING >> license.txt


copy /Y ..\Resources\win32\ReadMe.txt .\
copy /Y ..\Resources\win32\gc.ico .\

copy /Y ..\Resources\win32\GC3.6-Dev-Master-W64-QT5.nsi .\

:: Build the installer
makensis GC3.6-Dev-Master-W64-QT5.nsi
copy /Y GoldenCheetah_v3.6-DEV_64bit_Windows.exe ..\..\GoldenCheetah_v3.6-DEV_x64.exe

d:
cd \git\GoldenCheetah

