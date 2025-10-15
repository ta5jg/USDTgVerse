package com.usdtgverse.wallet.ui.trading

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel

class TradingViewModel : ViewModel() {

    private val _marketInfo = MutableLiveData<String>().apply {
        value = "Loading market data..."
    }
    val marketInfo: LiveData<String> = _marketInfo

    private val _isLoading = MutableLiveData<Boolean>().apply {
        value = false
    }
    val isLoading: LiveData<Boolean> = _isLoading

    fun loadMarketData() {
        _isLoading.value = true
        
        // Simulate loading market data
        // In a real app, this would fetch from trading API
        _marketInfo.value = "USDTgVerse Market\n\n" +
                "• Spot Trading: Available\n" +
                "• Margin Trading: Coming Soon\n" +
                "• Copy Trading: Coming Soon\n" +
                "• Derivatives: In Development"
        
        _isLoading.value = false
    }

    fun refreshMarketData() {
        loadMarketData()
    }
}