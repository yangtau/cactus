#ifndef _LLL_COMMON_ERROR_H_
#define _LLL_COMMON_ERROR_H_

#define E_SCANNER_INVALID_STRING (1)
#define E_SCANNER_INVALID_SYMBOL (2)

/* scanner_error_msg:
 * @error_code: the negative integer indicate error (the return value of the
 * function that occurs error)
 * */
const char *scanner_error_msg(int error_code);
#endif // _LLL_COMMON_ERROR_H_
