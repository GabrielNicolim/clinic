// TRABALHO DE ALGORITMOS II (Gabriel Nicolim - 231021909, Otávio Campezzi - 231021259, Yasmin Sobrinho - 231020031)

#include <stdio.h>

#ifdef _WIN32

#include <conio.h>
#include <windows.h>

#endif

typedef struct {
    char city[32], state[8], street[32];
    int houseNumber;
} typeAddress;

typedef struct {
    int clientCode;
    char clientName[32];
    char clientPhone[16];
    int clientAge;
    typeAddress clientAddress;
    int pacientexists;
} typeClient;

typedef struct {
    int clientCode;
    char date[16];
    char time[8];
    int isScheduled;
} typeAppointment;

void addClient();
void updateClient();
void addAppointment();
void removeAppointment();
void mapAppointments();
void getAppointments();
void end();
void listPatientsLastAppointmentOverSixMonths();

void addClient() {
    typeClient client, auxClient;
    char answer;

    FILE *clientData;

    if (!(clientData = fopen("clientes.dat", "a+b"))) {
        printf("Houve um erro na abertura do arquivo.\n");

        return;
    }

    do {
        printf("CADASTRO DE PACIENTE");

        client.pacientexists = 1;

		int codeExists;
		do {
			codeExists = 0;

			printf("\nCodigo: ");
        	scanf("%d", &client.clientCode);

        	rewind(clientData);
        	
        	while (fread(&auxClient, sizeof(typeClient), 1, clientData) == 1) {
	            if(client.clientCode == auxClient.clientCode) {
	            	codeExists = 1;

					printf("\nSelecione outro codigo, um outro paciente ja foi cadastrado com esse codigo.\n");

					break;
				}
        	}
		} while(codeExists);

        fflush(stdin);

        printf("Nome: ");
        gets(client.clientName);

        printf("Telefone: ");
        gets(client.clientPhone);

        printf("Idade: ");
        scanf("%2d", &client.clientAge);
        fflush(stdin);

        printf("Rua: ");
        gets(client.clientAddress.street);

        printf("Numero da casa: ");
        scanf("%5d", &client.clientAddress.houseNumber);
        fflush(stdin);

        printf("Cidade: ");
        gets(client.clientAddress.city);

        printf("Estado (XX): ");
        gets(client.clientAddress.state);

        fwrite(&client, sizeof(typeClient), 1, clientData);

        printf("\nDeseja adicionar outro paciente (S/N)? ");
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

    if (!(clientData = fopen("clientes.dat", "rb+"))) {
        printf("Houve um erro na abertura do arquivo.");
        return;
    }

    do {
        printf("CONSULTA DE PACIENTES PARA ATUALIZACAO\n");
        printf("Digite o codigo do paciente a ser buscado: ");
        scanf("%d", &searchCode);

        while (fread(&client, sizeof(typeClient), 1, clientData)) {
            if (client.pacientexists && client.clientCode == searchCode) {
                printf("\nDADOS ATUAIS DO PACIENTE BUSCADO\n");

                printf("Codigo: %d\n", client.clientCode);
                printf("Nome: %s\n", client.clientName);
                printf("Idade: %d\n", client.clientAge);
                printf("Endereco: %s, %d (%s-%s)\n", client.clientAddress.street, client.clientAddress.houseNumber,
                       client.clientAddress.city, client.clientAddress.state);

                printf("\nATUALIZACAO DO CADASTRO DO PACIENTE\n");

                fflush(stdin);

                printf("Nome: ");
                gets(client.clientName);

                printf("Telefone: ");
                gets(client.clientPhone);

                printf("Idade: ");
                scanf("%2d", &client.clientAge);
                fflush(stdin);

                printf("Rua: ");
                gets(client.clientAddress.street);

                printf("Numero da casa: ");
                scanf("%5d", &client.clientAddress.houseNumber);
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

        printf("\n Deseja alterar os dados de outro paciente (S/N)? ");
        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');

        system("cls");
    } while (answer == 'S');

    fclose(clientData);
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

        printf("Codigo do paciente: ");
        scanf("%d", &appointment.clientCode);
        fflush(stdin);

        fseek(appointmentData, 0, SEEK_SET);
        while (fread(&appointment, sizeof(typeAppointment), 1, appointmentData) == 1) {
            if (appointment.isScheduled) {
                printf("Esse paciente ja possui uma consulta marcada.\n");
                clientCodeExists = 1;
                break;
            }
        }
        if (!clientCodeExists) {
            printf("Data da consulta (DD/MM/YYYY): ");
            scanf("%10s", appointment.date);

            printf("Hora da consulta (HH:MM): ");
            scanf("%5s", appointment.time);

            appointment.isScheduled = 1;

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

void removeAppointment() {
	typeClient client;
    int searchCode;
    char answer;

    FILE *clientData;

    if (!(clientData = fopen("clientes.dat", "rb+"))) {
        printf("Houve um erro na abertura do arquivo de pacientes.");
        return;
    }

    do {
        printf("DESMARCAR CONSULTA\n");
        printf("Digite o codigo do paciente a ser buscado: ");
        scanf("%d", &searchCode);

        int clientFound = 0;

        while (fread(&client, sizeof(typeClient), 1, clientData)) {
            if (client.pacientexists && client.clientCode == searchCode) {
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

                        appointment.isScheduled = 0;

                        printf("\nConsulta desmarcada com sucesso.\n");
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
            printf("\nO paciente buscado nao foi encontrado no banco de dados de pacientes.\n");
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

void mapAppointments() {
    char searchDate[11];
    char answer;

    FILE *appointmentData;
    typeAppointment appointment;

    if (!(appointmentData = fopen("consultas.dat", "rb"))) {
        printf("Houve um erro ao tentar abrir o arquivo de consultas.\n");
        return;
    }

    do {
        printf("ESCREVER MAPA DE HORARIOS PARA UM DETERMINADO DIA\n");
        printf("Digite a data (DD/MM/YYYY) para a qual deseja ver o mapa: ");
        scanf("%10s", searchDate);

        printf("\nConsultas agendadas para %s:\n", searchDate);

        while (fread(&appointment, sizeof(typeAppointment), 1, appointmentData)) {
            if (appointment.isScheduled && strncmp(appointment.date, searchDate, 10) == 0) {
                printf("Codigo do paciente: %d\n", appointment.clientCode);
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
        printf("Houve um erro na abertura do arquivo de pacientes.\n");
        return;
    }

    if (!(appointmentData = fopen("consultas.dat", "rb"))) {
        printf("Houve um erro na abertura do arquivo de consultas.\n");
        fclose(clientData);
        return;
    }

    do {
        printf("\nOBTER CONSULTAS\n");
        printf("Digite o codigo do paciente a ser buscado: ");
        scanf("%d", &searchCode);

        int clientFound = 0;
        typeClient client;
        typeAppointment appointment;
        int index = 1;

        while (fread(&client, sizeof(typeClient), 1, clientData)) {
            if (client.pacientexists && client.clientCode == searchCode) {
                clientFound = 1;

                printf("\nPACIENTE:\n");
                printf("\nNome: %s\n", client.clientName);
                printf("Idade: %d\n", client.clientAge);

                printf("\nCONSULTAS:\n");
                fseek(appointmentData, 0, SEEK_SET);

                while (fread(&appointment, sizeof(typeAppointment), 1, appointmentData)) {
                    if (appointment.clientCode == searchCode && appointment.isScheduled == 1) {
                        printf("\nConsulta %d\n", index);
                        printf("Data: %s %s\n", appointment.date, appointment.time);
                        index++;
                    }
                }
            }
        }

        if (!clientFound) {
            printf("\nO paciente buscado nao foi encontrado no banco de dados de pacientes.\n");
        }

        rewind(clientData);

        printf("\nDeseja consultar outro paciente (S/N)? ");
        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');

        system("cls");
    } while (answer == 'S');

    fclose(clientData);
    fclose(appointmentData);
}

int isDateOverSixMonthsAgo(const char* date, const char* referenceDate) {
    int dayOne, monthOne, yearOne;
    int dayTwo, monthTwo, yearTwo;

    sscanf(date, "%d/%d/%d", &dayOne, &monthOne, &yearOne);
    sscanf(referenceDate, "%d/%d/%d", &dayTwo, &monthTwo, &yearTwo);

    int dateOne = yearOne * 12 + monthOne;
    int dateTwo = yearTwo * 12 + monthTwo;

    return (dateTwo - dateOne) > 6;
}

void listPatientsLastAppointmentOverSixMonths() {
    typeClient client;
    typeAppointment appointment;

    FILE *clientData;
    FILE *appointmentData;

    if (!(clientData = fopen("clientes.dat", "rb"))) {
        printf("Houve um erro na abertura do arquivo de pacientes.\n");
        return;
    }

    if (!(appointmentData = fopen("consultas.dat", "rb"))) {
        printf("Houve um erro na abertura do arquivo de consultas.\n");
        fclose(clientData);
        return;
    }

    char currentDate[11];
    printf("Digite a data atual (DD/MM/YYYY) para verificar os pacientes que devem ser contatados: ");
    scanf("%10s", currentDate);

    char answer;

    do {
        while (fread(&appointment, sizeof(typeAppointment), 1, appointmentData)) {
            if (appointment.isScheduled) {
                if (isDateOverSixMonthsAgo(appointment.date, currentDate)) {
                    fseek(clientData, 0, SEEK_SET);
                    while (fread(&client, sizeof(typeClient), 1, clientData)) {
                        if (client.pacientexists && client.clientCode == appointment.clientCode &&
                            client.clientAge > 50) {
                            printf("ENTRAR EM CONTATO COM:\n");
                            printf("Nome: %s\n", client.clientName);
                            printf("Telefone: %s\n\n", client.clientPhone);
                        }
                    }
                }
            }
        }

        rewind(appointmentData);

        printf("\nDeseja buscar por outra data (S/N)? ");
        do {
            answer = toupper(getch());
        } while (answer != 'S' && answer != 'N');

        system("cls");
    } while (answer == 'S');

    fclose(clientData);
    fclose(appointmentData);
}

int menu() {
    system("cls");
    int option = 0;

    do {
        printf("1 - Cadastrar paciente\n");
        printf("2 - Alterar dados de paciente\n");
        printf("3 - Marcar consulta\n");
        printf("4 - Desmarcar a consulta\n");
        printf("5 - Escrever mapa de horarios para um determinado dia\n");
        printf("6 - Obter consultas\n");
        printf("7 - Consultar maiores de 50 sem retorno a mais de 6 meses\n");
        printf("8 - Fim\n\n");

        printf("Escolha uma opcao (1 - 8): ");
        scanf("%d", &option);

        system("cls");

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
                listPatientsLastAppointmentOverSixMonths();
                break;
            case 8:
                end();
                break;
        }
    } while (option < 1 || option > 8);

    return option;
}

void end() {
    printf("Obrigado por utilizar nosso programa!\n");
}

int main() {
	int option;

    do {
		option = menu();
    } while (option != 8);
}
