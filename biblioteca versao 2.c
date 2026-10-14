#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define MAX 3
#define MAX_USER 3
#define TAM 121
#define SEM_EMPRESTIMO -1

int lerInteiro(const char mensagem[], int *valor);
int lerTexto(const char mensagem[], char texto[]);

int main(void) {
    int codigos[MAX] = {0};
    int estoques[MAX] = {0};
    int qtd = 0;
    int novoCodigo, estoque, repetido, codigoBusca, posBusca;
    long long qtdExemplares = 0;
    char descricao1[TAM] = "";
    char descricao2[TAM] = "";
    char descricao3[TAM] = "";
    while (qtd < MAX) {
        if (!lerInteiro("Codigo do livro: ", &novoCodigo)) {
            if (feof(stdin)) return 0;
            continue;
        }
        repetido = 0;
        for (int i = 0; i < qtdLivros; i++) {
            if (codigos[i] == novoCodigo) repetido = 1;
        }
        if (novoCodigo <= 0 || repetido) {
            printf("Erro: codigo deve ser positivo e unico.\n");
            continue;
        }
        if (!lerInteiro("Quantidade no estoque: ", &estoque)) {
            if (feof(stdin)) return 0;
            continue;
        }
        if (estoque < 0) {
            printf("Erro: nao pode haver estoque com numero negativo.\n");
            continue;
        }
        if (!lerTexto("Titulo; autor; ano; tema: ",
                      qtdLivros == 0 ? descricao1 : qtdLivros == 1 ? descricao2 : descricao3)) {
            if (feof(stdin)) return 0;
            continue;
        }
        codigos[qtdLivros] = novoCodigo;
        estoques[qtdLivros] = estoque;
        qtdLivros++;
        printf("Livro cadastrado!\n");
    }
    printf("===== ACERVO =====\n");
    for (int i = 0; i < qtdLivros; i++) {
        printf("Codigo: %d | Estoque: %d\n", codigos[i], estoques[i]);
        printf("Descricao: %s\n", i == 0 ? descricao1 : i == 1 ? descricao2 : descricao3);
        qtdExemplares += estoques[i];
    }
    printf("Total na biblioteca: %lld\n", qtdExemplares);
    while (!lerInteiro("Codigo: ", &codigoBusca)) {
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
        printf("Descricao: %s\n", posBusca == 0 ? descricao1 : posBusca == 1 ? descricao2 : descricao3);
        printf("%s\n", estoques[posBusca] > 0 ? "Livro disponivel." : "Livro temporariamente indisponivel.");
    }
    return 0;
}

int lerTexto(const char mensagem[], char texto[]) {
    char linha[TAM_TEXTO + 2];
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
    if (tamanho == 0 || tamanho >= TAM_TEXTO) {
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

int lerInteiro(const char mensagem[], int *valor) {
    char linha[TAM_TEXTO];
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
