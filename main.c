/**
 * gcc -o sudoku main.c -lpthread
 *
 **/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#define REVISIONES 27
char valido = '1';
int row = 0;
void *validoColumna(void *param);
void *validoFila(void * param);
void *validoCuadro(void *param);
void *checaCadaLinea(int start, int end, char* []);


int main(int argc, char *argv[]) {
    pthread_t tid[27];
    pthread_attr_t attr[27];
    FILE * file;
    if (argc < 2){
        printf("Please send fileName");
        return -1;
    }
    for(int i = 0; i < 18; i++){
        pthread_attr_init(&attr[i]);
    }
    file = fopen(argv[1], "r");
    int  sudoku[9][9];
    int index;
    for(int k = 0; k < 9; k++){
        for(int l = 0; l < 9; l++){
            fscanf(file, "%d", &sudoku[l][k]);
            index++;
        }
    }

    fclose(file);
    int x = 0;
    for (int x = 0; x < 9; x++)
        pthread_create(&tid[x], &attr[x], validoColumna, (void *)sudoku[x]);

    for(x = 9; x < 18; x++){
        pthread_create(&tid[x], &attr[x], validoFila, (void *)sudoku);
    }

    for(int j = 0; j < 18; j++)
        pthread_join(tid[j], NULL);

    if(valido == '1'){
        printf("Sudoku well done");
    }
    else{
        printf("Bad sudoku");
    }

    return 0;
}

void *validoColumna(void  * param) {
    int *nums = (int *) param;
    for (int i = 0; i < 9; i++){
        for (int j = i + 1; j < 9; j++) {
            if (nums[i] == nums[j]) {
                valido = '0';
            }
        }
    }

    pthread_exit(0);
}

void *validoFila(void  * param) {
    int id = row;
    row++;
    printf("%d\n", id);
    int (*nums)[9] = param;
    for (int i = 0; i < 9; i++){
        for (int j = i + 1; j < 9; j++) {
            if (nums[i][row] == nums[j][row]) {
                valido = '0';
            }
        }
    }

    pthread_exit(0);
}
