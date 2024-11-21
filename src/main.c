/**
 * IFJ projeKT 2024
 *
 * @author <xsucha18> Marek Sucharda
 * @author <xsvobov00> Veronika Svobodová
 * @author <247581> Martin Mendl
 * @author <253171> Vanesa Zimmermannová
 */

#include "code_generation/code_generator.h"
#include "syntaxical/parser.h"
#include "utility/enumerations.h"
#include "semantical/sem_analyzer.h"

int main(void) {
    enum ERR_CODES status;

    // init parser
    status = parser_init();
    if (status != SUCCESS) {
        return status;
    }

    // parse input
    struct Program program;
    status = parser_parse(stdin, &program);

    if (status != SUCCESS) {
        parser_cleanup();
        freeProgram(&program);
        return status;
    }

    status = analyzeProgram(&program);
    if (status != SUCCESS) {
        parser_cleanup();
        freeProgram(&program);
        return status;
    }

    // generate code
    generateCodeProgram(&program);

    parser_cleanup();
    freeProgram(&program);
    return SUCCESS;
}

// todo realloc muze delat na lexikalce memory leak
