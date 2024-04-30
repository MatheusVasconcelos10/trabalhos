#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura do produto
typedef struct {
    int codigo;
    char numero_serie[51];
    int dia_validade;
    int mes_validade;
    int ano_validade;
} Produto;

// Definição da estrutura de nó do heap
typedef struct {
    Produto produto;
    int prioridade; // Prioridade é a data de validade
} HeapNode;

// Definição da estrutura do heap
typedef struct {
    HeapNode *itens;
    int capacidade;
    int tamanho;
} MinHeap;

// Protótipos de funções
MinHeap *criarMinHeap(int capacidade);
void inserir(MinHeap *heap, Produto produto, int prioridade);
Produto removerMin(MinHeap *heap);
void exibirProduto(Produto produto);
void listarProdutos(MinHeap *heap);
void liberarHeap(MinHeap *heap);
void alterarProduto(MinHeap *heap, char *numero_serie);
void excluir(MinHeap *heap, int codigo);
void consultar(MinHeap *heap, int codigo);

int main() {
    // Inicialização do heap
    MinHeap *heap = criarMinHeap(100);

    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1 - Incluir\n");
        printf("2 - Alterar Produto\n");
        printf("3 - Excluir Produto\n");
        printf("4 - Consultar\n");
        printf("5 - Listar\n");
        printf("9 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch(opcao) {
            case 1: // Incluir
                printf("\nIncluir Produto:\n");
                Produto novoProduto;
                printf("Codigo do produto: ");
                scanf("%d", &novoProduto.codigo);
                printf("Numero de serie: ");
                scanf("%s", novoProduto.numero_serie);
                printf("Data de validade:\n");
                printf("Dia: ");
                scanf("%d", &novoProduto.dia_validade);
                printf("Mes: ");
                scanf("%d", &novoProduto.mes_validade);
                printf("Ano: ");
                scanf("%d", &novoProduto.ano_validade);
                // Supondo que a prioridade seja o ano, mês e dia separadamente
                inserir(heap, novoProduto, novoProduto.ano_validade * 10000 + novoProduto.mes_validade * 100 + novoProduto.dia_validade);
                break;
            case 2: // Alterar Produto
                printf("\nAlterar Produto:\n");
                char numero_serie_atual[51];
                printf("Informe o numero de serie do produto: ");
                scanf("%s", numero_serie_atual);
                alterarProduto(heap, numero_serie_atual);
                break;
            case 3: // Excluir Produto
                printf("\nExcluir Produto:\n");
                int codigo_exclusao;
                printf("Informe o codigo do produto: ");
                scanf("%d", &codigo_exclusao);
                excluir(heap, codigo_exclusao);
                break;
            case 4: // Consultar
                printf("\nConsultar Produto:\n");
                int codigo_consulta;
                printf("Informe o codigo do produto: ");
                scanf("%d", &codigo_consulta);
                consultar(heap, codigo_consulta);
                break;
            case 5: // Listar
                printf("\nListagem de Produtos:\n");
                listarProdutos(heap);
                break;
            case 9: // Sair
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpcao invalida. Por favor, escolha uma opcao valida.\n");
        }
    } while(opcao != 9);

    // Liberar memória alocada para o heap
    liberarHeap(heap);

    return 0;
}

// Cria um heap mínimo com capacidade especificada
MinHeap *criarMinHeap(int capacidade) {
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->itens = (HeapNode *)malloc(capacidade * sizeof(HeapNode));
    heap->capacidade = capacidade;
    heap->tamanho = 0;
    return heap;
}

// Insere um produto no heap com uma determinada prioridade
void inserir(MinHeap *heap, Produto produto, int prioridade) {
    if (heap->tamanho == heap->capacidade) {
        printf("Heap cheio. Impossivel inserir.\n");
        return;
    }

    // Incrementa o tamanho do heap
    int indice = heap->tamanho++;

    // Move o item para cima enquanto a prioridade for menor que a do pai
    while (indice > 0 && prioridade < heap->itens[(indice - 1) / 2].prioridade) {
        heap->itens[indice] = heap->itens[(indice - 1) / 2];
        indice = (indice - 1) / 2;
    }

    // Insere o novo item na posição correta
    heap->itens[indice].produto = produto;
    heap->itens[indice].prioridade = prioridade;
}

// Remove e retorna o produto com a menor prioridade (data de validade mais próxima)
Produto removerMin(MinHeap *heap) {
    if (heap->tamanho <= 0) {
        printf("Heap vazio. Impossivel remover.\n");
        Produto produtoVazio;
        return produtoVazio;
    }

    // Armazena o produto com a menor prioridade
    Produto produtoRemovido = heap->itens[0].produto;

    // Substitui o primeiro item pelo último
    heap->itens[0] = heap->itens[--heap->tamanho];

    // Reorganiza o heap para manter a propriedade do heap mínimo
    int indice = 0;
    while (1) {
        int filhoEsquerdo = 2 * indice + 1;
        int filhoDireito = 2 * indice + 2;
        int menorFilho = filhoEsquerdo;

        if (filhoEsquerdo >= heap->tamanho)
            break;

        if (filhoDireito < heap->tamanho && heap->itens[filhoDireito].prioridade < heap->itens[filhoEsquerdo].prioridade)
            menorFilho = filhoDireito;

        if (heap->itens[indice].prioridade <= heap->itens[menorFilho].prioridade)
            break;

        HeapNode temp = heap->itens[indice];
        heap->itens[indice] = heap->itens[menorFilho];
        heap->itens[menorFilho] = temp;

        indice = menorFilho;
    }

    return produtoRemovido;
}

