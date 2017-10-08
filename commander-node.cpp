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


// static void WorkAsyncComplete(uv_work_t *req, int status)
// {
//     auto isolate = Isolate::GetCurrent();
//     HandleScope handleScope(isolate); 

//     auto work = static_cast<Work*>(req->data);

//     Local<Array> result_list = Array::New(isolate);
//     int i{0};
//     for(auto item: work->results) {
//         Local<Object> result = Object::New(isolate);
//         result->Set(String::NewFromUtf8(isolate, "displayName"), 
//             String::NewFromUtf8(isolate, ws2utf8(item.display_name).c_str())); 
//         result->Set(String::NewFromUtf8(isolate, "size"), Number::New(isolate, static_cast<double>(item.size)));
//         result->Set(String::NewFromUtf8(isolate, "time"), Date::New(isolate, static_cast<double>(item.time)));
//         result->Set(String::NewFromUtf8(isolate, "isDirectory"), Boolean::New(isolate, item.is_directory));
//         result->Set(String::NewFromUtf8(isolate, "isHidden"), Boolean::New(isolate, item.is_hidden));
//         result_list->Set(i++, result);
//     }


//     // execute the callback
//     Local<Function>::New(isolate, 
//         work->callback)->Call(isolate->GetCurrentContext()->Global(), 
//         2, argv);

//     // Free up the persistent function callback
//     work->callback.Reset();

//     delete work;    
// }

NAN_METHOD(get_files) {
    cout << "Aufruf" << endl;

    Utf8String val(To<String>(info[0]).ToLocalChecked());
    auto directory = utf82ws(*val);

    auto callback = new Callback(info[1].As<Function>());
    auto items = make_shared<vector<File_item>>();
    AsyncQueueWorker(new Worker(callback, [directory, items]()-> void {
        wcout << L"Bin im Hintergrund aufgerufen: " << directory << endl;
        *items = move(get_file_items(directory));
    }, [items](Nan::Callback* callback)-> void {
        cout << "Bin wieder zurück!" << endl;

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

NAN_MODULE_INIT(init) {
    Nan::Set(target,
        New<String>("getFiles").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(get_files)).ToLocalChecked());
}
  
NODE_MODULE(commander_native, init)