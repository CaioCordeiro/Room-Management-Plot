#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "serial.h"
#define MAX 100
void marcar();
void excluir();
void clear();
void clear_txt();
void add_cad();
void get_time();
void led_green();
void led_yellow();
void led_red();
char *serialr();
char *get_string_time();
struct No *read_cad();
char *get_string_email();
int check_status(); // 0 == "ocupado" / 1 == "livre" / 2 == "timer"
char buf[255];

int main()
{
    struct No *BD = NULL;
    int choice;
    char *id = malloc(sizeof(char) * 40);
    char *Last_id = malloc(sizeof(char) * 40);
    char *email = malloc(sizeof(char) * 120);
    int x = 0;
    char *email2 = malloc(sizeof(char) * 120);
    int room_status;
    int is_confirmed = 0;
    int p;
    BD = read_cad(BD);
    while (1)
    {
        /* ---------------------- MENU DE OPÇÔES ---------------------------------------------- */
        printf("\n");
        printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
        printf("    Oque voce deseja?");
        printf("\n //--//--//--//--//--//--//--//--//--//--//\n");
        printf("\n1 - Criar/Cancelar agendamento\n");
        printf("\n-----------------------------------\n");
        printf("\n2 - Cadastramento\n");
        printf("\n-----------------------------------\n");
        printf("\n3 - Mostrar cadastros\n");
        printf("\n-----------------------------------\n");
        printf("\n4 - Mostrar Status\n");
        printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
        printf("\n\nSua escolha: ");
        /* -------------------------------------------------------------------------------------------- */
        scanf("%d", &choice); //SUA ESCOLHA
        printf("\n");
        switch (choice)
        {
        case 1:
            /* ----------------------------- OPÇÃO 1 ------------------------------------------------------ */
            room_status = check_status();
            // clear();
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
            printf("        Você Selecionou : Agendamento");
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");
            printf("Passe o cartão:\n "); //RFid
            do
            {
                //serial(id);
                strcpy(id, serialr());
            } while (return_email(BD, id) == NULL); //Lê varias vezes por um problema no RFid que não envia completamente o id
            printf("Checking room status\n");
            if (room_status == 0 || (room_status == 2 && is_confirmed == 2))
            {
                // printf("Is_confirmed? : %d",is_confirmed);
                if (strcmp(return_email(BD, id), get_string_email()) == 0 && is_confirmed == 2) //Se o id que quer agendar foi o ultimo a agendar ele cancela o ultimo agendamento
                {
                    excluir();                    //CANCELA
                    email = return_email(BD, id); //RETORNA E-MAIL DA ARVORE
                    printf("%s", email);
                    strcpy(buf, "");
                    clear_txt();
                    is_confirmed = 0;
                    clear();
                    printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
                    printf("                Cancelado!");
                    printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");
                    continue;
                }
                else if (is_confirmed != 2)
                {
                    email = return_email(BD, id); //RETORNA E-MAIL DA ARVORE
                    printf("%s", email);
                    excluir();
                    marcar(email);
                    is_confirmed = 2;
                    clear_txt();
                    continue;
                }
                else if (strcmp(return_email(BD, id), get_string_email()) != 0 && is_confirmed == 2)
                {
                    printf("%s -> %s", return_email(BD, id), get_string_email());
                    printf("Sala Ocupada!");
                    continue;
                }
            }
            else if (room_status == 2)
            {
                if (strcmp(return_email(BD, id), get_string_email()) == 0 && is_confirmed != 2)
                {
                    // printf("Is_confirmed? : %d",is_confirmed);
                    email = return_email(BD, id); //RETORNA E-MAIL DA ARVORE
                    printf("%s", email);
                    printf("Confirmado!");
                    room_status = 0;
                    is_confirmed = 2;
                    clear_txt();
                    continue;
                }
                else
                {
                    // printf("Is_confirmed? : %d",is_confirmed);
                    email = return_email(BD, id); //RETORNA E-MAIL DA ARVORE
                    printf("%s", email);
                    printf("Sala Ocupada!");
                    continue;
                }
            }
            else if (room_status == 1)
            {
                // printf("Is_confirmed? : %d",is_confirmed);
                email = return_email(BD, id); //RETORNA E-MAIL DA ARVORE
                printf("%s", email);
                marcar(email); //MARCA
                is_confirmed = 2;
                clear_txt();
                clear();
                printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
                printf("                Marcado!");
                printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");
                continue;
            }
            continue;

            /* ------------------------ OPÇÃO 2----------------------------------------------------------- */
        case 2:

            clear();
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
            printf("      Você Selecionou : Cadastramento!");
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");
            printf("Passe o cartão:\n"); //LER RFid

            while (x != 1)
            {
                //serial(id);
                strcpy(id, serialr());
                printf(" %s", id);
                printf("   O id está correto?(1=Correto | 0 = Incorreto");
                scanf("%d", &x);
            }
            printf("%s", id);
            printf("\n");

            printf("Insira seu e-mail: "); //LER E-MAIL
            scanf("%s", email);
            clear();

            printf("Cadastrando...\n");
            BD = inserir(email, id, BD); //INSERE NA ARVORE
            add_cad(id, email);
            x = 0;
            clear();
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
            printf("                Cadastrado!");
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");
            continue;

            /* ------------------------ OPÇÃO 3 --------------------------------------------------------- */
        case 3:
            clear();
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
            printf("    Você Selecionou : Verificar Cadastro");
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");
            printf("\n");
            inordem(BD); //MOSTRA A ARVORE
            printf("\n");
            continue;
            /* ------------------------ OPÇÃO 4 --------------------------------------------------------- */
        case 4:
            clear();
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n");
            int x = check_status();
            get_time();
            if (x == 1)
            {
                // printf("Is_confirmed? : %d",is_confirmed);
                printf("    STATUS : LIVRE");
                printf("    Proximo agendamento : %s", get_string_time());
                led_green();
                is_confirmed = 0;
            }
            if (x == 0)
            {

                if (is_confirmed != 2)
                {
                    excluir();
                    printf("    STATUS : LIVRE");
                    printf("    Proximo agendamento : %s", get_string_time());
                    is_confirmed = 0;
                    led_green();
                    continue;
                }
                printf("    STATUS : OCUPADO");
                printf("    Agendamento atual : %s", get_string_time());
                led_red();
            }
            if (x == 2)
            {
                // printf("Is_confirmed? : %d",is_confirmed);
                if (is_confirmed != 2)
                {
                    printf("    STATUS : TIMER");
                    printf("    Agendamento atual : %s", get_string_time());
                    led_yellow();
                    continue;
                }
                printf("    STATUS : OCUPADO");
                printf("    Agendamento atual : %s", get_string_time());
                led_red();
            }
            printf("\n//--//--//--//--//--//--//--//--//--//--//\n\n");

            continue;
        }
    }
    free(BD); //LIBERA O ESPAÇO NA MEMORIA
    return (0);
}
/* ----------------------- FUNÇÃO PARA MARCAR NO CALENDAR ----------------------------------- */
void marcar(char *email_)
{
    char command[50];
    char email[50];
    strcpy(email, email_);                   //CRIA A STRING E-MAIL
    strcpy(command, "python3 create.py 1 "); //CRIA A STRING COM O COMANDO PADRÃO
    strcat(command, email);                  //COLOCA O E-MAIL COMO ARGUMENTO
    system(command);                         //RODA O PROGRAMA NO BASH
}
/* ----------------------- FUNÇÃO PARA EXCLUIR NO CALENDAR ----------------------------------- */
void excluir()
{
    char command[50];
    strcpy(command, "python3 create.py 2 delete"); //CRIA A STRING COMANDO COM ARGUMENTO 2
    system(command);                               //RODA O PROGRAMA NO BASH
}
/* ----------------------- FUNÇÃO PARA CHECKAR O STATUS NO CALENDAR ----------------------------------- */
void check_py_status()
{
    char command[50];
    strcpy(command, "python3 create.py 4 check"); //CRIA A STRING COMANDO COM ARGUMENTO 4
    system(command);                              //RODA O PROGRAMA NO BASH
}
void check_py_email()
{
    char command[50];
    strcpy(command, "python3 create.py 3 check"); //CRIA A STRING COMANDO COM ARGUMENTO 3
    system(command);
}
void clear()
{
    char command[50];
    strcpy(command, "clear");
    system(command);
}
void get_time()
{
    char command[50];
    strcpy(command, "python3 create.py 5 check"); //CRIA A STRING COMANDO COM ARGUMENTO 5
    system(command);
}
void led_green()
{
    char command[50];
    strcpy(command, "python3 create.py 6 1"); //CRIA A STRING COMANDO COM ARGUMENTO 5
    system(command);
}
void led_yellow()
{
    char command[50];
    strcpy(command, "python3 create.py 6 2"); //CRIA A STRING COMANDO COM ARGUMENTO 5
    system(command);
}
void led_red()
{
    char command[50];
    strcpy(command, "python3 create.py 6 3"); //CRIA A STRING COMANDO COM ARGUMENTO 5
    system(command);
}
void clear_txt()
{
    char c = ' ';
    FILE *fp;

    fp = fopen("temp.txt", "w");
    fprintf(fp, "%c", c);
    fclose(fp);
}

