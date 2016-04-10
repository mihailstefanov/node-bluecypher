#include "aes_object.h"

using namespace v8;

Nan::Persistent<Function> AesObject::constructor;

AesObject::AesObject() {
    ctx_ = (aes_context*)calloc(1, sizeof(aes_context));
}

AesObject::~AesObject() {
    if(ctx_) {
        free(ctx_);
    }
}

void AesObject::Init(Local<Object> exports) {
    Nan::HandleScope scope;

    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Aes").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    Local<ObjectTemplate> itpl = tpl->InstanceTemplate();
    SetAccessor(itpl, Nan::New<String>("key").ToLocalChecked(),
            AesObject::GetKey, 
            AesObject::SetKey);
    SetPrototypeMethod(tpl, "encrypt", AesObject::Encrypt);
    SetPrototypeMethod(tpl, "decrypt", AesObject::Decrypt);
    constructor.Reset(tpl->GetFunction());
    exports->Set(Nan::New("Aes").ToLocalChecked(), tpl->GetFunction());
}

void AesObject::New(const Nan::FunctionCallbackInfo<Value>& info) {
    if (info.IsConstructCall()) {
        AesObject *obj = new AesObject();
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        Local<Function> cons = Nan::New<Function>(constructor);
        info.GetReturnValue().Set(cons->NewInstance(0, NULL));
    }
}

NAN_GETTER(AesObject::GetKey) {
}

NAN_SETTER(AesObject::SetKey) {
    AesObject* obj =
        ObjectWrap::Unwrap<AesObject>(info.Holder());
    const uint8_t *key = (uint8_t*)node::Buffer::Data(value);
    uint32_t len = node::Buffer::Length(value);
    if(len <= 0) {
        Nan::ThrowTypeError("key is empty");
    }
    aes_set_key(key, len, obj->ctx_);
}

NAN_METHOD(AesObject::Encrypt) {
    AesObject* obj =
        ObjectWrap::Unwrap<AesObject>(info.Holder());
    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    Local<Object> bufferObj = info[0]->ToObject();
    uint8_t *in = (uint8_t*)node::Buffer::Data(bufferObj);
    uint32_t in_len = node::Buffer::Length(bufferObj);
    if (in_len != N_BLOCK) {
        Nan::ThrowTypeError("Wrong lenght of in block");
        return;
    }

    uint8_t *out = (uint8_t*)calloc(1, N_BLOCK);
    aes_encrypt(in, out, obj->ctx_);
    info.GetReturnValue().Set(Nan::NewBuffer((char*)out, N_BLOCK).ToLocalChecked());
}

NAN_METHOD(AesObject::Decrypt) {
    AesObject* obj =
        ObjectWrap::Unwrap<AesObject>(info.Holder());
    if (info.Length() < 1) {
        Nan::ThrowTypeError("Wrong number of arguments");
        return;
    }

    Local<Object> bufferObj = info[0]->ToObject();
    uint8_t *in = (uint8_t*)node::Buffer::Data(bufferObj);
    uint32_t in_len = node::Buffer::Length(bufferObj);
    if (in_len != N_BLOCK) {
        Nan::ThrowTypeError("Wrong lenght of in block");
        return;
    }

    uint8_t *out = (uint8_t*)calloc(1, N_BLOCK);
    aes_decrypt(in, out, obj->ctx_);
    info.GetReturnValue().Set(Nan::NewBuffer((char*)out, N_BLOCK).ToLocalChecked());
}
