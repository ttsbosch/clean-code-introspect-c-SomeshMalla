#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "TradeRecord.h"
#include "CSVtoXMLTradeConverter.h"

void TryConvertCSVtoXML(FILE* tradeRecordFile) {
    char tradeData[1024];
    TradeResords TradeResordObjects[1024];
    int lineCount = 0;
    int TradeRecordCount = 0;

    while (fgets(tradeData, sizeof(tradeData), tradeRecordFile)) {
        char* fields[3];
        int fieldCount = 0;
        fieldCount = ;
        char* token = strtok(tradeData, ",");
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
        if (!TryGetIntegerValueFromString(fields[1], &tradeAmount)) {
            fprintf(stderr, "WARN: Trade amount on line %d not a valid integer: '%s'\n", lineCount + 1, fields[1]);
        }

        double tradePrice;
        if (!TryGetFloatValueFromString(fields[2], &tradePrice)) {
            fprintf(stderr, "WARN: Trade price on line %d not a valid decimal: '%s'\n", lineCount + 1, fields[2]);
        }

        strncpy(TradeResordObjects[TradeRecordCount].SourceCurrency, fields[0], 3);
        strncpy(TradeResordObjects[TradeRecordCount].DestinationCurrency, fields[0] + 3, 3);
        TradeResordObjects[TradeRecordCount].Lots = tradeAmount / LotSize;
        TradeResordObjects[TradeRecordCount].Price = tradePrice;
        TradeRecordCount++;
        lineCount++;
    }

    FILE* tradeRecordInXML = fopen("output.xml", "w");
    fprintf(tradeRecordInXML, "<TradeRecords>\n");
    for (int i = 0; i < TradeRecordCount; i++) {
        fprintf(tradeRecordInXML, "\t<TradeRecord>\n");
        fprintf(tradeRecordInXML, "\t\t<SourceCurrency>%s</SourceCurrency>\n", TradeResordObjects[i].SourceCurrency);
        fprintf(tradeRecordInXML, "\t\t<DestinationCurrency>%s</DestinationCurrency>\n", TradeResordObjects[i].DestinationCurrency);
        fprintf(tradeRecordInXML, "\t\t<Lots>%d</Lots>\n", TradeResordObjects[i].Lots);
        fprintf(tradeRecordInXML, "\t\t<Price>%f</Price>\n", TradeResordObjects[i].Price);
        fprintf(tradeRecordInXML, "\t</TradeRecord>\n");
    }
    fprintf(tradeRecordInXML, "</TradeRecords>");
    fclose(tradeRecordInXML);
    printf("INFO: %d trades processed\n", TradeRecordCount);
}
