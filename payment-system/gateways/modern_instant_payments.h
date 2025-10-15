/*
==============================================
| File:        modern_instant_payments.h
| Author:      Irfan Gedik
| Created:     01.10.2025
| Last Update: 01.10.2025
| Version:     1.0
|
| Description:
|   USDTgVerse Modern Instant Payment Systems Header
|   
|   Integration with latest instant payment networks:
|   - FedNow, RTP, PIX, UPI, Faster Payments
|   - SEPA Instant, PayNow, PromptPay, FAST, Bizum
|
| License:
|   MIT License
==============================================
*/

#ifndef USDTGVERSE_MODERN_INSTANT_PAYMENTS_H
#define USDTGVERSE_MODERN_INSTANT_PAYMENTS_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Modern Instant Payment Systems
typedef enum {
    INSTANT_FEDNOW = 1,           // US Federal Reserve FedNow
    INSTANT_RTP = 2,              // US Real-Time Payments
    INSTANT_PIX = 3,              // Brazil PIX
    INSTANT_UPI = 4,              // India UPI
    INSTANT_FASTER_PAYMENTS = 5,  // UK Faster Payments
    INSTANT_SEPA_INSTANT = 6,     // EU SEPA Instant
    INSTANT_PAYNOW = 7,           // Singapore PayNow
    INSTANT_PROMPTPAY = 8,        // Thailand PromptPay
    INSTANT_FAST = 9,             // Australia FAST
    INSTANT_BIZUM = 10            // Spain Bizum
} instant_payment_system_t;

// Instant Payment Transaction Structure
typedef struct {
    char transaction_id[64];
    char sender_account[64];
    char receiver_account[64];
    char amount[32];
    char currency[8];
    char reference[128];
    instant_payment_system_t system;
    time_t timestamp;
    bool is_completed;
    char status_message[256];
} instant_payment_t;

// Function Declarations

// FedNow Integration
int fednow_send_payment(const char *sender_account,
                       const char *receiver_account,
                       const char *amount,
                       const char *reference);

// RTP Integration
int rtp_send_payment(const char *sender_account,
                    const char *receiver_account,
                    const char *amount,
                    const char *reference);

// PIX Integration
int pix_send_payment(const char *sender_account,
                    const char *receiver_account,
                    const char *amount,
                    const char *reference);

// UPI Integration
int upi_send_payment(const char *sender_vpa,
                    const char *receiver_vpa,
                    const char *amount,
                    const char *reference);

// Faster Payments Integration
int faster_payments_send(const char *sender_sort_code,
                        const char *receiver_sort_code,
                        const char *sender_account,
                        const char *receiver_account,
                        const char *amount,
                        const char *reference);

// SEPA Instant Integration
int sepa_instant_send(const char *debtor_iban,
                     const char *creditor_iban,
                     const char *amount,
                     const char *reference);

// PayNow Integration
int paynow_send_payment(const char *sender_mobile,
                       const char *receiver_mobile,
                       const char *amount,
                       const char *reference);

// PromptPay Integration
int promptpay_send_payment(const char *sender_mobile,
                          const char *receiver_mobile,
                          const char *amount,
                          const char *reference);

// Universal Instant Payment Handler
int process_instant_payment(instant_payment_t *payment);

// Utility Functions
const char* get_instant_payment_system_name(instant_payment_system_t system);
int init_modern_instant_payments(void);

#endif // USDTGVERSE_MODERN_INSTANT_PAYMENTS_H
