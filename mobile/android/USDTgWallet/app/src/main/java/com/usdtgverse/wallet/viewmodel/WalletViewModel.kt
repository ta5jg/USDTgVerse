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
            
            // Real blockchain integration - no demo data
            val walletAddress = getWalletAddress()
            
            if (walletAddress.isEmpty()) {
                _uiState.value = _uiState.value.copy(
                    balance = "0.00",
                    tokens = emptyList(),
                    transactions = emptyList(),
                    isLoading = false
                )
                return@launch
            }
            
            // Fetch real data from blockchain
            val realTokens = fetchRealTokensFromBlockchain(walletAddress)
            val realTransactions = fetchRealTransactionsFromBlockchain(walletAddress)
            val realBalance = calculateTotalBalance(realTokens)
            
            _uiState.value = _uiState.value.copy(
                balance = realBalance,
                tokens = realTokens,
                transactions = realTransactions,
                isLoading = false
            )
        }
    }
    
    private fun getWalletAddress(): String {
        // Get wallet address from SharedPreferences
        val context = android.app.Activity()
        val sharedPref = context.getSharedPreferences("wallet_prefs", android.content.Context.MODE_PRIVATE)
        return sharedPref.getString("wallet_address", "") ?: ""
    }
    
    private suspend fun fetchRealTokensFromBlockchain(walletAddress: String): List<TokenData> {
        return try {
            val apiURL = "https://api.usdtgverse.com/api/v1/assets/$walletAddress"
            val response = ktorClient.get(apiURL)
            val jsonResponse = response.body<String>()
            val jsonObject = org.json.JSONObject(jsonResponse)
            val assetsArray = jsonObject.getJSONArray("assets")
            
            val tokens = mutableListOf<TokenData>()
            for (i in 0 until assetsArray.length()) {
                val asset = assetsArray.getJSONObject(i)
                val symbol = asset.getString("symbol")
                val balance = asset.getDouble("balance")
                val price = asset.getDouble("price")
                val change24h = asset.getDouble("change_24h")
                
                tokens.add(
                    TokenData(
                        symbol = symbol,
                        balance = String.format("%.2f", balance),
                        value = String.format("$%.2f", balance * price),
                        change24h = String.format("%+.1f%%", change24h)
                    )
                )
            }
            tokens
        } catch (e: Exception) {
            emptyList()
        }
    }
    
    private suspend fun fetchRealTransactionsFromBlockchain(walletAddress: String): List<TransactionData> {
        return try {
            val apiURL = "https://api.usdtgverse.com/api/v1/transactions/$walletAddress"
            val response = ktorClient.get(apiURL)
            val jsonResponse = response.body<String>()
            val jsonObject = org.json.JSONObject(jsonResponse)
            val transactionsArray = jsonObject.getJSONArray("transactions")
            
            val transactions = mutableListOf<TransactionData>()
            for (i in 0 until transactionsArray.length()) {
                val tx = transactionsArray.getJSONObject(i)
                val id = tx.getString("id")
                val type = tx.getString("type")
                val amount = tx.getString("amount")
                val timestamp = tx.getString("timestamp")
                val status = tx.getString("status")
                
                transactions.add(
                    TransactionData(
                        id = id,
                        type = type,
                        amount = amount,
                        timestamp = timestamp,
                        status = status
                    )
                )
            }
            transactions
        } catch (e: Exception) {
            emptyList()
        }
    }
    
    private fun calculateTotalBalance(tokens: List<TokenData>): String {
        var totalBalance = 0.0
        for (token in tokens) {
            val value = token.value.replace("$", "").replace(",", "").toDoubleOrNull() ?: 0.0
            totalBalance += value
        }
        return String.format("%.2f", totalBalance)
    }
    
    // Add ktor client for network requests
    private val ktorClient = io.ktor.client.HttpClient {
        install(io.ktor.client.plugins.contentnegotiation.ContentNegotiation) {
            io.ktor.serialization.kotlinx.json.json()
        }
    }

    fun refreshWalletData() {
        loadWalletData()
    }
}

