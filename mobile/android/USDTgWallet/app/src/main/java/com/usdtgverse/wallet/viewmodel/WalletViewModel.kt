package com.usdtgverse.wallet.viewmodel

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch
import javax.inject.Inject

data class WalletState(
    val balance: String = "0.00",
    val isLoading: Boolean = false,
    val tokens: List<TokenData> = emptyList(),
    val transactions: List<TransactionData> = emptyList()
)

data class TokenData(
    val symbol: String,
    val balance: String,
    val value: String,
    val change24h: String
)

data class TransactionData(
    val id: String,
    val type: String,
    val amount: String,
    val timestamp: String,
    val status: String
)

@HiltViewModel
class WalletViewModel @Inject constructor() : ViewModel() {
    private val _uiState = MutableStateFlow(WalletState())
    val uiState: StateFlow<WalletState> = _uiState.asStateFlow()

    init {
        loadWalletData()
    }

    private fun loadWalletData() {
        viewModelScope.launch {
            _uiState.value = _uiState.value.copy(isLoading = true)
            
            // Simulate loading wallet data
            val mockTokens = listOf(
                TokenData("USDTg", "1,250.00", "$1,250.00", "+2.5%"),
                TokenData("USDTgV", "500.00", "$498.50", "-1.2%"),
                
                TokenData("USDTgG", "750.00", "$752.25", "+1.8%"),
                TokenData("OZBC", "100.00", "$256.00", "+3.4%")
            )
            
            val mockTransactions = listOf(
                TransactionData("tx001", "Receive", "+500.00 USDTg", "2 hours ago", "Confirmed"),
                TransactionData("tx002", "Send", "-100.00 USDTgV", "1 day ago", "Confirmed"),
                TransactionData("tx003", "Swap", "100 USDTg → USDTgG", "3 days ago", "Confirmed")
            )
            
            _uiState.value = _uiState.value.copy(
                balance = "2,756.75",
                tokens = mockTokens,
                transactions = mockTransactions,
                isLoading = false
            )
        }
    }

    fun refreshWalletData() {
        loadWalletData()
    }
}

