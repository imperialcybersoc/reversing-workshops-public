// upx packed challenge. other than that nothing new on the reversing side
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

const char *COMPANY = "Umbryx Corporation";
const char *TOOLNAME = "UmbryxMiner Activator v1.5";

bool check_license(const char *s) {
    // a license should be of the form XXXX-XXXX-XXXX-XXXX
    if (strlen(s) != 19) {
        return false;
    }
    
    if (s[4] != '-' || s[9] != '-' || s[14] != '-') {
        return false;
    }
    
    char seg1[5], seg2[5], seg3[5], seg4[5];
    strncpy(seg1, s, 4); seg1[4] = '\0';
    strncpy(seg2, s + 5, 4); seg2[4] = '\0';
    strncpy(seg3, s + 10, 4); seg3[4] = '\0';
    strncpy(seg4, s + 15, 4); seg4[4] = '\0';
    
    uint32_t sum = 0;
    for (int i = 0; i < 4; i++) {
        sum += (uint8_t)seg1[i];
        sum += (uint8_t)seg2[i];
        sum += (uint8_t)seg3[i];
    }
    
    uint32_t expected = (sum ^ 0xDEAD) & 0xFFFF;
    
    uint32_t provided = 0;
    if (sscanf(seg4, "%4X", &provided) != 1) {
        return false;
    }
    // the last segment acts as a checksum, the sum of the characters in the
    // first 3 segments xorred with 0xDEAD should be the last segment in hex
    return (provided == expected);
}

int main(void) {
    puts("===============================================");
    printf(" %s - %s\n", COMPANY, TOOLNAME);
    puts("===============================================");
    puts("This software is designed for OFFLINE activation of UmbryxMiner ONLY.");
    puts("");
    
    char license[64];
    printf("Enter your license key: ");
    
    if (fgets(license, sizeof(license), stdin) == NULL) {
        puts("\n[ERROR] Failed to read license key.");
        return 1;
    }
    
    size_t len = strlen(license);
    if (len > 0 && license[len-1] == '\n') {
        license[len-1] = '\0';
    }
    
    puts("");
    puts("Validating license key...");
    
    if (check_license(license)) {
        puts("[SUCCESS] License key is valid!");
        puts("UmbryxMiner has been activated.");
        puts("Thank you for your purchase!");
        return 0;
    } else {
        puts("[ERROR] Invalid license key.");
        puts("Please check your key and try again.");
        puts("Contact support@umbryx.corp for assistance.");
        return 1;
    }
}