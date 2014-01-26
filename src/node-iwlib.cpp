#include <node.h>
#include <iwlib.h>
#include <errno.h>
#include "node-iwlib.h"

using namespace v8;

Persistent<Function> iwlib::constructor;
iwlib::~iwlib(){

}

void iwlib::Init(Handle<Object> exports){

  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->SetClassName(String::NewSymbol("iwlib"));
  NODE_SET_PROTOTYPE_METHOD(tpl,"scan",iwlib::IWScan);
  NODE_SET_PROTOTYPE_METHOD(tpl,"get_basic_config",iwlib::GetBasicConfig);
  NODE_SET_PROTOTYPE_METHOD(tpl,"get_kernel_we_version",iwlib::IWGetKernelWEVersion);
  
  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("iwlib"), constructor);
  
}


Handle<Value> iwlib::New(const Arguments& args){
  HandleScope scope;
  iwlib* obj = new iwlib();

  //Try to open a socket
  if((obj->socket = iw_sockets_open()) <0){
    ThrowException(String::New(strerror(errno)));
  }
  
  obj->Wrap(args.This());
  return args.This();
}

Handle<Value> iwlib::IWGetKernelWEVersion(const Arguments& args){
  HandleScope scope;
  int result = iw_get_kernel_we_version();
  return scope.Close(Number::New(result));
}

Handle<Value> iwlib::GetBasicConfig(const Arguments& args){
  HandleScope scope;
  iwlib* obj = ObjectWrap::Unwrap<iwlib>(args.This());
  
  wireless_config result;
  
  if( args.Length() != 1){
    ThrowException(Exception::TypeError(String::New("get_basic_config expects interface_name as first argument")));
    return scope.Close(Undefined());
  }
  if(!args[0]->IsString()){
    ThrowException(Exception::TypeError(String::New("get_basic_config expects first argument to be string")));
  }
  
  /* Coax our v8::string string into char* */
  Local<String> v8interface = args[0]->ToString();
  String::Utf8Value uinterface(v8interface);
  char* interface(*uinterface);
  
  if(iw_get_basic_config(obj->socket,interface,&result) < 0){
    return scope.Close(Undefined());
  }
  
  Local<Object> basic_config = ParseWirelessConfig(result);
  
  return scope.Close(basic_config);
  
}

Handle<Value> iwlib::IWScan(const Arguments& args){
  HandleScope scope;
  iwlib* obj = ObjectWrap::Unwrap<iwlib>(args.This());
  wireless_scan_head head;
  wireless_scan *result;
  iwrange range;

  if( args.Length() != 1){
    ThrowException(Exception::TypeError(String::New("Scan expects interface_name as first argument")));
    return scope.Close(Undefined());
  }
  
  if(!args[0]->IsString()){
    ThrowException(Exception::TypeError(String::New("Scan expects first argument to be string")));
  }
  
  /* Coax our v8::string string into char* */
  Local<String> v8interface = args[0]->ToString();
  String::Utf8Value uinterface(v8interface);
  char* interface(*uinterface);
  
  if(iw_get_range_info(obj->socket,interface,&range) < 0){
    return scope.Close(Undefined());
  }

  if(iw_scan(obj->socket,interface,range.we_version_compiled,&head) < 0){
    return scope.Close(Undefined());
  }

  result = head.result;
  
  /* Populate a return array with the contents of the wireless_scan linked list*/
  Local<Array> results =Array::New();
  int i = 0;

  while(result != NULL) { 
    Local<Object> ws = Object::New();
    Local<Object> iwscan = ParseWirelessConfig(result->b); 
    
    ws->Set(String::NewSymbol("wireless_scan"),iwscan);
    
    results->Set(i,ws);
    result = result->next;

    i++;
  }
  
  return scope.Close(results);
}


Local<Object> iwlib::ParseWirelessConfig(wireless_config& config){
  Local<Object> basic_config = Object::New();
  basic_config->Set(String::NewSymbol("name"),String::New(config.name));
  basic_config->Set(String::NewSymbol("has_nwid"),Number::New(config.has_nwid));
  basic_config->Set(String::NewSymbol("nwid"),Number::New(config.nwid.value)); /* Only returning the value of the iw_param struct for now*/
  basic_config->Set(String::NewSymbol("has_freq"),Number::New(config.has_freq));
  basic_config->Set(String::NewSymbol("freq"),Number::New(config.freq));
  basic_config->Set(String::NewSymbol("freq_flags"),Number::New(config.freq_flags));
  basic_config->Set(String::NewSymbol("has_key"),Number::New(config.has_key));
  basic_config->Set(String::NewSymbol("key"),String::New(reinterpret_cast<char*>(config.key)));
  basic_config->Set(String::NewSymbol("key_size"),Number::New(config.key_size));
  basic_config->Set(String::NewSymbol("key_flags"),Number::New(config.key_flags));
  basic_config->Set(String::NewSymbol("has_essid"),Number::New(config.has_essid));
  basic_config->Set(String::NewSymbol("essid_on"),Number::New(config.essid_on));
  basic_config->Set(String::NewSymbol("essid"),String::New(config.essid));
  basic_config->Set(String::NewSymbol("has_mode"),Number::New(config.has_mode));
  basic_config->Set(String::NewSymbol("mode"),Number::New(config.mode));
  return basic_config;
}
