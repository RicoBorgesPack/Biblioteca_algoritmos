#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define MAX_LIVROS 3
#define MAX_USUARIOS 3
#define TAM_TEXTO 121
#define SEM_EMPRESTIMO -1

int lerInteiro(const char mensagem[], int *valor);
int lerTexto(const char mensagem[], char texto[]);

int main(void) {
    int codigos[MAX_LIVROS] = {0};
    int estoques[MAX_LIVROS] = {0};
    int qtdLivros = 0;
    int novoCodigo, estoque, repetido, codigoBusca, posBusca;
    long long qtdExemplares = 0;
    char descricao1[TAM_TEXTO] = "";
    char descricao2[TAM_TEXTO] = "";
    char descricao3[TAM_TEXTO] = "";
    while (qtdLivros < MAX_LIVROS) {
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
        if (!lerInteiro("Quantidade em estoque: ", &estoque)) {
            if (feof(stdin)) return 0;
            continue;
        }
        if (estoque < 0) {
            printf("Erro: estoque nao pode ser negativo.\n");
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
        printf("Livro cadastrado com sucesso!\n");
    }
    printf("===== ACERVO =====\n");
    for (int i = 0; i < qtdLivros; i++) {
        printf("Codigo: %d | Estoque: %d\n", codigos[i], estoques[i]);
        printf("Descricao: %s\n", i == 0 ? descricao1 : i == 1 ? descricao2 : descricao3);
        qtdExemplares += estoques[i];
    }
    printf("Total de exemplares no acervo: %lld\n", qtdExemplares);
    while (!lerInteiro("Codigo para busca: ", &codigoBusca)) {
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
        printf("Livro nao encontrado.\n");
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
        printf("Erro: informe no maximo 120 caracteres.\n");
        return 0;
    }
    if (tamanho == 0 || tamanho >= TAM_TEXTO) {
        printf("Erro: informe um texto de 1 a 120 caracteres.\n");
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
