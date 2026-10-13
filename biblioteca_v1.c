#include <stdio.h>

#define MAX_LIVROS 3

int main() {
    int codigos[MAX_LIVROS] = {0};
    int estoques[MAX_LIVROS] = {0};
    int qtdLivros = 0;

    /* Cadastro dos livros */
    while (qtdLivros < MAX_LIVROS) {
        int novoCodigo;
        int repetido;

        /* Solicita código válido (positivo e não repetido) */
        do {
            printf("Digite o codigo do livro %d: ", qtdLivros + 1);
            scanf("%d", &novoCodigo);

            /* Assume inicialmente que não está repetido */
            repetido = 0;

            /* Percorre apenas as posições já preenchidas */
            for (int i = 0; i < qtdLivros; i++) {
                if (codigos[i] == novoCodigo) {
                    repetido = 1;
                    break;
                }
            }

            if (novoCodigo <= 0) {
                printf("Erro: o codigo deve ser um numero positivo.\n");
            } else if (repetido) {
                printf("Erro: codigo ja cadastrado. Informe outro codigo.\n");
            }
        } while (novoCodigo <= 0 || repetido);

        /* Armazena o código na próxima posição disponível */
        codigos[qtdLivros] = novoCodigo;

        /* Solicita estoque (não permite negativo; zero é permitido) */
        int estoque;
        do {
            printf("Digite a quantidade em estoque do codigo %d: ", novoCodigo);
            scanf("%d", &estoque);

            if (estoque < 0) {
                printf("Erro: a quantidade em estoque nao pode ser negativa.\n");
            }
        } while (estoque < 0);

        /* Armazena o estoque na mesma posição */
        estoques[qtdLivros] = estoque;

        /* Incrementa somente após cadastro correto */
        qtdLivros++;
        printf("Livro cadastrado com sucesso!\n\n");
    }

    /* Listagem do acervo e cálculo do estoque total */
    int qtdExemplares = 0;

    printf("===== ACERVO DA BIBLIOTECA =====\n");
    for (int i = 0; i < qtdLivros; i++) {
        printf("Codigo: %d | Estoque: %d\n", codigos[i], estoques[i]);
        qtdExemplares += estoques[i];
    }
    printf("================================\n");
    printf("Total de exemplares no acervo: %d\n\n", qtdExemplares);

    /* Busca de um livro */
    int codigoBusca;
    printf("Digite o codigo do livro que deseja buscar: ");
    scanf("%d", &codigoBusca);

    int posBusca = -1; /* -1 significa "não encontrado" */

    for (int i = 0; i < qtdLivros; i++) {
        if (codigos[i] == codigoBusca) {
            posBusca = i;
            break;
        }
    }

    /* Verificação da disponibilidade */
    if (posBusca == -1) {
        printf("Livro com codigo %d nao existe no acervo.\n", codigoBusca);
    } else {
        if (estoques[posBusca] == 0) {
            printf("Livro com codigo %d encontrado, mas esta temporariamente indisponivel.\n",
                   codigoBusca);
        } else {
            printf("Livro com codigo %d esta disponivel. Quantidade em estoque: %d\n",
                   codigoBusca, estoques[posBusca]);
        }
    }

    return 0;
}