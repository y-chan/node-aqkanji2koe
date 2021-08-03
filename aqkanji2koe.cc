#include <napi.h>
#include <string>

#include "aqkanji2koe.h"

using namespace Napi;

Napi::Object AqKanji2KoeWrapper::NewInstance(Napi::Env env, const Napi::CallbackInfo& info)
{
    Napi::EscapableHandleScope scope(env);
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "missing arguments").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }

    if (!info[0].IsString() || !info[1].IsString() || (info.Length() > 2 && !info[2].IsString())) {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }

    std::initializer_list<napi_value> initArgList = { info[0], info[1] };
    if (info.Length() > 2) {
        initArgList = { info[0], info[1], info[2] };
    }

    Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New(initArgList);
    return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Object AqKanji2KoeWrapper::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env, "AqKanji2KoeWrapper", {
            InstanceMethod("AqKanji2KoeCreate", &AqKanji2KoeWrapper::AqKanji2KoeCreate),
            InstanceMethod("AqKanji2KoeCreatePtr", &AqKanji2KoeWrapper::AqKanji2KoeCreatePtr),
            InstanceMethod("AqKanji2KoeConvertUtf8", &AqKanji2KoeWrapper::AqKanji2KoeConvertUtf8),
#if defined(_WIN32) || defined(_WIN64)
            InstanceMethod("AqKanji2KoeConvertSjis", &AqKanji2KoeWrapper::AqKanji2KoeConvertSjis),
            InstanceMethod("AqKanji2KoeConvertUtf16", &AqKanji2KoeWrapper::AqKanji2KoeConvertUtf16),
#endif
            InstanceMethod("AqKanji2KoeSetDevKey", &AqKanji2KoeWrapper::AqKanji2KoeSetDevKey),
            InstanceMethod("AqUsrDicImport", &AqKanji2KoeWrapper::AqUsrDicImport),
            InstanceMethod("AqUsrDicExport", &AqKanji2KoeWrapper::AqUsrDicExport),
            InstanceMethod("AqUsrDicCheck", &AqKanji2KoeWrapper::AqUsrDicCheck),
        });

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("AqKanji2KoeWrapper", func);
    return exports;
}

AqKanji2KoeWrapper::AqKanji2KoeWrapper(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<AqKanji2KoeWrapper>(info)
{
    Napi::Env env = info.Env();
    std::string aqKanji2KoeLibraryPath = info[0].As<Napi::String>().Utf8Value();
    std::string aqUsrDicLibraryPath = info[1].As<Napi::String>().Utf8Value();

    try {
        m_aqkanji2koe = new AqKanji2Koe(aqKanji2KoeLibraryPath, aqUsrDicLibraryPath);
    } catch (std::exception& err) {
        Napi::Error::New(env, err.what()).ThrowAsJavaScriptException();
        return;
    }

    if (info.Length() == 3) {
        std::string aqDicPath = info[2].As<Napi::String>().Utf8Value();
        int err;
        void* hAqKanji2Koe = m_aqkanji2koe->AqKanji2Koe_Create(aqDicPath.c_str(), &err);
        CheckAqKanji2KoeError(env, err);
        h_aqkanji2koe = hAqKanji2Koe;
        pathDic = aqDicPath;
    }
}

AqKanji2KoeWrapper::~AqKanji2KoeWrapper()
{
    ReleaseHandler();
    delete m_aqkanji2koe;
    m_aqkanji2koe = nullptr;
}

void AqKanji2KoeWrapper::ReleaseHandler()
{
    if (h_aqkanji2koe != nullptr) {
        m_aqkanji2koe->AqKanji2Koe_Release(h_aqkanji2koe);
        h_aqkanji2koe = nullptr;
    }
    if (pathDic.length() != 0) {
        pathDic = "";
    }
}

bool AqKanji2KoeWrapper::ValidateConvertArgs(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "missing arguments").ThrowAsJavaScriptException();
        return true;
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return true;
    }

    if (h_aqkanji2koe == nullptr) {
        Napi::TypeError::New(env, "please execute \"AqKanji2KoeCreate\" function").ThrowAsJavaScriptException();
        return true;
    }
    return false;
}

