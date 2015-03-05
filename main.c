#include "cscheme.h"

void prompt() {
    printf("-> ");
    fflush(stdout);
}

void repl() {
    const char *scheme_stdlib = "/Users/chstansbury/include/scheme_stdlib/core.scm";
    SchemeEnv *se = scheme_env_make(scheme_stdlib);
    if (se == NULL) return;

    char *buffer = NULL;
    size_t read_bytes = 0;
    prompt();

    while ((read_bytes = getline(&buffer, &read_bytes, stdin)) > 0) {
        if (strncmp(buffer, "quit", 4) == 0) {
            free(buffer);
            return;
        }
        Vector *tokens = lexer_lex(se->_lexer, buffer);
        if (tokens != NULL) {
            Vector *sig_tokens = v_filter(tokens, scheme_significant_token, NULL);
            Vector *program = (Vector *) parser_parse(se->_parser, "PROGRAM_P", sig_tokens);

            if (program != NULL) {
                if (v_size(program) != 0) {
                    for (size_t f_i = 0; f_i < v_size(program); f_i++) {
                        scheme_clear_state(se);
                        SchemeObject *form = *(SchemeObject **) v_at(program, f_i);
                        SchemeObject *e_form = scheme_eval(se, form);
                        if (e_form != NULL) {
                            scheme_object_fprint(stdout, e_form);
                            printf("\n");
                        } else {
                            if (scheme_state_okay(se)) printf("As you wish.\n");
                            else printf("\n");
                        }
                        fflush(stdout);
                    }
                } else {
                    printf("Well that was easy.\n");
                }
                v_free(program);
            } else {
                printf("Did not read a valid scheme form.\n");
            }

            v_free(tokens);
            v_free(sig_tokens);
        } else {
            printf("Did not parse a valid scheme form.\n");
        }
        free(buffer);
        buffer = NULL;
        prompt();
    }

    scheme_env_free(se);
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
    printf("Hello internet. No ports. ;)\n");
    repl();
    return 0;
}
