

// stdafx.h : fichier Include pour les fichiers Include système standard,
// ou les fichiers Include spécifiques aux projets qui sont utilisés fréquemment,
// et sont rarement modifiés
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclure les en-têtes Windows rarement utilisés
// Fichiers d'en-tête Windows :
#include <windows.h>
#include <windowsx.h>
#include <Shobjidl.h>

// TODO: faites référence ici aux en-têtes supplémentaires nécessaires au programme

#include <commdlg.h>
#include <Prntvpt.h>

#include <xpsobjectmodel_1.h>
#include <DocumentTarget.h>

#pragma comment(lib, "comdlg32")
#pragma comment(lib, "Prntvpt")

#include <objbase.h>
#include <shobjidl.h>     
#include <shlwapi.h>

#pragma comment(lib, "Shlwapi")

#include <Shlobj.h>

#include <wincodec.h>

#pragma comment(lib, "Windowscodecs")

#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1helper.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#include <d3d11.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

// Fichiers d'en-tête C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <new>
#include <string>
#include <vector>
#include <unordered_map>

///////////////////

extern int errorinitall(const wchar_t *errmessage);
extern int initall();
extern int infiniteloop();
extern HINSTANCE	gI;
#include "window1.h"
#include "window2.h"
#include "window4.h"
#include "window3.h"
#include "print1.h"
#include "file1.h"
#include "toys1.h"

#include "tilecache1.h"



#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")