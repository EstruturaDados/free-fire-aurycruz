/*
 * ============================================================
 *  TORRE DE FUGA - NIVEL FINAL (MESTRE)
 *  Sistema de Priorizacao e Montagem de Componentes
 * ============================================================
 *  Bibliotecas:
 *  - stdio.h  : entrada e saida padrao
 *  - stdlib.h : funcoes utilitarias
 *  - string.h : manipulacao de strings
 *  - time.h   : medicao de tempo de execucao
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* ============================================================
 *  CONSTANTE: capacidade maxima de componentes
 * ============================================================ */
#define MAX_COMPONENTES 20

/* ============================================================
 *  STRUCT: Componente
 *  Representa cada peca necessaria para montar a torre.
 *  Campos:
 *    - nome[30]    : nome do componente (ex: "chip central")
 *    - tipo[20]    : categoria (ex: "controle", "propulsao")
 *    - prioridade  : nivel de urgencia de 1 (baixa) a 10 (alta)
 * ============================================================ */
typedef struct {
    char nome[30];
    char tipo[20];
    int  prioridade;
} Componente;

/* ============================================================
 *  VARIAVEIS GLOBAIS
 *  torre[]       : vetor principal de componentes
 *  totalComp     : quantidade atual de componentes cadastrados
 *  contComp      : contador de comparacoes da ultima operacao
 *  ordeadoPorNome: flag que indica se o vetor esta ordenado
 *                  por nome (necessario para busca binaria)
 * ============================================================ */
Componente torre[MAX_COMPONENTES];
int totalComp      = 0;
int contComp       = 0;
int ordenadoPorNome = 0;

/* ============================================================
 *  FUNCAO: mostrarComponentes()
 *  Exibe o vetor formatado com nome, tipo e prioridade.
 *  Chamada apos cada operacao para mostrar o estado atual.
 * ============================================================ */
void mostrarComponentes(Componente v[], int n) {
    printf("\n==============================================\n");
    printf("     COMPONENTES DA TORRE DE FUGA            \n");
    printf("==============================================\n");

    if (n == 0) {
        printf("  Nenhum componente cadastrado ainda.\n");
        printf("==============================================\n");
        return;
    }

    printf("  %-4s %-22s %-14s %s\n", "Pos", "Nome", "Tipo", "Prioridade");
    printf("  ------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("  [%02d] %-22s %-14s %d\n",
               i + 1, v[i].nome, v[i].tipo, v[i].prioridade);
    }

    printf("==============================================\n");
    printf("  Total de componentes: %d / %d\n", n, MAX_COMPONENTES);
    printf("==============================================\n");
}

/* ============================================================
 *  FUNCAO: cadastrarComponente()
 *  Permite ao jogador inserir um novo componente na torre.
 *  Valida se a capacidade maxima foi atingida.
 * ============================================================ */
