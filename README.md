# Wrap mDNSResponder in C++

## Install mDNSResponder

1. dowload build source from apple: https://opensource.apple.com/tarballs/mDNSResponder/
2. extract and edit some file:
    1. File Client/dns-sd.c : insert `#include <stdarg.h>` `#define MIN(X, Y) (X < Y ? X : Y)`
3. for build source code in my computer `ubuntu 20.04`, i'm run `make os=linux`
4. and finally run `make install`

## Complie source code that use mDNSResponder
to use `mDNSResponder` in your source code, you must link object file with `-ldns_sd` flag </br>

### Example
``` C++
// test.cc
#include <dns_sd.h>

int main() {
    DNSServiceRef sdRef;
    DNSServiceRegister(&sdRef, 0, 0, "gialuong", "_http._tcp", "local.", NULL, 1, NULL, NULL, NULL);
    while (1);
    return 0;
}
```
```
g++ -o test test.cc -ldns_sd
./test
```
