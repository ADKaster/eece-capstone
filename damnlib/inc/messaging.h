/* \file messaging helper functions 




 */


 #ifndef MESSAGING_H
 #define MESSAGING_H

#include "damn_msgdef.h"
#include <stdbool.h>

#ifdef FREERTOS
    #include <FreeRTOS.h>
    #include <portmacro.h>
    #define DAMN_DISABLE_INTS() portDISABLE_INTERRUPTS()
    #define DAMN_ENABLE_INTS()  portENABLE_INTERRUPTS()
#else
    #error "Look up how to disable interrupts with TI RTOS. Or like, implement your own macros"
#endif

bool damn_msg_verify_header(damn_pkthdr_t *hdr);

bool damn_msg_verify_msg(uint32_t *buf, uint32_t len);

void damn_msg_create_header(damn_pkthdr_t *hdr,
                            damn_node_t src,
                            damn_node_t dest,
                            damn_msg_enum_t id,
                            uint32_t msg_size);

uint32_t damn_calculate_checksum(uint32_t *buf, uint32_t size);


 #endif /* MESSAGING_H */
