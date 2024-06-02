#include <arpa/inet.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <sys/socket.h>

using namespace std;

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

    cout << "Waiting for players to connect ...\n";

    int player_turn = 1; // Indica el turno del jugador (1 o 2)
    int socket_player1, socket_player2;
    char move_player1, move_player2;

    // Esperar a que se conecte el primer jugador
    socket_player1 = accept(socket_server, (struct sockaddr *)&direccionCliente, &addr_size);
    if (socket_player1 < 0) {
        cout << "Error accepting player 1 connection\n";
        exit(EXIT_FAILURE);
    }
    cout << "Player 1 connected\n";
    char player_type = '1';
    if (send(socket_player1, &player_type, 1, 0) != 1) {
        cout << "Error sending player type to player 1\n";
        close(socket_player1);
        exit(EXIT_FAILURE);
    }

    // Esperar a que se conecte el segundo jugador
    socket_player2 = accept(socket_server, (struct sockaddr *)&direccionCliente, &addr_size);
    if (socket_player2 < 0) {
        cout << "Error accepting player 2 connection\n";
        exit(EXIT_FAILURE);
    }
    cout << "Player 2 connected\n";
    player_type = '2';
    if (send(socket_player2, &player_type, 1, 0) != 1) {
        cout << "Error sending player type to player 2\n";
        close(socket_player2);
        exit(EXIT_FAILURE);
    }

    // Iniciar el juego
    while (true) {
        if (player_turn == 1) {
            cout << "Waiting for player 1 move...\n";
            if (recv(socket_player1, &move_player1, 1, 0) <= 0) {
                cout << "Player 1 disconnected\n";
                break;
            }
            cout << "Player 1 move: " << move_player1 << endl;
            if (send(socket_player2, &move_player1, 1, 0) != 1) {
                cout << "Error sending player 1 move to player 2\n";
                close(socket_player2);
                break;
            }
            player_turn = 2;
        } else {
            cout << "Waiting for player 2 move...\n";
            if (recv(socket_player2, &move_player2, 1, 0) <= 0) {
                cout << "Player 2 disconnected\n";
                break;
            }
            cout << "Player 2 move: " << move_player2 << endl;
            if (send(socket_player1, &move_player2, 1, 0) != 1) {
                cout << "Error sending player 2 move to player 1\n";
                close(socket_player1);
                break;
            }
            player_turn = 1;
        }
    }

    close(socket_player1);
    close(socket_player2);
    close(socket_server);
    return 0;
}



