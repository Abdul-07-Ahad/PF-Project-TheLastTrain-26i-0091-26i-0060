#include "util.h"
#include <cstring>
#include <iostream>
using namespace std;
void trimWhitespace(char* str) {
    int len = strlen(str);
    while ( len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r' || str[len - 1] == '\n') ) {
        len--;
    }
    str[len] = '\0';
}
const char* getValue(const char* line, const char* key) {
    int keyLen = strlen(key);
    if ( strncmp(line, key, keyLen) == 0 ) {
        const char* p = line + keyLen;
        while ( *p == ' ' ) {     
            p++;
        }
        return p;
    }
    return nullptr;
}
bool stringEquals(const char* a, const char* b) {
    return strcmp(a, b) == 0 ;
}
int clampInt(int val, int minVal, int maxVal) {
    if (val < minVal) {
        return minVal ;
    }
    if (val > maxVal) {
        return maxVal ;
    }
    return val;
}
void printHeader(const char* title){
    for ( int i = 0 ; i< 40 ; i++){
        cout <<'=';
    }
    cout<<endl;
    cout<<title <<endl;
    for ( int i = 0 ; i< 40 ; i++){
        cout <<'=';
    }
    cout<<endl;
}
void clearScreen(){
    for (int i = 0 ; i< 30 ; i++){
        cout<<endl;
    }

}