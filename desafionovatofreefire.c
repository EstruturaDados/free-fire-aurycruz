/*
 * ============================================================
 *  SISTEMA DE INVENTARIO - MOCHILA DE LOOT
 *  Jogo de Sobrevivencia - Nivel 1
 * ============================================================
 *  Bibliotecas:
 *  - stdio.h  : entrada e saida padrao (printf, scanf, fgets)
 *  - string.h : manipulacao de strings (strcmp, strcspn)
 *  - stdlib.h : funcoes utilitarias gerais
 * ============================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ============================================================
 *  DEFINICAO DA STRUCT Item
 *  Representa cada objeto coletado pelo jogador.
 *  Campos:
 *    - nome[30]   : nome do item (ex: "Espingarda")
 *    - tipo[20]   : categoria (ex: "arma", "cura")
 *    - quantidade : numero de unidades do item
 * ============================================================ */
typedef struct {
    char nome[30];
    char tipo[20];
    int  quantidade;
} Item;

/* ============================================================
 *  VARIAVEIS GLOBAIS
 *  mochila[]  : vetor que armazena ate 10 itens
 *  totalItens : contador de itens atualmente na mochila
 * ============================================================ */
#define CAPACIDADE_MAXIMA 10

Item mochila[CAPACIDADE_MAXIMA];
int  totalItens = 0;

/* Prototipos das funcoes */
void inserirItem();
void removerItem();
void listarItens();
void buscarItem();
void exibirMenu();

/* ============================================================
 *  FUNCAO: listarItens()
 *  Percorre o vetor com laco for e exibe cada item.
 * ============================================================ */
void listarItens() {
    printf("\n==============================================\n");
    printf("         ITENS NA MOCHILA                    \n");
    printf("==============================================\n");

    if (totalItens == 0) {
        printf("  Mochila vazia! Colete itens para comecar.\n");
        printf("==============================================\n");
        return;
    }

    for (int i = 0; i < totalItens; i++) {
        printf("  [%d] Nome      : %s\n", i + 1, mochila[i].nome);
        printf("      Tipo      : %s\n", mochila[i].tipo);
        printf("      Quantidade: %d\n", mochila[i].quantidade);
        printf("----------------------------------------------\n");
    }

    printf("  Total: %d / %d itens\n", totalItens, CAPACIDADE_MAXIMA);
    printf("==============================================\n");
}

/* ============================================================
 *  FUNCAO: inserirItem()
 *  Cadastra um novo item na mochila.
 *  Verifica se a capacidade maxima foi atingida.
 * ============================================================ */
void inserirItem() {
    printf("\n--- CADASTRAR NOVO ITEM ---\n");

    if (totalItens >= CAPACIDADE_MAXIMA) {
        printf("[!] Mochila cheia! Remova um item antes de adicionar.\n");
        return;
    }

    Item novoItem;

    printf("Nome do item                              : ");
    scanf(" ");
    fgets(novoItem.nome, sizeof(novoItem.nome), stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0'; /* Remove o '\n' do fgets */

    printf("Tipo (arma / municao / cura / ferramenta) : ");
    fgets(novoItem.tipo, sizeof(novoItem.tipo), stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';

    printf("Quantidade                                : ");
    scanf("%d", &novoItem.quantidade);

    /* Insere o item no vetor e incrementa o contador */
    mochila[totalItens] = novoItem;
    totalItens++;

    printf("[OK] Item '%s' adicionado a mochila!\n", novoItem.nome);
    listarItens(); /* Lista apos cada operacao */
}

/* ============================================================
 *  FUNCAO: removerItem()
 *  Busca sequencial pelo nome e remove o item do vetor.
 *  Reorganiza o vetor deslocando os elementos restantes.
 * ============================================================ */
void removerItem() {
    printf("\n--- REMOVER ITEM ---\n");

    if (totalItens == 0) {
        printf("[!] A mochila ja esta vazia.\n");
        return;
    }

    char nomeRemover[30];
    printf("Nome do item a remover: ");
    scanf(" ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    int posicao = -1;

    /* Busca sequencial: percorre item por item */
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            posicao = i;
            break;
        }
    }

    if (posicao == -1) {
        printf("[X] Item '%s' nao encontrado na mochila.\n", nomeRemover);
        return;
    }

    /* Desloca os elementos apos a posicao removida */
    for (int i = posicao; i < totalItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }

    totalItens--;
    printf("[OK] Item '%s' removido da mochila!\n", nomeRemover);
    listarItens(); /* Lista apos cada operacao */
}

/* ============================================================
 *  FUNCAO: buscarItem()
 *  Busca sequencial: percorre o vetor do indice 0
 *  ate totalItens-1 e exibe os dados se encontrado.
 * ============================================================ */
void buscarItem() {
    printf("\n--- BUSCAR ITEM ---\n");

    if (totalItens == 0) {
        printf("[!] A mochila esta vazia. Nada para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item a buscar: ");
    scanf(" ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int encontrado = 0;

    /* Busca sequencial */
    for (int i = 0; i < totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\n[OK] Item encontrado na posicao [%d]:\n", i + 1);
            printf("     Nome      : %s\n", mochila[i].nome);
            printf("     Tipo      : %s\n", mochila[i].tipo);
            printf("     Quantidade: %d\n", mochila[i].quantidade);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("[X] Item '%s' nao encontrado na mochila.\n", nomeBusca);
    }
}

/* ============================================================
 *  FUNCAO: exibirMenu()
 *  Exibe o menu principal com as opcoes disponiveis.
 * ============================================================ */
void exibirMenu() {
    printf("\n==============================================\n");
    printf("     MOCHILA DE LOOT - MENU PRINCIPAL        \n");
    printf("==============================================\n");
    printf("  1. Cadastrar item                          \n");
    printf("  2. Remover item                            \n");
    printf("  3. Listar itens                            \n");
    printf("  4. Buscar item                             \n");
    printf("  0. Sair do jogo                            \n");
    printf("==============================================\n");
    printf("Escolha uma opcao: ");
}

/* ============================================================
 *  FUNCAO PRINCIPAL: main()
 *  Controla o fluxo do programa com laco do-while.
 *  Chama as funcoes conforme a escolha do jogador.
 * ============================================================ */
int main() {
    int opcao;

    printf("\nBem-vindo ao sistema de inventario!\n");
    printf("Organize sua mochila e sobreviva!\n");

    /* Laco principal - executa ate o jogador escolher sair (opcao 0) */
    do {
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: inserirItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: buscarItem();  break;
            case 0: printf("\nSaindo do jogo... Ate a proxima!\n\n"); break;
            default: printf("[!] Opcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}