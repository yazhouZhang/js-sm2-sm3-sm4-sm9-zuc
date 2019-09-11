// hello.cc
#include <node.h>
#include "DES.h"
#include "SM4Alg.h"

namespace demo {

	using namespace v8;
	DES des;
	CSM4Alg sm4;

	void CalDES(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		if (args.Length() < 3) {
			isolate->ThrowException(v8::Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments")));
			return;
		}
		Local<Value> data = args[0];
		Local<Value> key = args[1];
		Local<Value> flag = args[2];
		String::Utf8Value sstr(data);
		String::Utf8Value sKey(key);

		char cFlag = (*String::Utf8Value(flag))[0];
		if(strlen(sKey.operator*())!=16&& strlen(sKey.operator*()) != 32){
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments key")));
			return;
		}
		char dest[10000];
		memset(dest, 0x00, sizeof(dest));
		des.CalDES(sstr.operator*(), dest, sKey.operator*(), cFlag);
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, dest));
	}

	void CalX99MAC(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		if (args.Length() < 3) {
			isolate->ThrowException(v8::Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments")));
			return;
		}
		Local<Value> data = args[0];
		Local<Value> len = args[1];
		Local<Value> key = args[2];

		String::Utf8Value sstr(data);
		String::Utf8Value sKey(key);

		int value = len->Int32Value();
		if (strlen(sKey.operator*()) != 16) {
			isolate->ThrowException(v8::Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments key")));
		}
		char dest[17];
		memset(dest, 0x00, sizeof(dest));
		des.CalX99MAC(sstr.operator*(), dest, value, sKey.operator*());
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, dest));
	}

	void CalANSI919MAC(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		if (args.Length() < 3) {
			isolate->ThrowException(v8::Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments")));
		}
		Local<Value> data = args[0];
		Local<Value> len = args[1];
		Local<Value> key = args[2];

		String::Utf8Value sstr(data);
		String::Utf8Value sKey(key);

		int value = len->Int32Value();
		if (strlen(sKey.operator*()) != 32) {
			isolate->ThrowException(v8::Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments key")));
		}
		char dest[33];
		memset(dest, 0x00, sizeof(dest));
		des.CalANSI919MAC(dest, sstr.operator*(), value, sKey.operator*());
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, dest));
	}

	void SM4CryptECB(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		if (args.Length() < 3) {
			isolate->ThrowException(v8::Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments")));
		}
		Local<Value> data = args[0];
		Local<Value> flag = args[1];
		Local<Value> key = args[2];
		
		String::Utf8Value sstr(data);
		String::Utf8Value sKey(key);
		
		int value = flag->Int32Value();
		if (strlen(sKey.operator*()) != 32) {
			isolate->ThrowException(v8::Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments key")));
		}
		char dest[10000];
		memset(dest, 0x00, sizeof(dest));
		sm4.SM4CryptECB(sstr.operator*(), value, sKey.operator*(), dest);
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, dest));
	}

	void SM4MACGenerated(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		if (args.Length() < 2) {
			isolate->ThrowException(v8::Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments")));
		}
		Local<Value> data = args[0];
		Local<Value> key = args[1];

		String::Utf8Value sstr(data);
		String::Utf8Value sKey(key);

		if (strlen(sKey.operator*()) != 32) {
			isolate->ThrowException(v8::Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments key")));
		}
		char dest[33];
		memset(dest, 0x00, sizeof(dest));

		sm4.SM4MACGenerated(sstr.operator*(), sKey.operator*(), dest);
		args.GetReturnValue().Set(String::NewFromUtf8(isolate, dest));
	}

	void init(Local<Object> exports) {
		NODE_SET_METHOD(exports, "CalDES", CalDES);
		NODE_SET_METHOD(exports, "Cal99MAC", CalX99MAC);
		NODE_SET_METHOD(exports, "Cal919MAC", CalANSI919MAC);
		NODE_SET_METHOD(exports, "CalSM4", SM4CryptECB);
		NODE_SET_METHOD(exports, "CalSM4MAC", SM4MACGenerated);
	}
	NODE_MODULE(CDES, init)

}  // namespace demo
