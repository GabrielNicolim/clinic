#include <stdio.h>
#include <ctype.h>

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

void addClient(FILE *clientData) {
    typeClient client;
    char answer;

    FILE *clientData;

    if (!(clientData = fopen("clientFile.dat", "a+b"))) {
        printf("Houve um erro na abertura do arquivo.\n");
        return 1;
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

        printf("Endereco: ");
        gets(client.clientAddress.street);
        gets(client.clientAddress.city);
        gets(client.clientAddress.state);
        scanf("%d", &client.clientAddress.houseNumber);

        fwrite(&client, sizeof(typeClient), 1, clientData);

        printf("\n Deseja adicionar outro cliente (S/N)? ");
        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');
    } while (answer == 'S');

    fclose(clientData);
}