/* \file messaging helper functions 




 */


 #ifndef MESSAGING_H
 #define MESSAGING_H

#include "damn_msgdef.h"
#include "Board.h"

bool damn_msg_verify_header(damn_pkthdr_t *hdr);

void damn_msg_create_header(damn_pkthdr_t *hdr,
                            damn_node_t src,
                            damn_node_t dest,
                            damn_msg_enum_t id,
                            uint32_t msg_size);



 #endif /* MESSAGING_H */