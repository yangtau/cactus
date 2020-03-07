#ifndef _CACTUS_DEBUG_DEBUG_H_
#define _CACTUS_DEBUG_DEBUG_H_

#define E_SCANNER_STRING (1)
#define E_SCANNER_INVALID (2)

/* scanner_error_msg:
 * @error_code: the negative integer indicate error (the return value of the
 * function that occurs error)
 * */
const char *scanner_error_msg(int error_code);
#endif // _CACTUS_DEBUG_DEBUG_H_