void cadastrarComponente() {
    printf("\n--- CADASTRAR COMPONENTE ---\n");

    if (totalComp >= MAX_COMPONENTES) {
        printf("[!] Capacidade maxima atingida (%d componentes).\n", MAX_COMPONENTES);
        return;
    }

    Componente novo;

    printf("Nome do componente (ex: chip central)     : ");
    scanf(" ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Tipo (controle / suporte / propulsao ...) : ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    /* Valida que a prioridade esteja entre 1 e 10 */
    do {
        printf("Prioridade (1 = baixa ... 10 = alta)      : ");
        scanf("%d", &novo.prioridade);
        if (novo.prioridade < 1 || novo.prioridade > 10)
            printf("[!] Prioridade invalida! Digite um valor entre 1 e 10.\n");
    } while (novo.prioridade < 1 || novo.prioridade > 10);

    torre[totalComp] = novo;
    totalComp++;
    ordenadoPorNome = 0; /* Nova insercao invalida a ordenacao por nome */

    printf("[OK] Componente '%s' adicionado com sucesso!\n", novo.nome);
    mostrarComponentes(torre, totalComp);
}

/* ==========================================================
 *
 *  SECAO 1 - ALGORITMOS DE ORDENACAO
 *
 * ========================================================== */

/* ============================================================
 *  FUNCAO: bubbleSortNome()
 *  Ordena os componentes por NOME usando Bubble Sort.
 *  Logica: compara pares adjacentes e os troca se estiverem
 *  fora de ordem, repetindo ate que nenhuma troca ocorra.
 *  Conta cada comparacao realizada.
 * ============================================================ */
void bubbleSortNome(Componente v[], int n) {
    contComp = 0;
    Componente temp;
    int trocou;

    for (int i = 0; i < n - 1; i++) {
        trocou = 0;
        for (int j = 0; j < n - 1 - i; j++) {
            contComp++; /* Conta a comparacao */
            if (strcmp(v[j].nome, v[j + 1].nome) > 0) {
                /* Troca os componentes de posicao */
                temp      = v[j];
                v[j]      = v[j + 1];
                v[j + 1]  = temp;
                trocou    = 1;
            }
        }
        /* Otimizacao: se nenhuma troca ocorreu, ja esta ordenado */
        if (!trocou) break;
    }
}

/* ============================================================
 *  FUNCAO: insertionSortTipo()
 *  Ordena os componentes por TIPO usando Insertion Sort.
 *  Logica: percorre o vetor e insere cada elemento na posicao
 *  correta dentro da parte ja ordenada (como ordenar cartas).
 *  Conta cada comparacao realizada.
 * ============================================================ */
void insertionSortTipo(Componente v[], int n) {
    contComp = 0;

    for (int i = 1; i < n; i++) {
        Componente chave = v[i]; /* Elemento a ser inserido */
        int j = i - 1;

        /* Desloca os elementos maiores que a chave para a direita */
        while (j >= 0) {
            contComp++; /* Conta a comparacao */
            if (strcmp(v[j].tipo, chave.tipo) > 0) {
                v[j + 1] = v[j];
                j--;
            } else {
                break;
            }
        }
        v[j + 1] = chave; /* Insere a chave na posicao correta */
    }
}

/* ============================================================
 *  FUNCAO: selectionSortPrioridade()
 *  Ordena os componentes por PRIORIDADE usando Selection Sort.
 *  Logica: a cada passagem, encontra o elemento de maior
 *  prioridade e o coloca na posicao correta.
 *  Conta cada comparacao realizada.
 * ============================================================ */
void selectionSortPrioridade(Componente v[], int n) {
    contComp = 0;
    Componente temp;

    for (int i = 0; i < n - 1; i++) {
        int indiceMaior = i; /* Assume que o maior esta na posicao i */

        /* Encontra o elemento de maior prioridade no restante */
        for (int j = i + 1; j < n; j++) {
            contComp++; /* Conta a comparacao */
            if (v[j].prioridade > v[indiceMaior].prioridade) {
                indiceMaior = j;
            }
        }

        /* Troca o maior encontrado com o da posicao i */
        if (indiceMaior != i) {
            temp            = v[i];
            v[i]            = v[indiceMaior];
            v[indiceMaior]  = temp;
        }
    }
}

/* ============================================================
 *  FUNCAO: medirTempo()
 *  Mede o tempo de execucao de um algoritmo de ordenacao
 *  usando clock() da biblioteca time.h.
 *  Exibe o tempo em milissegundos e o numero de comparacoes.
 * ============================================================ */
void medirTempo(void (*algoritmo)(Componente[], int),
                Componente v[], int n,
                const char* nomeAlgoritmo) {

    clock_t inicio = clock(); /* Marca o inicio */
    algoritmo(v, n);          /* Executa o algoritmo */
    clock_t fim    = clock(); /* Marca o fim */

    double tempoMs = ((double)(fim - inicio) / CLOCKS_PER_SEC) * 1000.0;

    printf("\n----------------------------------------------\n");
    printf("  Algoritmo  : %s\n", nomeAlgoritmo);
    printf("  Comparacoes: %d\n", contComp);
    printf("  Tempo       : %.4f ms\n", tempoMs);
    printf("----------------------------------------------\n");
}

/* ==========================================================
 *
 *  SECAO 2 - BUSCA BINARIA
 *
 * ========================================================== */

/* ============================================================
 *  FUNCAO: buscaBinariaPorNome()
 *  Realiza busca binaria no vetor ordenado por nome.
 *  Divide o espaco de busca ao meio a cada iteracao.
 *  Retorna o indice do componente ou -1 se nao encontrado.
 *  Exibe o numero de comparacoes ao final.
 * ============================================================ */
int buscaBinariaPorNome(Componente v[], int n, char nomeBusca[]) {
    int inicio = 0;
    int fim    = n - 1;
    int compBin = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        compBin++;

        int cmp = strcmp(v[meio].nome, nomeBusca);

        if (cmp == 0) {
            /* Componente-chave encontrado! */
            printf("\n[OK] COMPONENTE-CHAVE ENCONTRADO!\n");
            printf("     Posicao   : [%d]\n", meio + 1);
            printf("     Nome      : %s\n", v[meio].nome);
            printf("     Tipo      : %s\n", v[meio].tipo);
            printf("     Prioridade: %d\n", v[meio].prioridade);
            printf(">>> Comparacoes (busca binaria): %d\n", compBin);
            return meio;
        } else if (cmp < 0) {
            inicio = meio + 1; /* Busca na metade direita */
        } else {
            fim = meio - 1;    /* Busca na metade esquerda */
        }
    }

    printf("[X] Componente '%s' nao encontrado.\n", nomeBusca);
    printf(">>> Comparacoes (busca binaria): %d\n", compBin);
    return -1;
}

/* ==========================================================
 *
 *  SECAO 3 - MONTAGEM FINAL DA TORRE
 *
 * ========================================================== */

/* ============================================================
 *  FUNCAO: montagemFinal()
 *  Exibe o plano completo de montagem da torre.
 *  Solicita o componente-chave e confirma sua presenca.
 *  So pode ser executada apos ordenacao por nome.
 * ============================================================ */
