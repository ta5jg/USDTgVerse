/*
==============================================
| File:        payment_api.h
| Author:      Irfan Gedik
| Created:     01.10.2025
| Last Update: 01.10.2025
| Version:     1.0
|
| Description:
|   USDTgVerse Payment API Header
|
| License:
|   MIT License
==============================================
*/

#ifndef USDTGVERSE_PAYMENT_API_H
#define USDTGVERSE_PAYMENT_API_H

#include <stdbool.h>

// API Response Structure
typedef struct {
    int status_code;
    char *message;
    char *transaction_id;
    char *data;
} APIResponse;

// Function Declarations
int payment_api_init();
APIResponse* payment_api_process(const char *amount, const char *currency, 
                                  const char *method, const char *customer_id);
APIResponse* payment_api_get_status(const char *transaction_id);
bool payment_api_validate_method(const char *method);
APIResponse* payment_api_refund(const char *transaction_id, const char *amount);
int payment_api_webhook(const char *event_type, const char *payload);
void payment_api_free_response(APIResponse *response);
void payment_api_cleanup();

#endif // USDTGVERSE_PAYMENT_API_H

