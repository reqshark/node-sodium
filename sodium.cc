#include "nan.h"

using v8::FunctionTemplate;
using v8::Function;
using v8::Number;
using v8::Boolean;
using v8::String;
using v8::Object;
using v8::Value;
using v8::Local;

using Nan::HandleScope;
using Nan::MaybeLocal;
using Nan::NewBuffer;
using Nan::Callback;
using Nan::Maybe;
using Nan::Set;
using Nan::New;
using Nan::To;

extern "C" {
#include <sodium.h>
}

#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ref.h"

NAN_METHOD(test){
  if (sodium_init() == -1)
    abort();
  printf("Using libsodium %s\nGenerating nonces..\n", sodium_version_string());
  printf("test pass\n");
}

#define EXPORT_METHOD(C, S)                                                    \
  Set(C, New(#S).ToLocalChecked(),                                   \
    Nan::GetFunction(New<FunctionTemplate>(S)).ToLocalChecked());

NAN_MODULE_INIT(Init) {
  HandleScope scope;

  EXPORT_METHOD(target, test);
}

NODE_MODULE(sodium, Init)
