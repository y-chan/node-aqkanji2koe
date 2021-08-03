#include "win.h"

AqKanji2Koe::AqKanji2Koe(const std::string aqKanji2KoeLibraryPath, const std::string aqUsrDicLibraryPath)
{
    HMODULE aqKanji2KoeHandler = LoadLibrary(aqKanji2KoeLibraryPath.c_str());
    if (aqKanji2KoeHandler == nullptr) {
        throw std::exception("failed load library(AqKanji2Koe)");
    }
    FARPROC AqKanji2Koe_Create = GetProcAddress(aqKanji2KoeHandler, "AqKanji2Koe_Create");
    FARPROC AqKanji2Koe_Create_Ptr = GetProcAddress(aqKanji2KoeHandler, "AqKanji2Koe_Create_Ptr");
    FARPROC AqKanji2Koe_Release = GetProcAddress(aqKanji2KoeHandler, "AqKanji2Koe_Release");
    FARPROC AqKanji2Koe_Convert_utf8 = GetProcAddress(aqKanji2KoeHandler, "AqKanji2Koe_Convert_utf8");
    FARPROC AqKanji2Koe_Convert_utf16 = GetProcAddress(aqKanji2KoeHandler, "AqKanji2Koe_Convert_utf16");
    FARPROC AqKanji2Koe_Convert_sjis = GetProcAddress(aqKanji2KoeHandler, "AqKanji2Koe_Convert_sjis");
    FARPROC AqKanji2Koe_SetDevKey = GetProcAddress(aqKanji2KoeHandler, "AqKanji2Koe_SetDevKey");
    if (
        AqKanji2Koe_Create == nullptr ||
        AqKanji2Koe_Create_Ptr == nullptr ||
        AqKanji2Koe_Release == nullptr ||
        AqKanji2Koe_Convert_utf8 == nullptr ||
        AqKanji2Koe_Convert_utf16 == nullptr ||
        AqKanji2Koe_Convert_sjis == nullptr ||
        AqKanji2Koe_SetDevKey ==nullptr
    ) {
        throw std::exception("loading library is succeeded, but can't found needed functions(AqKanji2Koe)");
    }

    HMODULE aqUsrDicHandler = LoadLibrary(aqUsrDicLibraryPath.c_str());
    if (aqUsrDicHandler == nullptr) {
        throw std::exception("failed load library(AqUsrDic)");
    }
    void *AqUsrDic_Import = GetProcAddress(aqUsrDicHandler, "AqUsrDic_Import");
    void *AqUsrDic_Export = GetProcAddress(aqUsrDicHandler, "AqUsrDic_Export");
    void *AqUsrDic_Check = GetProcAddress(aqUsrDicHandler, "AqUsrDic_Check");
    void *AqUsrDic_GetLastError = GetProcAddress(aqUsrDicHandler, "AqUsrDic_GetLastError");
    if (
        AqUsrDic_Import == nullptr ||
        AqUsrDic_Export == nullptr ||
        AqUsrDic_Check == nullptr ||
        AqUsrDic_GetLastError == nullptr
    ) {
        throw std::exception("loading library is succeeded, but can't found needed functions(AqUsrDic)");
    }

    m_aqKanji2KoeHandler = aqKanji2KoeHandler;
    m_aqUsrDicHandler = aqUsrDicHandler;
}

AqKanji2Koe::~AqKanji2Koe()
{
	FreeLibrary(m_aqKanji2KoeHandler);
	FreeLibrary(m_aqUsrDicHandler);
}

void *AqKanji2Koe::AqKanji2Koe_Create(const char *pathDic, int *pErr)
{
	CREATE Create = (CREATE)GetProcAddress(m_aqKanji2KoeHandler, "AqKanji2Koe_Create");
	return Create(pathDic, pErr);
}

void *AqKanji2Koe::AqKanji2Koe_Create_Ptr(const void *pSysDic, const void *pUserDic, int *pErr)
{
	CREATE_PTR Create = (CREATE_PTR)GetProcAddress(m_aqKanji2KoeHandler, "AqKanji2Koe_Create_Ptr");
	return Create(pSysDic, pUserDic, pErr);
}

void AqKanji2Koe::AqKanji2Koe_Release(void *hAqKanji2Koe)
{
	RELEASE Release = (RELEASE)GetProcAddress(m_aqKanji2KoeHandler, "AqKanji2Koe_Release");
	return Release(hAqKanji2Koe);
}

int AqKanji2Koe::AqKanji2Koe_Convert_utf8(void *hAqKanji2Koe, const char *kanji, char *koe, int nBufKoe)
{
	CONVERT Convert = (CONVERT)GetProcAddress(m_aqKanji2KoeHandler, "AqKanji2Koe_Convert_utf8");
	return Convert(hAqKanji2Koe, kanji, koe, nBufKoe);
}

int AqKanji2Koe::AqKanji2Koe_Convert_utf16(void *hAqKanji2Koe, const char16_t *kanji, char16_t *koe, int nBufKoe)
{
	CONVERT_UTF16 Convert = (CONVERT_UTF16)GetProcAddress(m_aqKanji2KoeHandler, "AqKanji2Koe_Convert_utf16");
	return Convert(hAqKanji2Koe, kanji, koe, nBufKoe);
}

int AqKanji2Koe::AqKanji2Koe_Convert_sjis(void *hAqKanji2Koe, const char *kanji, char *koe, int nBufKoe)
{
	CONVERT Convert = (CONVERT)GetProcAddress(m_aqKanji2KoeHandler, "AqKanji2Koe_Convert_sjis");
	return Convert(hAqKanji2Koe, kanji, koe, nBufKoe);
}

int AqKanji2Koe::AqKanji2Koe_SetDevKey(const char *devKey)
{
	SETKEY SetKey = (SETKEY)GetProcAddress(m_aqKanji2KoeHandler, "AqKanji2Koe_SetDevKey");
	return SetKey(devKey);
}

int AqKanji2Koe::AqUsrDic_Import(const char *pathUserDic, const char *pathDicCsv)
{
	IO Import = (IO)GetProcAddress(m_aqUsrDicHandler, "AqUsrDic_Import");
	return Import(pathUserDic, pathDicCsv);
}

int AqKanji2Koe::AqUsrDic_Export(const char *pathUserDic, const char *pathDicCsv)
{
	IO Export = (IO)GetProcAddress(m_aqUsrDicHandler, "AqUsrDic_Export");
	return Export(pathUserDic, pathDicCsv);
}

int AqKanji2Koe::AqUsrDic_Check(const char *surface, const char *yomi, int posCode)
{
	CHECK Check = (CHECK)GetProcAddress(m_aqUsrDicHandler, "AqUsrDic_Check");
	return Check(surface, yomi, posCode);
}

const char *AqKanji2Koe::AqUsrDic_GetLastError()
{
	GETERROR GetError = (GETERROR)GetProcAddress(m_aqUsrDicHandler, "AqUsrDic_GetLastError");
	return GetError();
}
