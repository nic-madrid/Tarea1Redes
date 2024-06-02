#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>

using namespace std;

const int ROWS = 6;
const int COLS = 7;
char board[ROWS][COLS];

// Función para inicializar el tablero
void initializeBoard()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = '.';
        }
    }
}

// Función para imprimir el tablero
void printBoard()
{
    cout << "  0 1 2 3 4 5 6" << endl;
    for (int i = 0; i < ROWS; i++)
    {
        cout << i << " ";
        for (int j = 0; j < COLS; j++)
        {
            cout << board[i][j] << ' ';
        }
        cout << endl;
    }
}

// Función para actualizar el tablero con los datos recibidos del servidor
void updateBoard(const char *boardStr)
{
    int index = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            board[i][j] = boardStr[index++];
        }
    }
}

int main(int argc, char *argv[])
{
   int socket_cliente;
if (argc != 3)
{
    cout << "Uso: " << argv[0] << " <IP del servidor> <puerto>" << endl;
    return 1;
}

char *server_ip_char = argv[1];
int server_port = atoi(argv[2]);
struct sockaddr_in server_ip;
socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
if (socket_cliente < 0)
{
    cout << "Error creating socket\n";
    exit(EXIT_FAILURE);
}

server_ip.sin_family = AF_INET;
server_ip.sin_port = htons(server_port);
if (inet_pton(AF_INET, server_ip_char, &server_ip.sin_addr) <= 0)
{
    cout << "Invalid address/ Address not supported \n";
    return -1;
}

if (connect(socket_cliente, (struct sockaddr *)&server_ip, sizeof(server_ip)) < 0)
{
    cout << "Error connecting to server\n";
    close(socket_cliente);
    exit(EXIT_FAILURE);
}

cout << "Connected to server\n" << endl;

    char buffer[1024];
    int n_bytes;

    // Recibir el tablero inicial del servidor
    if ((n_bytes = recv(socket_cliente, buffer, 1024, 0)) > 0)
    {
        buffer[n_bytes] = '\0';
        updateBoard(buffer);
        printBoard();
    }
    else
    {
        cout << "Error receiving initial board from server\n";
        close(socket_cliente);
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        cout << "Enter column to play (0-6) or Q to quit: " << endl;
        cin >> buffer[1];
        if (buffer[1] == 'Q')
        {
            cout << "Terminando conexion\n" << endl;
            buffer[0] = 'Q';
            send(socket_cliente, buffer, 2, 0);
            break;
        }
        buffer[0] = 'C';
        buffer[2] = '\0';
        send(socket_cliente, buffer, 3, 0);
        // Recibir el tablero actualizado del servidor
        if ((n_bytes = recv(socket_cliente, buffer, 1024, 0)) > 0)
        {
            buffer[n_bytes] = '\0';
            updateBoard(buffer);
            printBoard();
    }
    }
    close(socket_cliente);
    return 0;
}