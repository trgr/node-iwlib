#define BUILDING_NODE_EXTENSION
#include <node.h>
#include "node-iwlib.h"
using namespace v8;

void InitAll(Handle<Object> exports){
  iwlib::Init(exports);
}

NODE_MODULE(iwlib,InitAll);
