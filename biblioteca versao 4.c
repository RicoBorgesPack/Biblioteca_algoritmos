#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define MAX 3
#define MAX_USERS 3
#define TAMANHO 121
#define SEM_EMPRESTIMO -1

int lerInteiro(const char mensagem[], int *valor);
int lerTexto(const char mensagem[], char texto[]);

typedef struct {
    int codigo;
    char descricao[TAMANHO];
    int estoque;
} Livro;

typedef struct {
    int codigo;
    char dados[TAMANHO];
    int codigoLivroEmprestado;
} Usuario;

int cadastrarLivro(Livro livros[], int qtd);
int cadastrarUsuario(Usuario usuarios[], int qtdUsuarios);
void listarLivros(Livro livros[], int qtd);
void listarUsuarios(Usuario usuarios[], int qtdUsuarios);
int buscarLivro(Livro livros[], int qtd, int codigo);
int buscarUsuario(Usuario usuarios[], int qtd, int codigo);
long long calcularQtdExemplares(Livro livros[], int qtd);
int disponibilidadeTitulo(int qtdEstoque);
void mostrarLivro(Livro livro);
void mostrarUsuario(Usuario usuario);

int main(void) {
    Livro livros[MAX] = {{0}};
    Usuario usuarios[MAX_USERS] = {{0}};
    int qtd = 0, qtdUsuarios = 0;
    int opcao, codigo, indice;
    printf("Sistema de Biblioteca - Versao 4\n");
    while (1) {
        printf("\n1 - Cadastrar livro\n2 - Cadastrar usuario\n3 - Listar livros\n4 - Buscar livro\n");
        printf("7 - Listar usuarios\n8 - Buscar usuario\n0 - Sair\n");
        if (!lerInteiro("Opcao: ", &opcao)) {
            if (feof(stdin)) break;
            continue;
        }
        if (opcao == 0) break;
        switch (opcao) {
            case 1:
                qtd = cadastrarLivro(livros, qtd);
                break;
            case 2:
                qtdUsuarios = cadastrarUsuario(usuarios, qtdUsuarios);
                break;
            case 3:
                listarLivros(livros, qtd);
                break;
            case 4:
                if (!lerInteiro("Codigo do livro: ", &codigo)) break;
                indice = buscarLivro(livros, qtd, codigo);
                if (indice == -1) printf("Livro nao encontrado.\n");
                else mostrarLivro(livros[indice]);
                break;
            case 7:
                listarUsuarios(usuarios, qtdUsuarios);
                break;
            case 8:
                if (!lerInteiro("Codigo do usuario: ", &codigo)) break;
                indice = buscarUsuario(usuarios, qtdUsuarios, codigo);
                if (indice == -1) printf("Usuario nao encontrado.\n");
                else mostrarUsuario(usuarios[indice]);
                break;
            default:
                printf("Erro: opcao invalida.\n");
        }
        if (feof(stdin)) break;
    }
    printf("Sistema encerrado.\n");
    return 0;
}

int buscarLivro(Livro livros[], int qtd, int codigo) {
    for (int i = 0; i < qtd; i++) {
        if (livros[i].codigo == codigo) return i;
    }
    return -1;
}

int buscarUsuario(Usuario usuarios[], int qtd, int codigo) {
    for (int i = 0; i < qtd; i++) {
        if (usuarios[i].codigo == codigo) return i;
    }
    return -1;
}

int cadastrarLivro(Livro livros[], int qtd) {
    Livro novoLivro;
    if (qtd >= MAX) {
        printf("Erro: limite de tres livros atingido.\n");
        return qtd;
    }
    if (!lerInteiro("Codigo do livro: ", &novoLivro.codigo)) return qtd;
    if (novoLivro.codigo <= 0 || buscarLivro(livros, qtd, novoLivro.codigo) != -1) {
        printf("Erro: codigo de livro deve ser positivo e unico.\n");
        return qtd;
    }
    if (!lerInteiro("Quantidade em estoque: ", &novoLivro.estoque)) return qtd;
    if (novoLivro.estoque < 0) {
        printf("Erro: estoque nao pode ser negativo.\n");
        return qtd;
    }
    if (!lerTexto("Titulo; autor; ano; tema: ", novoLivro.descricao)) return qtd;
    livros[qtd] = novoLivro;
    printf("Livro cadastrado com sucesso!\n");
    return qtd + 1;
}

int cadastrarUsuario(Usuario usuarios[], int qtdUsuarios) {
    Usuario novoUsuario;
    if (qtdUsuarios >= MAX_USERS) {
        printf("Erro: limite de tres usuarios atingido.\n");
        return qtdUsuarios;
    }
    if (!lerInteiro("Codigo do usuario: ", &novoUsuario.codigo)) return qtdUsuarios;
    if (novoUsuario.codigo <= 0 || buscarUsuario(usuarios, qtdUsuarios, novoUsuario.codigo) != -1) {
        printf("Erro: codigo de usuario deve ser positivo e unico.\n");
        return qtdUsuarios;
    }
    if (!lerTexto("Dados pessoais em uma linha: ", novoUsuario.dados)) return qtdUsuarios;
    novoUsuario.codigoLivroEmprestado = SEM_EMPRESTIMO;
    usuarios[qtdUsuarios] = novoUsuario;
    printf("Usuario cadastrado com sucesso!\n");
    return qtdUsuarios + 1;
}

void mostrarLivro(Livro livro) {
    printf("Codigo: %d | Estoque: %d\nDescricao: %s\n", livro.codigo, livro.estoque, livro.descricao);
    printf("%s\n", disponibilidadeTitulo(livro.estoque) ? "Livro disponivel." : "Livro temporariamente indisponivel.");
}

void mostrarUsuario(Usuario usuario) {
    printf("Usuario: %d | Dados: %s | Livro emprestado: %d\n", usuario.codigo, usuario.dados, usuario.codigoLivroEmprestado);
}

void listarLivros(Livro livros[], int qtd) {
    if (qtd == 0) printf("Nenhum livro cadastrado.\n");
    for (int i = 0; i < qtd; i++) mostrarLivro(livros[i]);
    printf("Total de exemplares disponiveis: %lld\n", calcularQtdExemplares(livros, qtd));
}

void listarUsuarios(Usuario usuarios[], int qtdUsuarios) {
    if (qtdUsuarios == 0) printf("Nenhum usuario cadastrado.\n");
    for (int i = 0; i < qtdUsuarios; i++) mostrarUsuario(usuarios[i]);
}

long long calcularQtdExemplares(Livro livros[], int qtd) {
    long long qtdExemplares = 0;
    for (int i = 0; i < qtd; i++) qtdExemplares += livros[i].estoque;
    return qtdExemplares;
}

int disponibilidadeTitulo(int qtdEstoque) {
    return qtdEstoque > 0;
}

int lerTexto(const char mensagem[], char texto[]) {
    char linha[TAMANHO + 2];
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
    if (tamanho == 0 || tamanho >= TAMANHO) {
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
    char linha[TAMANHO];
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
