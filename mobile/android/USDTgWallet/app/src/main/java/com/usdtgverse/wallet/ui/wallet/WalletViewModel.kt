package com.usdtgverse.wallet.ui.wallet

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import java.math.BigDecimal
import java.math.RoundingMode

class WalletViewModel : ViewModel() {

    private val _balance = MutableLiveData<BigDecimal>().apply {
        value = BigDecimal("0.00")
    }
    val balance: LiveData<BigDecimal> = _balance

    private val _balanceUsd = MutableLiveData<BigDecimal>().apply {
        value = BigDecimal("0.00")
    }
    val balanceUsd: LiveData<BigDecimal> = _balanceUsd

    private val _isLoading = MutableLiveData<Boolean>().apply {
        value = false
    }
    val isLoading: LiveData<Boolean> = _isLoading

    fun loadWalletData() {
        _isLoading.value = true
        
        // Real blockchain integration - no demo data
        fetchRealBalanceFromBlockchain()
    }
    
    private fun fetchRealBalanceFromBlockchain() {
        // Fetch real balance from USDTgVerse blockchain
        val walletAddress = getWalletAddress()
        
        if (walletAddress.isEmpty()) {
            _balance.value = BigDecimal("0.00")
            _balanceUsd.value = BigDecimal("0.00")
            _isLoading.value = false
            return
        }
        
        val apiURL = "https://api.usdtgverse.com/api/v1/balance/$walletAddress/usdtg"
        
        // Use coroutines for network call
        Thread {
            try {
                val url = java.net.URL(apiURL)
                val connection = url.openConnection() as java.net.HttpURLConnection
                connection.requestMethod = "GET"
                connection.connectTimeout = 10000
                connection.readTimeout = 10000
                
                val responseCode = connection.responseCode
                if (responseCode == 200) {
                    val inputStream = connection.inputStream
                    val response = inputStream.bufferedReader().use { it.readText() }
                    
                    // Parse JSON response
                    val jsonObject = org.json.JSONObject(response)
                    val balance = jsonObject.getDouble("balance")
                    val usdValue = jsonObject.getDouble("usd_value")
                    
                    // Update UI on main thread
                    android.os.Handler(android.os.Looper.getMainLooper()).post {
                        _balance.value = BigDecimal(balance.toString()).setScale(2, RoundingMode.HALF_UP)
                        _balanceUsd.value = BigDecimal(usdValue.toString()).setScale(2, RoundingMode.HALF_UP)
                        _isLoading.value = false
                    }
                } else {
                    // Error handling
                    android.os.Handler(android.os.Looper.getMainLooper()).post {
                        _balance.value = BigDecimal("0.00")
                        _balanceUsd.value = BigDecimal("0.00")
                        _isLoading.value = false
                    }
                }
            } catch (e: Exception) {
                // Error handling
                android.os.Handler(android.os.Looper.getMainLooper()).post {
                    _balance.value = BigDecimal("0.00")
                    _balanceUsd.value = BigDecimal("0.00")
                    _isLoading.value = false
                }
            }
        }.start()
    }
    
    private fun getWalletAddress(): String {
        // Get wallet address from SharedPreferences
        val sharedPref = android.content.Context.MODE_PRIVATE
        return android.app.Activity().getSharedPreferences("wallet_prefs", sharedPref)
            .getString("wallet_address", "") ?: ""
    }

    fun refreshBalance() {
        loadWalletData()
    }
}