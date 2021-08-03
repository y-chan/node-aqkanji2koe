#ifndef LINUX_H
#define LINUX_H

#include <napi.h>
#include <string>

#include "common.h"

class AqKanji2Koe {
public:
    AqKanji2Koe(const std::string aqKanji2KoeLibraryPath, const std::string aqUsrDicLibraryPath);
    ~AqKanji2Koe();

    void *AqKanji2Koe_Create(const char *pathDic, int *pErr);
    void *AqKanji2Koe_Create_Ptr(const void *pSysDic, const void *pUserDic, int *pErr);

    void AqKanji2Koe_Release(void *hAqKanji2Koe);

    int AqKanji2Koe_Convert(void *hAqKanji2Koe, const char *kanji, char *koe, int nBufKoe);

    int AqKanji2Koe_SetDevKey(const char *devKey);

    int AqUsrDic_Import(const char *pathUserDic, const char *pathDicCsv);
    int AqUsrDic_Export(const char *pathUserDic, const char *pathDicCsv);
    int AqUsrDic_Check(const char *surface, const char *yomi, int posCode);

    const char *AqUsrDic_GetLastError();
private:
    void *m_aqKanji2KoeHandler;
    void *m_aqUsrDicHandler;
};

#endif // LINUX_H
