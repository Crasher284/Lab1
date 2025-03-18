#include "dynamic.h"

static dynamic_vtable_t* int_vtable = NULL;
static dynamic_vtable_t* double_vtable = NULL;

char* dynamic_to_string(either_dynamic array){
    if(array.result == SUCCESS){
        char* out = array.data.array->funcs->to_string(array.data.array);
        return out;
    }else{
        return array.data.error_message;
    }
}

either_dynamic new_dynamic_empty(dynamic_vtable_t* funcs){
    either_dynamic out;
    if(funcs == NULL){
        out.result = NULL_POINTER_EXCEPTION;
        out.data.error_message = "null_pointer_exception: given pointer \"funcs\" is not initialized\n";
        return out;
    }
    printf("");
    dynamic_t* array = (dynamic_t*) malloc(sizeof(dynamic_t));
    if(array == NULL){
        out.result = NOT_ENOUGH_MEMORY_EXCEPTION;
        out.data.error_message = "not_enough_memory_exception: the memory couldn't be get for creation of array\n";
        return out;
    }
    array->funcs = funcs;
    array->data = (dynamic_data_t*) malloc(sizeof(dynamic_data_t));
    if(array->data == NULL){
        out.result = NOT_ENOUGH_MEMORY_EXCEPTION;
        out.data.error_message = "not_enough_memory_exception: the memory couldn't be get for creation of array->data\n";
        return out;
    }
    array->funcs->data_alloc(array->data, 1);
    if(array->data->data == NULL){
        out.result = NOT_ENOUGH_MEMORY_EXCEPTION;
        out.data.error_message = "not_enough_memory_exception: the memory couldn't be get for creation of array->data->data\n";
        return out;
    }
    out.result = SUCCESS;
    out.data.array = array;
    return out;
}

either_dynamic new_dynamic_sized(dynamic_vtable_t* funcs, int size){
    either_dynamic out;
    if(funcs == NULL){
        out.result = NULL_POINTER_EXCEPTION;
        out.data.error_message = "null_pointer_exception: given pointer \"funcs\" was not initialised\n";
        return out;
    }
    dynamic_t* array = (dynamic_t*) malloc(sizeof(dynamic_t));
    if(array == NULL){
        out.result = NOT_ENOUGH_MEMORY_EXCEPTION;
        out.data.error_message = "not_enough_memory_exception: the memory couldn't be get for creation of array\n";
        return out;
    }
    array->funcs = funcs;
    array->data = (dynamic_data_t*) malloc(sizeof(dynamic_data_t));
    if(array->data == NULL){
        out.result = NOT_ENOUGH_MEMORY_EXCEPTION;
        out.data.error_message = "not_enough_memory_exception: the memory couldn't be get for creation of array->data\n";
        return out;
    }
    array->funcs->data_alloc(array->data, size);
    if(array->data->data == NULL){
        out.result = NOT_ENOUGH_MEMORY_EXCEPTION;
        out.data.error_message = "not_enough_memory_exception: the memory couldn't be get for creation of array->data->data\n";
        return out;
    }
    out.result = SUCCESS;
    out.data.array = array;
    return out;
}

void dynamic_destroy(either_dynamic array){
    if(array.result==SUCCESS) {
        free(array.data.array->data->data);

        free(array.data.array->data);

        free(array.data.array);
    }
}

void dynamic_resize(either_dynamic array, int new_size){
    if(array.result == SUCCESS){
        if(new_size<1) {
            printf("illegal_size_exception: %d cannot be size of array\n", new_size);
            return;
        }
        array.data.array->funcs->resize(array.data.array, new_size);
    }
}

void dynamic_pushback(either_dynamic array, void* x){
    if(array.result == SUCCESS){
        if(x == NULL) {
            printf("null_pointer_exception: given pointer \"x\" was not initialised\n");
            return;
        }
        if(array.data.array->data->size==array.data.array->data->available_size){
            array.data.array->funcs->resize(array.data.array, (array.data.array->data->available_size)*2);
        }
        array.data.array->funcs->push_back(array.data.array, x);
        array.data.array->data->size+=1;
    }
}