void montagemFinal() {
    printf("\n==============================================\n");
    printf("       MONTAGEM FINAL DA TORRE DE FUGA       \n");
    printf("==============================================\n");

    if (totalComp == 0) {
        printf("[!] Nenhum componente cadastrado!\n");
        return;
    }

    if (!ordenadoPorNome) {
        printf("[!] Ordene os componentes por NOME antes\n");
        printf("    de iniciar a montagem (opcao 2 do menu).\n");
        return;
    }

    printf("\n  Plano de montagem (ordem alfabetica):\n");
    mostrarComponentes(torre, totalComp);

    /* Solicita o componente-chave para ativar a torre */
    char chave[30];
    printf("\nInforme o COMPONENTE-CHAVE para ativar a torre: ");
    scanf(" ");
    fgets(chave, sizeof(chave), stdin);
    chave[strcspn(chave, "\n")] = '\0';

    int resultado = buscaBinariaPorNome(torre, totalComp, chave);

    if (resultado != -1) {
        printf("\n");
        printf("  *** TORRE ATIVADA! FUGA INICIADA! ***\n");
        printf("  O componente-chave foi localizado e\n");
        printf("  encaixado com sucesso na torre!\n");
    } else {
        printf("\n");
        printf("  [X] FALHA NA ATIVACAO!\n");
        printf("  Componente-chave ausente. Verifique\n");
        printf("  os componentes cadastrados.\n");
    }

    printf("==============================================\n");
}

/* ==========================================================
 *
 *  SECAO 4 - MENU PRINCIPAL
 *
 * ========================================================== */

/* ============================================================
 *  FUNCAO: menuOrdenacao()
 *  Submenu para escolher o algoritmo de ordenacao.
 *  Cada opcao executa o algoritmo e mede seu desempenho.
 * ============================================================ */
void menuOrdenacao() {
    if (totalComp == 0) {
        printf("[!] Cadastre componentes antes de ordenar.\n");
        return;
    }

    int op;
    printf("\n==============================================\n");
    printf("         ESCOLHA O ALGORITMO                 \n");
    printf("==============================================\n");
    printf("  1. Bubble Sort    (ordenar por NOME)       \n");
    printf("  2. Insertion Sort (ordenar por TIPO)       \n");
    printf("  3. Selection Sort (ordenar por PRIORIDADE) \n");
    printf("  0. Voltar                                  \n");
    printf("==============================================\n");
    printf("Opcao: ");
    scanf("%d", &op);

    switch (op) {
        case 1:
            medirTempo(bubbleSortNome, torre, totalComp,
                       "Bubble Sort (por nome)");
            ordenadoPorNome = 1; /* Marca como ordenado por nome */
            mostrarComponentes(torre, totalComp);
            break;
        case 2:
            medirTempo(insertionSortTipo, torre, totalComp,
                       "Insertion Sort (por tipo)");
            ordenadoPorNome = 0;
            mostrarComponentes(torre, totalComp);
            break;
        case 3:
            medirTempo(selectionSortPrioridade, torre, totalComp,
                       "Selection Sort (por prioridade - maior primeiro)");
            ordenadoPorNome = 0;
            mostrarComponentes(torre, totalComp);
            break;
        case 0:
            break;
        default:
            printf("[!] Opcao invalida!\n");
    }
}

/* ============================================================
 *  FUNCAO: menuBuscaBinaria()
 *  Executa a busca binaria por nome.
 *  Exige que o vetor esteja ordenado por nome.
 * ============================================================ */
void menuBuscaBinaria() {
    printf("\n--- BUSCA BINARIA POR NOME ---\n");

    if (totalComp == 0) {
        printf("[!] Nenhum componente cadastrado.\n");
        return;
    }

    if (!ordenadoPorNome) {
        printf("[!] Use o Bubble Sort (opcao 2) para ordenar\n");
        printf("    por nome antes de realizar a busca binaria.\n");
        return;
    }

    char nomeBusca[30];
    printf("Nome do componente a buscar: ");
    scanf(" ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    buscaBinariaPorNome(torre, totalComp, nomeBusca);
}

/* ============================================================
 *  FUNCAO PRINCIPAL: main()
 *  Menu principal do sistema de montagem da torre.
 * ============================================================ */
int main() {
    int opcao;

    printf("\n==============================================\n");
    printf("   TORRE DE FUGA - NIVEL MESTRE              \n");
    printf("   A safe zone esta se fechando...           \n");
    printf("   Monte a torre e escape!                   \n");
    printf("==============================================\n");

    do {
        printf("\n==============================================\n");
        printf("           MENU PRINCIPAL                    \n");
        printf("==============================================\n");
        printf("  1. Cadastrar componente                    \n");
        printf("  2. Ordenar componentes                     \n");
        printf("  3. Busca binaria por nome                  \n");
        printf("  4. Mostrar todos os componentes            \n");
        printf("  5. MONTAGEM FINAL DA TORRE                 \n");
        printf("  0. Sair                                    \n");
        printf("==============================================\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarComponente(); break;
            case 2: menuOrdenacao();       break;
            case 3: menuBuscaBinaria();    break;
            case 4: mostrarComponentes(torre, totalComp); break;
            case 5: montagemFinal();       break;
            case 0: printf("\nSaindo... Boa sorte na fuga!\n\n"); break;
            default: printf("[!] Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}