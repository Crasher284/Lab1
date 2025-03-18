#include "dynamic.h"

void _dynamic_data_alloc_double(dynamic_data_t* data, int size){
    data->data = (double*)malloc(sizeof(double)*size);

    data->size = 0;
    data->available_size = size;
}

char* _dynamic_to_string_double(dynamic_t* array){
    int len_cnt = 0;
    double* data = (double*)array->data->data;
    for(int i=0;i<array->data->size;i++){
        len_cnt += snprintf(NULL, 0, "%f ", data[i]);
    }
    int current_size = 0;
    char* out = malloc(sizeof(char)*(len_cnt+2));
    for (int i = 0; i < array->data->size; i++) {
        current_size += snprintf(out+current_size, len_cnt+2-current_size, "%f ", data[i]);
    }
    snprintf(out+current_size, len_cnt+2-current_size, "%s", "\n");
    return out;
}

void _dynamic_resize_double(dynamic_t* array, int new_size){
    void* new_data = malloc(sizeof(double)*new_size);
    if(new_size>(array->data->size)){
        memcpy(new_data, array->data->data, sizeof(double)*(array->data->size));
    }else{
        memcpy(new_data, array->data->data, sizeof(double)*(new_size));
        array->data->size = new_size;
    }
    free(array->data->data);
    array->data->data = new_data;
    array->data->available_size = new_size;
}

void _dynamic_pushback_double(dynamic_t* array, void* x){
    double* elem = (double*)x;
    double* elems = (double*)(array->data->data);
    memcpy(elems+(array->data->size), elem, sizeof(double));
}

void _dynamic_set_double(dynamic_t* array, int i, void* x){
    double* elem = (double*)x;
    double* place = (double*)(array->data->data)+i;
    memcpy(place, elem, sizeof(double));
}

void* _dynamic_get_double(dynamic_t* array, int i){
    double* out = (double*)malloc(sizeof(double));
    memcpy(out, (double*)(array->data->data)+i, sizeof(double));
    return out;
}

either_dynamic _dynamic_map_double(dynamic_t* array, void* (*map_func) (void*)){
    double* data = (double*)array->data->data;
    either_dynamic out = new_dynamic_sized(array->funcs, array->data->size);
    if(out.result == SUCCESS) {
        for (int i = 0; i < array->data->size; i++) {
            double* res = (double*)map_func(data + i);
            dynamic_pushback(out, res);
            free(res);
        }
    }
    return out;
}

bool* _dynamic_where_double(dynamic_t* array, bool (*where_func) (void*)){
    bool* out = malloc(sizeof(bool)*(array->data->size));
    double* data = (double*)array->data->data;
    for (int i = 0; i < array->data->size; i++) {
        out[i] = where_func(data + i);
    }
    return out;
}

either_dynamic _dynamic_concat_double(dynamic_t* array, dynamic_t* other){
    either_dynamic out = new_dynamic_sized(array->funcs, array->data->size+other->data->size);
    if(out.result == SUCCESS){
        out.data.array->data->size = array->data->size+other->data->size;
        double* new = (double*)out.data.array->data->data;
        double* data = (double*)array->data->data;
        double* other_data = (double*)other->data->data;
        memcpy(new, data, sizeof(double)*(array->data->size));
        memcpy(new+(array->data->size), other_data, sizeof(double)*(other->data->size));
    }
    return out;
}

either_dynamic _dynamic_sort_double(dynamic_t* array){
    either_dynamic out = new_dynamic_sized(array->funcs, array->data->size);
    if(out.result == SUCCESS){
        out.data.array->data->size = array->data->size;
        double* sorted = _merge_sort_double(array->data->size, (double*)array->data->data);
        memcpy(out.data.array->data->data, sorted, sizeof(double)*array->data->size);
        free(sorted);
    }
    return out;
}

double* _merge_sort_double (int size, double* data){
    if(size<=1){
        double* out = (double*) malloc(sizeof(double)*size);
        memcpy(out, data, sizeof(double)*size);
        return out;
    }else{
        double* first = malloc(sizeof(double)*(size/2));
        memcpy(first, data, sizeof(double)*(size/2));
        double* second = malloc(sizeof(double)*(size/2+size%2));
        memcpy(second, data+size/2, sizeof(double)*(size/2+size%2));
        double* sorted_f = _merge_sort_double(size/2, first);
        double* sorted_s = _merge_sort_double(size/2+size%2, second);
        free(first);
        free(second);
        double* out = (double*) malloc(sizeof(double)*size);
        double* current_f;
        double* current_s;
        int id_f=0, id_s=0;
        for(int i=0;i<size;i++){
            current_f = sorted_f+id_f;
            current_s = sorted_s+id_s;
            if(*current_f<*current_s){
                memcpy(out+(id_f+id_s), current_f, sizeof(double));
                id_f++;
            }else{
                memcpy(out+(id_f+id_s), current_s, sizeof(double));
                id_s++;
            }
            if(id_f==size/2){
                memcpy(out+(id_f+id_s), current_s, sizeof(double)*(size/2+size%2-id_s));
                break;
            }
            if(id_s==size/2+size%2){
                memcpy(out+(id_f+id_s), current_f, sizeof(double)*(size/2-id_f));
                break;
            }
        }
        free(sorted_f);
        free(sorted_s);
        return out;
    }
}