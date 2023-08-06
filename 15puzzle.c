#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 4
#define MAX_SCORES 5

int games[10][SIZE][SIZE] = { // tableros con soluciones posibles
    {{11, 1, 10, 4}, {8, 3, 2, 12}, {7, 6, 15, 0}, {9, 5, 14, 13}},
    {{7, 12, 14, 1}, {0, 5, 13, 2}, {11, 8, 15, 3}, {6, 4, 10, 9}},
    {{11, 1, 3, 6}, {8, 13, 0, 9}, {7, 12, 2, 15}, {5, 4, 14, 10}},
    {{1, 6, 11, 12}, {5, 2, 4, 15}, {0, 3, 13, 10}, {8, 9, 14, 7}},
    {{3, 6, 1, 14}, {12, 15, 8, 5}, {0, 7, 11, 9}, {4, 2, 13, 10}},
    {{15, 10, 14, 4}, {1, 8, 3, 5}, {2, 9, 11, 12}, {13, 0, 7, 6}},
    {{13, 15, 9, 11}, {6, 14, 2, 7}, {0, 10, 5, 8}, {12, 3, 4, 1}},
    {{15, 14, 5, 2}, {3, 13, 0, 10}, {1, 12, 7, 6}, {11, 4, 8, 9}},
    {{9, 2, 0, 3}, {5, 4, 7, 15}, {10, 8, 6, 14}, {1, 11, 12, 13}},
    {{5, 6, 15, 11}, {9, 0, 2, 4}, {12, 3, 13, 7}, {14, 1, 10, 8}}};

int emptyX, emptyY;
int scores[MAX_SCORES] = {0}; // almacena las ultimas puntuaciones
int scoreIndex = 0;

void printBoard(int board[SIZE][SIZE]) {
    int i, j;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                printf("|   ");
                emptyX = i;
                emptyY = j;
            } else {
                printf("|%2d ", board[i][j]);
            }
        }
        printf("|\n");
    }
}
// mueve el espacio vacio
int move(char direction, int board[SIZE][SIZE]) {
    switch (direction) {
    case 'w':
        if (emptyX < SIZE - 1) {
            board[emptyX][emptyY] = board[emptyX + 1][emptyY];
            board[emptyX + 1][emptyY] = 0;
            return 1;
        }
        break;
    case 'a':
        if (emptyY < SIZE - 1) {
            board[emptyX][emptyY] = board[emptyX][emptyY + 1];
            board[emptyX][emptyY + 1] = 0;
            return 1;
        }
        break;
    case 's':
        if (emptyX > 0) {
            board[emptyX][emptyY] = board[emptyX - 1][emptyY];
            board[emptyX - 1][emptyY] = 0;
            return 1;
        }
        break;
    case 'd':
        if (emptyY > 0) {
            board[emptyX][emptyY] = board[emptyX][emptyY - 1];
            board[emptyX][emptyY - 1] = 0;
            return 1;
        }
        break;
    default:
        printf("Movimiento inválido\n");
    }
    return 0;
}
int checkWin(int board[SIZE][SIZE]) {
    int i, j, prev = 0;
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            if (board[i][j] != 0 && board[i][j] < prev) {
                return 0;
            }
            prev = board[i][j];
        }
    }
    return 1;
}
void registerScore(int moves) { // funcion que registra la puntuación
    scores[scoreIndex] = moves;
    scoreIndex = (scoreIndex + 1) % MAX_SCORES;
}
void showLastScores() { // funcion para mostrar los puntos de las ultimas 5
                        // partidas
    printf("\nPuntuaciones de las últimas 5 partidas:\n");
    for (int i = 0; i < MAX_SCORES; i++) {
        if (scores[i] != 0) {
            printf("Partida %d: %d movimientos\n", i + 1, scores[i]);
        }
    }
}
int main() {
    int ch = 'x';

    while (ch != 'q' && ch != 'Q') {
        initscr(); // Start curses mode
        raw();     // Line buffering disabled
        noecho();  // Don't echo() while we do getch

        int g = 0;
        int moveCounter = 0;
        while (g < 10 && (ch = getch()) != 'q' && ch != 'Q') {
            int board[SIZE][SIZE];
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    board[i][j] = games[g][i][j];
                    if (board[i][j] == 0) {
                        emptyX = i;
                        emptyY = j;
                    }
                }
            }

            if (move(ch, board)) {
                moveCounter++;
                printBoard(board);
                if (checkWin(board)) {
                    printf("¡Felicidades! Has ganado el juego\n");
                    printf("Has hecho %d movimientos\n", moveCounter);
                    // Mostrar la puntuación y las últimas puntuaciones al
                    // finalizar la partida
                    registerScore(moveCounter);
                    showLastScores();
                    g++;
                    moveCounter = 0;
                }
            }
        }

        endwin(); /* End curses mode          */
    }

    return 0;
}
