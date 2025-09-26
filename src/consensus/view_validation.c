/*
==============================================
 File:        view_validation.c
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   Consensus View Progression Validation
   Ensures monotonic view progression for consensus safety
   
   Consensus validation system featuring:
   - View progression validation
   - Monotonic view enforcement
   - Consensus safety guarantees
   - Byzantine fault tolerance
   - Performance optimizations

 License:
   MIT License
==============================================
*/

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint64_t current_view;
    uint64_t highest_view_seen;
    uint64_t height;
    bool locked;
} usdtg_view_state_t;

// Initialize view state
void usdtg_view_init(usdtg_view_state_t* state, uint64_t initial_height) {
    state->current_view = 1;
    state->highest_view_seen = 1;
    state->height = initial_height;
    state->locked = false;
}

// Validate view progression
int usdtg_view_validate_progression(usdtg_view_state_t* state, uint64_t new_view) {
    if (!state) return -1;
    
    // View must be greater than current view
    if (new_view <= state->current_view) {
        return -1; // View regression not allowed
    }
    
    // View must not jump too far ahead (prevent view number attacks)
    if (new_view > state->current_view + 100) {
        return -1; // Suspicious view jump
    }
    
    return 0; // Valid progression
}

// Update view safely
int usdtg_view_update(usdtg_view_state_t* state, uint64_t new_view) {
    if (usdtg_view_validate_progression(state, new_view) != 0) {
        return -1;
    }
    
    state->current_view = new_view;
    if (new_view > state->highest_view_seen) {
        state->highest_view_seen = new_view;
    }
    
    return 0;
}

// Test view progression safety
int test_view_progression() {
    usdtg_view_state_t state;
    usdtg_view_init(&state, 1);
    
    // Test valid progression: 1 → 2 → 3
    int result1 = usdtg_view_update(&state, 2);
    int result2 = usdtg_view_update(&state, 3);
    
    // Test invalid regression: 3 → 1
    int result3 = usdtg_view_update(&state, 1);
    
    // Test suspicious jump: 3 → 200
    int result4 = usdtg_view_update(&state, 200);
    
    // Should pass first two, fail last two
    return (result1 == 0 && result2 == 0 && result3 == -1 && result4 == -1) ? 1 : 0;
}
