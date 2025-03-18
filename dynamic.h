#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

struct dynamic_array;

typedef enum
{
    SUCCESS,
    NOT_ENOUGH_MEMORY_EXCEPTION,
    NULL_POINTER_EXCEPTION,
    INCOMPARABLE_TYPE_EXCEPTION
} result_t;

typedef enum
{
    INT,
    DOUBLE
} type_t;

typedef union
{
    char* error_message;
    struct dynamic_array* array;
} either_dynamic_data;

typedef struct
{
    result_t result;
    either_dynamic_data data;
} either_dynamic;

typedef struct _dynamic_data
{
    int size;
    int available_size;
    void* data;
} dynamic_data_t;

typedef void (*data_alloc_t) (dynamic_data_t*, int);
typedef char *(*to_string_t) (struct dynamic_array*);
typedef void (*resize_t) (struct dynamic_array*, int);
typedef void (*push_back_t) (struct dynamic_array*, void*);
typedef void (*set_t) (struct dynamic_array*, int, void*);
typedef void *(*get_t) (struct dynamic_array*, int);
typedef either_dynamic (*map_t) (struct dynamic_array*, void* (*map_func) (void*));
typedef bool *(*where_t) (struct dynamic_array*, bool (*where_func) (void*));
typedef either_dynamic (*concat_t) (struct dynamic_array*, struct dynamic_array*);
typedef either_dynamic (*sort_t) (struct dynamic_array*);

typedef struct _dynamic_vtable
{
    type_t type;
    data_alloc_t data_alloc;
    to_string_t to_string;
    resize_t resize;
    push_back_t push_back;
    set_t set;
    get_t get;
    map_t map;
    where_t where;
    concat_t concat;
    sort_t sort;
} dynamic_vtable_t;


typedef struct dynamic_array
{
    dynamic_data_t* data;
    dynamic_vtable_t* funcs;
} dynamic_t;

typedef either_dynamic dynamic;

dynamic_vtable_t* get_int_vtable();
dynamic_vtable_t* get_double_vtable();

void _dynamic_data_alloc_int(dynamic_data_t* data, int size);
void _dynamic_data_alloc_double(dynamic_data_t* data, int size);

char* dynamic_to_string(either_dynamic array);
char* _dynamic_to_string_int(dynamic_t* array);
char* _dynamic_to_string_double(dynamic_t* array);

either_dynamic new_dynamic_empty(dynamic_vtable_t* funcs);
either_dynamic new_dynamic_sized(dynamic_vtable_t* funcs, int size);

void dynamic_destroy(either_dynamic array);

void dynamic_resize(either_dynamic array, int size);
void _dynamic_resize_int(dynamic_t* array, int size);
void _dynamic_resize_double(dynamic_t* array, int size);

void dynamic_pushback(either_dynamic array, void* x);
void _dynamic_pushback_int(dynamic_t* array, void* x);
void _dynamic_pushback_double(dynamic_t* array, void* x);

void dynamic_set(either_dynamic array, int i, void* x);
void _dynamic_set_int(dynamic_t* array, int i, void* x);
void _dynamic_set_double(dynamic_t* array, int i, void* x);

void* dynamic_get(either_dynamic array, int i);
void* _dynamic_get_int(dynamic_t* array, int i);
void* _dynamic_get_double(dynamic_t* array, int i);

either_dynamic dynamic_map(either_dynamic array, void* (*map_func) (void*));
either_dynamic _dynamic_map_int(dynamic_t* array, void* (*map_func) (void*));
either_dynamic _dynamic_map_double(dynamic_t* array, void* (*map_func) (void*));

bool* dynamic_where(either_dynamic array, bool (*where_func) (void*));
bool* _dynamic_where_int(dynamic_t* array, bool (*where_func) (void*));
bool* _dynamic_where_double(dynamic_t* array, bool (*where_func) (void*));

either_dynamic dynamic_concat(either_dynamic array, either_dynamic other);
either_dynamic _dynamic_concat_int(dynamic_t* array, dynamic_t* other);
either_dynamic _dynamic_concat_double(dynamic_t* array, dynamic_t* other);

either_dynamic dynamic_sort(either_dynamic array);
either_dynamic _dynamic_sort_int(dynamic_t* array);
either_dynamic _dynamic_sort_double(dynamic_t* array);

int* _merge_sort_int (int size, int* data);
double* _merge_sort_double (int size, double* data);