#include <iostream>
#include "../native.h"
using namespace std;

vector<File_item> get_file_items(const wstring& directory) {
    cout << "In get_file_items" << endl;

    vector<File_item> results;
    return move(results);
}