#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <pthread.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ROWS = 6;
const int COLS = 7;
char board[ROWS][COLS];

// Función para inicializar el tablero
void initializeBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = '.';
        }
    }
}

// Función para imprimir el tablero
void printBoard(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
}

// Función para actualizar el tablero
void updateBoard(int col, char symbol, char board[ROWS][COLS]) {
    for (int i = ROWS - 1; i >= 0; i--) {
        if (board[i][col] == '.') {
            board[i][col] = symbol;
            break;
        }
    }
}

// Función para enviar el tablero al cliente
void sendBoard(int socket, char board[ROWS][COLS]) {
    char boardStr[ROWS * COLS + 1];
    int index = 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            boardStr[index++] = board[i][j];
        }
    }
    boardStr[index] = '\0';
    send(socket, boardStr, sizeof(boardStr), 0);
}

char checkWinner(char board[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == 'C' || board[i][j] == 'S') {
                // Check row
                if (j <= COLS - 4 && board[i][j] == board[i][j+1] && board[i][j] == board[i][j+2] && board[i][j] == board[i][j+3]) {
                    return board[i][j];
                }
                // Check column
                if (i <= ROWS - 4 && board[i][j] == board[i+1][j] && board[i][j] == board[i+2][j] && board[i][j] == board[i+3][j]) {
                    return board[i][j];
                }
                // Check diagonal /
                if (i <= ROWS - 4 && j <= COLS - 4 && board[i][j] == board[i+1][j+1] && board[i][j] == board[i+2][j+2] && board[i][j] == board[i+3][j+3]) {
                    return board[i][j];
                }
                // Check diagonal
                if (i >= 3 && j <= COLS - 4 && board[i][j] == board[i-1][j+1] && board[i][j] == board[i-2][j+2] && board[i][j] == board[i-3][j+3]) {
                    return board[i][j];
                }
            }
        }
    }
    return '.';
}


void* clientHandler(void* arg) {
    int socket_cliente = *((int*)arg);
    delete (int*)arg;

    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));
    int n_bytes = 0;

    srand(time(0)); // Inicializar la semilla aleatoria

    // Crear un tablero individual para este cliente
    char board[ROWS][COLS];
    initializeBoard(board);

    // Enviar el tablero inicial al cliente
    sendBoard(socket_cliente, board);

    while (true) {
        // Esperar el movimiento del cliente
        if ((n_bytes = recv(socket_cliente, buffer, 1024, 0)) > 0) {
            buffer[n_bytes] = '\0';

            if (buffer[0] == 'Q') {
                cout << "Client disconnected\n";
                close(socket_cliente);
                break;
            }

            // Imprimir y actualizar el movimiento del cliente
            int client_move = buffer[1] - '0';
            cout << "Client move: " << client_move << endl;
            updateBoard(client_move, 'C', board);
            printBoard(board);

            char winner = checkWinner(board);
            if (winner == 'C') {
                send(socket_cliente, "Client wins!\n", 13, 0);
                break;
            } else if (winner == 'S') {
                send(socket_cliente, "Server wins!\n", 13, 0);
                break;
            }

            // Generar movimiento aleatorio del servidor
            int server_move = rand() % 7; // Suponiendo que hay 7 columnas posibles (0-6)
            updateBoard(server_move, 'S', board);
            printBoard(board);

            // Enviar el tablero actualizado al cliente y el movimiento del servidor
            sendBoard(socket_cliente, board);
            char server_response[50];
            send(socket_cliente, server_response, strlen(server_response), 0);
        } else {
            cout << "Error receiving data\n";
            close(socket_cliente);
            break;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    int socket_server = 0;
    struct sockaddr_in direccionServidor, direccionCliente;

    cout << "Creating listening socket ...\n";
    if ((socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating listening socket\n";
        exit(EXIT_FAILURE);
    }

    cout << "Configuring socket address structure ...\n";
    memset(&direccionServidor, 0, sizeof(direccionServidor));
    direccionServidor.sin_family      = AF_INET;
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);
    direccionServidor.sin_port        = htons(port);

    cout << "Binding socket ...\n";
    if (bind(socket_server, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor)) < 0) {
        cout << "Error calling bind()\n";
        exit(EXIT_FAILURE);
    }

    cout << "Calling listening ...\n";
    if (listen(socket_server, 1024) < 0) {
        cout << "Error calling listen()\n";
        exit(EXIT_FAILURE);
    }

    socklen_t addr_size = sizeof(direccionCliente);

    cout << "Waiting for clients to connect ...\n";

    while (true) {
        int socket_cliente = accept(socket_server, (struct sockaddr *)&direccionCliente, &addr_size);
        if (socket_cliente < 0) {
            cout << "Error accepting client connection\n";
            continue;
        }

        cout << "Client connected\n";

        pthread_t thread_id;
        int* new_sock = new int(socket_cliente);
        if (pthread_create(&thread_id, NULL, clientHandler, (void*)new_sock) != 0) {
            cout << "Failed to create thread\n";
            delete new_sock;
        }
    }

    close(socket_server);
    return 0;
}