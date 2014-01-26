#ifndef NODE_IWLIB
#define NODE_IWLIB

#include <node.h>
#include <iwlib.h>

class iwlib : public node::ObjectWrap{
 public:
  static void Init(v8::Handle<v8::Object> exports);

 private:
  ~iwlib();
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> IWGetRangeInfo(const v8::Arguments& args);
  static v8::Handle<v8::Value> IWGetKernelWEVersion(const v8::Arguments& args);
  static v8::Handle<v8::Value> IWScan(const v8::Arguments& args);
  static v8::Handle<v8::Value> IWEnumDevices(const v8::Arguments& args);
  static v8::Handle<v8::Value> GetBasicConfig(const v8::Arguments& args);
  static v8::Local<v8::Object> ParseWirelessConfig(wireless_config& config);
  static v8::Persistent<v8::Function> constructor;
  int socket;
};
#endif
