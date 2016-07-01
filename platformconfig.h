#pragma once
#define CCPLATFORM_UNKNOWN		0
#define CCPLATFORM_WIN32		1
#define CCPLATFORM_LINUX		2

#define CCTARGET_PLATFORM		CCPLATFORM_UNKNOWN

//////////////////////////////////////////////////////////////////////////
// 平台配置
//////////////////////////////////////////////////////////////////////////

// win32
#if defined(_WIN32)
#undef  CCTARGET_PLATFORM
#define CCTARGET_PLATFORM         CCPLATFORM_WIN32
#endif

// linux
#if defined(__linux__) && !defined(__APPLE__)
#undef  CCTARGET_PLATFORM
#define CCTARGET_PLATFORM         CCPLATFORM_LINUX
#endif

#if ! CCTARGET_PLATFORM
	#error  "Cannot recognize the target platform; are you targeting an unsupported platform?"
#endif 