void dynamic_set(either_dynamic array, int i, void* x){
    if(array.result == SUCCESS){
        if(x == NULL) {
            printf("null_pointer_exception: given pointer \"x\" was not initialised\n");
            return;
        }
        if(i<0 || i>=(array.data.array->data->size)) {
            printf("illegal_index_exception: %d cannot be used as index in the array of size %d\n", i, array.data.array->data->size);
            return;
        }
        array.data.array->funcs->set(array.data.array, i, x);
    }
}

void* dynamic_get(either_dynamic array, int i){
    if(array.result == SUCCESS){
        if(i<0 || i>=(array.data.array->data->size)) {
            printf("illegal_index_exception: %d cannot be used as index in the array of size %d\n", i, array.data.array->data->size);
            return NULL;
        }
        void* out = array.data.array->funcs->get(array.data.array, i);
        return out;
    }else{
        return NULL;
    }
}

either_dynamic dynamic_map(either_dynamic array, void* (*map_func) (void*)) {
    if(array.result == SUCCESS){
        either_dynamic out;
        if(map_func == NULL){
            out.result = NULL_POINTER_EXCEPTION;
            out.data.error_message = "null_pointer_exception: given pointer \"map_func\" was not initialised\n";
            return out;
        }
        out = array.data.array->funcs->map(array.data.array, map_func);
        return out;
    }else{
        return array;
    }
}

bool* dynamic_where(either_dynamic array, bool (*where_func) (void*)) {
    if(array.result == SUCCESS){
        bool* out;
        if(where_func == NULL){
            printf("null_pointer_exception: given pointer \"where_func\" was not initialised\n");
            return NULL;
        }
        out = array.data.array->funcs->where(array.data.array, where_func);
        return out;
    }else{
        return NULL;
    }
}

either_dynamic dynamic_concat(either_dynamic array, either_dynamic other){
    if(array.result == SUCCESS && other.result == SUCCESS){
        either_dynamic out;
        if(array.data.array->funcs->type != other.data.array->funcs->type){
            out.result = INCOMPARABLE_TYPE_EXCEPTION;
            out.data.error_message = "incomparable_type_exception: types of \"array\" and \"other\" are not equal\n";
        }else {
            out = array.data.array->funcs->concat(array.data.array, other.data.array);
        }
        return out;
    }else{
        if(array.result != SUCCESS){
            return array;
        }else{
            return other;
        }
    }
}

either_dynamic dynamic_sort (either_dynamic array){
    if(array.result == SUCCESS){
        either_dynamic out = array.data.array->funcs->sort(array.data.array);
        return out;
    }
    return array;
}

dynamic_vtable_t* get_int_vtable() {
    if(int_vtable == NULL) {
        int_vtable = (dynamic_vtable_t*) malloc(sizeof(dynamic_vtable_t));
        int_vtable->type = INT;
        int_vtable->data_alloc = &_dynamic_data_alloc_int;
        int_vtable->to_string = &_dynamic_to_string_int;
        int_vtable->resize = &_dynamic_resize_int;
        int_vtable->push_back = &_dynamic_pushback_int;
        int_vtable->set = &_dynamic_set_int;
        int_vtable->get = &_dynamic_get_int;
        int_vtable->map = &_dynamic_map_int;
        int_vtable->where = &_dynamic_where_int;
        int_vtable->sort = &_dynamic_sort_int;
        int_vtable->concat = &_dynamic_concat_int;
    }

    return int_vtable;
}

dynamic_vtable_t* get_double_vtable() {
    if(double_vtable == NULL) {
        double_vtable = (dynamic_vtable_t*) malloc(sizeof(dynamic_vtable_t));
        double_vtable->type = DOUBLE;
        double_vtable->data_alloc = &_dynamic_data_alloc_double;
        double_vtable->to_string = &_dynamic_to_string_double;
        double_vtable->resize = &_dynamic_resize_double;
        double_vtable->push_back = &_dynamic_pushback_double;
        double_vtable->set = &_dynamic_set_double;
        double_vtable->get = &_dynamic_get_double;
        double_vtable->map = &_dynamic_map_double;
        double_vtable->where = &_dynamic_where_double;
        double_vtable->sort = &_dynamic_sort_double;
        double_vtable->concat = &_dynamic_concat_double;
    }

    return double_vtable;
}