#include <stdio.h>

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
    int clientExists;
} typeClient;

typedef struct {
    int clientCode;
    char date[11];
    char time[6];
    int isScheduled;
} typeAppointment;

void addClient();
void updateClient();
void removeAppointment();
void addAppointment();
void mapAppointments();
void end();

void addClient() {
    typeClient client;
    char answer;

    FILE *clientData;

    if (!(clientData = fopen("clientes.dat", "a+b"))) {
        printf("Houve um erro na abertura do arquivo.\n");

        return;
    }

    do {
        printf("CADASTRO DE CLIENTE\n");

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

        printf("Estado (XX): ");
        gets(client.clientAddress.state);

        fwrite(&client, sizeof(typeClient), 1, clientData);

        printf("\nDeseja adicionar outro cliente (S/N)? ");
        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');
        
        system("cls");
    } while (answer == 'S');

    fclose(clientData);
}

void updateClient() {
    typeClient client;
    int searchCode, answer;
    int clientFlag = 0;

    FILE *clientData;

    if (!(clientData = fopen("clientes.dat", "rb"))) {
        printf("Houve um erro na abertura do arquivo.");

        return;
    }

    do {
        printf("CONSULTA DE PACIENTES PARA ATUALIZACAO\n");
        printf("Digite o nome do cliente a ser buscado: ");
        scanf("%d", &searchCode);

        while (fread(&client, sizeof(typeClient), 1, clientData)) {
            if (client.clientExists && client.clientCode == searchCode) {
                printf("\nDADOS ATUAIS DO PACIENTE BUSCADO\n");

                printf("Codigo: %d\n", client.clientCode);
                printf("Nome: %s\n", client.clientName);
                printf("Idade: %d\n", client.clientAge);
                printf("Endereco: %s, %d (%s-%s)\n", client.clientAddress.street, client.clientAddress.houseNumber,
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

                printf("Endereco completo:\n");
                printf("Rua: ");
                gets(client.clientAddress.street);
                printf("Numero da casa: ");
                scanf("%d", &client.clientAddress.houseNumber);
                fflush(stdin);
                printf("Cidade: ");
                gets(client.clientAddress.city);
                printf("Estado (XX): ");
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

        rewind(clientData);

        printf("\n Deseja alterar os dados de outro cliente (S/N)? ");
        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');
        
        system("cls");
    } while (answer == 'S');

    fclose(clientData);
}


void removeAppointment() {
	typeClient client;
    int searchCode;
    char answer;

    FILE *clientData;

    if (!(clientData = fopen("clientes.dat", "rb+"))) {
        printf("Houve um erro na abertura do arquivo de clientes.");
        return;
    }

    do {
        printf("DESMARCAR CONSULTA\n");
        printf("Digite o codigo do cliente a ser buscado: ");
        scanf("%d", &searchCode);

        int clientFound = 0;

        while (fread(&client, sizeof(typeClient), 1, clientData)) {
            if (client.clientExists && client.clientCode == searchCode) {
                clientFound = 1;

                typeAppointment appointment;

                FILE *appointmentData;

                if (!(appointmentData = fopen("consultas.dat", "rb+"))) {
                    printf("Houve um erro na abertura do arquivo de consultas.");
                    fclose(clientData);
                    return;
                }

                int found = 0;
                while (fread(&appointment, sizeof(typeAppointment), 1, appointmentData)) {
                    if (appointment.isScheduled && appointment.clientCode == searchCode) {
                        found = 1;
                        appointment.isScheduled = 0;
                        fseek(appointmentData, -sizeof(typeAppointment), SEEK_CUR);
                        fwrite(&appointment, sizeof(typeAppointment), 1, appointmentData);
                        
                        printf("\nConsulta removida.\n");

                        break;
                    }
                }

                fclose(appointmentData);

                if (!found) {
                    printf("\nO paciente nao tem consultas ativas.\n");
                }
            }
        }

        if (!clientFound) {
            printf("\nO paciente buscado nao foi encontrado no banco de dados de clientes.\n");
        }

        rewind(clientData);

        printf("\nDeseja remover outra consulta (S/N)? ");
        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');
        
        system("cls");
    } while (answer == 'S');

    fclose(clientData);
}

void end() {
    printf("Obrigado por utilizar nosso programa!\n");
}

void addAppointment() {
    typeAppointment appointment;
    char answer;

    FILE *appointmentData;

    if (!(appointmentData = fopen("consultas.dat", "a+b"))) {
        printf("Houve um erro ao tentar abrir o arquivo de consultas.\n");
        return;
    }

    do {
        int clientCodeExists = 0;
        appointment.isScheduled = 1;

        printf("Codigo do cliente: ");
        scanf("%d", &appointment.clientCode);
        fflush(stdin);

        fseek(appointmentData, 0, SEEK_SET);
        while (fread(&appointment, sizeof(typeAppointment), 1, appointmentData) == 1) {
            if (appointment.isScheduled && appointment.clientCode == appointment.clientCode) {
                printf("Esse cliente ja possui uma consulta marcada.\n");
                clientCodeExists = 1;
                break;
            }
        }
        if (!clientCodeExists) {
            printf("Data da consulta (DD/MM/YYYY): ");
            scanf("%s", appointment.date);

            printf("Hora da consulta (HH:MM): ");
            scanf("%s", appointment.time);

            fwrite(&appointment, sizeof(typeAppointment), 1, appointmentData);
        }

        printf("\nDeseja adicionar outra consulta (S/N)? ");
        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');
        
        system("cls");
    } while (answer == 'S');

    fclose(appointmentData);
}

void mapAppointments() {
    char searchDate[11];
    char answer;

    FILE *appointmentData;
    typeAppointment appointment;

    if (!(appointmentData = fopen("appointments.dat", "rb"))) {
        printf("Houve um erro ao tentar abrir o arquivo de consultas.\n");
        return;
    }
    do {
        printf("ESCREVER MAPA DE HORARIOS PARA UM DETERMINADO DIA\n");
        printf("Digite a data (DD/MM/YYYY) para a qual deseja ver o mapa: ");
        scanf("%s", searchDate);

        printf("\nConsultas agendadas para %s:\n", searchDate);

        while (fread(&appointment, sizeof(typeAppointment), 1, appointmentData)) {
            if (appointment.isScheduled && strcmp(appointment.date, searchDate) == 0) {
                printf("Codigo do cliente: %d\n", appointment.clientCode);
                printf("Data: %s\n", appointment.date);
                printf("Hora: %s\n\n", appointment.time);
            }
        }

        rewind(appointmentData);

        printf("\nDeseja pesquisar o mapa de horarios para outro dia (S/N)? ");

        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');
        
        system("cls");
    } while (answer == 'S');

    fclose(appointmentData);
}


void getAppointments() {
    int searchCode;
    char answer;

    FILE *clientData;
    FILE *appointmentData;

    if (!(clientData = fopen("clientes.dat", "rb"))) {
        printf("Houve um erro na abertura do arquivo de clientes.\n");
        return;
    }

    if (!(appointmentData = fopen("consultas.dat", "rb"))) {
        printf("Houve um erro na abertura do arquivo de consultas.\n");
        fclose(clientData);
        return;
    }

    do {
        printf("\nOBTER CONSULTAS\n");
        printf("Digite o codigo do cliente a ser buscado: ");
        scanf("%d", &searchCode);

        int clientFound = 0;
        typeClient client;
        typeAppointment appointment;
        int index = 1;

        while (fread(&client, sizeof(typeClient), 1, clientData)) {
            if (client.clientExists && client.clientCode == searchCode) {
                clientFound = 1;

                printf("\nNome: %s\n", client.clientName);
                printf("Idade: %d\n", client.clientAge);

                printf("\nConsultas\n");
                fseek(appointmentData, 0, SEEK_SET);

                while (fread(&appointment, sizeof(typeAppointment), 1, appointmentData)) {
                    if (appointment.clientCode == searchCode) {
                        printf("\nConsulta %d\n", index);
                        printf("Data: %s %s\n", appointment.date, appointment.time);
                        index++;
                    }
                }
            }
        }

        if (!clientFound) {
            printf("\nO paciente buscado nao foi encontrado no banco de dados de clientes.\n");
        }

        rewind(clientData);

        printf("\nDeseja consultar outro cliente (S/N)? ");
        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');

        system("cls");
    } while (answer == 'S');

    fclose(clientData);
    fclose(appointmentData);
}

int menu()
{
	system("cls");

    int option = 0;

	do {
        printf("1 - Cadastrar cliente\n");
        printf("2 - Alterar dados de cliente\n");
        printf("3 - Marcar consulta\n");
        printf("4 - Desmarcar a consulta\n");
        printf("5 - Escrever mapa de horarios para um determinado dia\n");
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
            addAppointment();
            break;
        case 4:
            removeAppointment();
            break;
        case 5:
            mapAppointments();
            break;
        case 6:
        	getAppointments();
            break;
        case 7:
        	// Error
            break;
        case 8:
            end();
            break;
    }
    
    return option;
}

int main() {
	int option = 0;

    do {
		option = menu();
    } while (option != 8);
}

