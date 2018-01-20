/* \file messaging helper functions 




 */


 #ifndef MESSAGING_H
 #define MESSAGING_H

#include "dmcf_msgdef.h"
#include <stdbool.h>

#ifdef FREERTOS
    #include <FreeRTOS.h>
    #include <portmacro.h>
    #define DMCF_DISABLE_INTS() portDISABLE_INTERRUPTS()
    #define DMCF_ENABLE_INTS()  portENABLE_INTERRUPTS()
#else
    #error "Look up how to disable interrupts with TI RTOS. Or like, implement your own macros"
#endif

bool dmcf_msg_verify_header(dmcf_pkthdr_t *hdr);

bool dmcf_msg_verify_msg(uint32_t *buf, uint32_t len);

void dmcf_msg_create_header(dmcf_pkthdr_t *hdr,
                            dmcf_node_t src,
                            dmcf_node_t dest,
                            dmcf_msg_enum_t id,
                            uint32_t msg_size);

uint32_t dmcf_calculate_checksum(uint32_t *buf, uint32_t size);


 #endif /* MESSAGING_H */
