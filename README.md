# Wrap mDNSResponder in C++

## INSTALL mDNSResponder

1. dowload build source from apple: https://opensource.apple.com/tarballs/mDNSResponder/
2. extract and edit some file:
    2.1 File Client/dns-sd.c : insert `#include <stdarg.h> #define MIN(X, Y) (X < Y ? X : Y`
3. run `make`
4. run `make install`