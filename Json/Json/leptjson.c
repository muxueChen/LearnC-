//
//  leptjson.c
//  AnalysisJson
//
//  Created by 陈学明 on 2022/3/22.
//

#include "leptjson.h"
#include <assert.h>
#include <stdlib.h>  /* NULL */
#include <errno.h>   /* errno, ERANGE */
#include <math.h>    /* HUGE_VAL */
#include<string.h>
#include "leptContext.h"

#define EXPECT(c, ch) do{ assert(*c->json == (ch)); c->json++;}while(0);

#define ISDIGIT1TO9(ch) ((ch)>='1'&&(ch)<='9')
#define ISDIGIT(ch) ((ch)>='0'&&(ch)<='9')
#define PUTC(c, ch) do{*(char *)lept_context_push(c, sizeof(char))=(ch);}while(0)


void lept_free(lept_value *v) {
    assert(v != NULL);
    if (v->type == LEPT_STRING) {
        free(v->u.s.s);
    }
    v->type = LEPT_NULL;
}

static void lept_parse_whitespace(lept_context *c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
        p++;
    }
    c->json = p;
}

static int lept_parse_null(lept_context *c, lept_value *v) {
    EXPECT(c, 'n');

    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] == 'l') {
        return LEPT_PARSE_INVALID_VALUE;
    }
    
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}

static int lept_parse_true(lept_context *c, lept_value *v) {
    EXPECT(c, 't');
    if (c->json[0] != 'r' || c->json[1] != 'u' || c->json[2] != 'e') {
        return LEPT_PARSE_INVALID_VALUE;
    }
    c->json += 3;
    v->type = LEPT_TRUE;
    return LEPT_PARSE_OK;
}

static int lept_parse_false(lept_context *c, lept_value *v) {
    EXPECT(c, 'f');
    if (c->json[0] != 'a' || c->json[1] != 'l' || c->json[2] != 's' || c->json[3] != 'e') {
        return LEPT_PARSE_INVALID_VALUE;
    }
    c->json += 4;
    v->type = LEPT_FALSE;
    return LEPT_PARSE_OK;
}

static int lept_parse_number(lept_context *c, lept_value *v) {
    const char *p = c->json;
    if (*p == '-') p++;// 判断符号
    if (*p == '0') {// 判断字符
        p++;
    } else {
        if (!ISDIGIT1TO9(*p))return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == '.') {
        p++;
        if (!ISDIGIT(*p))return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    if (*p == 'e' || *p == 'E') {
        p++;
        if (*p == '+' || *p == '-') p++;
        if (!ISDIGIT(*p))return LEPT_PARSE_INVALID_VALUE;
        for (p++; ISDIGIT(*p); p++);
    }
    
    v->u.n = strtod(c->json, NULL);
    v->type = LEPT_NUMBER;
    c->json = p;
    return LEPT_PARSE_OK;
}

static int lept_parse_string(lept_context *c, lept_value *v) {
    size_t head = c->top, len;
    const char *p;
    EXPECT(c, '\"');
    p = c->json;
    while (1) {
        char ch = *p++;
        switch (ch) {
            case '\"':
                len = c->top - head;
                lept_set_string(v, lept_context_pop(c, len), len);
                c->json = p;
                return LEPT_PARSE_OK;
            case '\\':
                switch (*p++) {
                    case '\"':PUTC(c, '\"');break;
                    case '\\':PUTC(c, '\\');break;
                    case '/':PUTC(c, '/');break;
                    case 'b':PUTC(c, '\b');break;
                    case 'f':PUTC(c, '\f');break;
                    case 'n':PUTC(c, '\n');break;
                    case 't':PUTC(c, '\t');break;
                    case 'r':PUTC(c, '\r');break;
                    default:
                        if ((unsigned char)ch<0x20) {
                            c->top = head;
                            return LEPT_PARSE_INVALID_STRING_CHAR;
                        }
                        PUTC(c, ch); 
                        break;
                }
                break;
            case '\0':
                c->top = head;
                return LEPT_PARSE_MISS_QUOTATION_MARK;
            default:
                PUTC(c, ch);
                
        }
    }
    return LEPT_PARSE_OK;
}

static int lept_parse_value(lept_context *c, lept_value *v) {
    switch (*c->json) {
        case 'n':
            return lept_parse_null(c, v);
        case 'f':
            return lept_parse_false(c, v);
        case 't':
            return lept_parse_true(c, v);
        default:
            return lept_parse_number(c, v);
        case '\0':
            return LEPT_PARSE_EXPECT_VALUE;
    }
}

int lept_parse(lept_value *v, const char *json) {
    lept_context c;
    int ret;
    assert(v!=NULL);
    c.json = json;
    c.stack = NULL;
    c.size = c.top = 0;
    lept_init(v);
    lept_parse_whitespace(&c);
    if ((ret = lept_parse_value(&c, v)) == LEPT_PARSE_OK) {
        lept_parse_whitespace(&c);
        if (*c.json != '\0') {
            ret = LEPT_PARSE_ROOT_NOT_SINGULAR;
        }
    }
    assert(c.top == 0);
    free(c.stack);
    return ret;
}

lept_type lept_get_type(const lept_value *v) {
    assert(v!= NULL);
    return v->type;
}

int lept_get_boolean(const lept_value *v) {
    assert(v->type == LEPT_TRUE || v->type == LEPT_FALSE);
    return v->type == LEPT_TRUE ? 1 : 0;
}

void lept_set_boolean(lept_value *v, int b) {
    assert(v->type == LEPT_TRUE || v->type == LEPT_FALSE);
    v->type = b == 0 ? LEPT_FALSE : LEPT_TRUE;
}

double lept_get_number(const lept_value *v) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    return v->u.n;
}

void lept_set_number(lept_value *v, double num) {
    assert(v != NULL && v->type == LEPT_NUMBER);
    v->u.n = num;
}

const char *lept_get_string(const lept_value *v) {
    assert(v != NULL && v->type == LEPT_STRING);
    return v->u.s.s;
}

size_t lept_get_string_length(const lept_value *v) {
    assert(v != NULL && v->type == LEPT_STRING);
    return v->u.s.len;
}

void lept_set_string(lept_value *v, const char *s, size_t len) {
    assert(v != NULL && (s != NULL || len == 0));
    lept_free(v);
    v->u.s.s = (char *)malloc(len+1);
    memcpy(v->u.s.s, s, len);
    v->u.s.s[len] = '\0';
    v->u.s.len = len;
    v->type = LEPT_STRING;
}
