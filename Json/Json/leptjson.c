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


#define EXPECT(c, ch) do{ assert(*c->json == (ch)); c->json++;}while(0);

#define ISDIGIT1TO9(ch) ((ch)>='1'&&(ch)<='9')
#define ISDIGIT(ch) ((ch)>='0'&&(ch)<='9')
#define PUTC(c, ch) do{*(char *)lept_context_push(c, sizeof(char))=(ch);}while(0)
#define STRING_ERROR(ret) do { c->top = head; return ret; } while(0)

void lept_free(lept_value *v) {
    assert(v != NULL);
    if (v->type == LEPT_STRING) {
        free(v->u.s.s);
    } else if (v->type == LEPT_ARRAY) {
        size_t i;
        for (i = 0; i < v->u.a.size; i++)
            lept_free(&v->u.a.e[i]);
        free(v->u.a.e);
    } else if (v->type == LEPT_OBJECT) {
        size_t i;
        for (i = 0; i < v->u.o.size; i++)
            free(v->u.o.m[i].k);
            lept_free(&v->u.o.m[i].v);
        free(v->u.o.m);
    }
    v->type = LEPT_NULL;
}

void lept_recycle(lept_member *m) {
    m->k = NULL;
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

    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l') {
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

static unsigned int lept_convertHexChar(char ch, unsigned int *num) {
    if((ch>='0')&&(ch<='9')) {
        *num = ch-0x30;
            return 1;
    } else if((ch>='A')&&(ch<='F')) {
        *num = ch-'A'+10;
        return 1;
    } else if((ch>='a')&&(ch<='f')) {
        *num = ch-'a'+10;
        return 1;
    } else {
        return 0;
    }
}

static const char * lept_parse_hex4(const char *p, unsigned int *u) {
    char ch = *p++;
    *u = 0;
    unsigned int num = 0;
    if (!lept_convertHexChar(ch, &num)) {
        return NULL;
    }
    *u = *u|(num << 12);
    
    ch = *p++;
    if (!lept_convertHexChar(ch, &num)) {
        return NULL;
    }
    *u = *u|(num << 8);
    
    ch = *p++;
    if (!lept_convertHexChar(ch, &num)) {
        return NULL;
    }
    *u = *u|(num << 4);
    
    ch = *p++;
    if (!lept_convertHexChar(ch, &num)) {
        return NULL;
    }
    *u = *u|(num << 0);
    return p;
}

static void lept_encode_utf8(lept_context *c, unsigned int u) {
    if (u<= 0x007F) {
        PUTC(c, u & 0xFF);
    } else if (u <= 0x07FF) {
        PUTC(c, 0xC0 | ((u >> 6) & 0xFF));
        PUTC(c, 0x80 | (u & 0x3F));
    } else if (u <= 0xFFFF) {
        PUTC(c, 0xE0 | ((u >> 12) & 0xFF));
        PUTC(c, 0x80 | ((u >> 6) & 0x3F));
        PUTC(c, 0x80 | (u & 0x3F));
    } else if (u <= 0x10FFFF) {
        PUTC(c, 0xF0 | ((u >> 18) & 0xFF));
        PUTC(c, 0x80 | ((u >> 12) & 0x3F));
        PUTC(c, 0x80 | ((u >> 6)  & 0x3F));
        PUTC(c, 0x80 | (u         & 0x3F));
    }
}

static int lept_parse_string_raw(lept_context *c, char **str, size_t *len) {
    unsigned int u = 0;
    unsigned int u2 = 0;
    size_t head = c->top;
    const char *p;
    EXPECT(c, '\"');
    p = c->json;
    while (1) {
        char ch = *p++;
        switch (ch) {
            case '\"':
                *len = c->top - head;
                *str = lept_context_pop(c, *len);
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
                    case 'u':
                        if (!(p = lept_parse_hex4(p, &u)))
                            STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_HEX);
                        if (u >= 0xD800 && u <= 0xDBFF) {
                            if (*p++ != '\\')
                                STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
                            if (*p++ != 'u')
                                STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
                            if (!(p = lept_parse_hex4(p, &u2)))
                                STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_HEX);
                            if (u2 < 0xDC00 || u2 > 0xDFFF)
                                STRING_ERROR(LEPT_PARSE_INVALID_UNICODE_SURROGATE);
                            u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
                        }
                        lept_encode_utf8(c, u);
                        break;
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
}

static int lept_parse_string(lept_context *c, lept_value *v) {
    int ret;
    char* s;
    size_t len;
    if ((ret = lept_parse_string_raw(c, &s, &len)) == LEPT_PARSE_OK)
        lept_set_string(v, s, len);
    return LEPT_PARSE_OK;
}

static int lept_parse_array(lept_context *c, lept_value *v) {
    size_t size = 0;
    int ret;
    EXPECT(c, '[');
    lept_parse_whitespace(c);
    if (*c->json == ']') {
        c->json++;
        v->type = LEPT_ARRAY;
        v->u.a.size = 0;
        v->u.a.e = NULL;
        return LEPT_PARSE_OK;
    }
    while (1) {
        lept_parse_whitespace(c);
        lept_value e;
        lept_init(&e);
        if ((ret = lept_parse_value(c, &e)) != LEPT_PARSE_OK) {
            lept_free(&e);
            break;
        }
        memcpy(lept_context_push(c, sizeof(lept_value)), &e, sizeof(lept_value));
        size ++;
        lept_parse_whitespace(c);
        if (*c->json == ',') {
            c->json++;
        } else if (*c->json == ']') {
            c->json++;
            v->type = LEPT_ARRAY;
            v->u.a.size = size;
            size *= sizeof(lept_value);
            memcpy(v->u.a.e =(lept_value *)malloc(size), lept_context_pop(c, size), size);
            return LEPT_PARSE_OK;
        } else {
            ret = LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
            break;
        }
    }
    for (size_t i = 0; i < size; i++) {
        lept_free((lept_value *)lept_context_pop(c, sizeof(lept_value)));
    }
    return LEPT_PARSE_OK;
}

static int lept_parse_object(lept_context *c, lept_value *v) {
    size_t size;
    lept_member m;
    int ret;
    EXPECT(c, '{');
    if (*c->json == '}') {
        c->json++;
        v->type = LEPT_OBJECT;
        v->u.o.m = NULL;
        v->u.o.size = 0;
    }
    m.k = NULL;
    size = 0;
    while (1) {
        char* str;
        lept_parse_whitespace(c);
        lept_init(&m.v);
        /** todo parse key to m.k, m.len */
        /* 1. parse key */
        if (*c->json != '"') {
            ret = LEPT_PARSE_MISS_KEY;
            break;
        }
        if ((ret = lept_parse_string_raw(c, &str, &m.klen)) != LEPT_PARSE_OK) {
            break;
        }
        memcpy(m.k = (char*)malloc(m.klen + 1), str, m.klen);
        m.k[m.klen] = '\0';
        
        /** todo parse ws colon ws */
        lept_parse_whitespace(c);
        if (*c->json != ':') {
            ret = LEPT_PARSE_MISS_COLON;
            break;
        }
        lept_parse_whitespace(c);
        
        /** parse value */
        if ((ret = lept_parse_value(c, &m.v)) != LEPT_PARSE_OK) {
            lept_free(&m.v);
            free(m.k);
            m.k = NULL;
            break;
        }
        memcpy(lept_context_push(c, sizeof(lept_member)), &m, sizeof(lept_member));
        size++;
        m.k = NULL;
        
        lept_parse_whitespace(c);
        m.k = NULL;
        if (*c->json == ',') {
            c->json++;
        } else if (*c->json == '}') {
            c->json++;
            v->type = LEPT_OBJECT;
            v->u.o.size = size;
            size *= sizeof(lept_member);
            memcpy(v->u.o.m =(lept_member *)malloc(size), lept_context_pop(c, size), size);
            return LEPT_PARSE_OK;
        } else {
            ret = LEPT_PARSE_MISS_COMMA_OR_SQUARE_BRACKET;
            break;
        }
    }
    /* \todo Pop and free members on the stack */
    for (size_t i = 0; i < size; i++) {
        lept_member *member = (lept_member *)lept_context_pop(c, sizeof(lept_value));
        lept_free(&member->v);
        free(member->k);
        
    }
    v->type = LEPT_NULL;
    return ret;
}

int lept_parse_value(lept_context *c, lept_value *v) {
    switch (*c->json) {
        case 'n':
            return lept_parse_null(c, v);
        case 'f':
            return lept_parse_false(c, v);
        case 't':
            return lept_parse_true(c, v);
        case '\"':
            return lept_parse_string(c, v);
        case '[':
            return lept_parse_array(c, v);
        case '{':
            return lept_parse_object(c, v);
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

size_t lept_get_array_size(const lept_value *v) {
    assert(v != NULL && v->type == LEPT_ARRAY);
    return v->u.a.size;
}

lept_value *lept_get_array_element(const lept_value *v, size_t index) {
    assert(v != NULL && v->type == LEPT_ARRAY);
    assert(index < v->u.a.size);
    return &v->u.a.e[index];
}
