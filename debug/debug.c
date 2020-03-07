#include "debug.h"
static const char *ERROR_MSG[] = {
    [E_SCANNER_STRING] = "fail to parse string",
    [E_SCANNER_INVALID] = "something that Cactus does not recognize"
};

const char *scanner_error_msg(int error_code) {
    return ERROR_MSG[-error_code];
}