char *get_string_email()
{
    char buff[255];
    FILE *fp;
    check_py_email();
    fp = fopen("temp.txt", "r"); // read mode

    fscanf(fp, "%s", buff);
    strcpy(buf, buff);
    fclose(fp);
    return buf;
}
char *get_string_time()
{
    char buff[255];
    FILE *fp;
    fp = fopen("temp.txt", "r"); // read mode

    fscanf(fp, "%s", buff);
    strcpy(buf, buff);
    fclose(fp);
    return buf;
}

int check_status()
{
    char buff[255];
    char l[255];
    char t[255];
    char o[255];
    FILE *fp;

    check_py_status();
    strcpy(o, "ocupado");
    strcpy(t, "timer");
    strcpy(l, "livre");
    fp = fopen("temp.txt", "r"); // read mode
    fscanf(fp, "%s", buff);
    strcpy(buf, buff);
    fclose(fp);
    if (strcmp(buf, l) == 0)
    {
        return 1;
    }
    else if (strcmp(buf, t) == 0)
    {
        return 2;
    }
    else if (strcmp(buf, o) == 0)
    {
        return 0;
    }
}
char *serialr()
{
    char buff[255];
    FILE *fp;
    char command[50];
    strcpy(command, "python3 create.py 7 serial"); //CRIA A STRING COMANDO COM ARGUMENTO 5
    system(command);
    fp = fopen("temp.txt", "r"); // read mode
    fscanf(fp, "%s", buff);
    strcpy(buf, buff);
    fclose(fp);
    return buf;
}
/* ----------------------- FUNÇÃO PARA ESCREVER OS CADASTROS ----------------------------------- */
void add_cad(char *id, char *email)
{
    FILE *fp;
    fp = fopen("cadastro.txt", "a"); // read mode
    fprintf(fp, " %s\n", id);
    fprintf(fp, "%s\n", email);
    fclose(fp);
}
/* ----------------------- FUNÇÃO PARA LER OS CADASTROS ----------------------------------- */

struct No *read_cad(struct No *BD)
{
    FILE *fp;
    char id[40];
    char email[255];
    int i;
    fp = fopen("cadastro.txt", "r"); // read mode
    i = fscanf(fp, "%s", id);
    i = fscanf(fp, "%s", email);
    while (i == 1)
    {
        BD = inserir(email, id, BD);
        i = fscanf(fp, "%s", id);
        i = fscanf(fp, "%s", email);
    }
    fclose(fp);
    return BD;
}