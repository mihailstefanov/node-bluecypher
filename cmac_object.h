#ifndef CMAC_OBJECT_H
#define CMAC_OBJECT_H

#include <nan.h>

extern "C" { 
    #include "cmac.h"
}

class CMacObject : public node::ObjectWrap {
    
    public:
        static void Init(v8::Local<v8::Object> exports);
        static NAN_GETTER(GetKey);
        static NAN_SETTER(SetKey);
        static NAN_METHOD(Update);
        static NAN_METHOD(Final);
    private:
        explicit CMacObject();
        ~CMacObject();

        static Nan::Persistent<v8::Function> constructor;
        static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
        
        AES_CMAC_CTX *ctx_;
};


#endif
