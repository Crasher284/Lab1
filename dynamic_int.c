#include "dynamic.h"

void _dynamic_data_alloc_int(dynamic_data_t* data, int size){
    data->data = malloc(sizeof(int)*size);

    data->size = 0;
    data->available_size = size;
}

char* _dynamic_to_string_int(dynamic_t* array){
    int len_cnt = 0;
    int* data = (int*)array->data->data;
    for(int i=0;i<array->data->size;i++){
        len_cnt += snprintf(NULL, 0, "%d ", data[i]);
    }
    int current_size = 0;
    char* out = malloc(sizeof(char)*(len_cnt+2));
    for (int i = 0; i < array->data->size; i++) {
        current_size += snprintf(out+current_size, len_cnt+2-current_size, "%d ", data[i]);
    }
    snprintf(out+current_size, len_cnt+2-current_size, "%s", "\n");
    return out;
}

void _dynamic_resize_int(dynamic_t* array, int new_size){
    void* new_data = malloc(sizeof(int)*new_size);
    if(new_size>(array->data->size)){
        memcpy(new_data, array->data->data, sizeof(int)*(array->data->size));
    }else{
        memcpy(new_data, array->data->data, sizeof(int)*(new_size));
        array->data->size = new_size;
    }
    free(array->data->data);
    array->data->data = new_data;
    array->data->available_size = new_size;
}

void _dynamic_pushback_int(dynamic_t* array, void* x){
    int* elem = (int*)x;
    int* elems = (int*)(array->data->data);
    memcpy(elems+(array->data->size), elem, sizeof(int));
}

void _dynamic_set_int(dynamic_t* array, int i, void* x){
    int* elem = (int*)x;
    int* place = (int*)(array->data->data)+i;
    memcpy(place, elem, sizeof(int));
}

void* _dynamic_get_int(dynamic_t* array, int i){
    int* out = (int*)malloc(sizeof(int));
    memcpy(out, (int*)(array->data->data)+i, sizeof(int));
    return out;
}

either_dynamic _dynamic_map_int(dynamic_t* array, void* (*map_func) (void*)){
    int* data = (int*)array->data->data;
    either_dynamic out = new_dynamic_sized(array->funcs, array->data->size);
    if(out.result == SUCCESS) {
        for (int i = 0; i < array->data->size; i++) {
            int* res = (int*)map_func(data + i);
            dynamic_pushback(out, res);
            free(res);
        }
    }
    return out;
}

bool* _dynamic_where_int(dynamic_t* array, bool (*where_func) (void*)){
    bool* out = malloc(sizeof(bool)*(array->data->size));
    int* data = (int*)array->data->data;
    for (int i = 0; i < array->data->size; i++) {
        out[i] = where_func(data + i);
    }
    return out;
}

either_dynamic _dynamic_concat_int(dynamic_t* array, dynamic_t* other){
    either_dynamic out = new_dynamic_sized(array->funcs, array->data->size+other->data->size);
    if(out.result == SUCCESS){
        out.data.array->data->size = array->data->size+other->data->size;
        int* new = (int*)out.data.array->data->data;
        int* data = (int*)array->data->data;
        int* other_data = (int*)other->data->data;
        memcpy(new, data, sizeof(int)*(array->data->size));
        memcpy(new+(array->data->size), other_data, sizeof(int)*(other->data->size));
    }
    return out;
}

either_dynamic _dynamic_sort_int(dynamic_t* array){
    either_dynamic out = new_dynamic_sized(array->funcs, array->data->size);
    if(out.result == SUCCESS){
        out.data.array->data->size = array->data->size;
        int* sorted = _merge_sort_int(array->data->size, (int*)array->data->data);
        memcpy(out.data.array->data->data, sorted, sizeof(int)*array->data->size);
        free(sorted);
    }
    return out;
}

int* _merge_sort_int (int size, int* data){
    if(size<=1){
        int* out = (int*) malloc(sizeof(int)*size);
        memcpy(out, data, sizeof(int)*size);
        return out;
    }else{
        int* first = malloc(sizeof(int)*size/2);
        memcpy(first, data, sizeof(int)*(size/2));
        int* second = malloc(sizeof(int)*(size/2+size%2));
        memcpy(second, data+size/2, sizeof(int)*(size/2+size%2));
        int* sorted_f = _merge_sort_int(size/2, first);
        int* sorted_s = _merge_sort_int(size/2+size%2, second);
        free(first);
        free(second);
        int* out = (int*) malloc(sizeof(int)*size);
        int* current_f;
        int* current_s;
        int id_f=0, id_s=0;
        for(int i=0;i<size;i++){
            current_f = sorted_f+id_f;
            current_s = sorted_s+id_s;
            if(*current_f<*current_s){
                memcpy(out+(id_f+id_s), current_f, sizeof(int));
                id_f++;
            }else{
                memcpy(out+(id_f+id_s), current_s, sizeof(int));
                id_s++;
            }
            if(id_f==size/2){
                memcpy(out+(id_f+id_s), current_s, sizeof(int)*(size/2+size%2-id_s));
                break;
            }
            if(id_s==size/2+size%2){
                memcpy(out+(id_f+id_s), current_f, sizeof(int)*(size/2-id_f));
                break;
            }
        }
        free(sorted_f);
        free(sorted_s);
        return out;
    }
}