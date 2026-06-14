#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct {
    int rows;
    int cols;
    double **data;
} Matrix;

typedef struct Layer {
    Matrix *weights;
    Matrix *bias;
    double (*activate)(double x);
    Matrix *output;
} Layer;

typedef struct {
    Layer **layers;
    int num_layers;
} Network;

Layer *create_layer(int input_size, int output_size, double (*activate)(double x));
Matrix *create_matrix(int rows, int cols);
void free_matrix(Matrix *matrix);
void print_matrix(Matrix *matrix);
void set_value(Matrix *matrix, int row, int col, double value);
Matrix *multiply(Matrix *a, Matrix*b);
Matrix *add_matrices(Matrix *a, Matrix*b);
Matrix *apply_activation(Matrix *m, double (*activate)(double x));
Matrix *forward(Layer *layer, Matrix *input);
Network *create_network(int *layer_sizes, int num_layers, double (*activate)(double x));
Matrix *forward_network(Network *net, Matrix *input);
double relu(double x);
double sigmoid(double x);

int main () {
    srand(time(NULL));
    int sizes[] = {2, 3, 1};
    Network *net = create_network(sizes, 3, sigmoid);

    Matrix *input = create_matrix(2, 1);
    set_value(input, 0, 0, 0.5);
    set_value(input, 1, 0, 0.8);

    Matrix *output = forward_network(net, input);
    print_matrix(output);
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

Layer *create_layer(int input_size, int output_size, double (*activate)(double x)) {
    Layer *layer = malloc(sizeof(Layer));

    if (layer == NULL) return NULL;

    layer->weights = create_matrix(output_size, input_size);

    for (int i = 0; i < output_size; i++) {
        for (int k = 0; k < input_size; k++) {
            layer->weights->data[i][k] = ((double)rand() / RAND_MAX) * 2 - 1;
        }
    }

    layer->bias = create_matrix(output_size, 1);
    layer->activate = activate;
    layer->output = NULL;

    return layer;
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

double relu(double x) {
    if (x > 0) {
        return x;
    }
    return 0;
}

double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

Matrix *multiply(Matrix *a, Matrix *b) {
    if (a->cols != b->rows) return NULL;
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

Matrix *add_matrices(Matrix *a, Matrix *b) {
    if (a->rows != b->rows || a->cols != b->cols) return NULL;
    Matrix *res = create_matrix(a->rows, a->cols);

    for (int i = 0; i < res->rows; i++) {
        for (int k = 0; k < res->cols; k++) {
            res->data[i][k] = a->data[i][k] + b->data[i][k];
        }
    }

    return res;
}

Matrix *apply_activation(Matrix *m, double (*activate)(double x)) {
    if (m == NULL) return NULL;

    Matrix *res = create_matrix(m->rows, m->cols);

    for (int i = 0; i < res->rows; i++) {
        for (int k = 0; k < res->cols; k++) {
            res->data[i][k] = activate(m->data[i][k]);
        }
    }

    return res;
}

Matrix *forward(Layer *layer, Matrix *input) {
    if (layer == NULL) return NULL;

    Matrix *z = multiply(layer->weights, input); 
    Matrix *z_bias = add_matrices(z, layer->bias);
    layer->output = apply_activation(z_bias, layer->activate);

    free_matrix(z);
    free_matrix(z_bias);

    return layer->output;
}

Network *create_network(int *layer_sizes, int num_layers, double (*activate)(double x)) {
    Network *net = malloc(sizeof(Network));
    if (net == NULL) return NULL;
    net->num_layers = num_layers;
    net->layers = malloc((num_layers - 1) * sizeof(Layer *));

    for (int i = 0; i < num_layers - 1; i++) {
        net->layers[i] = create_layer(layer_sizes[i], layer_sizes[i + 1], activate);
    }

    return net;
}

Matrix *forward_network(Network *net, Matrix *input) {
    if (net == NULL) return NULL;
    Matrix *output = NULL;

    for (int i = 0; i < net->num_layers - 1; i++) {
        output = forward(net->layers[i], input);
        input = output;
    }
    return output;
}