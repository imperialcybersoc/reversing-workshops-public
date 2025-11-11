/* ICCYBERSOC: Source code for reverse engineering workshop01, follows the
   "Umbryx" series. */

/* artefact01.c
   appears to be an UmbryxCorp operations dashboard from a contractor,
   but is actually a benign decoy. 
   Intended solution is to use strings to find a password.
*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define DEV_EMAIL_LENGTH 15

const char *COMPANY = "Umbryx Corporation";
const char *TOOLNAME = "Umbryx Ops Dashboard v2.4";
const char *TRAIN_PASSWORD = "umbryx-access-pass"; /* intentionally strings-able */
const char *DEC0Y_TOKEN = "token=UMB-DEC-001";
const char *NOTICE = "WARNING: This panel is read-only. Contact sec-team@umbryx.corp for issues.";

typedef enum {
    CMD_UNKNOWN,
    CMD_STATUS,
    CMD_TELEMETRY,
    CMD_ACTUATOR,
    CMD_FIRMWARE,
    CMD_TICKETS,
    CMD_HELP,
    CMD_LOGIN,
    CMD_EXIT
} cmd_t;

static bool logged_in = false; 

static int check_pass(const char *p) {
    if (!p) return 0;
    return (strcmp(p, TRAIN_PASSWORD) == 0);
}

static cmd_t parse_cmd(const char *s) {
    if (strcmp(s, "status") == 0) return CMD_STATUS;
    if (strcmp(s, "telemetry") == 0) return CMD_TELEMETRY;
    if (strcmp(s, "actuator") == 0) return CMD_ACTUATOR;
    if (strcmp(s, "firmware") == 0) return CMD_FIRMWARE;
    if (strcmp(s, "tickets") == 0) return CMD_TICKETS;
    if (strcmp(s, "help") == 0) return CMD_HELP;
    if (strcmp(s, "login") == 0) return CMD_LOGIN;
    if (strcmp(s, "exit") == 0) return CMD_EXIT;
    return CMD_UNKNOWN;
}

int main(void) {
    char buf[128];

    puts("========================================");
    printf(" %s - %s\n", COMPANY, TOOLNAME);
    puts("========================================");
    puts("Last sync: 2025-10-18 09:12:03 UTC");
    puts(NOTICE);
    puts("Available modules: [telemetry] [actuator] [firmware] [tickets] [login] [exit]");
    puts("Type 'status' to view recent alerts, or enter operator passphrase to view tokens.");

    cmd_t userSelection;
    for (;;) {
        printf("\nSelect module: ");

        if (!fgets(buf, sizeof(buf), stdin)) {
            puts("Read error");
            return 2;
        }
        /* strip newline */
        char *nl = strchr(buf, '\n');
        if (nl) *nl = '\0';

        userSelection = parse_cmd(buf);

        /* re-prompt to log in if attempting to use a cmd while logged-out */
        if ((!logged_in && userSelection != CMD_LOGIN) && userSelection != CMD_EXIT) {
            puts("Not logged in.");
            continue;
        }
        switch (userSelection) {
            case CMD_UNKNOWN:
                puts("Unknown command.");
                break;
            case CMD_STATUS:
                puts("\n-- Recent Alerts --");
                puts("2025-10-18T08:45: CPU anomaly on node: ubx-node-7");
                puts("2025-10-18T08:46: Suspicious outbound connection to 203.0.113.5:443 (investigate)");
                break;
            case CMD_TELEMETRY:
                puts("Node: healthy\nCPU Temp: 75C\nConnection: none");
                break;
            case CMD_ACTUATOR: 
                puts("Actuation mode not available in this build.");
                break;
            case CMD_FIRMWARE:
                puts("Firmware information not available in this build.");
                break;
            case CMD_HELP: {
                int dev_email_encode[] = {100, 101, 118, 64, 117, 109, 98, 114, 121, 120, 46, 99, 111, 114, 112};
                char email_reconstructed[DEV_EMAIL_LENGTH + 1];
                for (int i=0; i < DEV_EMAIL_LENGTH; ++i) {
                    email_reconstructed[i] = (char)dev_email_encode[i];
                }
                email_reconstructed[DEV_EMAIL_LENGTH] = '\0';
                printf("Help page not documented. Contact developer %s", email_reconstructed);
                break;
            }
            case CMD_TICKETS:
                puts("No tickets.");
                break;
            case CMD_LOGIN:
                printf("Enter PASS\n> ");

                if (!fgets(buf, sizeof(buf), stdin)) return 1;

                char *nl = strchr(buf, '\n');
                if (nl) *nl = '\0';

                if (check_pass(buf)) {
                    logged_in = true;
                    puts("\nOperator access: AUTHORIZED");
                    puts("Active token:");
                    puts(DEC0Y_TOKEN);
                } else {
                    puts("\nOperator access: DENIED");
                    return 1;
                }
                break;
            case CMD_EXIT:
                puts("Exiting...");
                return 0;
            } 
    }
}
