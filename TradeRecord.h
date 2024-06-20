#ifndef TRADERECORD_H
#define TRADERECORD_H

typedef struct {
    char SourceCurrency[256];
    char DestinationCurrency[256];
    float Lots;
    double Price;
} TradeResords;

#endif 