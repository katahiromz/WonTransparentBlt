#include <windows.h>
#include "WonTransparentBlt.h"

BOOL APIENTRY
WonTransparentBlt(HDC hdc, INT x, INT y, INT cx, INT cy,
                  HDC hdcSrc, INT xSrc, INT ySrc, INT cxSrc, INT cySrc,
                  UINT clrTransparent)
{
    register UINT c;
    register LPDWORD pdw, pdwSrc;
    PVOID pvBits, pvBitsSrc;
    DWORD dw;
    HDC hdcMem, hdcMemSrc;
    BITMAPINFO bi;
    HBITMAP hbm, hbmSrc;
    HGDIOBJ hbmOld, hbmSrcOld;

    ZeroMemory(&bi.bmiHeader, sizeof(BITMAPINFOHEADER));
    bi.bmiHeader.biSize         = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth        = cx;
    bi.bmiHeader.biHeight       = cy;
    bi.bmiHeader.biPlanes       = 1;
    bi.bmiHeader.biBitCount     = 32;
    hbm = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &pvBits, NULL, 0);
    if (hbm != NULL)
    {
        hbmSrc = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, &pvBitsSrc, 
                                  NULL, 0);
        if (hbmSrc != NULL)
        {
            hdcMem = CreateCompatibleDC(hdc);
            if (hdcMem != NULL)
            {
                hdcMemSrc = CreateCompatibleDC(hdc);
                if (hdcMemSrc != NULL)
                    goto success;
            }
            DeleteObject(hbmSrc);
        }
        DeleteObject(hbm);
    }
    return FALSE;

success:
    if (GetStretchBltMode(hdcMemSrc) != HALFTONE);
        SetStretchBltMode(hdcMemSrc, COLORONCOLOR);

    hbmOld = SelectObject(hdcMem, hbm);
    hbmSrcOld = SelectObject(hdcMemSrc, hbmSrc);
    BitBlt(hdcMem, 0, 0, cx, cy, hdc, x, y, SRCCOPY);
    StretchBlt(hdcMemSrc, 0, 0, cx, cy,
               hdcSrc, xSrc, ySrc, cxSrc, cySrc, SRCCOPY);
    SelectObject(hdcMem, hbmOld);
    SelectObject(hdcMemSrc, hbmSrcOld);

    c = cx * cy;
    pdw = (LPDWORD)pvBits;
    pdwSrc = (LPDWORD)pvBitsSrc;
    clrTransparent = RGB(
        GetBValue(clrTransparent),
        GetGValue(clrTransparent),
        GetRValue(clrTransparent));
    while(c--)
    {
        dw = *pdwSrc;
        if ((dw & 0xFFFFFF) != clrTransparent)
            *pdw = dw;
        pdw++;
        pdwSrc++;
    }

    hbmOld = SelectObject(hdcMem, hbm);
    BitBlt(hdc, x, y, cx, cy, hdcMem, 0, 0, SRCCOPY);
    SelectObject(hdcMem, hbmOld);

    DeleteObject(hbm);
    DeleteObject(hbmSrc);
    DeleteDC(hdcMem);
    DeleteDC(hdcMemSrc);
    return TRUE;
}
