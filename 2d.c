#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

Matrix *create_matrix(int rows, int cols);
void free_matrix(Matrix *matrix);
void print_matrix(Matrix *matrix);
void set_value(Matrix *matrix, int row, int col, double value);
Matrix *multiply(Matrix *a, Matrix *b);

int main () {
    Matrix *aa = create_matrix(2, 2);
    Matrix *bb = create_matrix(2, 2);
    Matrix *m = NULL;

    set_value(aa, 0, 0, 1);
    set_value(aa, 0, 1, 2);
    set_value(aa, 1, 0, 3);
    set_value(aa, 1, 1, 4);

    set_value(bb, 0, 0, 5);
    set_value(bb, 0, 1, 6);
    set_value(bb, 1, 0, 7);
    set_value(bb, 1, 1, 8);

    print_matrix(aa);
    printf("--\n");
    print_matrix(bb);
    printf("--\n");
    m = multiply(aa, bb);
    print_matrix(m);

    free_matrix(aa);
    free_matrix(bb);
    free_matrix(m);

    return 0;
}

Matrix *create_matrix(int rows, int cols) {
    Matrix *matrix = malloc(sizeof(Matrix));

    if (matrix == NULL) {
        printf("what");
        return NULL;
    }

    matrix->rows = rows;
    matrix->cols = cols;

    matrix->data = malloc(rows * sizeof(double *));
    if (matrix->data == NULL) {
        printf("what");
        return NULL;
    }
    for (int i = 0; i < rows; i++) {
        matrix->data[i] = malloc(cols * sizeof(double));
        if (matrix->data[i] == NULL) {
            printf("what");
            return NULL;
        }
        for (int k = 0; k < cols; k++) {
            matrix->data[i][k] = 0;
        }
    }
    
    return matrix;
}

void free_matrix(Matrix *matrix) {
    for (int i = 0; i < matrix->rows; i++) {
        free(matrix->data[i]);
    }
    free(matrix->data);
    matrix->data = NULL;
    free(matrix);
}

void print_matrix(Matrix *matrix){
    for (int i = 0; i < matrix->rows; i++) {
        for (int k = 0; k < matrix->cols; k++) {
            printf("%f ", matrix->data[i][k]);
        }
        printf("\n");
    }
}

void set_value(Matrix *matrix, int row, int col, double value) {
    if (row >= matrix->rows || col >= matrix->cols) return;
    matrix->data[row][col] = value;
}

Matrix *multiply(Matrix *a, Matrix *b) {
    if (a->rows != b->cols) return NULL;
    Matrix *res = create_matrix(a->rows, b->cols);

    for (int i = 0; i < a->rows; i++) {
        for (int k = 0; k < a->cols; k++) {
            for (int j = 0; j < b->cols; j++) {
                res->data[i][j] += a->data[i][k] * b->data[k][j];
            }
        }
    }

    return res;
}