// Exibe os dados de um produto
void exibirProduto(Produto produto) {
    printf("Codigo: %d | Numero de Serie: %s | Data de Validade: %02d/%02d/%04d\n", produto.codigo, produto.numero_serie, produto.dia_validade, produto.mes_validade, produto.ano_validade);
}

// Lista todos os produtos do heap em ordem crescente de código
void listarProdutos(MinHeap *heap) {
    if (heap->tamanho == 0) {
        printf("Nenhum produto cadastrado.\n");
        return;
    }

    printf("Produtos cadastrados:\n");
    // Ordena os produtos por código
    for (int i = 0; i < heap->tamanho - 1; i++) {
        for (int j = 0; j < heap->tamanho - i - 1; j++) {
            if (heap->itens[j].produto.codigo > heap->itens[j + 1].produto.codigo) {
                HeapNode temp = heap->itens[j];
                heap->itens[j] = heap->itens[j + 1];
                heap->itens[j + 1] = temp;
            }
        }
    }
    // Exibe os produtos ordenados
    for (int i = 0; i < heap->tamanho; i++) {
        printf("%d. ", i + 1);
        exibirProduto(heap->itens[i].produto);
    }
}

// Libera a memória alocada para o heap
void liberarHeap(MinHeap *heap) {
    free(heap->itens);
    free(heap);
}

// Altera os dados de um produto com base no número de série fornecido
void alterarProduto(MinHeap *heap, char *numero_serie) {
    int encontrados = 0;

    // Percorre o heap em busca de produtos com o número de série fornecido
    for (int i = 0; i < heap->tamanho; i++) {
        if (strcmp(heap->itens[i].produto.numero_serie, numero_serie) == 0) {
            encontrados++;
            printf("\nDados atuais do produto:\n");
            exibirProduto(heap->itens[i].produto);
            printf("\nNovos dados do produto:\n");
            printf("Codigo do produto: ");
            scanf("%d", &heap->itens[i].produto.codigo);
            printf("Numero de serie: ");
            scanf("%s", heap->itens[i].produto.numero_serie);
            printf("Data de validade:\n");
            printf("Dia: ");
            scanf("%d", &heap->itens[i].produto.dia_validade);
            printf("Mes: ");
            scanf("%d", &heap->itens[i].produto.mes_validade);
            printf("Ano: ");
            scanf("%d", &heap->itens[i].produto.ano_validade);
            // Supondo que a prioridade seja o ano, mês e dia separadamente
            heap->itens[i].prioridade = heap->itens[i].produto.ano_validade * 10000 + heap->itens[i].produto.mes_validade * 100 + heap->itens[i].produto.dia_validade;
            printf("Produto alterado com sucesso.\n");
        }
    }

    if (encontrados == 0) {
        printf("Nenhum produto encontrado com o numero de serie fornecido.\n");
    }
}

// Exclui um produto do heap com base no código fornecido
void excluir(MinHeap *heap, int codigo) {
    int menorPrioridade = 99999999; // Um valor alto para garantir que seja inicializado corretamente
    int indiceMenorPrioridade = -1;

    // Procura o produto com o menor prioridade (data de validade mais próxima) para o código fornecido
    for (int i = 0; i < heap->tamanho; i++) {
        if (heap->itens[i].produto.codigo == codigo && heap->itens[i].prioridade < menorPrioridade) {
            menorPrioridade = heap->itens[i].prioridade;
            indiceMenorPrioridade = i;
        }
    }

    if (indiceMenorPrioridade != -1) {
        // Remove o produto do heap
        for (int i = indiceMenorPrioridade; i < heap->tamanho - 1; i++) {
            heap->itens[i] = heap->itens[i + 1];
        }
        heap->tamanho--;
        printf("Produto excluido com sucesso.\n");
    } else {
        printf("Nenhum produto encontrado com o codigo fornecido.\n");
    }
}

// Consulta produtos com um determinado código e os exibe em ordem de data de validade
void consultar(MinHeap *heap, int codigo) {
    int encontrados = 0;

    // Percorre o heap em busca de produtos com o código fornecido
    for (int i = 0; i < heap->tamanho; i++) {
        if (heap->itens[i].produto.codigo == codigo) {
            encontrados++;
            printf("%d. ", encontrados);
            exibirProduto(heap->itens[i].produto);
        }
    }

    if (encontrados == 0) {
        printf("Nenhum produto encontrado com o codigo fornecido.\n");
        return;
    }

    if (encontrados > 1) {
        printf("Produtos ordenados por data de validade:\n");
        // Ordena os produtos encontrados com base na data de validade
        for (int i = 0; i < heap->tamanho - 1; i++) {
            for (int j = 0; j < heap->tamanho - i - 1; j++) {
                if (heap->itens[j].produto.ano_validade > heap->itens[j + 1].produto.ano_validade ||
                    (heap->itens[j].produto.ano_validade == heap->itens[j + 1].produto.ano_validade &&
                    heap->itens[j].produto.mes_validade > heap->itens[j + 1].produto.mes_validade) ||
                    (heap->itens[j].produto.ano_validade == heap->itens[j + 1].produto.ano_validade &&
                    heap->itens[j].produto.mes_validade == heap->itens[j + 1].produto.mes_validade &&
                    heap->itens[j].produto.dia_validade > heap->itens[j + 1].produto.dia_validade)) {
                    HeapNode temp = heap->itens[j];
                    heap->itens[j] = heap->itens[j + 1];
                    heap->itens[j + 1] = temp;
                }
            }
        }
        // Exibe os produtos ordenados
        for (int i = 0; i < heap->tamanho; i++) {
            if (heap->itens[i].produto.codigo == codigo) {
                printf("%d. ", i + 1);
                exibirProduto(heap->itens[i].produto);
            }
        }
    }
}
