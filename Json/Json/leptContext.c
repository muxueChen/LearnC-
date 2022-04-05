//
//  leptContext.c
//  Json
//
//  Created by 陈学明 on 2022/3/27.
//

#include "leptContext.h"
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include<string.h>

#define LEPT_PARSE_STACK_INIT_SIZE 256

void * lept_context_push(lept_context *c, size_t size) {
    void *ret;
    assert(size > 0);
    if (c->top + size >= c->size) {
        if (c->size == 0) {
            c->size = LEPT_PARSE_STACK_INIT_SIZE;
        }
        while (c->top + size >= c->size) {
            c->size += c->size >>1;// c->size * 1.5;
        }
        c->stack = (char *)realloc(c->stack, c->size);
    }
    ret = c->stack + c->top;
    c->top += size;
    return ret;
}

void *lept_context_pop(lept_context *c, size_t size) {
    assert(c->top >= size);
    return c->stack + (c->top -= size);
}
