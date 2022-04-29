#include <stdio.h>
#include <dns_sd.h>

#include <map>
#include <iostream>

typedef int (*operation)(int a, int b);

typedef struct _str {
    int result;
    operation opt;
} STR;

int sum(int a , int b) {
    return a + b;
}

int minus(int a, int b) {
    return a - b;
}


void run(void *param) {
    STR *str_obj = (STR *) param;
    str_obj->result = str_obj->opt(10, 6);

    printf("result: %d\n", str_obj->result);
}


int main() {
    STR str_obj;
    str_obj.opt = sum;

    run((void *) &str_obj);

    
    return 0;
}
