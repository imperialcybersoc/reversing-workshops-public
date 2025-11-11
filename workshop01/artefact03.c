#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "lib/aes.h"

const char *COMPANY = "Umbryx Corporation";
const char *TOOLNAME = "UmbryxMiner v4.2";

// this is just for obfuscation purposes
unsigned char *sleep(
    const uint8_t *data, size_t datalen
)
{
    uint8_t key[16] = {
        0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,
        0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff
    };

    if (!data || datalen == 0 || (datalen % 16) != 0)
        return NULL;

    uint8_t iv[16] = {0};

    unsigned char *buf = malloc(datalen + 1);
    if (!buf) return NULL;
    memcpy(buf, data, datalen);

    struct AES_ctx ctx;
    AES_init_ctx_iv(&ctx, key, iv);
    AES_ECB_decrypt(&ctx, buf);

    uint8_t pad = datalen;
    while (pad > 0 && buf[pad - 1] == 0x00) {
        --pad;
    }

    size_t plen = pad;
    unsigned char *plain = malloc(plen + 1);
    if (!plain) { free(buf); return NULL; }

    memcpy(plain, buf, plen);
    plain[plen] = '\0';
    free(buf);

    return plain;
}

// UBX_MINER_KEY
const uint8_t data_1[] = {
    200, 189, 34, 184, 167, 74, 98, 54, 93, 102, 231, 8, 251, 223, 147, 114
};

// ./config_u8x
const uint8_t data_2[] = {
    217, 79, 128, 178, 209, 71, 244, 31, 248, 191, 194, 218, 94, 241, 29, 251
};

// UMBRIX\n
const uint8_t data_3[] = {
    248, 84, 156, 76, 242, 170, 59, 142, 181, 176, 22, 244, 110, 107, 89, 119
};

const uint8_t data_4[] = {
    25, 74, 5, 189, 240, 240, 182, 24, 144, 24, 111, 254, 225, 245, 244, 28
};

const uint8_t data_5[] = {
    38, 83, 196, 64, 75, 113, 203, 221, 48, 192, 191, 22, 38, 119, 22, 138
};

int main(void) {
    puts("===============================================");
    printf(" %s - %s\n", COMPANY, TOOLNAME);
    puts("===============================================");
    puts("This software is designed for OFFLINE activation of UmbryxMiner ONLY.");
    puts("");

    unsigned char *pt_1 = sleep(data_1, sizeof(data_1));
    if (!getenv((char*)pt_1)) {
        printf("Missing MINER_KEY.\n");
        printf("Terminating.\n");
        exit(0);
    };
    free(pt_1);

    unsigned char *pt_2 = sleep(data_2, sizeof(data_2));

    FILE *fptr;
    if (!(fptr = fopen((char*)pt_2, "r"))) {
        printf("Configuration file at ./config_ubx missing.\n");
        printf("Terminating.\n");
        exit(0);
    }
    free(pt_2);

    char config_header[8];
    fgets(config_header, 8, fptr);
    int res = 0;
    unsigned char *pt_3 = sleep(data_3, sizeof(data_3));
    res = strcmp(config_header, (char*) pt_3);
    free(pt_3);

    if (res != 0) {
        printf("Header incorrect (expected UMBRYX)\n");
        printf("Terminating.\n");
        exit(0);
    }
    fclose(fptr); 
    unsigned char *pt_4 = sleep(data_4, sizeof(data_4));

    printf("Hello %s\n", (char*) pt_4);
    unsigned char *pt_5 = sleep(data_5, sizeof(data_5));
    printf("Your wallet is located at address: %s\n", (char*) pt_5);
    free(pt_4);
    free(pt_5);
    return 0;
}