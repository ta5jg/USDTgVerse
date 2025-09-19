package com.usdtgverse.wallet

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.navigation.compose.rememberNavController
import com.usdtgverse.wallet.navigation.USDTgWalletNavigation
import com.usdtgverse.wallet.ui.theme.USDTgWalletTheme
import com.usdtgverse.wallet.viewmodel.WalletViewModel
import dagger.hilt.android.AndroidEntryPoint

@AndroidEntryPoint
class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        
        setContent {
            USDTgWalletTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    USDTgWalletApp()
                }
            }
        }
    }
}

@Composable
fun USDTgWalletApp() {
    val navController = rememberNavController()
    val walletViewModel: WalletViewModel = hiltViewModel()
    
    Scaffold(
        modifier = Modifier.fillMaxSize()
    ) { innerPadding ->
        USDTgWalletNavigation(
            navController = navController,
            walletViewModel = walletViewModel,
            modifier = Modifier.padding(innerPadding)
        )
    }
}

@Preview(showBackground = true)
@Composable
fun USDTgWalletAppPreview() {
    USDTgWalletTheme {
        USDTgWalletApp()
    }
}
