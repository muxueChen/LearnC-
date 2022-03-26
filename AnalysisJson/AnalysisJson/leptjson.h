//
//  leptjson.h
//  AnalysisJson
//
//  Created by 陈学明 on 2022/3/22.
//

#ifndef leptjson_h
#define leptjson_h

#include <stdio.h>
typedef enum {
    LEPT_NULL,
    LEPT_FALSE,
    LEPT_TRUE,
    LEPT_NUMBER,
    LEPT_STRING,
    LEPT_ARRAY,
    LEPT_OBJECT
}lept_type;

typedef struct {
    lept_type type;
    
}lept_value;

enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR,
};

int lept_parse(lept_value *v, const char *json);

lept_type lept_get_type(const lept_value *v);
#endif /* leptjson_h */
