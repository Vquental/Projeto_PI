#include "../dstlib_include.h"

// Declarações de funções externas
Token* tokenizer(const char *linha);                     // Quebra a expressão em tokens (números, operadores, parênteses)
int verificar_parenteses(Token *tokens);                 // Verifica se os parênteses estão balanceados
void liberar_tokens(Token *tokens);                      // Libera a memória alocada para os tokens
void converter_para_posfixa(Token *tokens, Fila *saida); // Converte para notação pós-fixada usando algoritmo de Shunting Yard

// Função que avalia uma expressão já convertida para pós-fixada
// Retorna:
//   1  → resultado calculado com sucesso
//   0  → erro na expressão (ex: operadores mal posicionados)
//  -1  → divisão por zero
int avaliar_posfixada(Fila *fila, int *resultado) {
    Pilha_int pilha;
    initPilhaInt(&pilha);

    NO *no = fila->inicio;
    while (no != NULL) {
        Token t = no->tk;

        if (t.type == NUMERO) {
            push_int(&pilha, t.value - '0');
        } else if (t.type == OPERADOR) {
            if (vaziaPilhaInt(&pilha)) return 0;
            int b = top_int(&pilha); pop_int(&pilha);

            if (vaziaPilhaInt(&pilha)) return 0;
            int a = top_int(&pilha); pop_int(&pilha);

            int r = 0;
            switch (t.value) {
                case '+': r = a + b; break;
                case '-': r = a - b; break;
                case '*': r = a * b; break;
                case '/':
                    if (b == 0) return -1;
                    r = a / b;
                    break;
                case '^':
                    r = 1;
                    for (int i = 0; i < b; i++) r *= a;
                    break;
                default: return 0;
            }

            push_int(&pilha, r);
        }

        no = no->prox;
    }

    if (vaziaPilhaInt(&pilha)) return 0;
    *resultado = top_int(&pilha);
    return 1;
}

// Função principal: recebe os arquivos de entrada e saída como argumentos
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso correto: %s in.txt out.txt\n", argv[0]);
        return 1;
    }

    FILE *entrada = fopen(argv[1], "r");
    FILE *saida = fopen(argv[2], "w");
    
    if (!entrada || !saida) {
        perror("Erro ao abrir arquivos");
        return 1;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), entrada)) {
        // Remove o caractere de quebra de linha, se existir
        linha[strcspn(linha, "\n")] = '\0';

        Token *tokens = tokenizer(linha);

        // Etapa 1: verificação de parênteses
        if (!verificar_parenteses(tokens)) {
            fprintf(saida, "Erro: Parênteses desbalanceados\n");
            liberar_tokens(tokens);
            continue;
        }

        // Etapa 2: verificação de caracteres inválidos
        int invalido = 0;
        for (int i = 0; tokens[i].type != NONE; i++) {
            if (tokens[i].type == NONE && tokens[i].value != '\0') {
                invalido = 1;
                break;
            }
        }
        if (invalido) {
            fprintf(saida, "Erro: Caracteres inválidos\n");
            liberar_tokens(tokens);
            continue;
        }

        // Etapa 3: conversão para pós-fixada
        Fila fila_saida;
        initFila(&fila_saida);
        converter_para_posfixa(tokens, &fila_saida);

        // Etapa 4: avaliação da expressão pós-fixada
        int resultado;
        int status = avaliar_posfixada(&fila_saida, &resultado);

        // Etapa 5: tratamento do resultado
        if (status == -1) {
            fprintf(saida, "Erro: Divisão por zero\n");
        } else if (status == 0) {
            fprintf(saida, "Erro: Expressão malformada\n");
        } else {
            fprintf(saida, "%d\n", resultado);
        }

        liberar_tokens(tokens);
    }

    fclose(entrada);
    fclose(saida);
    return 0;
}
