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

    DEBUG_PRINT_IF(!table, "Table not created");
    DEBUG_PRINT("Table created");

    if (!table) return E_INTERNAL;

    // init parser
    status = parser_init(table);
    if (status != SUCCESS) {
        DEBUG_PRINT("Error in parser init");
        return status;
    }

    // parse input
    struct Program program;
    program.functions = NULL; // important, for faild first pass
    status = parser_parse(stdin, &program);

    DEBUG_PRINT("parser parse status: %d", status); 

    if (status != SUCCESS) {
        DEBUG_PRINT("cleaning up");
        parser_cleanup();
        DEBUG_PRINT("cleaning ast");
        freeProgram(&program);
        return status;
    }

    // analyze program
    DEBUG_PRINT("Analyzing program");

    status = analyzeProgram(&program, table);
    if (status != SUCCESS) {
        parser_cleanup();
        freeProgram(&program);
        return status;
    }

    // generate code
    DEBUG_PRINT("Generating code");

    generateCodeProgram(&program);

    parser_cleanup();
    freeProgram(&program);
    return SUCCESS;
}

// todo realloc muze delat na lexikalce memory leak
