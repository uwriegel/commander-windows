#include <iostream>
#include "../native.h"
using namespace std;

vector<File_item> get_file_items(const wstring& directory) {
    cout << "In get_file_items" << endl;

    vector<File_item> results;
    return move(results);
}

vector<char> get_icon(const wstring& extension) {
    vector<char> results;

    return move(results);
}

wstring get_file_info_version(const wstring& file_name) {
    return L""s;
}

vector<wstring> get_file_info_versions(const wstring& path, const vector<wstring>& file_names) {
    return vector<wstring>;
}
