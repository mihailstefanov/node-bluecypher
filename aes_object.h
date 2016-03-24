#ifndef AES_OBJECT_H
#define AES_OBJECT_H

#include <nan.h>

extern "C" {
    #include "aes.h"
}


class AesObject : public node::ObjectWrap {
    
    public:
        static void Init(v8::Local<v8::Object> exports);
        static NAN_GETTER(GetKey);
        static NAN_SETTER(SetKey);
        static NAN_METHOD(Encrypt);
    private:
        explicit AesObject();
        ~AesObject();

        static Nan::Persistent<v8::Function> constructor;
        static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

        aes_context *ctx_;
};

#endif
