package com.usdtgverse.wallet.ui.pay

import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel

class PayViewModel : ViewModel() {

    private val _isLoading = MutableLiveData<Boolean>().apply {
        value = false
    }
    val isLoading: LiveData<Boolean> = _isLoading

    fun processPayment(amount: String, recipient: String) {
        _isLoading.value = true
        
        // Simulate payment processing
        // In a real app, this would process the payment
        
        _isLoading.value = false
    }

    fun generateQRCode(data: String): String {
        // Simulate QR code generation
        return "QR_CODE_DATA_FOR_$data"
    }
}