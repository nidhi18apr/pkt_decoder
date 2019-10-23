//
//  main.c
//  test_decoder
//
//  Created by Nidhi on 10/22/19.
//  Copyright © 2019 Nidhi. All rights reserved.
//

#include <stdio.h>
#include "pkt_decoder.h"


static void pkt_printer(void *ctx, size_t len, const uint8_t *data) {
    (void)ctx;
    printf("pkt (%zd bytes) -", len);
    for(size_t i = 0; i < len; i++)
    {
        printf(" %02x", data[i]);
    }
    printf("\n");
    
}

void test_case_1(){
    const uint8_t pkt1[] = {0x02,0x11,0x12,0x13,0x14,0x15,0x03 };
    pkt_decoder_t* decoder = pkt_decoder_create(pkt_printer, NULL);
    pkt_decoder_write_bytes(decoder, sizeof(pkt1), pkt1);
    pkt_decoder_destroy(decoder);
}

void test_case_2(){
    pkt_decoder_t* decoder = pkt_decoder_create(pkt_printer, NULL);
    pkt_decoder_write_bytes(decoder, 0, 0);
    pkt_decoder_destroy(decoder);
}
void test_case_3(){
    const uint8_t pkt1[] = {0x02};
    pkt_decoder_t* decoder = pkt_decoder_create(pkt_printer, NULL);
    pkt_decoder_write_bytes(decoder, sizeof(pkt1), pkt1);
    pkt_decoder_destroy(decoder);
}
void test_case_4(){
    const uint8_t pkt1[] = {0x03};
    pkt_decoder_t* decoder = pkt_decoder_create(pkt_printer, NULL);
    pkt_decoder_write_bytes(decoder, sizeof(pkt1), pkt1);
    pkt_decoder_destroy(decoder);
}
void test_case_5(){
    const uint8_t pkt1[] = {0x09};
    pkt_decoder_t* decoder = pkt_decoder_create(pkt_printer, NULL);
    pkt_decoder_write_bytes(decoder, sizeof(pkt1), pkt1);
    pkt_decoder_destroy(decoder);
}
void test_case_6(){
    const uint8_t pkt1[] = {0x02,0x03};
    pkt_decoder_t* decoder = pkt_decoder_create(pkt_printer, NULL);
    pkt_decoder_write_bytes(decoder, sizeof(pkt1), pkt1);
    pkt_decoder_destroy(decoder);
}
void test_case_7(){
    const uint8_t pkt1[] = {0x02,0x11,0x03 };
    pkt_decoder_t* decoder = pkt_decoder_create(pkt_printer, NULL);
    pkt_decoder_write_bytes(decoder, sizeof(pkt1), pkt1);
    pkt_decoder_destroy(decoder);
}
void test_case_8(){
    const uint8_t pkt1[] = {0x02,0x11,0x12,0x13,0x14,0x15,0x30 };
    pkt_decoder_t* decoder = pkt_decoder_create(pkt_printer, NULL);
    pkt_decoder_write_bytes(decoder, sizeof(pkt1), pkt1);
    pkt_decoder_destroy(decoder);
}
void test_case_9(){
    uint8_t pkt1[MAX_DECODED_PACKET_LEN+2] ;
    pkt1[0] = 0x02 ;
    pkt1[MAX_DECODED_PACKET_LEN +1] = 0x03 ;
    for (int i = 1 ; i<MAX_DECODED_PACKET_LEN +1; i++) {
        pkt1[i] = 0x12 ;
    }
    pkt_decoder_t* decoder = pkt_decoder_create(pkt_printer, NULL);
    pkt_decoder_write_bytes(decoder, sizeof(pkt1), pkt1);
    pkt_decoder_destroy(decoder);
}
void test_case_10(){
    const uint8_t pkt1[] = {0x02, 0xFF, 0x10, 0x22, 0x45, 0x03 , 0x12 , 0x02, 0xFF, 0x24, 0x45, 0x43 , 0x44};
    const uint8_t pkt2[] = {0x45,0x03,0x02, 0x6, 0x34, 0x45, 0x03 , 0x21 , 0x02, 0xFa, 0x24, 0x45, 0x03 };
    const uint8_t pkt3[] = {0x02, 0x06,0x05,  0x04, 0x10};
    const uint8_t pkt4[] = {0x30, 0x45, 0x03 };
    
    pkt_decoder_t* decoder = pkt_decoder_create(pkt_printer, NULL);
    pkt_decoder_write_bytes(decoder, sizeof(pkt1), pkt1);
    pkt_decoder_write_bytes(decoder, sizeof(pkt2), pkt2);
    pkt_decoder_write_bytes(decoder, sizeof(pkt3), pkt3);
    pkt_decoder_write_bytes(decoder, sizeof(pkt4), pkt4);
    pkt_decoder_destroy(decoder);
}
// Wrong byte after 0x10
void test_case_11(){
    const uint8_t pkt1[] = {0x02, 0xFF, 0x10, 0x25, 0x45, 0x03};
    
    pkt_decoder_t* decoder = pkt_decoder_create(pkt_printer, NULL);
    pkt_decoder_write_bytes(decoder, sizeof(pkt1), pkt1);
    pkt_decoder_destroy(decoder);
}


int main() {
    
    test_case_1();
    //test_case_2();
    test_case_3();
    test_case_4();
    test_case_5();
    test_case_6();
    test_case_7();
    test_case_8();
    test_case_9();
    test_case_10();
    test_case_11();
    return 0;
}
