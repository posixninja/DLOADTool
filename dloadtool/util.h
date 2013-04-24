//
//  util.h
//  usblogger
//
//  Created by Joshua Hill on 1/29/13.
//
//

#ifndef usblogger_util_h
#define usblogger_util_h

#include <sys/types.h>


typedef u_int8_t boolean;
#ifndef TRUE
#define TRUE ((u_int8_t) 1)
#endif
#ifndef FALSE
#define FALSE ((u_int8_t) 0)
#endif

void hexdump (unsigned char *data, unsigned int amount);


#define DIAG_CONTROL_CHAR 0x7E
#define DIAG_TRAILER_LEN  3

u_int16_t dm_crc16 (const char *buffer, size_t len);

size_t dm_escape (const char *inbuf,
                  size_t inbuf_len,
                  char *outbuf,
                  size_t outbuf_len);

size_t dm_unescape (const char *inbuf,
                    size_t inbuf_len,
                    char *outbuf,
                    size_t outbuf_len,
                    boolean *escaping);

size_t dm_encapsulate_buffer (char *inbuf,
                              size_t cmd_len,
                              size_t inbuf_len,
                              char *outbuf,
                              size_t outbuf_len);

boolean dm_decapsulate_buffer (const char *inbuf,
                               size_t inbuf_len,
                               char *outbuf,
                               size_t outbuf_len,
                               size_t *out_decap_len,
                               size_t *out_used,
                               boolean *out_need_more);


#endif
