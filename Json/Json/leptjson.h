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

typedef struct lept_value lept_value;

struct lept_value {
    union {
        struct {
            lept_value *e;
            size_t size;
        }a;
        struct {
            char *s;
            size_t len;
        }s;
        double n;
    }u;
    lept_type type;
};

enum {
    LEPT_PARSE_OK = 0,
    LEPT_PARSE_EXPECT_VALUE,
    LEPT_PARSE_INVALID_VALUE,
    LEPT_PARSE_ROOT_NOT_SINGULAR,
    LEPT_PARSE_MISS_QUOTATION_MARK,
    LEPT_PARSE_INVALID_STRING_ESCAPE,
    LEPT_PARSE_INVALID_STRING_CHAR,
    LEPT_PARSE_INVALID_UNICODE_HEX,
    LEPT_PARSE_INVALID_UNICODE_SURROGATE,
    LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET,
};

// 初始化
#define lept_init(v) do{(v)->type = LEPT_NULL;}while(0)

#define lept_set_null(v) lept_free(v)

int lept_parse(lept_value *v, const char *json);

lept_type lept_get_type(const lept_value *v);

int lept_get_boolean(const lept_value *v);
void lept_set_boolean(lept_value *v, int b);

double lept_get_number(const lept_value *v);
void lept_set_number(lept_value *v, double num);

const char *lept_get_string(const lept_value *v);
size_t lept_get_string_length(const lept_value *v);
void lept_set_string(lept_value *v, const char *s, size_t len);

size_t lept_get_array_size(const lept_value *v);
lept_value *lept_get_array_element(const lept_value *v, size_t index);

int lept_parse_value(lept_context *c, lept_value *v);

void lept_free(lept_value *v);
#endif /* leptjson_h */
