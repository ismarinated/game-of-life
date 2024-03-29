#include <stdio.h>
#include <stdlib.h>
#define HEIGH 25
#define WIDTH 80

void fill_matrix(int **matrix, int *path_file);
void creature(int **matrix, int **tmp);
void creature_live(const int *cell, int *tmp, int sum);
void clear_matrix(int **matrix);
void copy_matrix(int **matrix1, int **matrix2);
int check_condition(int **matrix, int **temp, int **cycle, int *count);
void output_the_board(int **board);
void input_time_stop(int *time_stop);
void input_path_file(int *path_file);
void time_sleep(int time_life);
void edges(int **matrix, int **tmp, int *sum);
void corners(int **matrix, int **tmp, int *sum);

int main() {
    int count = 0;
    int time_life;
    int path_file;
    int **cycle = malloc(HEIGH * sizeof(int *));
    int **life = malloc(HEIGH * sizeof(int *));
    int **temp = malloc(HEIGH * sizeof(int *));
    int **tmp = malloc(HEIGH * sizeof(int *));
    for (int i = 0; i < HEIGH; i++) {
        life[i] = malloc(WIDTH * sizeof(int));
        temp[i] = malloc(WIDTH * sizeof(int));
        tmp[i] = malloc(WIDTH * sizeof(int));
        cycle[i] = malloc(WIDTH * sizeof(int));
    }

    input_time_stop(&time_life);
    input_path_file(&path_file);
    fill_matrix(life, &path_file);
    output_the_board(life);
    time_sleep(time_life);
    copy_matrix(life, cycle);
    copy_matrix(life, temp);
    copy_matrix(life, tmp);
    creature(life, tmp);
    output_the_board(life);
    time_sleep(time_life);
    while (!check_condition(life, temp, cycle, &count)) {
        if (count == 0) {
            copy_matrix(life, cycle);
        }
        copy_matrix(life, temp);
        copy_matrix(life, tmp);
        creature(life, tmp);
        output_the_board(life);
        time_sleep(time_life);
        count++;
    }
    clear_matrix(life);
    clear_matrix(temp);
    clear_matrix(tmp);
    clear_matrix(cycle);
    return 0;
}

void clear_matrix(int **matrix) {
    for (int i = 0; i < HEIGH; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fill_matrix(int **matrix, int *path_file) {
    FILE *file;
    char ch;

    switch (*path_file) {
        case 1:
            file = fopen("./1.frm", "r");
            break;
        case 2:
            file = fopen("./2.frm", "r");
            break;
        case 3:
            file = fopen("./3.frm", "r");
            break;
        case 4:
            file = fopen("./4.frm", "r");
            break;
        case 5:
            file = fopen("./5.frm", "r");
            break;
    }

    for (int i = 0; i < HEIGH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            fscanf(file, "%c", &ch);
            if (ch == '1') {
                matrix[i][j] = 1;
            } else if (ch == '0') {
                matrix[i][j] = 0;
            }
        }
    }
    fclose(file);
}

void creature(int **matrix, int **tmp) {
    int sum = 0;

    for (int i = 0; i < HEIGH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i <= HEIGH - 2 && i >= 1 && j >= 1 && j <= WIDTH - 2) {
                sum = matrix[i - 1][j - 1] + matrix[i - 1][j] + matrix[i - 1][j + 1] + matrix[i][j - 1] +
                      matrix[i][j + 1] + matrix[i + 1][j - 1] + matrix[i + 1][j] + matrix[i + 1][j + 1];
                creature_live(&matrix[i][j], &tmp[i][j], sum);
            }
        }
    }
    edges(matrix, tmp, &sum);
    corners(matrix, tmp, &sum);
    copy_matrix(tmp, matrix);
}

void edges(int **matrix, int **tmp, int *sum) {
    for (int i = 0; i < HEIGH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (HEIGH - 2 >= i && i >= 1) {
                if (j == 0) {
                    *sum = matrix[i - 1][WIDTH - 1] + matrix[i - 1][j] + matrix[i - 1][j + 1] +
                           matrix[i][WIDTH - 1] + matrix[i][j + 1] + matrix[i + 1][WIDTH - 1] +
                           matrix[i + 1][j] + matrix[i + 1][j + 1];
                    creature_live(&matrix[i][j], &tmp[i][j], *sum);
                } else if (j == WIDTH - 1) {
                    *sum = matrix[i - 1][j - 1] + matrix[i - 1][j] + matrix[i - 1][0] + matrix[i][j - 1] +
                           matrix[i][0] + matrix[i + 1][j - 1] + matrix[i + 1][j] + matrix[i + 1][0];
                    creature_live(&matrix[i][j], &tmp[i][j], *sum);
                }
            } else if (WIDTH - 2 >= j && j >= 1) {
                if (i == 0) {
                    *sum = matrix[HEIGH - 1][j - 1] + matrix[HEIGH - 1][j] + matrix[HEIGH - 1][j + 1] +
                           matrix[i][j - 1] + matrix[i][j + 1] + matrix[i + 1][j - 1] + matrix[i + 1][j] +
                           matrix[i + 1][j + 1];
                    creature_live(&matrix[i][j], &tmp[i][j], *sum);
                } else if (i == HEIGH - 1) {
                    *sum = matrix[i - 1][j - 1] + matrix[i - 1][j] + matrix[i - 1][j + 1] + matrix[i][j - 1] +
                           matrix[i][j + 1] + matrix[0][j - 1] + matrix[0][j] + matrix[0][j + 1];
                    creature_live(&matrix[i][j], &tmp[i][j], *sum);
                }
            }
        }
    }
}

