
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "semantical/sem_enums.h"
#include "utility/linked_list.h"
#include "code_generation/code_generator.h"
#include "utility/enumerations.h"
#include "semantical/sem_enums.h"
#include "ast_assets/good_asts.h"


int main(void) {

    char *string = "retezec s lomitkem \\ a\nnovym#radkem";

    // Escape the string
    char *escapedString = escapeString(string);
    if (escapedString == NULL) {
        printf("Failed to escape the string\n");
        return 1;
    }

    printf("Escaped string: string@%s\n", escapedString);

    // Free the allocated memory
    free(escapedString);
    return 0;
}
