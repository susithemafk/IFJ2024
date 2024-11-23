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
    
    SymTable *table = symTableInit();
    #ifdef DEBUG
    if (!table) printf("Table not created\n");
    printf("Table created\n");
    #endif
    if (!table) return E_INTERNAL;

    // init parser
    status = parser_init(table);
    if (status != SUCCESS) {
        #ifdef DEBUG
        printf("Error in parser init\n");
        #endif
        return status;
    }

    // parse input
    struct Program program;
    program.functions = NULL; // important, for faild first pass
    status = parser_parse(stdin, &program);

    #ifdef DEBUG
    printf("Parser parse status: %d\n", status);
    #endif

    if (status != SUCCESS) {
        #ifdef DEBUG
        printf("cleaning up\n");
        #endif
        parser_cleanup();
        #ifdef DEBUG
        printf("cleaning ast\n");
        #endif
        freeProgram(&program);
        return status;
    }

    #ifdef DEBUG
    printf("Analyzing program\n");
    #endif

    status = analyzeProgram(&program, table);
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
