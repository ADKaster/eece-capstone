/* \file implementation of ring buffer 
 *
 *
 * 
 *
 *
 */
#include "ringbuf.h"
#include "messaging.h"
#include <string.h>

bool ringbuf_isempty(volatile ringbuf_t *rbuf)
{
    uint32_t count;

    DAMN_DISABLE_INTS();
    count = rbuf->count;
    DAMN_ENABLE_INTS();
    return ( count == 0);
}
bool ringbuf_isfull(volatile ringbuf_t *rbuf)
{
    uint32_t count;
    uint32_t size;

    DAMN_DISABLE_INTS();
    count = rbuf->count;
    size = rbuf->size;
    DAMN_ENABLE_INTS();

    return (count >= size);
}

uint32_t ringbuf_getcount(volatile ringbuf_t *rbuf)
{
    uint32_t count;

    DAMN_DISABLE_INTS();
    count = rbuf->count;
    DAMN_ENABLE_INTS();

    return count;   
}

uint32_t ringbuf_peek(volatile ringbuf_t *rbuf)
{
    uint32_t value;

    DAMN_DISABLE_INTS();
    value = rbuf->buf[rbuf->tail];
    DAMN_ENABLE_INTS();

    return value;
}

uint32_t ringbuf_get(volatile ringbuf_t *rbuf, uint8_t *getbuf, uint32_t numbytes)
{
    uint32_t bytescopied;
    /* Check how many bytes are available, then copy to getbuf */
    DAMN_DISABLE_INTS();
    if((rbuf->count) > 0)
    {
        if((rbuf->count) >= numbytes)
        {
            bytescopied = numbytes;
        }
        else
        {
            bytescopied = rbuf->count;
        }

        memcpy((void *)getbuf, (void *)(&(rbuf->buf[rbuf->tail])), bytescopied);
        rbuf->tail = (rbuf->tail + bytescopied) % (rbuf->size);
        rbuf->count -= bytescopied;
    }
    else
    {
        bytescopied = -1;
    }
    DAMN_ENABLE_INTS();
    return bytescopied;
}

void ringbuf_put(volatile ringbuf_t *rbuf, uint8_t val)
{
    DAMN_DISABLE_INTS();
    if((rbuf->count) < (rbuf->size))
    {
        rbuf->buf[rbuf->head] = val;
        rbuf->head = (rbuf->head + 1) % rbuf->size;
        rbuf->count++;
    }
    DAMN_ENABLE_INTS();
    return;
}

