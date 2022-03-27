//
//  leptContext.h
//  Json
//
//  Created by 陈学明 on 2022/3/27.
//

#ifndef leptContext_h
#define leptContext_h

#include <stdio.h>

typedef struct {
    const char *json;
    char *stack;
    size_t size, top;
}lept_context;

void *lept_context_push(lept_context *context, size_t size);

void *lept_context_pop(lept_context *context, size_t size);
#endif /* leptContext_h */
