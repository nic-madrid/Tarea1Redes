#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>

using namespace std;

const int ROWS = 6;
const int COLS = 7;
char Tablero[ROWS][COLS];

// Función para inicializar el tablero
void InicializarTablero()
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            Tablero[i][j] = '.';
        }
    }
}

// Función para imprimir el tablero
void ImprimirTablero()
{
    cout << "  0 1 2 3 4 5 6" << endl;
    for (int i = 0; i < ROWS; i++)
    {
        cout << i << " ";
        for (int j = 0; j < COLS; j++)
        {
            cout << Tablero[i][j] << ' ';
        }
        cout << endl;
    }
}

// Función para actualizar el tablero con los datos recibidos del servidor
void ActualizarTablero(const char *TableroStr)
{
    int indice = 0;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            Tablero[i][j] = TableroStr[indice++];
        }
    }
}

// Función para validar la entrada del usuario
bool ValidarEntrada(const char *entrada)
{
    if (entrada[0] == 'Q') return true;
    if (entrada[0] >= '0' || entrada[0] <= '6') return true;
    return false;
}

int main(int argc, char *argv[])
{
    // Crear el socket del cliente
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
        cout << "Direccion incorrecta\n";
        return -1;
    }

    if (connect(socket_cliente, (struct sockaddr *)&server_ip, sizeof(server_ip)) < 0)
    {
        cout << "Error conectando con el servidor\n";
        close(socket_cliente);
        exit(EXIT_FAILURE);
    }

    cout << "Conectado con el servidor\n" << endl;

    char buffer[1024];
    int n_bytes;

    // Recibir el tablero inicial del servidor
    if ((n_bytes = recv(socket_cliente, buffer, 1024, 0)) > 0)
    {
        buffer[n_bytes] = '\0';
        ActualizarTablero(buffer);
        ImprimirTablero();
    }
    else
    {
        cout << "Error recibiendo Tablero inicial del server\n";
        close(socket_cliente);
        exit(EXIT_FAILURE);
    }

    while (true)
    {
        cout << "Ingrese la columna a jugar (0-6) o Q para salir: " << endl;
        cin >> buffer[1];
        if (!ValidarEntrada(buffer))
        {
            cout << "Entrada incorrecta, por favor utilice numeros del 0 al 6, o Q para salir." << endl;
            continue;
        }
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
            if (buffer[0] == 'W') {
                cout << buffer + 1 << endl;
                break;
            } else {
                ActualizarTablero(buffer);
                ImprimirTablero();
            }
        }
    }
    close(socket_cliente);
    return 0;
}