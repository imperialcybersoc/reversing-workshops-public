/* ICCYBERSOC: Source code for reverse engineering workshop01, follows the
   "Umbryx" series */

/* artefact05.c
   A suspicious "relay client" supposedly communicating to relay servers,
   which don't really exist. Features a check for an environment variable
   printing the encoded "relay credential", the target for recovery in
   this challenge. 
    */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

const char *COMPANY = "Umbryx Corporation";
const char *TOOLNAME = "Umbryx Relay Client v1.2";

/* Encoded "relay key" */
static const uint8_t relay_table[] = {
    0x80, 0xE1, 0xD1, 0xC9, 0xE0, 0xC8, 0x91, 0xD0,
    0x50, 0xE3, 0x61, 0x19, 0x70, 0x00, 0x00, 0xF1,
    0xD9, 0x01
};

/* misleading “host list” */
static const char *fake_hosts[] = {
    "node-east.umbryx.corp",
    "node-west.umbryx.corp",
    "node-relay.umbryx.corp"
};

/* internal secret alphabet */
static const char alphabet[] =
    "Z$M0!Vx-9B@1Xr#o2f4yqL3wNs8K5j7TgQ6h+RcpHEtiCUAdJeFbvGlmnkSPauWYzOD";

/* constant for XOR */
static uint8_t xor_key = 0x3D;

/* build environment variable name at runtime */
char envname[16];
const char *parts[] = { "UM", "BRYX", "_", "DE", "BUG" };

/* first pass e*/
static void decode_stage1(uint8_t *buf, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        uint8_t b = buf[i];

        /* rotate right by 3 bits */
        b = (uint8_t) (((b >> 3) | (b << 5)) & 0xFF); 
        
        /* XOR each character with xor_key */
        buf[i] = b ^ xor_key;
    }
}

static void decode_stage2(uint8_t *buf, size_t n, char *out) {
    for (size_t i = 0; i < n; ++i) {
        uint8_t idx = (buf[i] + i) % (sizeof(alphabet) - 1);
        out[i] = alphabet[idx];
    }
    out[n] = '\0';
}

int main(void) {
    puts("===============================================");
    printf(" %s - %s\n", COMPANY, TOOLNAME);
    puts("===============================================");
    puts("Establishing relay uplink...");
    sleep(8);
    printf("Connected to node: %s\n", fake_hosts[rand() % 3]);
    puts("Synchronizing configuration table...\n");

    size_t n = sizeof(relay_table);
    uint8_t *work = malloc(n);
    if (!work) return 1;
    memcpy(work, relay_table, n);

    decode_stage1(work, n);

    char decoded[n + 1];
    decode_stage2(work, n, decoded);

    envname[0] = '\0';
    for (size_t i = 0; i < sizeof(parts)/sizeof(parts[0]); ++i)
        strncat(envname, parts[i], sizeof(envname)-strlen(envname)-1);

    /* figure out env var check and recover creds */
    if (getenv(envname)) {
        printf("Recovered relay credential: %s\n", decoded);
    } else {
        puts("Relay configuration verified.");
    }

    free(work);
    return 0;
}