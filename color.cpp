#include <ctype.h>
#include <string.h>
#include <emscripten.h>

static int hex_to_bin(char ch) {
    if ((ch >= '0') && (ch <= '9')) {
        return ch - '0';
    }
    ch = tolower(ch);
    if ((ch >= 'a') && (ch <= 'f')) {
        return ch - 'a' + 10;
    }
    return -1;
}

EMSCRIPTEN_KEEPALIVE extern "C" int (*hex_to_bin_ptr)(char ch) = hex_to_bin;

static int hex_to_bin2(char c0, char c1) {
    return (*hex_to_bin_ptr)(c0) * 16 + (*hex_to_bin_ptr)(c1);
}

EMSCRIPTEN_KEEPALIVE extern "C" void colorFromHexString(const char *hex, float *result) {
    if (*hex == '#') {
        hex++;
    }

    result[0] = result[1] = result[2] = result[3] = 1.0;
    if (strlen(hex) == 6) {
        result[0] = hex_to_bin2(hex[0], hex[1]) / 255.0;
        result[1] = hex_to_bin2(hex[2], hex[3]) / 255.0;
        result[2] = hex_to_bin2(hex[4], hex[5]) / 255.0;
    }
}
