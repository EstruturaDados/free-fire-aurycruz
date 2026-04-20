/*
 * ============================================================
 *  SISTEMA DE INVENTARIO - MOCHILA DE LOOT - NIVEL 2
 *  Comparacao: Vetor (Lista Sequencial) x Lista Encadeada
 * ============================================================
 *  Bibliotecas:
 *  - stdio.h  : entrada e saida padrao
 *  - stdlib.h : alocacao dinamica de memoria (malloc, free)
 *  - string.h : manipulacao de strings
 *  - time.h   : medicao de tempo das operacoes
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================
 *  CONSTANTE: capacidade maxima do vetor
 * ============================================================ */
#define CAPACIDADE_MAXIMA 10

/* ============================================================
 *  STRUCT: Item
 *  Representa um objeto coletado pelo jogador.
 *  Campos:
 *    - nome[30]   : nome do item
 *    - tipo[20]   : categoria (arma, municao, cura, ferramenta)
 *    - quantidade : numero de unidades
 * ============================================================ */
typedef struct {
    char nome[30];
    char tipo[20];
    int  quantidade;
} Item;

/* ============================================================
 *  STRUCT: No
 *  Representa um no da lista encadeada.
 *  Campos:
 *    - dados    : o item armazenado neste no
 *    - proximo  : ponteiro para o proximo no da lista
 * ============================================================ */
typedef struct No {
    Item        dados;
    struct No*  proximo;
} No;

/* ============================================================
 *  VARIAVEIS GLOBAIS - VETOR
 * ============================================================ */
Item mochilаVetor[CAPACIDADE_MAXIMA];
int  totalVetor = 0;

/* ============================================================
 *  VARIAVEIS GLOBAIS - LISTA ENCADEADA
 * ============================================================ */
No* cabeca = NULL;  /* Ponteiro para o primeiro no da lista */
int totalLista = 0;

/* ============================================================
 *  CONTADORES DE COMPARACOES (busca sequencial e binaria)
 * ============================================================ */
int comparacoesSequencial = 0;
int comparacoesBinaria    = 0;

/* ==========================================================
 *
 *  SECAO 1 - OPERACOES COM VETOR
 *
 * ========================================================== */

/* ============================================================
 *  FUNCAO: listarItensVetor()
 *  Percorre o vetor com laco for e exibe cada item.
 * ============================================================ */
void listarItensVetor() {
    printf("\n==============================================\n");
    printf("     [VETOR] ITENS NA MOCHILA                \n");
    printf("==============================================\n");

    if (totalVetor == 0) {
        printf("  Mochila (vetor) vazia!\n");
        printf("==============================================\n");
        return;
    }

    for (int i = 0; i < totalVetor; i++) {
        printf("  [%d] Nome      : %s\n", i + 1, mochilаVetor[i].nome);
        printf("      Tipo      : %s\n", mochilаVetor[i].tipo);
        printf("      Quantidade: %d\n", mochilаVetor[i].quantidade);
        printf("----------------------------------------------\n");
    }
    printf("  Total: %d / %d itens\n", totalVetor, CAPACIDADE_MAXIMA);
    printf("==============================================\n");
}

/* ============================================================
 *  FUNCAO: inserirItemVetor()
 *  Cadastra um novo item no vetor.
 *  Verifica se a capacidade maxima foi atingida.
 * ============================================================ */
