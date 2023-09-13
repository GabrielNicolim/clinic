#include <stdio.h>
#include <ctype.h>

#ifdef _WIN32

#include <conio.h>
#include <windows.h>

#endif

typedef struct {
    char city[20], state[2], street[30];
    int houseNumber;
} typeAddress;

typedef struct {
    int clientCode;
    char clientName[20];
    int clientAge;
    typeAddress clientAddress;
    char clientExists;
} typeClient;

void addClient() {
    typeClient client;
    char answer;

    FILE *clientData;

    if (!(clientData = fopen("clientFile.dat", "a+b"))) {
        printf("Houve um erro na abertura do arquivo.\n");

        return;
    }

    do {
        client.clientExists = 1;

        printf("Codigo: ");
        scanf("%d", &client.clientCode);
        fflush(stdin);

        printf("Nome: ");
        gets(client.clientName);

        printf("Idade: ");
        scanf("%d", &client.clientAge);
        fflush(stdin);

        printf("Rua: ");
        gets(client.clientAddress.street);

        printf("Numero da casa: ");
        scanf("%d", &client.clientAddress.houseNumber);
        fflush(stdin);

        printf("Cidade: ");
        gets(client.clientAddress.city);

        printf("Estado: ");
        gets(client.clientAddress.state);


        fwrite(&client, sizeof(typeClient), 1, clientData);

        printf("\n Deseja adicionar outro cliente (S/N)? ");
        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');
    } while (answer == 'S');

    fclose(clientData);
}

void updateClient() {
    typeClient client;
    char searchName[30], answer;
    int clientFlag = 0;

    FILE *clientData;

    if (!(clientData = fopen("clientFile.dat", "a+b"))) {
        printf("Houve um erro na abertura do arquivo.");

        return;
    }
    do {

        printf("\nCONSULTA DE PACIENTES PARA ATUALIZACAO\n");
        printf("Digite o nome do cliente a ser buscado: ");
        scanf("%s", searchName);

        while (fread(&client, sizeof(typeClient), 1, clientData)) {
            if (client.clientExists && strcmp(client.clientName, searchName) == 0) {

                printf("\nDADOS ATUAIS DO PACIENTE BUSCADO\n");

                printf("Codigo: %d\n", client.clientCode);
                printf("Nome: %s\n", client.clientName);
                printf("Idade: %d", client.clientAge);
                printf("Endereco: %s, %d (%s-%s)", client.clientAddress.street, client.clientAddress.houseNumber,
                       client.clientAddress.city, client.clientAddress.state);

                printf("\nATUALIZACAO DO CADASTRO DO PACIENTE\n");

                printf("Codigo: ");
                scanf("%d", &client.clientCode);
                fflush(stdin);

                printf("Nome: ");
                gets(client.clientName);

                printf("Idade: ");
                scanf("%d", &client.clientAge);
                fflush(stdin);

                printf("Endereco: ");
                gets(client.clientAddress.street);
                scanf("%d", &client.clientAddress.houseNumber);
                gets(client.clientAddress.city);
                gets(client.clientAddress.state);


                fseek(clientData, -sizeof(typeClient), SEEK_CUR);
                fwrite(&client, sizeof(typeClient), 1, clientData);
                clientFlag = 1;

                break;
            }
        }

        if (!clientFlag) {
            printf("\nO paciente buscado nao foi encontrado no banco de dados.\n");
        }

        printf("\n Deseja adicionar outro cliente (S/N)? ");
        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');
    } while (answer == 'S');
}

int main() {
    int option = 0;

    do {
        printf("1 - Cadastrar cliente\n");
        printf("2 - Alterar dados de cliente\n");
        printf("3 - Marcar consulta\n");
        printf("4 - Desmarcar a consulta\n");
        printf("5 - Escrever mapa de hor�rios para um determinado dia\n");
        printf("6 - Obter consultas\n");
        printf("7 - Consultar maiores de 50\n");
        printf("8 - Fim\n\n");

        printf("Escolhar uma opcao (1 - 8): ");
        scanf("%d", &option);

        system("cls");
    } while (option < 1 || option > 8);

    switch (option) {
        case 1:
            addClient();
            break;
        case 2:
            updateClient();
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
    }
}

