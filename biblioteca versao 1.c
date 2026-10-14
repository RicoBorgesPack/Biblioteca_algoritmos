#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define MAX_ 3
#define MAX_USER 3
#define TAM 121
#define SEM_EMPRESTIMO -1

int lerInt(const char mensagem[], int *valor);
int lerTxt(const char mensagem[], char texto[]);

int main(void) {
    int codigos[MAX] = {0};
    int estoques[MAX] = {0};
    int qtd = 0;
    int novoCodigo, estoque, repetido, codigoBusca, posBusca;
    long long qtdExemp = 0;
    while (qtd < MAX) {
        if (!lerInt("Codigo do livro: ", &novoCodigo)) {
            if (feof(stdin)) return 0;
            continue;
        }
        repetido = 0;
        for (int i = 0; i < qtd; i++) {
            if (codigos[i] == novoCodigo) repetido = 1;
        }
        if (novoCodigo <= 0 || repetido) {
            printf("Erro: codigo tem que ser positivo e unico.\n");
            continue;
        }
        if (!lerInt("Quantidade no estoque: ", &estoque)) {
            if (feof(stdin)) return 0;
            continue;
        }
        if (estoque < 0) {
            printf("Erro: nao pode haver estoque com numero negativo.\n");
            continue;
        }
        codigos[qtd] = novoCodigo;
        estoques[qtd] = estoque;
        qtd++;
        printf("Livro cadastrado!\n");
    }
    printf("===== Biblioteca =====\n");
    for (int i = 0; i < qtd; i++) {
        printf("Codigo: %d | Estoque: %d\n", codigos[i], estoques[i]);
        qtdExemplares += estoques[i];
    }
    printf("Total de exemplares na biblioteca: %lld\n", qtdExemp);
    while (!lerInt("Codigo: ", &codigoBusca)) {
        if (feof(stdin)) return 0;
    }
    posBusca = -1;
    for (int i = 0; i < qtdLivros; i++) {
        if (codigos[i] == codigoBusca) {
            posBusca = i;
            break;
        }
    }
    if (posBusca == -1) {
        printf("Livro nao existente no estoque.\n");
    } else {
        printf("Codigo: %d | Estoque: %d\n", codigos[posBusca], estoques[posBusca]);
        printf("%s\n", estoques[posBusca] > 0 ? "Livro disponivel." : "Livro temporariamente indisponivel.");
    }
    return 0;
}

int lerTxt(const char mensagem[], char texto[]) {
    char linha[TAM + 2];
    int caractere;
    size_t tamanho;
    printf("%s", mensagem);
    if (fgets(linha, sizeof linha, stdin) == NULL) {
        return 0;
    }
    tamanho = strcspn(linha, "\r\n");
    if (strchr(linha, '\n') == NULL && !feof(stdin)) {
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
        }
        printf("Erro: informe ate 120 caracteres.\n");
        return 0;
    }
    if (tamanho == 0 || tamanho >= TAM) {
        printf("Erro: informe um texto de um a 120 caracteres.\n");
        return 0;
    }
    linha[tamanho] = '\0';
    for (size_t i = 0; i < tamanho; i++) {
        if (!isspace((unsigned char) linha[i])) {
            strcpy(texto, linha);
            return 1;
        }
    }
    printf("Erro: o texto nao pode conter apenas espacos.\n");
    return 0;
}

int lerInt(const char mensagem[], int *valor) {
    char linha[TAM];
    char *fim;
    long numero;
    int caractere;
    printf("%s", mensagem);
    if (fgets(linha, sizeof linha, stdin) == NULL) {
        return 0;
    }
    if (strchr(linha, '\n') == NULL && !feof(stdin)) {
        while ((caractere = getchar()) != '\n' && caractere != EOF) {
        }
        printf("Erro: entrada numerica muito longa.\n");
        return 0;
    }
    errno = 0;
    numero = strtol(linha, &fim, 10);
    if (fim == linha || errno == ERANGE || numero < INT_MIN || numero > INT_MAX) {
        printf("Erro: informe um numero inteiro valido.\n");
        return 0;
    }
    while (isspace((unsigned char) *fim)) {
        fim++;
    }
    if (*fim != '\0') {
        printf("Erro: informe somente um numero inteiro.\n");
        return 0;
    }
    *valor = (int) numero;
    return 1;
}
