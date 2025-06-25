#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "../Token.h"

Token* tokenizer(const char *linha) {
    Token *tokens = malloc(MAX * sizeof(Token));
    if (!tokens) {
        perror("Erro ao alocar memória para tokens.");
        exit(EXIT_FAILURE);
    }

    int index = 0;
    for (int i = 0; linha[i] != '\0' && index < MAX - 1; i++) {
        char c = linha[i];
        if (c == ' ') continue;

        Token tk;
        tk.value = c;

        if (isdigit(c)) {
            tk.type = NUMERO;
            tk.peso = 0;
        } else if (c == '(' || c == ')') {
            tk.type = PARENTESES;
            tk.peso = -1;
        } else if (strchr("+-*/^", c)) {
            tk.type = OPERADOR;
            tk.peso = (c == '+' || c == '-') ? 1 :
                      (c == '*' || c == '/') ? 2 :
                      (c == '^') ? 3 : -1;
        } else {
            tk.type = NONE;
            tk.peso = -1;
        }

        tokens[index++] = tk;
    }

    tokens[index].type = NONE;
    tokens[index].value = '\0';
    tokens[index].peso = -1;

    return tokens;
}

void liberar_tokens(Token *tokens) {
    if (tokens) free(tokens);
}

int verificar_parenteses(Token *tokens) {
    int contador = 0;

    for (int i = 0; tokens[i].type != NONE; i++) {
        if (tokens[i].type == PARENTESES) {
            if (tokens[i].value == '(')
                contador++;
            else if (tokens[i].value == ')') {
                contador--;
                if (contador < 0) return 0;
            }
        }
    }

    return contador == 0;
}
