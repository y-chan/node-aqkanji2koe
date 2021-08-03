// AqKanji2Koe
typedef void *(*CREATE)(const char *pathDic, int *pErr);
typedef void *(*CREATE_PTR)(const void *pSysDic, const void *pUserDic, int *pErr);

typedef int (*CONVERT)(void *hAqKanji2Koe, const char *kanji, char *koe, int nBufKoe);
typedef int (*CONVERT_UTF16)(void *hAqKanji2Koe, const char16_t *kanji, char16_t *koe, int nBufKoe);

typedef void (*RELEASE)(void *hAqKanji2Koe);
typedef int (*SETKEY)(const char *devKey);

// AqUsrDic
typedef int (*IO)(const char *pathUserDic, const char *pathDicCsv);
typedef int (*CHECK)(const char *surface, const char *yomi, int posCode);

typedef const char *(*GETERROR)();