void inserirItemVetor() {
    printf("\n--- [VETOR] CADASTRAR ITEM ---\n");

    if (totalVetor >= CAPACIDADE_MAXIMA) {
        printf("[!] Vetor cheio! Remova um item antes.\n");
        return;
    }

    Item novo;

    printf("Nome      : ");
    scanf(" ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Tipo      : ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);

    mochilаVetor[totalVetor] = novo;
    totalVetor++;

    printf("[OK] '%s' inserido no vetor!\n", novo.nome);
    listarItensVetor();
}

/* ============================================================
 *  FUNCAO: removerItemVetor()
 *  Busca sequencial pelo nome e remove do vetor.
 *  Reorganiza os elementos apos a remocao.
 * ============================================================ */
void removerItemVetor() {
    printf("\n--- [VETOR] REMOVER ITEM ---\n");

    if (totalVetor == 0) {
        printf("[!] Vetor vazio.\n");
        return;
    }

    char nomeRem[30];
    printf("Nome do item a remover: ");
    scanf(" ");
    fgets(nomeRem, sizeof(nomeRem), stdin);
    nomeRem[strcspn(nomeRem, "\n")] = '\0';

    int pos = -1;
    for (int i = 0; i < totalVetor; i++) {
        if (strcmp(mochilаVetor[i].nome, nomeRem) == 0) {
            pos = i;
            break;
        }
    }

    if (pos == -1) {
        printf("[X] Item '%s' nao encontrado no vetor.\n", nomeRem);
        return;
    }

    /* Desloca os elementos para preencher o espaco removido */
    for (int i = pos; i < totalVetor - 1; i++) {
        mochilаVetor[i] = mochilаVetor[i + 1];
    }
    totalVetor--;

    printf("[OK] Item '%s' removido do vetor!\n", nomeRem);
    listarItensVetor();
}

/* ============================================================
 *  FUNCAO: ordenarVetor()
 *  Ordena os itens do vetor por nome usando Bubble Sort.
 *  O Bubble Sort compara pares adjacentes e os troca
 *  se estiverem fora de ordem, repetindo ate ordenar tudo.
 * ============================================================ */
void ordenarVetor() {
    printf("\n[VETOR] Ordenando itens por nome (Bubble Sort)...\n");

    Item temp;
    /* Laco externo: cada passagem garante que o maior vai ao fim */
    for (int i = 0; i < totalVetor - 1; i++) {
        /* Laco interno: compara pares adjacentes */
        for (int j = 0; j < totalVetor - 1 - i; j++) {
            if (strcmp(mochilаVetor[j].nome, mochilаVetor[j + 1].nome) > 0) {
                /* Troca os itens de posicao */
                temp              = mochilаVetor[j];
                mochilаVetor[j]   = mochilаVetor[j + 1];
                mochilаVetor[j+1] = temp;
            }
        }
    }

    printf("[OK] Vetor ordenado com sucesso!\n");
    listarItensVetor();
}

/* ============================================================
 *  FUNCAO: buscarSequencialVetor()
 *  Busca sequencial no vetor: percorre do indice 0 ate o fim.
 *  Conta e exibe o numero de comparacoes realizadas.
 * ============================================================ */
void buscarSequencialVetor() {
    printf("\n--- [VETOR] BUSCA SEQUENCIAL ---\n");

    if (totalVetor == 0) {
        printf("[!] Vetor vazio.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item a buscar: ");
    scanf(" ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    comparacoesSequencial = 0; /* Zera o contador antes de buscar */
    int encontrado = 0;

    for (int i = 0; i < totalVetor; i++) {
        comparacoesSequencial++; /* Incrementa a cada comparacao */
        if (strcmp(mochilаVetor[i].nome, nomeBusca) == 0) {
            printf("\n[OK] Item encontrado na posicao [%d]:\n", i + 1);
            printf("     Nome      : %s\n", mochilаVetor[i].nome);
            printf("     Tipo      : %s\n", mochilаVetor[i].tipo);
            printf("     Quantidade: %d\n", mochilаVetor[i].quantidade);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
        printf("[X] Item '%s' nao encontrado.\n", nomeBusca);

    printf(">>> Comparacoes realizadas (sequencial): %d\n", comparacoesSequencial);
}

/* ============================================================
 *  FUNCAO: buscarBinariaVetor()
 *  Busca binaria no vetor (exige vetor ordenado).
 *  Divide o espaco de busca ao meio a cada iteracao,
 *  reduzindo drasticamente o numero de comparacoes.
 *  Conta e exibe o numero de comparacoes realizadas.
 * ============================================================ */
void buscarBinariaVetor() {
    printf("\n--- [VETOR] BUSCA BINARIA ---\n");
    printf("[!] Certifique-se de que o vetor esta ordenado antes!\n");

    if (totalVetor == 0) {
        printf("[!] Vetor vazio.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item a buscar: ");
    scanf(" ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    comparacoesBinaria = 0; /* Zera o contador antes de buscar */
    int inicio = 0;
    int fim    = totalVetor - 1;
    int encontrado = 0;

    /* Laco da busca binaria: divide o intervalo ao meio */
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        comparacoesBinaria++; /* Conta cada comparacao */

        int cmp = strcmp(mochilаVetor[meio].nome, nomeBusca);

        if (cmp == 0) {
            /* Item encontrado exatamente no meio */
            printf("\n[OK] Item encontrado na posicao [%d]:\n", meio + 1);
            printf("     Nome      : %s\n", mochilаVetor[meio].nome);
            printf("     Tipo      : %s\n", mochilаVetor[meio].tipo);
            printf("     Quantidade: %d\n", mochilаVetor[meio].quantidade);
            encontrado = 1;
            break;
        } else if (cmp < 0) {
            /* Item esta na metade direita */
            inicio = meio + 1;
        } else {
            /* Item esta na metade esquerda */
            fim = meio - 1;
        }
    }

    if (!encontrado)
        printf("[X] Item '%s' nao encontrado.\n", nomeBusca);

    printf(">>> Comparacoes realizadas (binaria)   : %d\n", comparacoesBinaria);
}

/* ==========================================================
 *
 *  SECAO 2 - OPERACOES COM LISTA ENCADEADA
 *
 * ========================================================== */

/* ============================================================
 *  FUNCAO: listarItensLista()
 *  Percorre a lista encadeada usando ponteiro auxiliar
 *  e exibe cada no ate chegar ao final (NULL).
 * ============================================================ */
void listarItensLista() {
    printf("\n==============================================\n");
    printf("   [LISTA ENCADEADA] ITENS NA MOCHILA        \n");
    printf("==============================================\n");

    if (cabeca == NULL) {
        printf("  Lista encadeada vazia!\n");
        printf("==============================================\n");
        return;
    }

    No* atual = cabeca; /* Ponteiro auxiliar começa na cabeca */
    int pos   = 1;

    /* Percorre a lista ate o proximo ser NULL (fim da lista) */
    while (atual != NULL) {
        printf("  [%d] Nome      : %s\n", pos, atual->dados.nome);
        printf("      Tipo      : %s\n", atual->dados.tipo);
        printf("      Quantidade: %d\n", atual->dados.quantidade);
        printf("----------------------------------------------\n");
        atual = atual->proximo; /* Avanca para o proximo no */
        pos++;
    }

    printf("  Total: %d itens\n", totalLista);
    printf("==============================================\n");
}

/* ============================================================
 *  FUNCAO: inserirItemLista()
 *  Aloca dinamicamente um novo No com malloc e o insere
 *  no inicio da lista encadeada (insercao na cabeca).
 * ============================================================ */
void inserirItemLista() {
    printf("\n--- [LISTA] CADASTRAR ITEM ---\n");

    if (totalLista >= CAPACIDADE_MAXIMA) {
        printf("[!] Lista cheia! Remova um item antes.\n");
        return;
    }

    /* Aloca memoria dinamicamente para o novo no */
    No* novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("[ERRO] Falha ao alocar memoria!\n");
        return;
    }

    printf("Nome      : ");
    scanf(" ");
    fgets(novoNo->dados.nome, sizeof(novoNo->dados.nome), stdin);
    novoNo->dados.nome[strcspn(novoNo->dados.nome, "\n")] = '\0';

    printf("Tipo      : ");
    fgets(novoNo->dados.tipo, sizeof(novoNo->dados.tipo), stdin);
    novoNo->dados.tipo[strcspn(novoNo->dados.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novoNo->dados.quantidade);

    /* Insere no inicio: novo no aponta para a cabeca atual */
    novoNo->proximo = cabeca;
    cabeca = novoNo;
    totalLista++;

    printf("[OK] '%s' inserido na lista!\n", novoNo->dados.nome);
    listarItensLista();
}

/* ============================================================
 *  FUNCAO: removerItemLista()
 *  Percorre a lista procurando o item pelo nome.
 *  Ajusta os ponteiros para desconectar o no removido
 *  e libera a memoria com free().
 * ============================================================ */
void removerItemLista() {
    printf("\n--- [LISTA] REMOVER ITEM ---\n");

    if (cabeca == NULL) {
        printf("[!] Lista vazia.\n");
        return;
    }

    char nomeRem[30];
    printf("Nome do item a remover: ");
    scanf(" ");
    fgets(nomeRem, sizeof(nomeRem), stdin);
    nomeRem[strcspn(nomeRem, "\n")] = '\0';

    No* atual   = cabeca;
    No* anterior = NULL;

    /* Percorre a lista procurando o no a remover */
    while (atual != NULL && strcmp(atual->dados.nome, nomeRem) != 0) {
        anterior = atual;
        atual    = atual->proximo;
    }

    if (atual == NULL) {
        printf("[X] Item '%s' nao encontrado na lista.\n", nomeRem);
        return;
    }

    /* Ajusta os ponteiros para remover o no */
    if (anterior == NULL) {
        /* Remocao da cabeca: cabeca avanca para o proximo */
        cabeca = atual->proximo;
    } else {
        /* Remocao do meio/fim: anterior pula o no removido */
        anterior->proximo = atual->proximo;
    }

    free(atual); /* Libera a memoria do no removido */
    totalLista--;

    printf("[OK] Item '%s' removido da lista!\n", nomeRem);
    listarItensLista();
}

/* ============================================================
 *  FUNCAO: buscarSequencialLista()
 *  Busca sequencial na lista encadeada: percorre no a no
 *  do inicio ate o fim, contando as comparacoes.
 * ============================================================ */
void buscarSequencialLista() {
    printf("\n--- [LISTA] BUSCA SEQUENCIAL ---\n");

    if (cabeca == NULL) {
        printf("[!] Lista vazia.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do item a buscar: ");
    scanf(" ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    comparacoesSequencial = 0;
    No* atual = cabeca;
    int pos   = 1;
    int encontrado = 0;

    while (atual != NULL) {
        comparacoesSequencial++;
        if (strcmp(atual->dados.nome, nomeBusca) == 0) {
            printf("\n[OK] Item encontrado na posicao [%d]:\n", pos);
            printf("     Nome      : %s\n", atual->dados.nome);
            printf("     Tipo      : %s\n", atual->dados.tipo);
            printf("     Quantidade: %d\n", atual->dados.quantidade);
            encontrado = 1;
            break;
        }
        atual = atual->proximo;
        pos++;
    }

    if (!encontrado)
        printf("[X] Item '%s' nao encontrado.\n", nomeBusca);

    printf(">>> Comparacoes realizadas (sequencial lista): %d\n", comparacoesSequencial);
}

/* ==========================================================
 *
 *  SECAO 3 - COMPARATIVO DE DESEMPENHO
 *
 * ========================================================== */

/* ============================================================
 *  FUNCAO: exibirComparativo()
 *  Exibe lado a lado os contadores das ultimas buscas
 *  para facilitar a analise de desempenho.
 * ============================================================ */
void exibirComparativo() {
    printf("\n==============================================\n");
    printf("     COMPARATIVO DE DESEMPENHO               \n");
    printf("==============================================\n");
    printf("  Busca Sequencial (ultima):  %d comparacoes\n", comparacoesSequencial);
    printf("  Busca Binaria   (ultima):   %d comparacoes\n", comparacoesBinaria);
    printf("----------------------------------------------\n");
    printf("  Observacao:\n");
    printf("  A busca binaria divide o espaco pela metade\n");
    printf("  a cada passo (log2 N comparacoes).\n");
    printf("  A sequencial pode percorrer todos os nos.\n");
    printf("==============================================\n");
}

/* ==========================================================
 *
 *  SECAO 4 - MENUS
 *
 * ========================================================== */

/* ============================================================
 *  FUNCAO: menuVetor()
 *  Submenu com todas as operacoes do vetor.
 * ============================================================ */
void menuVetor() {
    int op;
    do {
        printf("\n==============================================\n");
        printf("          MENU - MOCHILA (VETOR)             \n");
        printf("==============================================\n");
        printf("  1. Inserir item                            \n");
        printf("  2. Remover item                            \n");
        printf("  3. Listar itens                            \n");
        printf("  4. Ordenar (Bubble Sort)                   \n");
        printf("  5. Busca Sequencial                        \n");
        printf("  6. Busca Binaria (ordene antes!)           \n");
        printf("  0. Voltar ao menu principal                \n");
        printf("==============================================\n");
        printf("Opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1: inserirItemVetor();       break;
            case 2: removerItemVetor();       break;
            case 3: listarItensVetor();       break;
            case 4: ordenarVetor();           break;
            case 5: buscarSequencialVetor();  break;
            case 6: buscarBinariaVetor();     break;
            case 0: break;
            default: printf("[!] Opcao invalida!\n");
        }
    } while (op != 0);
}

/* ============================================================
 *  FUNCAO: menuLista()
 *  Submenu com todas as operacoes da lista encadeada.
 * ============================================================ */
void menuLista() {
    int op;
    do {
        printf("\n==============================================\n");
        printf("       MENU - MOCHILA (LISTA ENCADEADA)      \n");
        printf("==============================================\n");
        printf("  1. Inserir item                            \n");
        printf("  2. Remover item                            \n");
        printf("  3. Listar itens                            \n");
        printf("  4. Busca Sequencial                        \n");
        printf("  0. Voltar ao menu principal                \n");
        printf("==============================================\n");
        printf("Opcao: ");
        scanf("%d", &op);

        switch (op) {
            case 1: inserirItemLista();       break;
            case 2: removerItemLista();       break;
            case 3: listarItensLista();       break;
            case 4: buscarSequencialLista();  break;
            case 0: break;
            default: printf("[!] Opcao invalida!\n");
        }
    } while (op != 0);
}

/* ============================================================
 *  FUNCAO PRINCIPAL: main()
 *  Menu principal que direciona para vetor ou lista.
 * ============================================================ */
int main() {
    int opcao;

    printf("\n==============================================\n");
    printf("   MOCHILA DE LOOT - NIVEL 2                 \n");
    printf("   Vetor vs Lista Encadeada                  \n");
    printf("==============================================\n");

    do {
        printf("\n==============================================\n");
        printf("           MENU PRINCIPAL                    \n");
        printf("==============================================\n");
        printf("  1. Mochila com VETOR                       \n");
        printf("  2. Mochila com LISTA ENCADEADA             \n");
        printf("  3. Ver comparativo de desempenho           \n");
        printf("  0. Sair                                    \n");
        printf("==============================================\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: menuVetor();         break;
            case 2: menuLista();         break;
            case 3: exibirComparativo(); break;
            case 0: printf("\nSaindo... Ate a proxima!\n\n"); break;
            default: printf("[!] Opcao invalida!\n");
        }
    } while (opcao != 0);

    /* Libera toda a memoria alocada pela lista encadeada */
    No* atual = cabeca;
    while (atual != NULL) {
        No* temp = atual;
        atual    = atual->proximo;
        free(temp);
    }

    return 0;
}