bool AqKanji2KoeWrapper::ValidateUsrDicArgs(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "missing arguments").ThrowAsJavaScriptException();
        return true;
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return true;
    }

    if (h_aqkanji2koe == nullptr && pathDic.length() == 0) {
        Napi::TypeError::New(env, "please execute \"AqKanji2KoeCreate\" function").ThrowAsJavaScriptException();
        return true;
    }

    if (pathDic.length() == 0) {
        Napi::TypeError::New(env, "please regenerate class").ThrowAsJavaScriptException();
        return true;
    }
    return false;
}

bool AqKanji2KoeWrapper::CheckAqKanji2KoeError(const Napi::Env env, int err)
{
    if (err != 0) {
        char errorMsg[20];
        sprintf(errorMsg, "ERR: %d", err);
        Napi::Error::New(env, errorMsg).ThrowAsJavaScriptException();
        return true;
    }
    return false;
}

bool AqKanji2KoeWrapper::CheckAqUsrDicError(const Napi::Env env, int err)
{
    if (err != 0) {
        Napi::Error::New(env, m_aqkanji2koe->AqUsrDic_GetLastError()).ThrowAsJavaScriptException();
        return true;
    }
    return false;
}

Napi::Value AqKanji2KoeWrapper::AqKanji2KoeCreate(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "missing arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string pathDir = info[0].As<Napi::String>().Utf8Value();
    int err;
    void *hAqKanji2Koe = m_aqkanji2koe->AqKanji2Koe_Create(pathDir.c_str(), &err);
    if (CheckAqKanji2KoeError(env, err)) {
        return env.Null();
    }
    ReleaseHandler();
    h_aqkanji2koe = hAqKanji2Koe;
    pathDic = pathDir;
    return env.Null();
}

Napi::Value AqKanji2KoeWrapper::AqKanji2KoeCreatePtr(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "missing arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber() || !info[1].IsNumber()) {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    void *pSysDic = (void *)info[0].As<Napi::Number>().Int64Value();
    void *pUserDic = (void *)info[1].As<Napi::Number>().Int64Value();
    int err;
    void* hAqKanji2Koe = m_aqkanji2koe->AqKanji2Koe_Create_Ptr(pSysDic, pUserDic, &err);
    if (CheckAqKanji2KoeError(env, err)) {
        return env.Null();
    }
    ReleaseHandler();
    h_aqkanji2koe = hAqKanji2Koe;
    return env.Null();
}

Napi::Value AqKanji2KoeWrapper::AqKanji2KoeConvertUtf8(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (ValidateConvertArgs(info)) {
        return env.Null();
    }

    std::string kanji = info[0].As<Napi::String>().Utf8Value();
    // 念のため、元の文字列の4倍の長さ分の配列を作る
    int size = kanji.length() * 4;
    char* koe = (char *)malloc(sizeof(char) * size);
    memset(koe, 0, sizeof(char) * size);

    int err;
#if defined(_WIN32) || defined(_WIN64)
    err = m_aqkanji2koe->AqKanji2Koe_Convert_utf8(h_aqkanji2koe, kanji.c_str(), koe, size);
#else
    err = m_aqkanji2koe->AqKanji2Koe_Convert(hAqKanji2Koe, kanji.c_str(), koe, size);
#endif
    if (CheckAqKanji2KoeError(env, err)) {
        return env.Null();
    }
    Napi::String koeString = Napi::String::New(env, koe);

    return koeString;
}

