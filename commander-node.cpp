#include <nan.h>
#include <uv.h>
#include <locale>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <codecvt>
#include "worker.h"
#include "native.h"
using namespace std;
using namespace Nan;
using namespace v8;

string ws2utf8(const wstring &input) {
	wstring_convert<codecvt_utf8<wchar_t>> utf8conv;
	return utf8conv.to_bytes(input);
}

wstring utf82ws(const string &input) {
	wstring_convert<codecvt_utf8<wchar_t>> utf8conv;
	return utf8conv.from_bytes(input);
}

NAN_METHOD(get_files) {

    Utf8String val(To<String>(info[0]).ToLocalChecked());
    auto directory = utf82ws(*val);

    auto callback = new Callback(info[1].As<Function>());
    auto items = make_shared<vector<File_item>>();
    AsyncQueueWorker(new Worker(callback, [directory, items]()-> void {
        *items = move(get_file_items(directory));
    }, [items](Nan::Callback* callback)-> void {

        Local<Array> result_list = New<Array>(items->size());
        int i{0};
        for(auto item: *items) {
            Local<Object> result = New<Object>();

            result->Set(New<String>("displayName").ToLocalChecked(), 
                New<String>(ws2utf8(item.display_name).c_str()).ToLocalChecked()); 
            result->Set(New<String>("size").ToLocalChecked(), New<Number>(static_cast<double>(item.size)));
            result->Set(New<String>("time").ToLocalChecked(), 
                New<Date>(static_cast<double>(item.time)).ToLocalChecked());
            result->Set(New<String>("isDirectory").ToLocalChecked(), New<Boolean>(item.is_directory));
            result->Set(New<String>("isHidden").ToLocalChecked(), New<Boolean>(item.is_hidden));
            result_list->Set(i++, result);
        }        

        // set up return arguments
        Local<Value> argv[] = { Nan::Null(), result_list };
        callback->Call(2, argv);
    }));
}

void buffer_delete_callback(char* unused, void* the_vector) {
    delete reinterpret_cast<vector<char>*>(the_vector);
}

NAN_METHOD(get_icon) {
    
    Utf8String val(To<String>(info[0]).ToLocalChecked());
    auto extension = utf82ws(*val);

    auto callback = new Callback(info[1].As<Function>());
    auto icon = new vector<char>;
    AsyncQueueWorker(new Worker(callback, [extension, icon]()-> void {
        *icon = move(get_icon(extension));
    }, [icon](Nan::Callback* callback)-> void {
        // set up return arguments
        auto bytes = NewBuffer(icon->data(), icon->size(), buffer_delete_callback, icon).ToLocalChecked();
        Local<Value> argv[] = { Nan::Null(), bytes };
        callback->Call(2, argv);
    }));
}

NAN_MODULE_INIT(init) {
    Nan::Set(target,
        New<String>("getFiles").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(get_files)).ToLocalChecked());
    Nan::Set(target,
        New<String>("getIcon").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(get_icon)).ToLocalChecked());
}
  
NODE_MODULE(commander_native, init)