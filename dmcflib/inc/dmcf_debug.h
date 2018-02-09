/*
 * dmcf_debug.h
 *
 *  Created on: Jan 25, 2018
 *      Author: Andrew
 */

#ifndef INC_DMCF_DEBUG_H_
#define INC_DMCF_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>
#include <ti/display/Display.h>

void dmcf_debugprintf(char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* INC_DMCF_DEBUG_H_ */
