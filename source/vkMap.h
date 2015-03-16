//vkMap.h
#pragma once
#ifndef _VKMAPH_
#define _VKMAPH_

#include <windows.h>

typedef struct {
	byte	vkeyValue;
	TCHAR*	keyname;
	}vkMAP;

#endif

extern vkMAP myVKmap[];