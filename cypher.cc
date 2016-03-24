#include <nan.h>
#include "cmac_object.h"
#include "aes_object.h"

using namespace v8;

NAN_MODULE_INIT(InitAll) {
      CMacObject::Init(target);
      AesObject::Init(target);
}

NODE_MODULE(cypher, InitAll)

