


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Application Define
#pragma once
#if defined(_UNICODE) && !defined(RC_INVOKED)
#define _STR(x) L##x
#else
#define _STR(x) x
#endif
#define STR(x) _STR(x)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Version Information
#define _MakeStr(v)				#v
#define _MakeVer(a, b, c, d)	_MakeStr(a.b.c.d)

#define VER_Major				2
#define VER_Minor				0
#define VER_Release				686 
#define VER_Build				1777
#define VER_Version				MAKELONG(MAKEWORD(VER_Major, VER_Minor), VER_Release)
#define STR_Version				STR(_MakeVer(VER_Major, VER_Minor, VER_Release, VER_Build))

#define STR_BuildDate			STR(__DATE__)
#define STR_BuildTime			STR(__TIME__)
#define STR_BuildStamp			STR(__DATE__) STR(" ") STR(__TIME__)

#if defined(_M_IX86)
#define STR_Manifest			STR("x86")
#ifdef _UNICODE
#define STR_VersionStamp		STR_Version STR(" X86U")
#else
#define STR_VersionStamp		STR_Version STR(" X86")
#endif
#elif defined(_M_X64)
#define STR_Manifest			"amd64"
#define STR_VersionStamp		STR_Version STR(" X64")
#elif defined(WIN32_PLATFORM_WFSP)
#define STR_VersionStamp		STR_Version //STR(" SP")
#elif defined(WIN32_PLATFORM_PSPC)
#define STR_VersionStamp		STR_Version //STR(" PPC")
#else
#define STR_VersionStamp		STR_Version
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Program information
#define STR_AppName				STR("CeleCmd")
#define STR_AppDesc				STR("Celerity Command Interpreter")

#define STR_Author				STR("Yonsm")
#define STR_Company				STR("Yonsm.NET")

#define STR_Web					STR("WWW.Yonsm.NET")
#define STR_Mail				STR("Yonsm@163.com")
#define STR_Phone				STR("13957162565")

#define STR_WebCmd				STR("http://") STR_Web STR("/") STR_AppName
#define STR_MailCmd				STR("mailto:") STR_Mail STR("?subject=") STR_AppName
#define STR_PhoneCmd			STR("tmail.exe"), STR("-service \"SMS\" -to \"") STR_Phone STR("\" -body \"") STR_AppName STR("\"")

#define STR_Copyright			STR("Copyright (C) 2007-2009 ") STR_Company STR(". All Rights Reserved.")
#define STR_Comments			STR("Powered by ") STR_Author STR("\n") STR_Mail STR("\n") STR_Web
#define STR_About				STR_AppName STR(" ") STR_VersionStamp STR("\n\n") STR_BuildStamp STR("\n\n") STR_Comments STR("\n\n") STR_Copyright
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
