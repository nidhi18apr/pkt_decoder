//
//  pkt_decoder.h
//  pkt_decoder
//
//  Created by Nidhi on 10/22/19.
//  Copyright © 2019 Nidhi. All rights reserved.
//

#ifndef PKT_DECODER_H_INCLUDED
#define PKT_DECODER_H_INCLUDED
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
extern "C" {
#endif

#define MAX_DECODED_PACKET_LEN (512)
    
#define MAX_ENCODED_PACKET_LEN (1026) /* Since Maximum Decoded length is 512. So maximum encoded length is : 2 * decoded length (assuminng if all bytes STX/ETX/DEL) + 1 (for STX) + 1 (for ETX) */

#define STX     0x02
#define ETX     0x03
#define DLE     0x10
    
#define PKT_BYTE_22     0x22
#define PKT_BYTE_23     0x23
#define PKT_BYTE_30     0x30
    
#define PKT_BYTE_2     0x02
#define PKT_BYTE_3     0x03
#define PKT_BYTE_10    0x10
    
#define  TRUE   1
#define FALSE   0

typedef struct pkt_decoder pkt_decoder_t;

typedef void (*pkt_read_fn_t)(void *ctx, size_t len, const uint8_t *data);

// Constructor for a pkt_decoder
pkt_decoder_t* pkt_decoder_create(pkt_read_fn_t callback, void *callback_ctx);

// Destructor for a pkt_decoder
void pkt_decoder_destroy(pkt_decoder_t *decoder);

// Called on incoming, undecoded bytes to be translated into packets
void pkt_decoder_write_bytes(pkt_decoder_t *decoder, size_t len, const uint8_t *data);

#ifdef __cplusplus
}
#endif
#endif //PKT_DECODER_H_INCLUDED
