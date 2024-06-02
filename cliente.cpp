#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>

using namespace std;

int main(int argc, char **argv) {
    if (argc < 3) {
        cout << "Usage: " << argv[0] << " <server_ip> <port>" << endl;
        exit(EXIT_FAILURE);
    }

    const char* server_ip = argv[1];
    int port = atoi(argv[2]);
    int socket_cliente = 0;
    struct sockaddr_in direccionServidor;

    cout << "Creating socket ...\n";
    if ((socket_cliente = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "Error creating socket\n";
        exit(EXIT_FAILURE);
    }

    cout << "Configuring server address structure ...\n";
    memset(&direccionServidor, 0, sizeof(direccionServidor));
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &direccionServidor.sin_addr) <= 0) {
        cout << "Invalid address/ Address not supported\n";
        exit(EXIT_FAILURE);
    }

    cout << "Connecting to the game server ...\n";
    if (connect(socket_cliente, (struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) < 0) {
        cout << "Connection Failed\n";
        exit(EXIT_FAILURE);
    }

    cout << "Connected to the game server\n";

    // Recibir el tipo de jugador del servidor
    char player_type;
    recv(socket_cliente, &player_type, 1, 0);

    if (player_type == '1') {
        cout << "You are the first player\n";
        // Bucle para mantener la conexión abierta
        char input;
        while (true) {
            // Leer entrada del usuario
            cout << "Enter your move (or 'Q' to quit): ";
            cin >> input;

            // Enviar movimiento al servidor
            send(socket_cliente, &input, 1, 0);

            // Si el usuario envía 'Q', cerrar la conexión y salir del bucle
            if (input == 'Q') {
                break;
            }
        }
    } else if (player_type == '2') {
        cout << "You are the second player\n";
        // Bucle para mantener la conexión abierta
        char input;
        while (true) {
            // Leer entrada del usuario
            cout << "Enter your move (or 'Q' to quit): ";
            cin >> input;

            // Enviar movimiento al servidor
            send(socket_cliente, &input, 1, 0);

            // Si el usuario envía 'Q', cerrar la conexión y salir del bucle
            if (input == 'Q') {
                break;
            }
        }
    } else {
        cout << "Invalid player type received from server\n";
    }

    close(socket_cliente);
    return 0;
}



