#ifndef CONVERTERS_H
#define CONVERTERS_H

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

#endif
