/*
    territorios.c
    Um CLI simples em C para gerenciar registros de "Territorio" em memória.
    Compilar (exemplo):
        gcc -std=c11 -O2 -o territorios.exe territorios.c
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_TERRITORIOS 100
#define MAX_NOME 64
#define MAX_COR 32

typedef struct {
        int id;
        char nome[MAX_NOME];
        int area;
        char cor[MAX_COR];
} Territorio;

static Territorio territorios[MAX_TERRITORIOS];
static int quantidade = 0;
static int next_id = 1;

/* Lê uma linha do stdin e remove CR/LF. Retorna buffer passado. */
char *read_line(char *buffer, size_t size) {
        if (!fgets(buffer, (int)size, stdin)) {
                buffer[0] = '\0';
                return buffer;
        }
        /* remover CR/LF */
        size_t len = strlen(buffer);
        while (len > 0 && (buffer[len-1] == '\n' || buffer[len-1] == '\r')) {
                buffer[--len] = '\0';
        }
        return buffer;
}

/* Lê inteiro simples com atoi (comportamento original: linhas vazias -> 0) */
int read_int(void) {
        char buf[32];
        read_line(buf, sizeof(buf));
        return atoi(buf);
}

/* Comparação case-insensitive; retorna 0 se iguais */
int strcmp_ci(const char *a, const char *b) {
        unsigned char ca, cb;
        while (*a && *b) {
                ca = (unsigned char) tolower((unsigned char)*a);
                cb = (unsigned char) tolower((unsigned char)*b);
                if (ca != cb) return (int)ca - (int)cb;
                a++; b++;
        }
        ca = (unsigned char) tolower((unsigned char)*a);
        cb = (unsigned char) tolower((unsigned char)*b);
        return (int)ca - (int)cb;
}

/* Busca um territorio por nome (case-insensitive). Retorna índice ou -1 */
int buscarTerritorio(const char *nome) {
        for (int i = 0; i < quantidade; ++i) {
                if (strcmp_ci(territorios[i].nome, nome) == 0) return i;
        }
        return -1;
}

void cadastrarTerritorio(void) {
        if (quantidade >= MAX_TERRITORIOS) {
                printf("Limite de territorios atingido (%d).\n", MAX_TERRITORIOS);
                return;
        }
        Territorio t;
        t.id = next_id++;
        char buf[MAX_NOME];

        printf("Nome: ");
        read_line(buf, sizeof(buf));
        strncpy(t.nome, buf, MAX_NOME - 1);
        t.nome[MAX_NOME - 1] = '\0';

        printf("Area (inteiro): ");
        t.area = read_int();

        printf("Cor: ");
        read_line(buf, sizeof(buf));
        strncpy(t.cor, buf, MAX_COR - 1);
        t.cor[MAX_COR - 1] = '\0';

        territorios[quantidade++] = t;
        printf("Territorio cadastrado (id=%d).\n", t.id);
}

void exibirTerritorios(void) {
        if (quantidade == 0) {
                printf("Nenhum territorio cadastrado.\n");
                return;
        }
        printf("Lista de territorios (%d):\n", quantidade);
        for (int i = 0; i < quantidade; ++i) {
                Territorio *t = &territorios[i];
                printf("[%d] Nome: %s | Area: %d | Cor: %s\n", t->id, t->nome, t->area, t->cor);
        }
}

void buscarTerritorioMenu(void) {
        char buf[MAX_NOME];
        printf("Nome a buscar: ");
        read_line(buf, sizeof(buf));
        int idx = buscarTerritorio(buf);
        if (idx < 0) {
                printf("Territorio '%s' não encontrado.\n", buf);
        } else {
                Territorio *t = &territorios[idx];
                printf("Encontrado: [%d] Nome: %s | Area: %d | Cor: %s\n", t->id, t->nome, t->area, t->cor);
        }
}

void editarTerritorio(void) {
        char buf[MAX_NOME];
        printf("Nome do territorio a editar: ");
        read_line(buf, sizeof(buf));
        int idx = buscarTerritorio(buf);
        if (idx < 0) {
                printf("Territorio '%s' não encontrado.\n", buf);
                return;
        }
        Territorio *t = &territorios[idx];
        printf("Editando territorio [%d] %s\n", t->id, t->nome);

        printf("Novo nome (enter para manter: %s): ", t->nome);
        read_line(buf, sizeof(buf));
        if (buf[0] != '\0') {
                strncpy(t->nome, buf, MAX_NOME - 1);
                t->nome[MAX_NOME - 1] = '\0';
        }

        printf("Nova area (enter para manter: %d): ", t->area);
        read_line(buf, sizeof(buf));
        if (buf[0] != '\0') {
                t->area = atoi(buf);
        }

        printf("Nova cor (enter para manter: %s): ", t->cor);
        read_line(buf, sizeof(buf));
        if (buf[0] != '\0') {
                strncpy(t->cor, buf, MAX_COR - 1);
                t->cor[MAX_COR - 1] = '\0';
        }

        printf("Territorio atualizado.\n");
}

void removerTerritorio(void) {
        char buf[MAX_NOME];
        printf("Nome do territorio a remover: ");
        read_line(buf, sizeof(buf));
        int idx = buscarTerritorio(buf);
        if (idx < 0) {
                printf("Territorio '%s' não encontrado.\n", buf);
                return;
        }
        /* deslocar elementos à esquerda */
        for (int i = idx; i + 1 < quantidade; ++i) {
                territorios[i] = territorios[i + 1];
        }
        quantidade--;
        printf("Territorio '%s' removido.\n", buf);
}

void mostrarMenu(void) {
        printf("\n=== Guerra de Territorios ===\n");
        printf("1. Cadastrar territorio\n");
        printf("2. Exibir territorios\n");
        printf("3. Buscar territorio por nome\n");
        printf("4. Editar territorio\n");
        printf("5. Remover territorio\n");
        printf("0. Sair\n");
        printf("Escolha: ");
}

int main(void) {
        char escolha_buf[8];
        for (;;) {
                mostrarMenu();
                read_line(escolha_buf, sizeof(escolha_buf));
                int escolha = atoi(escolha_buf);
                switch (escolha) {
                        case 1: cadastrarTerritorio(); break;
                        case 2: exibirTerritorios(); break;
                        case 3: buscarTerritorioMenu(); break;
                        case 4: editarTerritorio(); break;
                        case 5: removerTerritorio(); break;
                        case 0:
                                printf("Saindo...\n");
                                return 0;
                        default:
                                printf("Opcao invalida.\n");
                }
        }
        return 0;
}