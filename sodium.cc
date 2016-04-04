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

#define MAX_INPUT_LEN 4096

static unsigned char *nonce[crypto_box_NONCEBYTES];
static char nhex[crypto_box_NONCEBYTES * 2 + 1];

/* nbuf uses a pointer kept back in the binding */
static char *nbuf() {
  size_t sz = crypto_box_NONCEBYTES;
  randombytes_buf(&nonce, sz);
  sodium_bin2hex(nhex, sz * 2 + 1, (unsigned char *)nonce, sz);
  return nhex;
}

/* return a nonce hex to node */
NAN_METHOD(nstr){
  char *n = nbuf();
  info.GetReturnValue().Set(New(n).ToLocalChecked());
}

#define EXPORT_METHOD(C, S)                                                    \
  Set(C, New(#S).ToLocalChecked(),                                   \
    Nan::GetFunction(New<FunctionTemplate>(S)).ToLocalChecked());

NAN_MODULE_INIT(Init) {
  HandleScope scope;

  if (sodium_init() == -1)
    abort();

  EXPORT_METHOD(target, nstr);
}

NODE_MODULE(sodium, Init)
