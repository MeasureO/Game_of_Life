#include <math.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define X 80
#define Y 25
#define lowest_speed 62500

void graphField(int **a);
void calculateNextField(int **a);
int inputStartField(int **a);
// int countField(int **a);
int countNeighbors(int **a, int i, int j);
int compareFields(int **a, int **b);
int getKey(int *speed, int speed_change, int *speed_coef);

int main() {
    initscr();
    noecho();
    int **current_field = malloc(X * Y * sizeof(int) + Y * sizeof(int *));
    int *pc = (int *)(current_field + Y);
    for (int i = 0; i < Y; i++) current_field[i] = pc + X * i;

    if (inputStartField(current_field)) {
        int epoch_number = 1;
        int speed = 30001;
        int speed_coef = 13;
        int speed_change = 2500;
        FILE *f = freopen("/dev/tty", "r", stdin);
        while (1) {
            keypad(stdscr, TRUE);
            nodelay(stdscr, TRUE);
            graphField(current_field);
            epoch_number++;
            mvprintw(25, 30, "Speed mode");
            mvprintw(27, 32, "%3d", speed_coef);
            mvprintw(29, 30, "Epoch number");
            mvprintw(31, 15, "%20llu", epoch_number);
            usleep(ceil(speed));
            refresh();

            if (!getKey(&speed, speed_change, &speed_coef)) {
                break;
            }
            calculateNextField(current_field);
            // if (countField(current_field) == 0) break;
        }
        fclose(f);
    }

    free(current_field);
    endwin();
    return 0;
}

int getKey(int *speed, int speed_change, int *speed_coef) {
    int flag = 1;
    switch (getch()) {
        case KEY_UP:
            if (*speed - speed_change > 0) {
                *speed -= speed_change;
                *speed_coef += 1;
            }
            break;
        case KEY_DOWN:
            if (*speed + speed_change < lowest_speed) {
                *speed += speed_change;
                *speed_coef -= 1;
            }
            break;

        case 27:
            flag = 0;
    }
    return flag;
}

int inputStartField(int **a) {
    int flag = 1;
    int d = -1;
    int code;
    for (int i = 0; i < Y && flag == 1; i++) {
        for (int j = 0; j < X && flag == 1; j++) {
            code = scanf("%d", &d);
            if (code == 1 && (d == 0 || d == 1)) {
                a[i][j] = d;
            } else {
                flag = 0;
            }
        }
    }
    return flag;
}

void graphField(int **a) {
    // printf("\033c");
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (a[i][j] == 1) mvprintw(i, j, "*");
            if (a[i][j] == 0) mvprintw(i, j, "%c ", ' ');
        }
    }
}

void calculateNextField(int **current_field) {
    int **next_field = malloc(X * Y * sizeof(int) + Y * sizeof(int *));
    int *pn = (int *)(next_field + Y);
    for (int i = 0; i < Y; i++) next_field[i] = pn + X * i;

    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            next_field[i][j] = 0;
        }
    }

    int tmp;
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            tmp = countNeighbors(current_field, i, j);
            if ((current_field[i][j] == 0 && tmp == 3) ||
                (current_field[i][j] == 1 && (tmp == 2 || tmp == 3)))
                next_field[i][j] = 1;
            else
                next_field[i][j] = 0;
        }
    }

    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            current_field[i][j] = next_field[i][j];
        }
    }

    free(next_field);
}

/*
int countField(int **a) {
    int count = 0;
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (a[i][j] == 1) count += 1;
        }
    }
    return count;
}
*/

int countNeighbors(int **a, int i, int j) {
    int count = 0;
    if (a[(i + 1 + 25) % 25][(j - 1 + 80) % 80] == 1) {
        count++;
    }
    if (a[(i + 1 + 25) % 25][(j + 80) % 80] == 1) {
        count++;
    }
    if (a[(i + 1 + 25) % 25][(j + 1 + 80) % 80] == 1) {
        count++;
    }
    if (a[(i + 25) % 25][(j - 1 + 80) % 80] == 1) {
        count++;
    }
    if (a[(i + 25) % 25][(j + 1 + 80) % 80] == 1) {
        count++;
    }
    if (a[(i - 1 + 25) % 25][(j - 1 + 80) % 80] == 1) {
        count++;
    }
    if (a[(i - 1 + 25) % 25][(j + 80) % 80] == 1) {
        count++;
    }
    if (a[(i - 1 + 25) % 25][(j + 1 + 80) % 80] == 1) {
        count++;
    }
    return count;
}

/*
int compareFields(int **a, int **b) {
    int flag = 1;
    // int count = 0;
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (a[i][j] != b[i][j]) {
                flag = 0;
            }
        }
    }
    return flag;
}
*/
