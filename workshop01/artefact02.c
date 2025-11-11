/* ICCYBERSOC: Source code for reverse engineering workshop01, follows the
   "Umbryx" series */

/* artefact02.c
   Supposedly a "Umbryx Updater" binary dropped by a suspicious contractor.
   Intended solution is to use a static analysis tool to identify XOR_KEY 
   and use it to decode the text.
*/

#include <stdio.h>
#include <stdint.h>

const char *COMPANY = "Umbryx Corporation";
const char *TOOLNAME = "Umbryx Updater v3.0";

/* XOR key */
const uint8_t XOR_KEY = 0x1A;

/* Hidden token pre-XOR'd (UMBRYX-SEC-OP-TOK-GJAK) */
const uint8_t ENCODED_TOKEN[] = {
    0x4f, 0x57, 0x58, 0x48, 0x43, 0x42, 
    0x37, 0x49, 0x5f, 0x59, 0x37, 0x55, 
    0x4a, 0x37, 0x4e, 0x55, 0x51, 0x37, 
    0x5d, 0x50, 0x5b, 0x51
};

int main(void) {
    /* Find length of ENCODED_TOKEN */
    size_t n = sizeof(ENCODED_TOKEN)/sizeof(ENCODED_TOKEN[0]);
    uint8_t decoded[n + 1];

    /* Theming fluff */
    puts("========================================");
    printf(" %s - %s\n", COMPANY, TOOLNAME);
    puts("========================================");
    puts("Initializing environment...");
    puts("Checking system integrity... done.");
    puts("No immediate issues detected.\n");

    /* Encodes token by XOR */
    for (size_t i = 0; i < n; ++i) {
        decoded[i] = ENCODED_TOKEN[i] ^ XOR_KEY;
    }
    decoded[n] = '\0';

    return 0;
}
