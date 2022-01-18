#pragma once

#ifndef _INC_WINDOWS
    #include <windows.h>
#endif

BOOL APIENTRY
WonTransparentBlt(HDC hdc, INT x, INT y, INT cx, INT cy,
                  HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
                  UINT clrTransparent);

#if defined(_WONVER) && (_WONVER < 0x0500)
    #define TransparentBlt WonTransparentBlt
    #define GdiTransparentBlt WonTransparentBlt
#endif
