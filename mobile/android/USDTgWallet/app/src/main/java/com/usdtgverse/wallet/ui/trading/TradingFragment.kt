package com.usdtgverse.wallet.ui.trading

import android.os.Bundle
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.TextView
import androidx.fragment.app.Fragment
import androidx.lifecycle.ViewModelProvider
import com.usdtgverse.wallet.R

class TradingFragment : Fragment() {

    private lateinit var tradingViewModel: TradingViewModel
    private lateinit var btnSpotTrading: Button
    private lateinit var btnMarginTrading: Button
    private lateinit var btnCopyTrading: Button
    private lateinit var marketInfo: TextView

    override fun onCreateView(
        inflater: LayoutInflater,
        container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        tradingViewModel = ViewModelProvider(this)[TradingViewModel::class.java]
        val root = inflater.inflate(R.layout.fragment_trading, container, false)
        
        // Initialize views
        btnSpotTrading = root.findViewById(R.id.btn_spot_trading)
        btnMarginTrading = root.findViewById(R.id.btn_margin_trading)
        btnCopyTrading = root.findViewById(R.id.btn_copy_trading)
        marketInfo = root.findViewById(R.id.market_info)
        
        // Set up click listeners
        btnSpotTrading.setOnClickListener {
            // Navigate to DEX
            showToast("Opening USDTgDEX...")
            // TODO: Open DEX webview or navigate to DEX activity
        }
        
        btnMarginTrading.setOnClickListener {
            showToast("Margin Trading coming soon!")
        }
        
        btnCopyTrading.setOnClickListener {
            showToast("Copy Trading coming soon!")
        }
        
        // Load market data
        loadMarketData()
        
        return root
    }
    
    private fun loadMarketData() {
        marketInfo.text = "USDTgVerse Market\n\n" +
                "• Spot Trading: Available\n" +
                "• Margin Trading: Coming Soon\n" +
                "• Copy Trading: Coming Soon"
    }
    
    private fun showToast(message: String) {
        android.widget.Toast.makeText(context, message, android.widget.Toast.LENGTH_SHORT).show()
    }
}