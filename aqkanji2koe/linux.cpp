#include <dlfcn.h>

#include "linux.h"

AqKanji2Koe::AqKanji2Koe(const std::string aqKanji2KoeLibraryPath, const std::string aqUsrDicLibraryPath)
{
    void *aqKanji2KoeHandler = dlopen(aqKanji2KoeLibraryPath.c_str(), RTLD_LAZY);
    if (aqKanji2KoeHandler == nullptr) {
        throw std::runtime_error("failed load library(AqKanji2Koe)");
    }
    void *AqKanji2Koe_Create = dlsym(aqKanji2KoeHandler, "AqKanji2Koe_Create");
    void *AqKanji2Koe_Create_Ptr = dlsym(aqKanji2KoeHandler, "AqKanji2Koe_Create_Ptr");
    void *AqKanji2Koe_Release = dlsym(aqKanji2KoeHandler, "AqKanji2Koe_Release");
    void *AqKanji2Koe_Convert = dlsym(aqKanji2KoeHandler, "AqKanji2Koe_Convert");
    void *AqKanji2Koe_SetDevKey = dlsym(aqKanji2KoeHandler, "AqKanji2Koe_SetDevKey");
    if (
        AqKanji2Koe_Create == nullptr ||
        AqKanji2Koe_Create_Ptr == nullptr ||
        AqKanji2Koe_Release == nullptr ||
        AqKanji2Koe_Convert == nullptr ||
        AqKanji2Koe_SetDevKey ==nullptr
    ) {
        throw std::runtime_error("loading library is succeeded, but can't found needed functions(AqKanji2Koe)");
    }

    void *aqUsrDicHandler = dlopen(aqUsrDicLibraryPath.c_str(), RTLD_LAZY);
    if (aqUsrDicHandler == nullptr) {
        throw std::runtime_error("failed load library(AqUsrDic)");
    }
    void *AqUsrDic_Import = dlsym(aqUsrDicHandler, "AqUsrDic_Import");
    void *AqUsrDic_Export = dlsym(aqUsrDicHandler, "AqUsrDic_Export");
    void *AqUsrDic_Check = dlsym(aqUsrDicHandler, "AqUsrDic_Check");
    void *AqUsrDic_GetLastError = dlsym(aqUsrDicHandler, "AqUsrDic_GetLastError");
    if (
        AqUsrDic_Import == nullptr ||
        AqUsrDic_Export == nullptr ||
        AqUsrDic_Check == nullptr ||
        AqUsrDic_GetLastError ==nullptr
    ) {
        throw std::runtime_error("loading library is succeeded, but can't found needed functions(AqUsrDic)");
    }

    m_aqKanji2KoeHandler = aqKanji2KoeHandler;
    m_aqUsrDicHandler = aqUsrDicHandler;
}

AqKanji2Koe::~AqKanji2Koe()
{
	dlclose(m_aqKanji2KoeHandler);
	dlclose(m_aqUsrDicHandler);
}

void *AqKanji2Koe::AqKanji2Koe_Create(const char *pathDic, int *pErr)
{
	CREATE Create = (CREATE)dlsym(m_aqKanji2KoeHandler, "AqKanji2Koe_Create");
	return Create(pathDic, pErr);
}

void *AqKanji2Koe::AqKanji2Koe_Create_Ptr(const void *pSysDic, const void *pUserDic, int *pErr)
{
	CREATE_PTR Create = (CREATE_PTR)dlsym(m_aqKanji2KoeHandler, "AqKanji2Koe_Create_Ptr");
	return Create(pSysDic, pUserDic, pErr);
}

void AqKanji2Koe::AqKanji2Koe_Release(void *hAqKanji2Koe)
{
	RELEASE Release = (RELEASE)dlsym(m_aqKanji2KoeHandler, "AqKanji2Koe_Release");
	return Release(hAqKanji2Koe);
}

int AqKanji2Koe::AqKanji2Koe_Convert(void *hAqKanji2Koe, const char *kanji, char *koe, int nBufKoe)
{
	CONVERT Convert = (CONVERT)dlsym(m_aqKanji2KoeHandler, "AqKanji2Koe_Convert");
	return Convert(hAqKanji2Koe, kanji, koe, nBufKoe);
}

int AqKanji2Koe::AqKanji2Koe_SetDevKey(const char *devKey)
{
	SETKEY SetKey = (SETKEY)dlsym(m_aqKanji2KoeHandler, "AqKanji2Koe_SetDevKey");
	return SetKey(devKey);
}

int AqKanji2Koe::AqUsrDic_Import(const char *pathUserDic, const char *pathDicCsv)
{
	IO Import = (IO)dlsym(m_aqUsrDicHandler, "AqUsrDic_Import");
	return Import(pathUserDic, pathDicCsv);
}

int AqKanji2Koe::AqUsrDic_Export(const char *pathUserDic, const char *pathDicCsv)
{
	IO Export = (IO)dlsym(m_aqUsrDicHandler, "AqUsrDic_Export");
	return Export(pathUserDic, pathDicCsv);
}

int AqKanji2Koe::AqUsrDic_Check(const char *surface, const char *yomi, int posCode)
{
	CHECK Check = (CHECK)dlsym(m_aqUsrDicHandler, "AqUsrDic_Check");
	return Check(surface, yomi, posCode);
}

const char *AqKanji2Koe::AqUsrDic_GetLastError()
{
	GETERROR GetError = (GETERROR)dlsym(m_aqUsrDicHandler, "AqUsrDic_GetLastError");
	return GetError();
}