#if defined(_WIN32) || defined(_WIN64)
Napi::Value AqKanji2KoeWrapper::AqKanji2KoeConvertSjis(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (ValidateConvertArgs(info)) {
        return env.Null();
    }

    std::string kanji = info[0].As<Napi::String>().Utf8Value();
    // 念のため、元の文字列の4倍の長さ分の配列を作る
    int size = kanji.length() * 4;
    char *koe = (char *)malloc(sizeof(char) * size);
    memset(koe, 0, sizeof(char) * size);

    int err = m_aqkanji2koe->AqKanji2Koe_Convert_sjis(h_aqkanji2koe, kanji.c_str(), koe, size);
    if (CheckAqKanji2KoeError(env, err)) {
        return env.Null();
    }
    Napi::String koeString = Napi::String::New(env, koe);

    return koeString;
}

Napi::Value AqKanji2KoeWrapper::AqKanji2KoeConvertUtf16(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (ValidateConvertArgs(info)) {
        return env.Null();
    }

    std::u16string kanji = info[0].As<Napi::String>().Utf16Value();
    // 念のため、元の文字列の4倍の長さ分の配列を作る
    int size = kanji.length() * 4;
    char16_t *koe = (char16_t *)malloc(sizeof(char16_t) * size);
    memset(koe, 0, sizeof(char16_t) * size);

    int err = m_aqkanji2koe->AqKanji2Koe_Convert_utf16(h_aqkanji2koe, kanji.c_str(), koe, size);
    if (CheckAqKanji2KoeError(env, err)) {
        return env.Null();
    }
    Napi::String koeString = Napi::String::New(env, koe);

    return koeString;
}
#endif

Napi::Value AqKanji2KoeWrapper::AqKanji2KoeSetDevKey(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "missing arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    std::string key = info[0].As<Napi::String>().Utf8Value();
    int success = m_aqkanji2koe->AqKanji2Koe_SetDevKey(key.c_str());
    return Napi::Boolean::New(env, !(bool)success);
}

Napi::Value AqKanji2KoeWrapper::AqUsrDicImport(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (ValidateUsrDicArgs(info)) {
        return Napi::Boolean::New(env, false);
    }

    std::string pathDicCsv = info[0].As<Napi::String>().Utf8Value();
    int err = m_aqkanji2koe->AqUsrDic_Import(pathDic.c_str(), pathDicCsv.c_str());
    if (CheckAqUsrDicError(env, err)) {
        return Napi::Boolean::New(env, false);
    }
    return Napi::Boolean::New(env, true);
}

Napi::Value AqKanji2KoeWrapper::AqUsrDicExport(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (ValidateUsrDicArgs(info)) {
        return Napi::Boolean::New(env, false);
    }

    std::string pathDicCsv = info[0].As<Napi::String>().Utf8Value();
    int err = m_aqkanji2koe->AqUsrDic_Export(pathDic.c_str(), pathDicCsv.c_str());
    if (CheckAqUsrDicError(env, err)) {
        return Napi::Boolean::New(env, false);
    }
    return Napi::Boolean::New(env, true);
}

Napi::Value AqKanji2KoeWrapper::AqUsrDicCheck(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 3) {
        Napi::TypeError::New(env, "missing arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString() || !info[1].IsString() || !info[2].IsNumber()) {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string surface = info[0].As<Napi::String>().Utf8Value();
    std::string yomi = info[1].As<Napi::String>().Utf8Value();
    int32_t posCode = info[2].As<Napi::Number>().Int32Value();
    int err = m_aqkanji2koe->AqUsrDic_Check(surface.c_str(), yomi.c_str(), posCode);
    if (err != 0) {
        return Napi::String::New(env, m_aqkanji2koe->AqUsrDic_GetLastError());
    }
    return env.Null();
}

Napi::Object CreateObject(const Napi::CallbackInfo& info) {
    return AqKanji2KoeWrapper::NewInstance(info.Env(), info);
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports) {
    Napi::Object new_exports = Napi::Function::New(env, CreateObject);
    return AqKanji2KoeWrapper::Init(env, new_exports);
}

NODE_API_MODULE(aqkanji2koe, Initialize)
