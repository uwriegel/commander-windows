#pragma once
#include <string>
#include <vector>
using namespace std;

struct File_item {
    wstring display_name;
    bool is_directory;
    bool is_hidden;
    uint64_t size;
    uint64_t time;
};

extern vector<File_item> get_file_items(const wstring& directory);
extern vector<char> get_icon(const wstring& extension);
extern vector<wstring> get_file_info_versions(const wstring& path, const vector<wstring>& file_names);