#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct {
    char SourceCurrency[256];
    char DestinationCurrency[256];
    float Lots;
    double Price;
} TradeResords;


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


bool TryGetIntegerValueFromString(const char* inputString, int* value) {
    char* endptr;
    *value = strtol(inputString, &endptr, 10);
    if (endptr == inputString) {
        return 0;
    }
    return 1;
}

bool TryGetFloatValueFromString(const char* inputString, double* value) {
    char* endptr;
    *value = strtod(inputString, &endptr);
    if (endptr == inputString) {
        return 0;
    }
    return 1;
}

void TryConvertCSVtoXML(FILE* tradeRecordFile) {
    char line[1024];
    TradeResords TradeResordObjects[1024];
    int lineCount = 0;
    int objectCount = 0;

    while (fgets(line, sizeof(line), tradeRecordFile)) {
        char* fields[3];
        int fieldCount = 0;
        char* token = strtok(line, ",");
        while (token != NULL) {
            fields[fieldCount++] = token;
            token = strtok(NULL, ",");
        }

        if (fieldCount != 3) {
            fprintf(stderr, "WARN: Line %d malformed. Only %d field(s) found.\n", lineCount + 1, fieldCount);
            continue;
        }

        if (strlen(fields[0]) != 6) {
            fprintf(stderr, "WARN: Trade currencies on line %d malformed: '%s'\n", lineCount + 1, fields[0]);
            continue;
        }

        int tradeAmount;
        if (!GetIntegerValueFromString(fields[1], &tradeAmount)) {
            fprintf(stderr, "WARN: Trade amount on line %d not a valid integer: '%s'\n", lineCount + 1, fields[1]);
        }

        double tradePrice;
        if (!GetFloatValueFromString(fields[2], &tradePrice)) {
            fprintf(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", lineCount + 1, fields[2]);
        }

        strncpy(TradeResordObjects[objectCount].SourceCurrency, fields[0], 3);
        strncpy(TradeResordObjects[objectCount].DestinationCurrency, fields[0] + 3, 3);
        TradeResordObjects[objectCount].Lots = tradeAmount / LotSize;
        TradeResordObjects[objectCount].Price = tradePrice;
        objectCount++;
        lineCount++;
    }

    FILE* outFile = fopen("output.xml", "w");
    fprintf(outFile, "<TradeRecords>\n");
    for (int i = 0; i < objectCount; i++) {
        fprintf(outFile, "\t<TradeRecord>\n");
        fprintf(outFile, "\t\t<SourceCurrency>%s</SourceCurrency>\n", TradeResordObjects[i].SourceCurrency);
        fprintf(outFile, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", TradeResordObjects[i].DestinationCurrency);
        fprintf(outFile, "\t\t<Lots>%d</Lots>\n", TradeResordObjects[i].Lots);
        fprintf(outFile, "\t\t<Price>%f</Price>\n", TradeResordObjects[i].Price);
        fprintf(outFile, "\t</TradeRecord>\n");
    }
    fprintf(outFile, "</TradeRecords>");
    fclose(outFile);
    printf("INFO: %d trades processed\n", objectCount);
}
