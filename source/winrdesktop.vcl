<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: winrdesktop - Win32 (WCE ARMV4) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\Temp\RSP118.tmp" with contents
[
/nologo /W3 /Zi /Od /D "DEBUG" /D "ARM" /D "_ARM_" /D "ARMV4" /D UNDER_CE=420 /D _WIN32_WCE=420 /D "WIN32_PLATFORM_PSPC=400" /D "UNICODE" /D "_UNICODE" /Fp"ARMV4Dbg/winrdesktop.pch" /YX /Fo"ARMV4Dbg/" /Fd"ARMV4Dbg/" /MC /c 
"D:\C-Source\test\win32-rdesktop\win32.c"
]
Creating command line "clarm.exe @C:\Temp\RSP118.tmp" 
Creating temporary file "C:\Temp\RSP119.tmp" with contents
[
commctrl.lib coredll.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:yes /pdb:"ARMV4Dbg/winrdesktop.pdb" /debug /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"ARMV4Dbg/winrdesktop.exe" /subsystem:windowsce,4.20 /align:"4096" /MACHINE:ARM 
".\ARMV4Dbg\bitmap.obj"
".\ARMV4Dbg\bsops.obj"
".\ARMV4Dbg\cache.obj"
".\ARMV4Dbg\channels.obj"
".\ARMV4Dbg\iso.obj"
".\ARMV4Dbg\licence.obj"
".\ARMV4Dbg\mcs.obj"
".\ARMV4Dbg\mppc.obj"
".\ARMV4Dbg\orders.obj"
".\ARMV4Dbg\pstcache.obj"
".\ARMV4Dbg\rdp.obj"
".\ARMV4Dbg\rdp5.obj"
".\ARMV4Dbg\secure.obj"
".\ARMV4Dbg\ssl_calls.obj"
".\ARMV4Dbg\tcp.obj"
".\ARMV4Dbg\uimain.obj"
".\ARMV4Dbg\win32.obj"
]
Creating command line "link.exe @C:\Temp\RSP119.tmp"
<h3>Output Window</h3>
Compiling...
win32.c
D:\C-Source\test\win32-rdesktop\win32.c(875) : warning C4133: 'function' : incompatible types - from 'char [32]' to 'const unsigned short *'
D:\C-Source\test\win32-rdesktop\win32.c(896) : warning C4133: 'function' : incompatible types - from 'char [38]' to 'const unsigned short *'
D:\C-Source\test\win32-rdesktop\win32.c(898) : warning C4133: 'function' : incompatible types - from 'char [32]' to 'const unsigned short *'
D:\C-Source\test\win32-rdesktop\win32.c(907) : warning C4133: 'function' : incompatible types - from 'char [30]' to 'const unsigned short *'
D:\C-Source\test\win32-rdesktop\win32.c(922) : warning C4133: 'function' : incompatible types - from 'char [17]' to 'const unsigned short *'
Linking...
rdp.obj : warning LNK1166: cannot adjust code at offset=0x00001000, rva=0x000195C4




<h3>Results</h3>
winrdesktop.exe - 0 error(s), 6 warning(s)
</pre>
</body>
</html>
