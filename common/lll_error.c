#include "lll_error.h"

static const char *ERROR_MSG[] = {
    [E_SCANNER_INVALID_STRING] = "fail to parse the string",
    [E_SCANNER_INVALID_SYMBOL] = "something that Cactus does not recognize"
};

const char *scanner_error_msg(int error_code) {
    return ERROR_MSG[-error_code];
}
