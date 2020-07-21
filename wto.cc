#include <node_api.h>
#include <assert.h>
#include <string>
#include <unistd.h>
#include "wtoexec.h"
#include <iostream>

napi_value Method(napi_env env, napi_callback_info info) {
  napi_status status;

  size_t argc = 2;
  napi_value args[2];
  status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
  assert(status == napi_ok);

  if (argc < 2) {
    napi_throw_type_error(env, nullptr, "Wrong number of arguments");
    return nullptr;
  }

  napi_valuetype valuetype0;
  status = napi_typeof(env, args[0], &valuetype0);
  assert(status == napi_ok);

  napi_valuetype valuetype1;
  status = napi_typeof(env, args[1], &valuetype1);
  assert(status == napi_ok);

  if (valuetype0 != napi_string || valuetype1 != napi_number) {
    napi_throw_type_error(env, nullptr, "Wrong arguments");
    return nullptr;
  }

  char buffer[100] = {0};
  size_t read;
  status = napi_get_value_string_utf8(env, args[0], buffer, sizeof(buffer), &read);
  assert(status == napi_ok);
  __atoe(buffer); // convert to EBCDIC

  int value = 0;
  status = napi_get_value_int32(env, args[1], &value);
  assert(status == napi_ok);

  int rc = 0;
  rc = WTOEXE(&value, buffer);

  napi_value returnCode;
  status = napi_create_int32(env, rc, &returnCode);
  assert(status == napi_ok);

  return returnCode;
}

#define DECLARE_NAPI_METHOD(name, func)                          \
  { name, 0, func, 0, 0, 0, napi_default, 0 }

napi_value Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc = DECLARE_NAPI_METHOD("wto", Method);
  status = napi_define_properties(env, exports, 1, &desc);
  assert(status == napi_ok);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
