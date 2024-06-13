#ifndef STRINGHELPERS_H
#define STRINGHELPERS_H

char** SplitStringWithDelimeter(const char* inputString, char delimiter) {
    int numberOfDeleimeters = 0;
    const char* copyOfInputString = inputString;
    while (*copyOfInputString != '\0') {
        if (*copyOfInputString++ == delimiter) {
            numberOfDeleimeters++;
        }
    }

    char** tokens = (char**)malloc(sizeof(char*) * (numberOfDeleimeters + 2));
    int i = 0;
    copyOfInputString = inputString;
    char* token = (char*)malloc(strlen(inputString) + 1);
    int j = 0;
    while (*copyOfInputString != '\0') {
        if (*copyOfInputString == delimiter) {
            token[j] = '\0';
            tokens[i++] = strdup(token);
            j = 0;
        } else {
            token[j++] = *copyOfInputString;
        }
        copyOfInputString++;
    }
    token[j] = '\0';
    tokens[i++] = strdup(token);
    tokens[i] = NULL;
    free(token);
    return tokens;
}

#endif
