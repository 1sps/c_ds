/* St: 2016-09-26 Mon 06:28 PM */
/* Up: 2016-09-26 Mon 06:28 PM */

#ifndef FUNCUTILS_H
#define FUNCUTILS_H


/* 
 * Function prototypes
 */

/* Int functions */
void *cpy_i(void *src);
int cmp_i(void *val1, void *val2);
int get_int_size(void *iptr);
int test_cpy_i(void *(*cpy)(void *));
int test_cmp_i(int (*cmp)(void *, void *));
int test_get_size_i(int (*get_key_size)(void *));
void dval_i(void *ival);
/* void printn_i(void *); */

/* Str functions */
void *cpy_s(void *src);
int cmp_s(void *str1, void *str2);
int get_str_size(void *str);
int test_cpy_s(void *(*cpy)(void *));
int test_cmp_s(int (*cmp)(void *, void *));
int test_get_size_s(int (*get_key_size)(void *));
void dval_s(void *sval);
/* void printn_s(void *); */

#endif /* FUNCUTILS_H */

