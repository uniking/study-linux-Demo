#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
using namespace std;
int main() {
    string s = "Clare";
    // toUpper
    transform(s.begin(), s.end(), s.begin(), ::toupper);
    // toLower
    //transform(s.begin(),s.end(),s.begin(), ::tolower);
    cout << s << endl;
}
