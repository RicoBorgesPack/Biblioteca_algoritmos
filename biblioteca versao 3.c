#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define MAX 3
#define MAX_USERs 3
#define TAMANHO 121
#define SEM_EMPRESTIMO -1

int lerInteiro(const char mensagem[], int *valor);
int lerTexto(const char mensagem[], char texto[]);

int buscaTitulo(int codigos[], int qtdTitulos, int codigoBuscado);
int cadastrarTitulo(int codigos[], int estoques[], char descricao1[], char descricao2[], char descricao3[], int qtdTitulos);
void listarTitulos(int codigos[], int estoques[], char descricao1[], char descricao2[], char descricao3[], int qtdTitulos);
long long calculaQtdExemplares(int estoques[], int qtdTitulos);
int disponibilidadeTitulo(int qtdEstoque);

int main(void) {
    int codigos[MAX] = {0}, estoques[MAX] = {0};
    char descricao1[TAMANHO] = "", descricao2[TAMANHO] = "", descricao3[TAM_TEXTO] = "";
    int qtdTitulos = 0, codigoBuscado, posBusca;
    while (qtdTitulos < MAX) {
        qtdTitulos = cadastrarTitulo(codigos, estoques, descricao1, descricao2, descricao3, qtdTitulos);
        if (feof(stdin)) return 0;
    }
    listarTitulos(codigos, estoques, descricao1, descricao2, descricao3, qtdTitulos);
    while (!lerInteiro("Codigo para busca: ", &codigoBuscado)) {
        if (feof(stdin)) return 0;
    }
    posBusca = buscaTitulo(codigos, qtdTitulos, codigoBuscado);
    if (posBusca == -1) {
        printf("Titulo nao encontrado.\n");
    } else {
        printf("Codigo: %d | Estoque: %d\n", codigos[posBusca], estoques[posBusca]);
        printf("Descricao: %s\n", posBusca == 0 ? descricao1 : posBusca == 1 ? descricao2 : descricao3);
        printf("%s\n", disponibilidadeTitulo(estoques[posBusca]) ? "Titulo disponivel." : "Titulo temporariamente indisponivel.");
    }
    printf("Total de livros na biblioteca: %lld\n", calculaQtdExemplares(estoques, qtdTitulos));
    return 0;
}

int buscaTitulo(int codigos[], int qtdTitulos, int codigoBuscado) {
    for (int i = 0; i < qtdTitulos; i++) {
        if (codigos[i] == codigoBuscado) return i;
    }
    return -1;
}

int cadastrarTitulo(int codigos[], int estoques[], char descricao1[], char descricao2[], char descricao3[], int qtdTitulos) {
    int codigo, estoque;
    char descricao[TAMANHO];
    if (qtdTitulos >= MAX) {
        printf("Erro: limite de livros ultrapassado.\n");
        return qtdTitulos;
    }
    if (!lerInteiro("Codigo: ", &codigo)) return qtdTitulos;
    if (codigo <= 0 || buscaTitulo(codigos, qtdTitulos, codigo) != -1) {
        printf("Erro: codigo deve ser positivo e unico.\n");
        return qtdTitulos;
    }
    if (!lerInteiro("Quantidade em estoque: ", &estoque)) return qtdTitulos;
    if (estoque < 0) {
        printf("Erro: estoque nao pode ser negativo.\n");
        return qtdTitulos;
    }
    if (!lerTexto("Titulo; autor; ano; tema: ", descricao)) return qtdTitulos;
    codigos[qtdTitulos] = codigo;
    estoques[qtdTitulos] = estoque;
    strcpy(qtdTitulos == 0 ? descricao1 : qtdTitulos == 1 ? descricao2 : descricao3, descricao);
    printf("Livro cadastrado com sucesso!\n");
    return qtdTitulos + 1;
}

void listarTitulos(int codigos[], int estoques[], char descricao1[], char descricao2[], char descricao3[], int qtdTitulos) {
    for (int i = 0; i < qtdTitulos; i++) {
        printf("Codigo: %d | Estoque: %d\n", codigos[i], estoques[i]);
        printf("Descricao: %s\n", i == 0 ? descricao1 : i == 1 ? descricao2 : descricao3);
    }
}

long long calculaQtdExemplares(int estoques[], int qtdTitulos) {
    long long qtdExemplares = 0;
    for (int i = 0; i < qtdTitulos; i++) qtdExemplares += estoques[i];
    return qtdExemplares;
}

int disponibilidadeTitulo(int qtdEstoque) {
    return qtdEstoque > 0;
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
        printf("Erro: informe um valor valido.\n");
        return 0;
    }
    while (isspace((unsigned char) *fim)) {
        fim++;
    }
    if (*fim != '\0') {
        printf("Erro: informe somente um numero valido.\n");
        return 0;
    }
    *valor = (int) numero;
    return 1;
}
