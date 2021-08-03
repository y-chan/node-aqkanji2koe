#ifndef AQKANJI2KOE_H
#define AQKANJI2KOE_H

#include <napi.h>

#include "aqkanji2koe/common.h"
#if defined(_WIN32) || defined(_WIN64)
#include "aqkanji2koe/win.h"
#else
#include "aqkanji2koe/linux.h"
#endif

class AqKanji2KoeWrapper : public Napi::ObjectWrap<AqKanji2KoeWrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Env env, const Napi::CallbackInfo& info);

    AqKanji2KoeWrapper(const Napi::CallbackInfo& info);
    ~AqKanji2KoeWrapper();

    Napi::Value AqKanji2KoeCreate(const Napi::CallbackInfo& info);
    Napi::Value AqKanji2KoeCreatePtr(const Napi::CallbackInfo& info);

    Napi::Value AqKanji2KoeConvertUtf8(const Napi::CallbackInfo& info);
#if defined(_WIN32) || defined(_WIN64)
    Napi::Value AqKanji2KoeConvertSjis(const Napi::CallbackInfo& info);
    Napi::Value AqKanji2KoeConvertUtf16(const Napi::CallbackInfo& info);
#endif

    Napi::Value AqKanji2KoeSetDevKey(const Napi::CallbackInfo& info);

    Napi::Value AqUsrDicImport(const Napi::CallbackInfo& info);
    Napi::Value AqUsrDicExport(const Napi::CallbackInfo& info);
    Napi::Value AqUsrDicCheck(const Napi::CallbackInfo& info);

private:
    void ReleaseHandler();
    bool ValidateConvertArgs(const Napi::CallbackInfo& info);
    bool ValidateUsrDicArgs(const Napi::CallbackInfo& info);
    bool CheckAqKanji2KoeError(const Napi::Env env, int err);
    bool CheckAqUsrDicError(const Napi::Env env, int err);

    AqKanji2Koe* m_aqkanji2koe;
    void* h_aqkanji2koe = nullptr;
    std::string pathDic = "";
};

#endif // AQKANJI2KOE_H
