package com.usdtgverse.wallet.ui.pay

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.usdtgverse.wallet.R

class PayFragment : Fragment() {

    private lateinit var payViewModel: PayViewModel
    private lateinit var btnScanQr: Button
    private lateinit var btnSendPayment: Button
    private lateinit var btnRequestPayment: Button

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        payViewModel = ViewModelProvider(this)[PayViewModel::class.java]
        val root = inflater.inflate(R.layout.fragment_pay, container, false)
        
        // Initialize views
        btnScanQr = root.findViewById(R.id.btn_scan_qr)
        btnSendPayment = root.findViewById(R.id.btn_send_payment)
        btnRequestPayment = root.findViewById(R.id.btn_request_payment)
        
        // Set up click listeners
        btnScanQr.setOnClickListener {
            showToast("QR Scanner coming soon!")
        }
        
        btnSendPayment.setOnClickListener {
            showToast("Send Payment coming soon!")
        }
        
        btnRequestPayment.setOnClickListener {
            showToast("Request Payment coming soon!")
        }
        
        return root
    }
    
    private fun showToast(message: String) {
        android.widget.Toast.makeText(context, message, android.widget.Toast.LENGTH_SHORT).show()
    }
}