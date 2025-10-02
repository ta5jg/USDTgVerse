package com.usdtgverse.wallet.navigation

import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.navigation.NavHostController
import androidx.comavigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.currentBackStackEntryAsState
import androidx.navigation.compose.rememberNavController
import androidx.compose.material3.*
import androidx.compose.runtime.getValue
import com.usdtgverse.wallet.viewmodel.WalletViewModel
import com.usdtgverse.wallet.ui.screens.WalletScreen
import com.usdtgverse.wallet.ui.screens.ODIXPAYScreen
import com.usdtgverse.wallet.ui.screens.TradingScreen

sealed class Screen(val route: String, val title: String, val icon: androidx.compose.ui.graphics.vector.ImageVector) {
    object Wallet : Screen("wallet", "Wallet", Icons.Default.AccountBalanceWallet)
    object ODIXPAY : Screen("odixpay", "ODIX PAY", Icons.Default.CreditCard)
    object Trading : Screen("trading", "Trading", Icons.Default.TrendingUp)
}

@Composable
fun USDTgWalletNavigation(
    navController: NavHostController = rememberNavController(),
    walletViewModel: WalletViewModel,
    modifier: Modifier = Modifier
) {
    val items = listOf(
        Screen.Wallet,
        Screen.ODIXPAY,
        Screen.Trading
    )

    Scaffold(
        bottomBar = {
            NavigationBar {
                val navBackStackEntry by navController.currentBackStackEntryAsState()
                val currentRoute = navBackStackEntry?.destination?.route

                items.forEach { screen ->
                    NavigationBarItem(
                        icon = { Icon(screen.icon, contentDescription = screen.title) },
                        label = { Text(screen.title) },
                        selected = currentRoute == screen.route,
                        onClick = {
                            navController.navigate(screen.route) {
                                popUpTo(navController.graph.startDestinationId) {
                                    saveState = true
                                }
                                launchSingleTop = true
                                restoreState = true
                            }
                        }
                    )
                }
            }
        }
    ) { innerPadding ->
        NavHost(
            navController = navController,
            startDestination = Screen.Wallet.route,
            modifier = modifier.padding(innerPadding)
        ) {
            composable(Screen.Wallet.route) {
                WalletScreen(walletViewModel = walletViewModel)
            }
            composable(Screen.ODIXPAY.route) {
                ODIXPAYScreen()
            }
            composable(Screen.Trading.route) {
                TradingScreen()
            }
        }
    }
}

