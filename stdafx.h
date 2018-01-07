/* 2018 leresidue
	© 2018 Frédérique Brisson-Lambert
*/


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


// TODO: faites référence ici aux en-têtes supplémentaires nécessaires au programme

#include <commdlg.h>


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

// Fichiers d'en-tête C RunTime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <new>
#include <string>
#include <vector>


///////////////////

extern int initall();
extern int infiniteloop();
extern HINSTANCE	gI;
#include "window1.h"
#include "window2.h"
#include "file1.h"
#include "toys1.h"

#include "tilecache1.h"



#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")