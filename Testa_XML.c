#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura de nó da pilha
typedef struct Node {
    char data[100];
    struct Node* next;
} Node;

// Função para criar um novo nó
Node* newNode(const char* data) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (node == NULL) {
        fprintf(stderr, "Erro na alocação de memória\n");
        exit(1);
    }
    strcpy(node->data, data);
    node->next = NULL;
    return node;
}

// Função para empilhar um nó na pilha
void push(Node** top, const char* data) {
    Node* node = newNode(data);
    node->next = *top;
    *top = node;
}

// Função para desempilhar um nó da pilha
void pop(Node** top) {
    if (*top == NULL) {
        fprintf(stderr, "Erro: tentativa de desempilhar uma pilha vazia\n");
        exit(1);
    }
    Node* temp = *top;
    *top = (*top)->next;
    free(temp);
}

// Função para verificar se as tags XML são válidas
int validateXML(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro na abertura do arquivo\n");
        exit(1);
    }

    Node* stack = NULL;
    char line[1000];
    int lineNumber = 1;

    while (fgets(line, sizeof(line), file)) {
        char* token = strtok(line, "<>");
        while (token) {
            if (token[0] == '/') {
                // Tag de fechamento
                if (stack == NULL) {
                    printf("ERRO na linha %d do arquivo XML\n", lineNumber);
                    fclose(file);
                    return 0;
                }
                if (strcmp(stack->data, &token[1]) != 0) {
                    printf("ERRO na linha %d do arquivo XML\n", lineNumber);
                    fclose(file);
                    return 0;
                }
                pop(&stack);
            } else if (token[0] != '\n' && token[0] != ' ' && token[0] != '\t') {
                // Tag de abertura
                push(&stack, token);
            }
            token = strtok(NULL, "<>");
        }
        lineNumber++;
    }

    if (stack == NULL) {
        printf("Arquivo XML correto\n");
    } else {
        printf("ERRO na linha %d do arquivo XML\n", lineNumber - 1);
    }

    fclose(file);
    return 0;
}

int main() {
    const char* filename = "teste.xml"; // Substitua pelo nome do arquivo XML que deseja validar
    validateXML(filename);

    return 0;
}