void corners(int **matrix, int **tmp, int *sum) {
    for (int i = 0; i < HEIGH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 && j == 0) {
                *sum = matrix[HEIGH - 1][WIDTH - 1] + matrix[HEIGH - 1][0] + matrix[HEIGH - 1][1] +
                       matrix[0][WIDTH - 1] + matrix[i][j + 1] + matrix[1][WIDTH - 1] + matrix[i + 1][j] +
                       matrix[i + 1][j + 1];
                creature_live(&matrix[i][j], &tmp[i][j], *sum);
            } else if (i == 0 && j == WIDTH - 1) {
                *sum = matrix[HEIGH - 1][WIDTH - 2] + matrix[HEIGH - 1][WIDTH - 1] + matrix[HEIGH - 1][0] +
                       matrix[i][j - 1] + matrix[0][0] + matrix[i + 1][j - 1] + matrix[i + 1][j] +
                       matrix[1][0];
                creature_live(&matrix[i][j], &tmp[i][j], *sum);
            } else if (i == HEIGH - 1 && j == 0) {
                *sum = matrix[HEIGH - 2][WIDTH - 1] + matrix[i - 1][j] + matrix[i - 1][j + 1] +
                       matrix[HEIGH - 1][WIDTH - 1] + matrix[i][j + 1] + matrix[0][WIDTH - 1] + matrix[0][0] +
                       matrix[0][1];
                creature_live(&matrix[i][j], &tmp[i][j], *sum);
            } else if (i == HEIGH - 1 && j == WIDTH - 1) {
                *sum = matrix[i - 1][j - 1] + matrix[i - 1][j] + matrix[HEIGH - 2][0] + matrix[i][j - 1] +
                       matrix[HEIGH - 1][0] + matrix[0][WIDTH - 2] + matrix[0][WIDTH - 1] + matrix[0][0];
                creature_live(&matrix[i][j], &tmp[i][j], *sum);
            }
        }
    }
}

void creature_live(const int *cell, int *tmp, int sum) {
    if (*cell == 1 && (sum < 2 || sum > 3)) {
        *tmp = 0;
    } else if (*cell == 0 && sum == 3) {
        *tmp = 1;
    } else {
        *tmp = *cell;
    }
}

void copy_matrix(int **matrix1, int **matrix2) {
    for (int i = 0; i < HEIGH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            matrix2[i][j] = matrix1[i][j];
        }
    }
}

int check_condition(int **matrix, int **temp, int **cycle, int *count) {
    int is_stable1 = 0;
    int is_stable2 = 0;
    for (int i = 0; i < HEIGH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (matrix[i][j] == temp[i][j]) {
                is_stable1 += 1;
            }
        }
    }
    int sum = 0;
    for (int i = 0; i < HEIGH; i++) {
        for (int j = 0; j < WIDTH; j++) {
            sum += matrix[i][j];
        }
    }
    if (*count == 2) {
        *count = 0;
        for (int i = 0; i < HEIGH; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (matrix[i][j] == cycle[i][j]) {
                    is_stable2 += 1;
                }
            }
        }
    }
    return ((is_stable1 == HEIGH * WIDTH) || (is_stable2 == HEIGH * WIDTH) || sum == 0);
}

void output_the_board(int **board) {
    // system("clear");
    printf("\n");
    for (int i = 0; i < HEIGH; i++) {
        // if (i == 0) {
        //     for (int k = 0; k < HEIGH + 2; k++) {
        //         printf(" ——");
        //     }
        //     printf("\n");
        // }
        printf("|");
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == 1) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        // printf("|");
        printf("\n");
        // if (i == HEIGH - 1) {
        //     for (int k = 0; k < HEIGH + 2; k++) {
        //         printf(" ——");
        //     }
        // }
    }
    printf("\n");
}

void time_sleep(int time_life) {
    long long time_life_full = 9999999 * time_life;
    long long n = 1;

    for (long long i = n; i < time_life_full; i++) {
    }
}

void input_time_stop(int *time_stop) {
    printf("Введите время жизни первого поколения (по умолчанию 1 секунда)\n");

    if ((!scanf("%d", time_stop)) || (*time_stop == 0)) {
        *time_stop = 1;
    }
}

void input_path_file(int *path_file) {
    printf("Введите номер файла в котором хранится конфигурация стартового существа (от 1 до 5)\n");

    if ((!scanf("%d", path_file)) || (*path_file < 1) || (*path_file > 5)) {
        *path_file = 1;
    }
}
