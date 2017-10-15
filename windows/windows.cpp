#include <windows.h>
#include <sstream>
#include <utility>
#include <algorithm>
#include <iterator>
#include "../native.h"
#include "iconextractor.h"
using namespace std;

using GetFileVersionInfoSizeFunction = DWORD (__stdcall*)(const wchar_t* filename, DWORD* handle);
using GetFileVersionInfoFunction = BOOL (__stdcall*)(const wchar_t* filename, DWORD nill, DWORD length, void* data);
using VerQueryValueFunction = BOOL (__stdcall*)(void* block, const wchar_t* sub_block, void** buffer, UINT* length);

GetFileVersionInfoSizeFunction create_get_file_version_info_size() {
    auto module = LoadLibraryW(L"Api-ms-win-core-version-l1-1-0.dll");
    return reinterpret_cast<GetFileVersionInfoSizeFunction>(GetProcAddress(module, "GetFileVersionInfoSizeW"));
}

GetFileVersionInfoFunction create_get_file_version_info() {
    auto module = LoadLibraryW(L"Api-ms-win-core-version-l1-1-0.dll");
    return reinterpret_cast<GetFileVersionInfoFunction>(GetProcAddress(module, "GetFileVersionInfoW"));
}

VerQueryValueFunction create_ver_query_value() {
    auto module = LoadLibraryW(L"Api-ms-win-core-version-l1-1-0.dll");
    return reinterpret_cast<VerQueryValueFunction>(GetProcAddress(module, "VerQueryValueW"));
}

static GetFileVersionInfoSizeFunction get_file_version_info_size{create_get_file_version_info_size()};
static GetFileVersionInfoFunction get_file_version_info{create_get_file_version_info()};
static VerQueryValueFunction ver_query_value{create_ver_query_value()};

uint64_t convertWindowsTimeToUnixTime(const FILETIME& ft) {
    ULARGE_INTEGER ull;
    ull.LowPart = ft.dwLowDateTime;
    ull.HighPart = ft.dwHighDateTime;
    return (ull.QuadPart / 10000000ULL - 11644473600ULL) * 1000;
}

vector<File_item> get_file_items(const wstring& directory) {
    vector<File_item> results;
    auto search_string = directory + L"*.*"s;

    WIN32_FIND_DATAW w32fd{ 0 };
    auto ret = FindFirstFileW(search_string.c_str(), &w32fd);
    while (FindNextFileW(ret, &w32fd) == TRUE) {
        File_item item {
            w32fd.cFileName,
            (w32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY,
            (w32fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN,
            static_cast<uint64_t>(w32fd.nFileSizeHigh) << 32 | w32fd.nFileSizeLow,
            convertWindowsTimeToUnixTime(w32fd.ftLastWriteTime)
        };
        if (item.display_name != L".."s)
            results.push_back(item);
    }
    return move(results);
}

vector<char> get_icon(const wstring& extension) {
    gdiplus_initialize();
	auto bytes = extract_icon(extension); 
    gdiplus_uninitialize();
    return move(bytes);
}

wstring get_file_info_version(const wstring& file_name) {
    DWORD _{0};
    auto size = get_file_version_info_size(file_name.c_str(), &_);
    if (size == 0)
        return L""s;
	vector<char> data(size);
	auto ok = get_file_version_info(file_name.c_str(), 0, size, data.data());
	VS_FIXEDFILEINFO *info{nullptr};
	uint32_t len{0};
	ver_query_value(data.data(), L"\\", reinterpret_cast<void**>(&info), &len);

	int file_major = HIWORD(info->dwFileVersionMS);
	int file_minor = LOWORD(info->dwFileVersionMS);
	int file_build = HIWORD(info->dwFileVersionLS);
	int file_private = LOWORD(info->dwFileVersionLS);

	wstringstream result;
    result << file_major << L"." << file_minor << L"." << file_private << L"." << file_build;
    return move(result.str());
}

vector<wstring> get_file_info_versions(const wstring& path, const vector<wstring>& file_names) {
    vector<wstring> infos;
    transform(file_names.begin(), file_names.end(), back_inserter(infos), [path](const wstring& file)-> wstring {
        wstring file_path = path + L"\\"s + file;
        return get_file_info_version(file_path);
    });
    return move(infos);
}
