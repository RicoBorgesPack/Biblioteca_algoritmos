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

typedef struct {
    int codigo;
    char descricao[TAM_TEXTO];
    int estoque;
} Livro;

typedef struct {
    int codigo;
    char dados[TAM_TEXTO];
    int codigoLivroEmprestado;
} Usuario;

int cadastrarLivro(Livro livros[], int qtdLivros);
int cadastrarUsuario(Usuario usuarios[], int qtdUsuarios);
void listarLivros(Livro livros[], int qtdLivros);
void listarUsuarios(Usuario usuarios[], int qtdUsuarios);
int buscarLivro(Livro livros[], int qtd, int codigo);
int buscarUsuario(Usuario usuarios[], int qtd, int codigo);
long long calcularQtdExemplares(Livro livros[], int qtdLivros);
int disponibilidadeTitulo(int qtdEstoque);
void mostrarLivro(Livro livro);
void mostrarUsuario(Usuario usuario);

void alterarEstoque(Livro *livro, int valor);
void registrarEmprestimo(Usuario *usuario, int codigoLivro);
void encerrarEmprestimo(Usuario *usuario);
int emprestarLivro(Livro *livro, Usuario *usuario);
int devolverLivro(Livro *livro, Usuario *usuario);

int main(void) {
    Livro livros[MAX_LIVROS] = {{0}};
    Usuario usuarios[MAX_USUARIOS] = {{0}};
    int qtdLivros = 0, qtdUsuarios = 0;
    int opcao, codigo, indice;
    int codigoLivro, codigoUsuario, indiceLivro, indiceUsuario, sucesso;
    printf("Sistema de Biblioteca - Versao 6\n");
    while (1) {
        printf("\n1 - Cadastrar livro\n2 - Cadastrar usuario\n3 - Listar livros\n4 - Buscar livro\n");
        printf("5 - Emprestar livro\n6 - Devolver livro\n");
        printf("7 - Listar usuarios\n8 - Buscar usuario\n0 - Sair\n");
        if (!lerInteiro("Opcao: ", &opcao)) {
            if (feof(stdin)) break;
            continue;
        }
        if (opcao == 0) break;
        switch (opcao) {
            case 1:
                qtdLivros = cadastrarLivro(livros, qtdLivros);
                break;
            case 2:
                qtdUsuarios = cadastrarUsuario(usuarios, qtdUsuarios);
                break;
            case 3:
                listarLivros(livros, qtdLivros);
                break;
            case 4:
                if (!lerInteiro("Codigo do livro: ", &codigo)) break;
                indice = buscarLivro(livros, qtdLivros, codigo);
                if (indice == -1) printf("Livro nao encontrado.\n");
                else mostrarLivro(livros[indice]);
                break;
            case 5:
            case 6:
                if (!lerInteiro("Codigo do livro: ", &codigoLivro)) break;
                indiceLivro = buscarLivro(livros, qtdLivros, codigoLivro);
                if (indiceLivro == -1) {
                    printf("Livro nao encontrado.\n");
                    break;
                }
                if (!lerInteiro("Codigo do usuario: ", &codigoUsuario)) break;
                indiceUsuario = buscarUsuario(usuarios, qtdUsuarios, codigoUsuario);
                if (indiceUsuario == -1) {
                    printf("Usuario nao encontrado.\n");
                    break;
                }
                if (opcao == 5) sucesso = emprestarLivro(&livros[indiceLivro], &usuarios[indiceUsuario]);
                else sucesso = devolverLivro(&livros[indiceLivro], &usuarios[indiceUsuario]);
                if (sucesso) {
                    printf("%s\n", opcao == 5 ? "Emprestimo realizado." : "Devolucao realizada.");
                    mostrarLivro(livros[indiceLivro]);
                    mostrarUsuario(usuarios[indiceUsuario]);
                }
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

int cadastrarLivro(Livro livros[], int qtdLivros) {
    Livro novoLivro;
    if (qtdLivros >= MAX_LIVROS) {
        printf("Erro: limite de tres livros atingido.\n");
        return qtdLivros;
    }
    if (!lerInteiro("Codigo do livro: ", &novoLivro.codigo)) return qtdLivros;
    if (novoLivro.codigo <= 0 || buscarLivro(livros, qtdLivros, novoLivro.codigo) != -1) {
        printf("Erro: codigo de livro deve ser positivo e unico.\n");
        return qtdLivros;
    }
    if (!lerInteiro("Quantidade em estoque: ", &novoLivro.estoque)) return qtdLivros;
    if (novoLivro.estoque < 0) {
        printf("Erro: estoque nao pode ser negativo.\n");
        return qtdLivros;
    }
    if (!lerTexto("Titulo; autor; ano; tema: ", novoLivro.descricao)) return qtdLivros;
    livros[qtdLivros] = novoLivro;
    printf("Livro cadastrado com sucesso!\n");
    return qtdLivros + 1;
}

int cadastrarUsuario(Usuario usuarios[], int qtdUsuarios) {
    Usuario novoUsuario;
    if (qtdUsuarios >= MAX_USUARIOS) {
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

void listarLivros(Livro livros[], int qtdLivros) {
    if (qtdLivros == 0) printf("Nenhum livro cadastrado.\n");
    for (int i = 0; i < qtdLivros; i++) mostrarLivro(livros[i]);
    printf("Total de exemplares disponiveis: %lld\n", calcularQtdExemplares(livros, qtdLivros));
}

void listarUsuarios(Usuario usuarios[], int qtdUsuarios) {
    if (qtdUsuarios == 0) printf("Nenhum usuario cadastrado.\n");
    for (int i = 0; i < qtdUsuarios; i++) mostrarUsuario(usuarios[i]);
}

long long calcularQtdExemplares(Livro livros[], int qtdLivros) {
    long long qtdExemplares = 0;
    for (int i = 0; i < qtdLivros; i++) qtdExemplares += livros[i].estoque;
    return qtdExemplares;
}

int disponibilidadeTitulo(int qtdEstoque) {
    return qtdEstoque > 0;
}

void alterarEstoque(Livro *livro, int valor) {
    livro->estoque = valor;
}

void registrarEmprestimo(Usuario *usuario, int codigoLivro) {
    usuario->codigoLivroEmprestado = codigoLivro;
}

void encerrarEmprestimo(Usuario *usuario) {
    usuario->codigoLivroEmprestado = SEM_EMPRESTIMO;
}

int emprestarLivro(Livro *livro, Usuario *usuario) {
    if (livro == NULL || usuario == NULL) {
        printf("Erro: livro ou usuario nao encontrado.\n");
        return 0;
    }
    if (usuario->codigoLivroEmprestado != SEM_EMPRESTIMO) {
        printf("Erro: usuario ja possui um livro emprestado.\n");
        return 0;
    }
    if (!disponibilidadeTitulo(livro->estoque)) {
        printf("Erro: livro sem estoque disponivel.\n");
        return 0;
    }
    alterarEstoque(livro, livro->estoque - 1);
    registrarEmprestimo(usuario, livro->codigo);
    return 1;
}

int devolverLivro(Livro *livro, Usuario *usuario) {
    if (livro == NULL || usuario == NULL) {
        printf("Erro: livro ou usuario nao encontrado.\n");
        return 0;
    }
    if (usuario->codigoLivroEmprestado == SEM_EMPRESTIMO) {
        printf("Erro: usuario nao possui livro emprestado.\n");
        return 0;
    }
    if (usuario->codigoLivroEmprestado != livro->codigo) {
        printf("Erro: livro nao corresponde ao emprestimo do usuario.\n");
        return 0;
    }
    if (livro->estoque == INT_MAX) {
        printf("Erro: estoque atingiu o limite numerico.\n");
        return 0;
    }
    alterarEstoque(livro, livro->estoque + 1);
    encerrarEmprestimo(usuario);
    return 1;
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
