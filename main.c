#include <stdlib.h>
#include <stdio.h>
#include "dynamic.h"

void* qqq_i(void* x){
    int* in = malloc(sizeof(int));
    *in=(*(int*)x)*3;
    return (void*)in;
}

void* qqq_d(void* x){
    double* in = malloc(sizeof(double));
    *in=(*(double*)x)*3;
    return (void*)in;
}

bool rrr_i(void* x){
    return ((*((int*)x))>2.0)==1;
}

bool rrr_d(void* x){
    return ((*((double*)x))>2.0)==1;
}

int main()
{
    FILE *test = fopen("test_int.txt", "r");
    dynamic metarray[100];
    char command[1];
    char name[3];
    int cnt=0;
    int array_i, array_i2, i, *out_i, in_int;
    double in_d, *out_d;
    bool *filter;
    if (!test)
    {
        printf("Error: test cannot be opened\n");
        return 1;
    }
    while((fscanf(test, "%1s ", command))!=EOF)
    {
        if(strcmp(command, "n") == 0){
            fscanf(test, "%3s ", name);
            if(strcmp(name, "int") == 0){
                metarray[cnt] = new_dynamic_empty(get_int_vtable());
                cnt++;
            }else if(strcmp(name, "dbl") == 0){
                metarray[cnt] = new_dynamic_empty(get_double_vtable());
                cnt++;
            }else if(strcmp(name, "map") == 0){
                fscanf(test, "%d\n", &array_i);
                if(metarray[array_i-1].result == SUCCESS && metarray[array_i-1].data.array->funcs->type==INT) {
                    metarray[cnt] = dynamic_map(metarray[array_i-1], &qqq_i);
                }else if(metarray[array_i-1].result == SUCCESS && metarray[array_i-1].data.array->funcs->type==DOUBLE){
                    metarray[cnt] = dynamic_map(metarray[array_i-1], &qqq_d);
                }
                cnt++;
            }else if(strcmp(name, "cnt") == 0){
                fscanf(test, "%d %d\n", &array_i, &array_i2);
                metarray[cnt] = dynamic_concat(metarray[array_i-1], metarray[array_i2-1]);
                cnt++;
            }else if(strcmp(name, "srt") == 0){
                fscanf(test, "%d\n", &array_i);
                metarray[cnt] = dynamic_sort(metarray[array_i-1]);
                cnt++;
            }else{
                printf("Error: corrupted test\n");
                return 1;
            }
        }else if(strcmp(command, "e") == 0) {
            fscanf(test,"%d %3s ", &array_i, name);
            if(array_i>cnt){
                printf("Error: corrupted test\n");
                return 1;
            }
            if(strcmp(name, "out") == 0) {
                printf("%s", dynamic_to_string(metarray[array_i-1]));
            }else if(strcmp(name, "rsz") == 0){
                fscanf(test, "%d\n", &in_int);
                dynamic_resize(metarray[array_i-1], in_int);
            }else if(strcmp(name, "add") == 0){
                if(metarray[array_i-1].result == SUCCESS && metarray[array_i-1].data.array->funcs->type==INT) {
                    fscanf(test, "%d\n", &in_int);
                    dynamic_pushback(metarray[array_i-1], &in_int);
                }else if(metarray[array_i-1].result == SUCCESS && metarray[array_i-1].data.array->funcs->type==DOUBLE){
                    fscanf(test, "%lf\n", &in_d);
                    dynamic_pushback(metarray[array_i-1], &in_d);
                }
            }else if(strcmp(name, "get") == 0){
                fscanf(test, "%d\n", &i);
                if(metarray[array_i-1].result == SUCCESS && metarray[array_i-1].data.array->funcs->type==INT) {
                    out_i = (int*)dynamic_get(metarray[array_i-1], i-1);
                    printf("%d\n", *out_i);
                    free(out_i);
                }else if(metarray[array_i-1].result == SUCCESS && metarray[array_i-1].data.array->funcs->type==DOUBLE){
                    out_d = (double*)dynamic_get(metarray[array_i-1], i-1);
                    printf("%lf\n", *out_d);
                    free(out_d);
                }
            }else if(strcmp(name, "set") == 0) {
                if (metarray[array_i - 1].result == SUCCESS && metarray[array_i - 1].data.array->funcs->type == INT) {
                    fscanf(test, "%d %d\n", &i, &in_int);
                    dynamic_set(metarray[array_i - 1], i-1, &in_int);
                } else if (metarray[array_i - 1].result == SUCCESS && metarray[array_i - 1].data.array->funcs->type == DOUBLE) {
                    fscanf(test, "%d %lf\n", &i, &in_d);
                    dynamic_set(metarray[array_i - 1], i-1, &in_d);
                }
            }else if(strcmp(name, "whr") == 0) {
                if(metarray[array_i-1].result == SUCCESS && metarray[array_i-1].data.array->funcs->type==INT) {
                    filter = dynamic_where(metarray[array_i-1], &rrr_i);
                    for(int j=0;j<metarray[array_i-1].data.array->data->size;j++){
                        printf("%d ", filter[j]);
                    }
                    printf("\n");
                    free(filter);
                }else if(metarray[array_i-1].result == SUCCESS && metarray[array_i-1].data.array->funcs->type==DOUBLE){
                    filter = dynamic_where(metarray[array_i-1], &rrr_d);
                    for(int j=0;j<metarray[array_i-1].data.array->data->size;j++){
                        printf("%d ", filter[j]);
                    }
                    printf("\n");
                    free(filter);
                }
                else{
                    printf("Error: corrupted test\n");
                    return 1;
                }
            }
        }else{
            printf("Error: corrupted test\n");
            return 1;
        }
    }
    for(int j=0;j<cnt;j++){
        dynamic_destroy(metarray[j]);
    }
    return EXIT_SUCCESS;
}