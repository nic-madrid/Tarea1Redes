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

// Función para actualizar el tablero con los datos recibidos del cliente
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

// Función para revisar si hay un ganador en base a si existen 4 fichas iguales en línea horizontal, vertical o diagonal
char RevisarGanador(char board[Fila][Columnas])
{
    for (int i = 0; i < Fila; i++)
    {
        for (int j = 0; j < Columnas; j++)
        {
            if (board[i][j] == 'C' || board[i][j] == 'S')
            {
                if (j <= Columnas - 4 && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3])
                {
                    return board[i][j];
                }
                if (i <= Fila - 4 && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] && board[i][j] == board[i + 3][j])
                {
                    return board[i][j];
                }
                if (i <= Fila - 4 && j <= Columnas - 4 && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i][j] == board[i + 3][j + 3])
                {
                    return board[i][j];
                }
                if (i >= 3 && j <= Columnas - 4 && board[i][j] == board[i - 1][j + 1] && board[i][j] == board[i - 2][j + 2] && board[i][j] == board[i - 3][j + 3])
                {
                    return board[i][j];
                }
            }
        }
    }
    return '.';
}

// Función para manejar la conexión con el cliente
void *ManejoCliente(void *arg)
{
    int socket_cliente = *((int *)arg);
    delete (int *)arg;

    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));
    int n_bytes = 0;

    srand(time(0));

    char board[Fila][Columnas];
    InicializarTablero(board);

    EnviarTablero(socket_cliente, board);
    bool turnoCliente = rand() % 2 == 0;
    while (true)
    {
        // Si es el turno del cliente, se le notifica y se espera su movimiento
        if (turnoCliente)
        {
            send(socket_cliente, "Tu turno\n", 10, 0);
        }
        else
        {
            int MovServer = rand() % 7;
            ActualizarTablero(MovServer, 'S', board);

            EnviarTablero(socket_cliente, board);
            char server_response[50];
            send(socket_cliente, server_response, strlen(server_response), 0);
        }
        if ((n_bytes = recv(socket_cliente, buffer, 1024, 0)) > 0)
        {
            buffer[n_bytes] = '\0';

            if (buffer[0] == 'Q')
            {
                cout << "Cliente desconectado\n";
                close(socket_cliente);
                break;
            }

            int MovCliente = buffer[1] - '0';
            cout << "Client move: " << MovCliente << endl;
            ActualizarTablero(MovCliente, 'C', board);

            // Se revisa si hay un ganador
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
            // El servidor juega su turno
            int MovServer = rand() % 7;
            ActualizarTablero(MovServer, 'S', board);

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

    cout << "Esperando por jugadores ...\n";

    while (true)
    {
        int socket_cliente = accept(socket_server, (struct sockaddr *)&direccionCliente, &TamanoDireccion);
        if (socket_cliente < 0)
        {
            cout << "Error accepting client connection\n";
            continue;
        }

        cout << "Cliente conectado\n";

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