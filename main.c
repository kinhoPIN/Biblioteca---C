#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------- Constantes ---------------- */
#define LIVROS_MAX 100
#define TITULO_MAX 50
#define CLIENTE_MAX 50
#define ID_MAX 50
#define DATA_MAX 20
#define CPF_MAX 12

/* ---------------- Estrutura ---------------- */
typedef struct {
    char titulo[TITULO_MAX];
    char nome_cliente[CLIENTE_MAX];
    char id_cliente[ID_MAX];
    char data_retirada[DATA_MAX];
    char data_devolucao[DATA_MAX];
    char cpf_cliente[CPF_MAX];
} LivroAlugado;

/* ---------------- Variáveis globais ---------------- */
LivroAlugado alugueis[LIVROS_MAX];
int total_alugueis = 0;

/* ---------------- Funções utilitárias ---------------- */
void clear_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void read_line(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

/* ---------------- Operações principais ---------------- */
void alugar_livro(void) {
    if (total_alugueis >= LIVROS_MAX) {
        printf("  Limite de alugueis atingido.\n");
        return;
    }

    printf("Título do livro: ");
    read_line(alugueis[total_alugueis].titulo, TITULO_MAX);

    printf("Nome do cliente: ");
    read_line(alugueis[total_alugueis].nome_cliente, CLIENTE_MAX);

    printf("ID do cliente: ");
    read_line(alugueis[total_alugueis].id_cliente, ID_MAX);

    printf("Data de retirada (DD/MM/AAAA): ");
    read_line(alugueis[total_alugueis].data_retirada, DATA_MAX);

    printf("Data prevista para devolução (DD/MM/AAAA): ");
    read_line(alugueis[total_alugueis].data_devolucao, DATA_MAX);

    printf("CPF do cliente (11 dígitos): ");
    read_line(alugueis[total_alugueis].cpf_cliente, CPF_MAX);

    if (strlen(alugueis[total_alugueis].cpf_cliente) != 11) {
        printf("  CPF inválido. Deve ter 11 dígitos.\n");
        return;
    }

    total_alugueis++;
    printf("✅ Livro alugado com sucesso!\n");
}

void salvar_dados(void) {
    FILE *file = fopen("alugueis.dat", "wb");
    if (file == NULL) {
        printf(" Erro ao salvar dados.\n");
        return;
    }

    fwrite(&total_alugueis, sizeof(int), 1, file);
    fwrite(alugueis, sizeof(LivroAlugado), (size_t)total_alugueis, file);
    fclose(file);

    printf(" Dados salvos com sucesso!\n");
}

void carregar_dados(void) {
    FILE *file = fopen("alugueis.dat", "rb");
    if (file == NULL) {
        printf("  Arquivo de dados não encontrado. Inicializando vazio.\n");
        return;
    }

    if (fread(&total_alugueis, sizeof(int), 1, file) != 1) {
        printf("  Erro ao ler quantidade de registros. Iniciando vazio.\n");
        total_alugueis = 0;
        fclose(file);
        return;
    }

    if (total_alugueis < 0 || total_alugueis > LIVROS_MAX) {
        printf("  Arquivo corrompido. Iniciando vazio.\n");
        total_alugueis = 0;
        fclose(file);
        return;
    }

    size_t lidos = fread(alugueis, sizeof(LivroAlugado), (size_t)total_alugueis, file);
    if (lidos != (size_t)total_alugueis) {
        printf("️  Erro ao ler registros. Iniciando vazio.\n");
        total_alugueis = 0;
    }

    fclose(file);
}

void listar_alugueis(void) {
    if (total_alugueis == 0) {
        printf(" Nenhum aluguel encontrado.\n");
        return;
    }

    printf("\n===== Lista de livros alugados =====\n");
    for (int i = 0; i < total_alugueis; i++) {
        printf("%d.\n", i + 1);
        printf("   Título:            %s\n", alugueis[i].titulo);
        printf("   Nome do cliente:   %s\n", alugueis[i].nome_cliente);
        printf("   ID do cliente:     %s\n", alugueis[i].id_cliente);
        printf("   Retirada:          %s\n", alugueis[i].data_retirada);
        printf("   Devolução:         %s\n", alugueis[i].data_devolucao);
        printf("   CPF:               %s\n", alugueis[i].cpf_cliente);
    }
}

void buscar_aluguel(void) {
    char titulo[TITULO_MAX];

    if (total_alugueis == 0) {
        printf("📂 Nenhum aluguel para buscar.\n");
        return;
    }

    printf("Digite o título do livro que deseja buscar: ");
    read_line(titulo, TITULO_MAX);

    for (int i = 0; i < total_alugueis; i++) {
        if (strcmp(alugueis[i].titulo, titulo) == 0) {
            printf("✅ Aluguel encontrado:\n");
            printf("   Título:  %s\n", alugueis[i].titulo);
            printf("   Cliente: %s\n", alugueis[i].nome_cliente);
            printf("   ID:      %s\n", alugueis[i].id_cliente);
            printf("   Retirada: %s\n", alugueis[i].data_retirada);
            printf("   Devolução: %s\n", alugueis[i].data_devolucao);
            printf("   CPF: %s\n", alugueis[i].cpf_cliente);
            return;
        }
    }

    printf(" Aluguel não encontrado.\n");
}

void editar_aluguel(void) {
    if (total_alugueis == 0) {
        printf(" Nenhum aluguel para editar.\n");
        return;
    }

    listar_alugueis();

    int id;
    printf("Digite o número do aluguel que deseja editar: ");
    if (scanf("%d", &id) != 1) {
        printf(" Entrada inválida.\n");
        clear_buffer();
        return;
    }
    clear_buffer();

    if (id < 1 || id > total_alugueis) {
        printf(" Aluguel inválido.\n");
        return;
    }
    id--;

    printf("Editando aluguel %d. Deixe em branco para manter os valores atuais.\n", id + 1);

    char buffer[100];

    printf("Novo título (atual: %s): ", alugueis[id].titulo);
    read_line(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) strcpy(alugueis[id].titulo, buffer);

    printf("Novo nome do cliente (atual: %s): ", alugueis[id].nome_cliente);
    read_line(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) strcpy(alugueis[id].nome_cliente, buffer);

    printf("Novo ID (atual: %s): ", alugueis[id].id_cliente);
    read_line(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) strcpy(alugueis[id].id_cliente, buffer);

    printf("Nova retirada (atual: %s): ", alugueis[id].data_retirada);
    read_line(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) strcpy(alugueis[id].data_retirada, buffer);

    printf("Nova devolução (atual: %s): ", alugueis[id].data_devolucao);
    read_line(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) strcpy(alugueis[id].data_devolucao, buffer);

    printf("Novo CPF (atual: %s): ", alugueis[id].cpf_cliente);
    read_line(buffer, sizeof(buffer));
    if (strlen(buffer) == 11) strcpy(alugueis[id].cpf_cliente, buffer);

    printf("  Aluguel editado com sucesso!\n");
}

void excluir_aluguel(void) {
    if (total_alugueis == 0) {
        printf(" Nenhum aluguel para excluir.\n");
        return;
    }

    listar_alugueis();

    int id;
    printf("Digite o número do aluguel que deseja excluir: ");
    if (scanf("%d", &id) != 1) {
        printf(" Entrada inválida.\n");
        clear_buffer();
        return;
    }
    clear_buffer();

    if (id < 1 || id > total_alugueis) {
        printf("Aluguel inválido.\n");
        return;
    }

    id--;
    for (int i = id; i < total_alugueis - 1; i++) {
        alugueis[i] = alugueis[i + 1];
    }
    total_alugueis--;

    printf("Aluguel excluído com sucesso!\n");
}

/* ---------------- Main ---------------- */
int main(void) {
    carregar_dados();

    int opcao;
    do {
        printf("\n========== MENU ==========\n");
        printf("1. Alugar livro\n");
        printf("2. Salvar dados\n");
        printf("3. Listar alugueis\n");
        printf("4. Buscar aluguel\n");
        printf("5. Editar aluguel\n");
        printf("6. Excluir aluguel\n");
        printf("7. Sair\n");
        printf("==========================\n");
        printf("Escolha uma opção: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida.\n");
            clear_buffer();
            continue;
        }
        clear_buffer();

        switch (opcao) {
            case 1: alugar_livro();     break;
            case 2: salvar_dados();     break;
            case 3: listar_alugueis();  break;
            case 4: buscar_aluguel();   break;
            case 5: editar_aluguel();   break;
            case 6: excluir_aluguel();  break;
            case 7:
                salvar_dados();
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 7);

    return 0;
}
