/**
 * IFJ projeKT 2024
 *
 * @author <xsucha18> Marek Sucharda
 * @author <xsvobov00> Veronika Svobodová
 * @author <247581> Martin Mendl
 * @author <253171> Vanesa Zimmermannová
 */

#ifdef USE_CUSTOM_STRUCTURE
#include "code_generation/code_generator.h"
#include "syntaxical/parser.h"
#include "utility/enumerations.h"
#include "semantical/sem_analyzer.h"
#else 
#include "code_generator.h"
#include "parser.h"
#include "enumerations.h"
#include "sem_analyzer.h"
#endif

int main(void) {
    enum ERR_CODES status;
    
    SymTable *table = symTableInit();
    LinkedList *buffer = initLinkedList(false);

    DEBUG_PRINT_IF(!table, "Table not created");
    DEBUG_PRINT("Table created");

    if (!table) return E_INTERNAL;

    // init parser
    status = parser_init(table, buffer);
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
        freeBuffer(&buffer);
        symTableFree(&table);
        freeProgram(&program);
        return status;
    }

    // analyze program
    DEBUG_PRINT("Analyzing program");

    status = analyzeProgram(&program, table);
    if (status != SUCCESS) {
        DEBUG_PRINT("cleaning up");
        freeBuffer(&buffer);
        symTableFree(&table);
        freeProgram(&program);
        return status;
    }

    // generate code
    DEBUG_PRINT("Generating code");

    generateCodeProgram(&program);

    // clean up
    freeBuffer(&buffer);
    symTableFree(&table);
    freeProgram(&program);
    return SUCCESS;
}

// todo realloc muze delat na lexikalce memory leak
