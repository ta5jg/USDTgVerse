package com.usdtgverse.wallet.ui.wallet

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.usdtgverse.wallet.R

class WalletFragment : Fragment() {

    private lateinit var walletViewModel: WalletViewModel
    private lateinit var balanceAmount: TextView
    private lateinit var balanceUsd: TextView
    private lateinit var btnSend: Button
    private lateinit var btnReceive: Button

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        walletViewModel = ViewModelProvider(this)[WalletViewModel::class.java]
        val root = inflater.inflate(R.layout.fragment_wallet, container, false)
        
        // Initialize views
        balanceAmount = root.findViewById(R.id.balance_amount)
        balanceUsd = root.findViewById(R.id.balance_usd)
        btnSend = root.findViewById(R.id.btn_send)
        btnReceive = root.findViewById(R.id.btn_receive)
        
        // Set up click listeners
        btnSend.setOnClickListener {
            showToast("Send functionality coming soon!")
        }
        
        btnReceive.setOnClickListener {
            showToast("Receive functionality coming soon!")
        }
        
        // Load wallet data
        loadWalletData()
        
        return root
    }
    
    private fun loadWalletData() {
        // Real blockchain integration - no demo data
        walletViewModel.loadWalletData()
        
        // Observe real balance data
        walletViewModel.balance.observe(viewLifecycleOwner) { balance ->
            balanceAmount.text = "${balance} USDTg"
        }
        
        walletViewModel.balanceUsd.observe(viewLifecycleOwner) { balanceUsd ->
            balanceUsd.text = "$${balanceUsd} USD"
        }
    }
    
    private fun showToast(message: String) {
        android.widget.Toast.makeText(context, message, android.widget.Toast.LENGTH_SHORT).show()
    }
}