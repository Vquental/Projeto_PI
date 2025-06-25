#include <stdlib.h>
#include <stdio.h>
#include "../Token.h"

// Estruturas de dados auxiliares (Pilha e Fila)
// Essas definições refletem os tipos que já existem nos headers prontos

typedef struct no {
    Token tk;
    struct no *prox;
} No;

typedef struct {
    No *cabeca;
} Pilha;

typedef struct nof {
    Token tk;
    struct nof *prox;
} NO;

typedef struct {
    NO *fim, *inicio;
} Fila;

// Protótipos das funções (já implementadas nos headers, mas aqui são só declarações)
void initFila(Fila *F);
int inserir(Fila *F, Token tk);

void initPilha(Pilha *P);
int vaziaPilha(Pilha *P);
int push(Pilha *P, Token tk);
int pop(Pilha *P);
Token top(Pilha *P);

// Função responsável por converter uma expressão em notação infixada (ex: 2 + 3 * 4)
// para notação pós-fixada (ex: 2 3 4 * +), usando o algoritmo de Shunting Yard
void converter_para_posfixa(Token *tokens, Fila *saida) {
    initFila(saida);
    Pilha operadores;
    initPilha(&operadores);

    for (int i = 0; tokens[i].type != NONE; i++) {
        Token atual = tokens[i];

        if (atual.type == NUMERO) {
            inserir(saida, atual);
        } else if (atual.type == OPERADOR) {
            while (!vaziaPilha(&operadores) &&
                   top(&operadores).type == OPERADOR &&
                   top(&operadores).peso >= atual.peso) {
                inserir(saida, top(&operadores));
                pop(&operadores);
            }
            push(&operadores, atual);
        } else if (atual.value == '(') {
            push(&operadores, atual);
        } else if (atual.value == ')') {
            while (!vaziaPilha(&operadores) &&
                   top(&operadores).value != '(') {
                inserir(saida, top(&operadores));
                pop(&operadores);
            }
            if (!vaziaPilha(&operadores)) {
                pop(&operadores); // remove o '('
            }
        }
    }

    while (!vaziaPilha(&operadores)) {
        inserir(saida, top(&operadores));
        pop(&operadores);
    }
}
