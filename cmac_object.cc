#include "cmac_object.h"

using namespace v8;

Nan::Persistent<Function> CMacObject::constructor;

CMacObject::CMacObject() {
   ctx_ = (AES_CMAC_CTX*)calloc(1, sizeof(AES_CMAC_CTX));
   AES_CMAC_Init(ctx_);
}

CMacObject::~CMacObject() {
    if(ctx_) {
        free(ctx_);
    }
}

void CMacObject::Init(Local<Object> exports) {   
    Nan::HandleScope scope;

    Local<FunctionTemplate> tpl = Nan::New<FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("CMacObject").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    Local<ObjectTemplate> itpl = tpl->InstanceTemplate();
    SetAccessor(itpl, Nan::New<String>("key").ToLocalChecked(),
                CMacObject::GetKey,
                CMacObject::SetKey);
    SetPrototypeMethod(tpl, "update", CMacObject::Update);
    SetPrototypeMethod(tpl, "final", CMacObject::Final);
    constructor.Reset(tpl->GetFunction());
    exports->Set(Nan::New("CMacObject").ToLocalChecked(), tpl->GetFunction()); 
}


void CMacObject::New(const Nan::FunctionCallbackInfo<Value>& info)
{
    if (info.IsConstructCall()) {
        CMacObject *obj = new CMacObject();
        obj->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    } else {
        Local<Function> cons = Nan::New<Function>(constructor);
        info.GetReturnValue().Set(cons->NewInstance(0, NULL));
    }
}

NAN_GETTER(CMacObject::GetKey) {
}

NAN_SETTER(CMacObject::SetKey) {
    CMacObject* obj =
            ObjectWrap::Unwrap<CMacObject>(info.Holder());
    const uint8_t *key = (uint8_t*)node::Buffer::Data(value);
    AES_CMAC_SetKey(obj->ctx_, key);
}

NAN_METHOD(CMacObject::Update) {
    CMacObject* obj =
            ObjectWrap::Unwrap<CMacObject>(info.Holder());
    if (info.Length() < 1) {
            Nan::ThrowTypeError("Wrong number of arguments");
                return;
    }
    Local<Object> bufferObj = info[0]->ToObject();
    uint8_t *data = (uint8_t*)node::Buffer::Data(bufferObj);
    uint32_t len = node::Buffer::Length(bufferObj);
    AES_CMAC_Update(obj->ctx_, data, len);
}

NAN_METHOD(CMacObject::Final) {
    CMacObject* obj =
            ObjectWrap::Unwrap<CMacObject>(info.Holder());
    uint8_t *digest = (uint8_t*)calloc(1, AES_CMAC_DIGEST_LENGTH * sizeof(uint8_t));
    AES_CMAC_Final(digest, obj->ctx_);
    info.GetReturnValue().Set(Nan::NewBuffer((char*)digest, AES_CMAC_DIGEST_LENGTH).ToLocalChecked());
}
