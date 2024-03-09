#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#define CLEAR_SCREEN system("cls")
#else
#define CLEAR_SCREEN system("clear")
#endif

#define MIN_BOARD_SIZE 5
#define MAX_BOARD_SIZE 15
#define MAX_SHIP_TYPES 5
#define MAX_NAME_LENGTH 50

typedef struct {
    int size;
    int hits;
    int isSunk;
} Ship;

typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
    Ship ships[MAX_SHIP_TYPES];
} Player;

char **createBoard(int size) {
    char **board = (char **)malloc(size * sizeof(char *));
    for (int i = 0; i < size; ++i) {
        board[i] = (char *)malloc(size * sizeof(char));
        for (int j = 0; j < size; ++j) {
            board[i][j] = '~';
        }
    }
    return board;
}

void freeBoard(char **board, int size) {
    for (int i = 0; i < size; ++i) {
        free(board[i]);
    }
    free(board);
}

void initializeBoards(char **board1, char **board2, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            board1[i][j] = '~';
            board2[i][j] = '~';
        }
    }
}

void printHiddenBoard(char **board, int size) {
    printf("   ");
    for (int i = 0; i < size; ++i) {
        printf("%d ", i + 1);
    }
    printf("\n");

    for (int i = 0; i < size; ++i) {
        printf("%c |", 'A' + i);
        for (int j = 0; j < size; ++j) {
            if (board[i][j] == 'X' || board[i][j] == 'O') {
                printf("%c ", board[i][j]);
            } else {
                printf("~ ");
            }
        }
        printf("\n");
    }
}

void printPlayerInfo(Player *player, const char *playerName, int shipCount) {
    printf("%s's Info:\n", playerName);
    printf("Score: %d\n", player->score);
    for (int i = 0; i < shipCount; ++i) {
        printf("Ship %d - Size: %d, Hits: %d\n", i + 1, player->ships[i].size, player->ships[i].hits);
    }
}

void placeShip(Player *player, char **board, int size, int shipIndex) {
    int x, y;
    int direction;

    do {
        printf("Enter the starting coordinates for Ship %d (size %d): ", shipIndex + 1, player->ships[shipIndex].size);
        char row;
        scanf(" %c %d", &row, &y);
        x = row - 'A';

        printf("Downward (1) or Rightward (2)? ");
        scanf("%d", &direction);

        if (direction == 1) {
            if (x + player->ships[shipIndex].size > size) {
                printf("Invalid placement. Try again.\n");
            } else {
                for (int i = 0; i < player->ships[shipIndex].size; ++i) {
                    if (board[x + i][y - 1] != '~') {
                        printf("Invalid placement. Try again.\n");
                        break;
                    } else {
                        board[x + i][y - 1] = shipIndex + '1';
                    }
                }
            }
        } else if (direction == 2) {
            if (y + player->ships[shipIndex].size > size) {
                printf("Invalid placement. Try again.\n");
            } else {
                for (int i = 0; i < player->ships[shipIndex].size; ++i) {
                    if (board[x][y + i - 1] != '~') {
                        printf("Invalid placement. Try again.\n");
                        break;
                    } else {
                        board[x][y + i - 1] = shipIndex + '1';
                    }
                }
            }
        } else {
            printf("Invalid direction. Try again.\n");
        }
    } while (direction != 1 && direction != 2);
}

void placeShips(Player *player, char **board, int size, int shipCount) {
    for (int i = 0; i < shipCount; ++i) {
        player->ships[i].size = i + 3;
        player->ships[i].hits = 0;
        player->ships[i].isSunk = 0;

        printHiddenBoard(board, size);
        printPlayerInfo(player, "Player", shipCount);
        placeShip(player, board, size, i);
        CLEAR_SCREEN;  // Ekranı temizle
    }
}

