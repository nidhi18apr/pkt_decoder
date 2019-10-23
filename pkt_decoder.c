//
//  pkt_decoder.c
//  pkt_decoder
//
//  Created by Nidhi on 10/22/19.
//  Copyright © 2019 Nidhi. All rights reserved.
//

#include "pkt_decoder.h"
#include <stdio.h>
#include "assert.h"

struct pkt_decoder {
    pkt_read_fn_t   callback; /* callback for printing / processing decoded data */
    void *          callback_ctx; /* context for passing parameters to callback function */
    uint8_t         decoded_frame[MAX_DECODED_PACKET_LEN]; /* Decoded data   */
    uint8_t         encoded_frame[MAX_ENCODED_PACKET_LEN ] ; /* Input / Encoded data */
    int             curr_stx_index; /* current stx index in decoder array */
    int             current_decoder_index; /* current index in decoder array */
    int             stx_found ; /* set after STX byte is found */
    int             last_byte_was_DLE ;
} ;


// Constructor for a pkt_decoder
pkt_decoder_t* pkt_decoder_create(pkt_read_fn_t callback, void *callback_ctx){
    assert(callback); // Callback should not be NULL
    // Allocate Memory and initialize parameters
    pkt_decoder_t* decoder = ( pkt_decoder_t* )calloc(1, sizeof(pkt_decoder_t));
    decoder->callback = callback ;
    decoder->callback_ctx = callback_ctx ;
    return decoder ;
}

// Destructor for a pkt_decoder
void pkt_decoder_destroy(pkt_decoder_t *decoder){
    free(decoder);
    decoder = NULL ;
}


/**************************************************************************************************************
 * @fn        get_decoded_byte
 * @param     enc_byte   encoded byte
 * @brief     This function returns decoded byte.
 *              Encoding algorithm :If an 0x02 , 0x03 , 0x10 appears inside a packet it is escaped with  (0x10, 0x20 | value)
 *                                  So replace 0x02 with 0x10 , 0x22
 *                                             0x03 with 0x10 , 0x23
 *                                             0x10 with 0x10 , 0x30
 *              get_decoded_byte is returning corresponding decoded byte.
 *@return       decoded byte.
 *************************************************************************************************************/
uint8_t get_decoded_byte(uint8_t enc_byte){
    uint8_t dec_byte ;
    switch (enc_byte) {
        case PKT_BYTE_22:
            dec_byte = PKT_BYTE_2;
            break;
        case PKT_BYTE_23:
            dec_byte = PKT_BYTE_3;
            break;
        case PKT_BYTE_30:
            dec_byte = PKT_BYTE_10;
            break;
        default:
            dec_byte = enc_byte ;
            break;
    }
    return dec_byte ;
}
/**************************************************************************************************************
 * @fn        pkt_decoder_write_bytes
 * @param     decoder   pointer to decoder
 *            len       length of encoded data
 *            data      encoded data bytes
 * @brief     This function is called on incoming, undecoded bytes to be translated into packets.
 *            Algorithm :
 *            1.  For each byte in input(data) array
 *                1.1 If its STX , set stx_found = true and continue to next iteration
 *                1.2 Else If its ETX  and stx_found is true
 *                      1.2.1 Print decoded packets
 *                      1.2.2 Set stx_found to FALSE
 *                      1.2.2 Recurse algorithm for left over bytes if any
 *                1.3 If stx_found is true , decode bytes
 *                      1.3.1 if current byte is DLE , set  last_byte_was_DLE = TRUE
 *                      1.3.2 if last_byte_was_DLE is TRUE, then decode byte
 *                               1.3.1.1 If its 0x22 , set decoded byte to 0x02
 *                               1.3.1.2 If its 0x23 , set decoded byte to 0x03
 *                               1.3.1.3 If its 0x30 , set decoded byte to 0x10
 *                               1.3.1.4 If its some other byte , copy as it is.
 *                      1.3.2 Else copy byte as it is
 *************************************************************************************************************/
void pkt_decoder_write_bytes(pkt_decoder_t *decoder, size_t len, const uint8_t *data){
   
    assert(decoder);
    assert(decoder->callback);
    assert(data);
    
    for (int data_index = 0; (data_index < len) && (decoder->current_decoder_index <= MAX_DECODED_PACKET_LEN); data_index++) {
       
        if (data[data_index] == STX) { /* Step 1.1 */
            decoder->stx_found = TRUE;
            decoder->curr_stx_index = decoder->current_decoder_index ;
            continue ;
        }
        else if ((data[data_index] == ETX)&&( decoder->stx_found == TRUE)) { /* Step 1.2 */
            data_index++;
            // ETX found , call callback function to process/ print data
            decoder->callback(decoder->callback_ctx , (decoder->current_decoder_index - decoder->curr_stx_index),&(decoder->decoded_frame[decoder->curr_stx_index]));
            // Reset stx_found
            decoder->stx_found = FALSE ;
            // Recurse function for left over data if any
            if (data_index < len ) {
                pkt_decoder_write_bytes(decoder , len - data_index , &data[data_index]);
            }
            break ;
        }
        if ( decoder->stx_found == TRUE) { /* Step 1.3 */
            if ((data[data_index] == DLE)){
                    decoder->last_byte_was_DLE = TRUE ;
            }
            else if (decoder->last_byte_was_DLE == TRUE){
                // last byte was DLE , so decode this byte.
                decoder->decoded_frame[decoder->current_decoder_index++] = get_decoded_byte(data[data_index ]) ;
                decoder->last_byte_was_DLE = FALSE;
            }
            else{
                decoder->decoded_frame[decoder->current_decoder_index++] = data[data_index] ;
            }
        }
    }
}

