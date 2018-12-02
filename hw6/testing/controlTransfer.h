/* Allen Tran
 *
 * Intermediary between C and C++ code
 */
#ifndef _CONTROL_TRANSFER_H_
#define _CONTROL_TRANSFER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "jansson.h"

void passOnControl(void * serverPtr, json_t *root);

#ifdef __cplusplus
}
#endif

#endif