int makeMove(Player *player, char **opponentBoard, int size, int shipCount) {
    int x, y;

    printf("Enter the coordinates to make a move: ");
    char row;
    scanf(" %c %d", &row, &y);
    x = row - 'A';

    if (x < 0 || x >= size || y < 1 || y > size) {
        printf("Invalid coordinates. Try again.\n");
        return makeMove(player, opponentBoard, size, shipCount);
    }

    if (opponentBoard[x][y - 1] == 'X' || opponentBoard[x][y - 1] == 'O') {
        printf("Already guessed that location. Try again.\n");
        return makeMove(player, opponentBoard, size, shipCount);
    }

    if (opponentBoard[x][y - 1] != '~') {
        int shipIndex = opponentBoard[x][y - 1] - '1';
        printf("Hit!\n");
        opponentBoard[x][y - 1] = 'X';
        player->ships[shipIndex].hits++;

        if (player->ships[shipIndex].hits == player->ships[shipIndex].size) {
            printf("You sank a ship!\n");
            player->ships[shipIndex].isSunk = 1;
            player->score += player->ships[shipIndex].size;

            int allSunk = 1;
            for (int i = 0; i < shipCount; ++i) {
                if (opponentBoard[x][y - 1] == i + '1' && !player->ships[i].isSunk) {
                    allSunk = 0;
                    break;
                }
            }

            if (allSunk) {
                printf("All opponent's ships are sunk! You win!\n");
                return 0;  // Oyunu bitir
            }
        }
    } else {
        printf("Miss!\n");
        opponentBoard[x][y - 1] = 'O';
    }

    return 1;  // Oyun devam ediyor
}
int main() {
    int boardSize;
    int shipCount;
    printf("This game was made by Yasin Karadavut with student number 152420211024 on 18.01.2024.\nEskisehir Osmangazi University Computer Enginnering Introduction to Programing Lab \n");
    printf("Enter the board size (between %d and %d): ", MIN_BOARD_SIZE, MAX_BOARD_SIZE);
    scanf("%d", &boardSize);
    
    if (boardSize < MIN_BOARD_SIZE || boardSize > MAX_BOARD_SIZE) {
        printf("Invalid board size. Exiting...\n");
        return 1;
    }

    printf("Enter the number of ships(max 5): ");
    scanf("%d", &shipCount);

    if (shipCount < 1 || shipCount > MAX_SHIP_TYPES) {
        printf("Invalid number of ships. Exiting...\n");
        return 1;
    }

    char **player1Board = createBoard(boardSize);
    char **player2Board = createBoard(boardSize);

    Player player1, player2;
    player1.score = 0;
    player2.score = 0;

    printf("Enter Player 1's name: ");
    scanf("%s", player1.name);

    printf("Enter Player 2's name: ");
    scanf("%s", player2.name);

    printf("%s, place your ships:\n", player1.name);
    placeShips(&player1, player1Board, boardSize, shipCount);

    printf("%s, place your ships:\n", player2.name);
    placeShips(&player2, player2Board, boardSize, shipCount);

    int currentPlayer = 1;

    while (1) {
        if (currentPlayer == 1) {
            CLEAR_SCREEN;
            printf("%s's Turn:\n", player1.name);
            printHiddenBoard(player2Board, boardSize);
            printPlayerInfo(&player1, player1.name, shipCount);
            if (!makeMove(&player1, player2Board, boardSize, shipCount)) {
                break;  // Oyun bitti
            }
        } else {
            CLEAR_SCREEN;
            printf("%s's Turn:\n", player2.name);
            printHiddenBoard(player1Board, boardSize);
            printPlayerInfo(&player2, player2.name, shipCount);
            if (!makeMove(&player2, player1Board, boardSize, shipCount)) {
                break;  // Oyun bitti
            }
        }

        currentPlayer = 3 - currentPlayer;  // Sıradaki oyuncu
    }

    printf("Final Scores:\n");
    printPlayerInfo(&player1, player1.name, shipCount);
    printPlayerInfo(&player2, player2.name, shipCount);

    freeBoard(player1Board, boardSize);
    freeBoard(player2Board, boardSize);

    return 0;
}