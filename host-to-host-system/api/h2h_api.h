/*
==============================================
| File:        h2h_api.h
| Author:      Irfan Gedik
| Created:     01.10.2025
| Last Update: 01.10.2025
| Version:     1.0
|
| Description:
|   USDTgVerse H2H API Header
|
| License:
|   MIT License
==============================================
*/

#ifndef USDTGVERSE_H2H_API_H
#define USDTGVERSE_H2H_API_H

#include <stdbool.h>

// API Response Structure
typedef struct {
    int status_code;
    char *message;
    char *message_id;
    char *data;
} H2HAPIResponse;

// Function Declarations
int h2h_api_init();
H2HAPIResponse* h2h_api_send_swift(const char *sender_bic, 
                                    const char *receiver_bic,
                                    const char *amount,
                                    const char *currency);
H2HAPIResponse* h2h_api_send_iso20022(const char *message_type,
                                       const char *xml_payload);
H2HAPIResponse* h2h_api_send_fix(const char *fix_message);
H2HAPIResponse* h2h_api_send_ach(const char *routing_number,
                                  const char *account_number,
                                  const char *amount);
H2HAPIResponse* h2h_api_send_sepa(const char *iban,
                                   const char *bic,
                                   const char *amount);
H2HAPIResponse* h2h_api_send_fedwire(const char *sender_aba,
                                      const char *receiver_aba,
                                      const char *amount);
H2HAPIResponse* h2h_api_get_status(const char *message_id);
void h2h_api_free_response(H2HAPIResponse *response);
void h2h_api_cleanup();

#endif // USDTGVERSE_H2H_API_H

