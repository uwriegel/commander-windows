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

    results.push_back('A');
    results.push_back('f');
    results.push_back('f');
    results.push_back('e');
    results.push_back('n');
    results.push_back('k');
    results.push_back('o');
    results.push_back('p');
    results.push_back('f');

    return move(results);
}
