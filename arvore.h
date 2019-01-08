#include <stdio.h>
#include <stdlib.h>
struct No
{
    char info[12];
    struct No *esquerda;
    struct No *direita;
    struct No *pai;
    char email[40];
};

struct No *criararvore(char *email, char *info)
{
    struct No *raiz;
    raiz = (struct No *)malloc(sizeof(struct No));
    strcpy(raiz->info, info);
    strcpy(raiz->email, email);
    raiz->esquerda = NULL;
    raiz->direita = NULL;
    return raiz;
}
char *return_email(struct No *arvore, char* valor)
{
    char *error = "NAO_EXISTENTE";
    if (arvore == NULL)
    {
        return error;
    }
    else
    {
        if (strcmp(valor, arvore->info) == 0)
        {
            return arvore->email;
        }
        if (strcmp(valor, arvore->info) < 0)
        {
            return (return_email(arvore->esquerda, valor));
        }
        else
        {
            return (return_email(arvore->direita, valor));
        }
    }
}
struct No *inserir(char *email, char *info, struct No *arvore)
{
    if(strcmp(return_email(arvore,info),"NAO_EXISTENTE")!= 0){
        printf("Já cadastrado");
        return arvore;
    }
    if (arvore == NULL)
    {
        arvore = criararvore(email, info);
    }
    else if (strcmp(info, arvore->info) < 0)
    {
        arvore->esquerda = inserir(email, info, arvore->esquerda);
    }
    else
    {
        arvore->direita = inserir(email, info, arvore->direita);
    }
    return arvore;
}


int inordem(struct No *x)
{
    if (x != NULL)
    {
        inordem(x->esquerda);
        printf(" %s", x->info);
        printf(" : %s \n", x->email);
        inordem(x->direita);
    }
}
int preordem(struct No *x)
{
    if (x != NULL)
    {

        printf(" %s", x->info);
        printf(" : %s \n", x->email);
        inordem(x->esquerda);
        inordem(x->direita);
    }
}
int posordem(struct No *x)
{
    if (x != NULL)
    {
        posordem(x->esquerda);
        posordem(x->direita);
        printf(" %s", x->info);
        printf(" : %s \n", x->email);
    }
}