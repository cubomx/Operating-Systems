#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define THREADS 27
char validSudoku = '1';
int row = 0;
int square = 0;
void *validColumn(void *param);
void *validRow(void *param);
void *validSquare(void *param);
void gettingInfoFromFile(void * param, FILE * file);
void printBoard(void * param);

int main(int argc, char *argv[]) {
    pthread_t tid[THREADS];
    pthread_attr_t attr[THREADS];
    FILE * file;
    if (argc < 2){
        printf("Please send fileName");
        return -1;
    }
    for(int i = 0; i < THREADS; i++){
        pthread_attr_init(&attr[i]);
    }
    file = fopen(argv[1], "r");

    int  sudoku[THREADS/3][THREADS/3];
    /*Getting all the information of the file */
    gettingInfoFromFile((void *) sudoku, file);
    fclose(file);
    printBoard(sudoku);
    int x = 0;
    /* Creation of the threads of validation of column */
    for (int x = 0; x < 9; x++)
        pthread_create(&tid[x], &attr[x], validColumn, (void *) sudoku[x]);
    /*Creation of the threads of validation of row*/
    for(; x < 18; x++){
        pthread_create(&tid[x], &attr[x], validRow, (void *) sudoku);
    }
    /*Creation of the threads of validation of squares in Sudoku*/
    for(; x < THREADS; x++){
        pthread_create(&tid[x], &attr[x], validSquare, (void *) sudoku);
    }
    /*Waiting of all threads to perfomed their task*/
    for(int j = 0; j < THREADS; j++)
        pthread_join(tid[j], NULL);
    /*Checking if there's was repeated value*/
    if(validSudoku == '1'){
        printf("Sudoku well done\n");
    }
    else{
        printf("Bad sudoku\n");
    }

    return 0;
}


void printBoard(void * param){
    int(*sudoku)[9] = param;
    for(int x = 0; x < 9; x++){
        for(int y = 0; y < 9; y++){
            printf("%d ", sudoku[x][y]);
        }
        printf("\n");
    }
}

void gettingInfoFromFile(void * param, FILE * file){
    int index;
    int (*sudoku)[9] = param;
    for(int k = 0; k < 9; k++){
        for(int l = 0; l < 9; l++){
            fscanf(file, "%d", &sudoku[l][k]);
            index++;
        }
    }
}

/*
 * Checking all the columns of sudoku
 */

void *validColumn(void *param) {
    int *nums = (int *) param;
    for (int i = 0; i < 9; i++){
        for (int j = i + 1; j < 9; j++) {
            if (nums[i] == nums[j]) {
                validSudoku = '0';
            }
        }
    }
    pthread_exit(0);
}

/*
 * Checking all the rows of sudoku
 */

void *validRow(void *param) {
    int id = row;
    row++;
    int (*nums)[9] = param;

    for (int i = 0; i < 9; i++){
        for (int j = i+1; j < 9; j++) {
            if (nums[i][id] == nums[j][id]) {
                validSudoku = '0';
            }
        }
    }

    pthread_exit(0);
}


/*
 * Checking all the squares inside the sudoku
 */

void * validSquare(void *param){
    int idSquare = square;
    square++;
    int (*nums)[9] = param;
    int x = 6, y;

    if(idSquare < 3) x = 0;
    else if (idSquare < 6) x = 3;

    y = (idSquare % 3)*3;

    for(int posX = x; posX < x+3; posX++){
        for(int posY = y; posY < y+3; posY++){
            for(int i = posX; i < x+3; i++){
                for(int j = posY; j < y+3; j++){
                    if(posX != i || posY != j){
                        if(nums[posX][posY] == nums[i][j]){
                            validSudoku = '0';
                        }
                    }
                }
            }
        }
    }
    pthread_exit(0);
}
