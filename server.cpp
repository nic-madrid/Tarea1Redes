#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>
#include <pthread.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int Fila = 6;
const int Columnas = 7;
char board[Fila][Columnas];

// Función para inicializar el tablero
void InicializarTablero(char board[Fila][Columnas])
{
    for (int i = 0; i < Fila; i++)
    {
        for (int j = 0; j < Columnas; j++)
        {
            board[i][j] = '.';
        }
    }
}

// Función para imprimir el tablero
void ImprimirTablero(char board[Fila][Columnas])
{
    for (int i = 0; i < Fila; i++)
    {
        for (int j = 0; j < Columnas; j++)
        {
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
}

// Función para actualizar el tablero
void ActualizarTablero(int col, char symbol, char board[Fila][Columnas])
{
    for (int i = Fila - 1; i >= 0; i--)
    {
        if (board[i][col] == '.')
        {
            board[i][col] = symbol;
            break;
        }
    }
}

// Función para enviar el tablero al cliente
void EnviarTablero(int socket, char board[Fila][Columnas])
{
    char boardStr[Fila * Columnas + 1];
    int index = 0;
    for (int i = 0; i < Fila; i++)
    {
        for (int j = 0; j < Columnas; j++)
        {
            boardStr[index++] = board[i][j];
        }
    }
    boardStr[index] = '\0';
    send(socket, boardStr, sizeof(boardStr), 0);
}

char RevisarGanador(char board[Fila][Columnas])
{
    for (int i = 0; i < Fila; i++)
    {
        for (int j = 0; j < Columnas; j++)
        {
            if (board[i][j] == 'C' || board[i][j] == 'S')
            {
                // Check row
                if (j <= Columnas - 4 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3])
                {
                    return board[i][j];
                }
                // Check column
                if (i <= Fila - 4 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] && board[i][j] == board[i + 3][j])
                {
                    return board[i][j];
                }
                // Check diagonal /
                if (i <= Fila - 4 && j <= Columnas - 4 && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i][j] == board[i + 3][j + 3])
                {
                    return board[i][j];
                }
                // Check diagonal
                if (i >= 3 && j <= Columnas - 4 && board[i][j] == board[i - 1][j + 1] && board[i][j] == board[i - 2][j + 2] && board[i][j] == board[i - 3][j + 3])
                {
                    return board[i][j];
                }
            }
        }
    }
    return '.';
}

void *ManejoCliente(void *arg)
{
    int socket_cliente = *((int *)arg);
    delete (int *)arg;

    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));
    int n_bytes = 0;

    srand(time(0)); // Inicializar la semilla aleatoria

    // Crear un tablero individual para este cliente
    char board[Fila][Columnas];
    InicializarTablero(board);

    // Enviar el tablero inicial al cliente
    EnviarTablero(socket_cliente, board);

    while (true)
    {
        // Esperar el movimiento del cliente
        if ((n_bytes = recv(socket_cliente, buffer, 1024, 0)) > 0)
        {
            buffer[n_bytes] = '\0';

            if (buffer[0] == 'Q')
            {
                cout << "Client disconnected\n";
                close(socket_cliente);
                break;
            }

            // Imprimir y actualizar el movimiento del cliente
            int MovCliente = buffer[1] - '0';
            cout << "Client move: " << MovCliente << endl;
            ActualizarTablero(MovCliente, 'C', board);
            // ImprimirTablero(board); // Comentado

            char Ganador = RevisarGanador(board);
            if (Ganador == 'C')
            {
                send(socket_cliente, "WClient wins!\n", 14, 0);
                cout << "Client wins!\n";
                break;
            }
            else if (Ganador == 'S')
            {
                send(socket_cliente, "WServer wins!\n", 14, 0);
                cout << "Server wins!\n";
                break;
            }

            // Generar movimiento aleatorio del servidor
            int MovServer = rand() % 7; // Suponiendo que hay 7 columnas posibles (0-6)
            ActualizarTablero(MovServer, 'S', board);
            // ImprimirTablero(board); // Comentado

            // Enviar el tablero actualizado al cliente y el movimiento del servidor
            EnviarTablero(socket_cliente, board);
            char server_response[50];
            send(socket_cliente, server_response, strlen(server_response), 0);
        }
        else
        {
            cout << "Error receiving data\n";
            close(socket_cliente);
            break;
        }
    }

    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <port>" << endl;
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    int socket_server = 0;
    struct sockaddr_in direccionServidor, direccionCliente;

    cout << "Creating listening socket ...\n";
    if ((socket_server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        cout << "Error creating listening socket\n";
        exit(EXIT_FAILURE);
    }

    cout << "Configuring socket address structure ...\n";
    memset(&direccionServidor, 0, sizeof(direccionServidor));
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = htonl(INADDR_ANY);
    direccionServidor.sin_port = htons(port);

    cout << "Binding socket ...\n";
    if (bind(socket_server, (struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) < 0)
    {
        cout << "Error calling bind()\n";
        exit(EXIT_FAILURE);
    }

    cout << "Calling listening ...\n";
    if (listen(socket_server, 1024) < 0)
    {
        cout << "Error calling listen()\n";
        exit(EXIT_FAILURE);
    }

    socklen_t TamanoDireccion = sizeof(direccionCliente);

    cout << "Waiting for clients to connect ...\n";

    while (true)
    {
        int socket_cliente = accept(socket_server, (struct sockaddr *)&direccionCliente, &TamanoDireccion);
        if (socket_cliente < 0)
        {
            cout << "Error accepting client connection\n";
            continue;
        }

        cout << "Client connected\n";

        pthread_t thread_id;
        int *nuevo_socket = new int(socket_cliente);
        if (pthread_create(&thread_id, NULL, ManejoCliente, (void *)nuevo_socket) != 0)
        {
            cout << "Failed to create thread\n";
            delete nuevo_socket;
        }
    }

    close(socket_server);
    return 0;
}