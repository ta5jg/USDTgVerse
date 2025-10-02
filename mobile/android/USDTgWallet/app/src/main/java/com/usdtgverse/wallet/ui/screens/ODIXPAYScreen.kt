package com.usdtgverse.wallet.ui.screens

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.LazyRow
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.foundation.lazy.items
import androidx.compose.foundation.shape.CircleShape
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.*
import androidx.compose.material3.*
import androidx.compose.runtime.*
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Brush
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.usdtgverse.wallet.ui.theme.USDTgColors
import com.usdtgverse.wallet.ui.theme.USDTgGradients
import com.usdtgverse.wallet.viewmodel.ODIXPAYViewModel

enum class ODIXPAYFeature(
    val title: String,
    val description: String,
    val icon: ImageVector,
    val color: Color
) {
    SEND_PAYMENT(
        "Send Payment",
        "Send payments instantly",
        Icons.Default.Send,
        USDTgColors.Success
    ),
    RECEIVE_PAYMENT(
        "Receive Payment",
        "Receive payments securely",
        Icons.Default.CallReceived,
        USDTgColors.Info
    ),
    PAYMENT_HISTORY(
        "Payment History",
        "View transaction history",
        Icons.Default.History,
        USDTgColors.Analytics
    ),
    MERCHANT_PAYMENTS(
        "Merchant Payments",
        "Pay merchants worldwide",
        Icons.Default.Store,
        USDTgColors.Warning
    ),
    BILL_PAY(
        "Bill Pay",
        "Pay bills automatically",
        Icons.Default.Receipt,
        USDTgColors.Error
    ),
    SUBSCRIPTIONS(
        "Subscriptions",
        "Manage subscriptions",
        Icons.Default.Subscriptions,
        USDTgColors.Success
    ),
    INTERNATIONAL_TRANSFER(
        "International Transfer",
        "Global money transfers",
        Icons.Default.Public,
        USDTgColors.Info
    ),
    CRYPTO_PAYMENTS(
        "Crypto Payments",
        "Crypto payment gateway",
        Icons.Default.CurrencyBitcoin,
        USDTgColors.Warning
    ),
    QUANTUM_SAFE_PAY(
        "Quantum-Safe Pay",
        "Quantum-safe transactions",
        Icons.Default.Security,
        USDTgColors.Analytics
    )
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ODIXPAYScreen(
    viewModel: ODIXPAYViewModel = hiltViewModel(),
    onFeatureClick: (ODIXPAYFeature) -> Unit = {},
    onSendPaymentClick: () -> Unit = {},
    onReceivePaymentClick: () -> Unit = {}
) {
    val uiState by viewModel.uiState.collectAsStateWithLifecycle()
    
    LazyColumn(
        modifier = Modifier
            .fillMaxSize()
            .background(
                brush = Brush.verticalGradient(USDTgGradients.BackgroundGradient)
            ),
        contentPadding = PaddingValues(16.dp),
        verticalArrangement = Arrangement.spacedBy(16.dp)
    ) {
        item {
            ODIXPAYHeader(
                totalBalance = uiState.totalBalance,
                usdtgBalance = uiState.usdtgBalance,
                usdtgvBalance = uiState.usdtgvBalance,
                usdtggBalance = uiState.usdtggBalance
            )
        }
        
        item {
            QuickActionsRow(
                onSendClick = onSendPaymentClick,
                onReceiveClick = onReceivePaymentClick,
                onScanClick = { /* Handle QR scan */ },
                onHistoryClick = { onFeatureClick(ODIXPAYFeature.PAYMENT_HISTORY) }
            )
        }
        
        item {
            Text(
                text = "ODIX PAY Features",
                style = MaterialTheme.typography.headlineSmall,
                color = Color.White,
                modifier = Modifier.padding(vertical = 8.dp)
            )
        }
        
        item {
            LazyVerticalGrid(
                columns = GridCells.Fixed(2),
                horizontalArrangement = Arrangement.spacedBy(12.dp),
                verticalArrangement = Arrangement.spacedBy(12.dp),
                modifier = Modifier.height(400.dp)
            ) {
                items(ODIXPAYFeature.values()) { feature ->
                    FeatureCard(
                        feature = feature,
                        onClick = { onFeatureClick(feature) }
                    )
                }
            }
        }
        
        item {
            Text(
                text = "Recent Transactions",
                style = MaterialTheme.typography.headlineSmall,
                color = Color.White,
                modifier = Modifier.padding(vertical = 8.dp)
            )
        }
        
        items(uiState.recentTransactions.take(5)) { transaction ->
            TransactionCard(transaction = transaction)
        }
    }
}

@Composable
fun ODIXPAYHeader(
    totalBalance: Double,
    usdtgBalance: Double,
    usdtgvBalance: Double,
    usdtggBalance: Double
) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(
            containerColor = Color.Transparent
        ),
        shape = RoundedCornerShape(20.dp)
    ) {
        Box(
            modifier = Modifier
                .fillMaxWidth()
                .background(
                    brush = Brush.linearGradient(
                        colors = listOf(
                            Color(0xFF6B46C1), // Purple
                            Color(0xFF3B82F6)  // Blue
                        )
                    ),
                    shape = RoundedCornerShape(20.dp)
                )
                .padding(20.dp)
        ) {
            Column(
                horizontalAlignment = Alignment.CenterHorizontally
            ) {
                // ODIX PAY Logo
                Box(
                    modifier = Modifier
                        .size(60.dp)
                        .clip(CircleShape)
                        .background(Color.White.copy(alpha = 0.2f)),
                    contentAlignment = Alignment.Center
                ) {
                    Text(
                        text = "ODIX",
                        style = MaterialTheme.typography.titleLarge,
                        color = Color.White,
                        fontWeight = FontWeight.Bold
                    )
                }
                
                Spacer(modifier = Modifier.height(16.dp))
                
                Text(
                    text = "ODIX PAY",
                    style = MaterialTheme.typography.headlineMedium,
                    color = Color.White,
                    fontWeight = FontWeight.Bold
                )
                
                Text(
                    text = "Quantum-Safe Payment Platform",
                    style = MaterialTheme.typography.bodyMedium,
                    color = Color.White.copy(alpha = 0.8f)
                )
                
                Spacer(modifier = Modifier.height(16.dp))
                
                Text(
                    text = "Total Balance",
                    style = MaterialTheme.typography.bodyMedium,
                    color = Color.White.copy(alpha = 0.8f)
                )
                
                Text(
                    text = "$${String.format("%.2f", totalBalance)}",
                    style = MaterialTheme.typography.displayMedium,
                    color = Color.White,
                    fontWeight = FontWeight.Bold
                )
                
                Spacer(modifier = Modifier.height(16.dp))
                
                // Token Balances
                Row(
                    modifier = Modifier.fillMaxWidth(),
                    horizontalArrangement = Arrangement.SpaceEvenly
                ) {
                    TokenBalanceItem("USDTg", usdtgBalance)
                    TokenBalanceItem("USDTgV", usdtgvBalance)
                    TokenBalanceItem("USDTgG", usdtggBalance)
                }
            }
        }
    }
}

@Composable
fun TokenBalanceItem(
    symbol: String,
    balance: Double
) {
    Column(
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(
            text = symbol,
            style = MaterialTheme.typography.bodySmall,
            color = Color.White.copy(alpha = 0.8f)
        )
        Text(
            text = String.format("%.2f", balance),
            style = MaterialTheme.typography.bodyMedium,
            color = Color.White,
            fontWeight = FontWeight.Medium
        )
    }
}

@Composable
fun QuickActionsRow(
    onSendClick: () -> Unit,
    onReceiveClick: () -> Unit,
    onScanClick: () -> Unit,
    onHistoryClick: () -> Unit
) {
    LazyRow(
        horizontalArrangement = Arrangement.spacedBy(12.dp),
        contentPadding = PaddingValues(horizontal = 4.dp)
    ) {
        item {
            QuickActionButton(
                icon = Icons.Default.Send,
                label = "Send",
                color = USDTgColors.Success,
                onClick = onSendClick
            )
        }
        
        item {
            QuickActionButton(
                icon = Icons.Default.CallReceived,
                label = "Receive",
                color = USDTgColors.Info,
                onClick = onReceiveClick
            )
        }
        
        item {
            QuickActionButton(
                icon = Icons.Default.QrCodeScanner,
                label = "Scan",
                color = USDTgColors.Warning,
                onClick = onScanClick
            )
        }
        
        item {
            QuickActionButton(
                icon = Icons.Default.History,
                label = "History",
                color = USDTgColors.Analytics,
                onClick = onHistoryClick
            )
        }
    }
}

@Composable
fun FeatureCard(
    feature: ODIXPAYFeature,
    onClick: () -> Unit
) {
    Card(
        onClick = onClick,
        modifier = Modifier
            .fillMaxWidth()
            .height(140.dp),
        colors = CardDefaults.cardColors(
            containerColor = Color.White.copy(alpha = 0.1f)
        ),
        shape = RoundedCornerShape(16.dp)
    ) {
        Column(
            modifier = Modifier
                .fillMaxSize()
                .padding(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally,
            verticalArrangement = Arrangement.Center
        ) {
            Icon(
                imageVector = feature.icon,
                contentDescription = feature.title,
                tint = feature.color,
                modifier = Modifier.size(32.dp)
            )
            
            Spacer(modifier = Modifier.height(8.dp))
            
            Text(
                text = feature.title,
                style = MaterialTheme.typography.bodyLarge,
                color = Color.White,
                fontWeight = FontWeight.Medium,
                textAlign = TextAlign.Center
            )
            
            Spacer(modifier = Modifier.height(4.dp))
            
            Text(
                text = feature.description,
                style = MaterialTheme.typography.bodySmall,
                color = Color.White.copy(alpha = 0.7f),
                textAlign = TextAlign.Center
            )
        }
    }
}

@Composable
fun TransactionCard(transaction: ODIXPAYTransaction) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(
            containerColor = Color.White.copy(alpha = 0.05f)
        ),
        shape = RoundedCornerShape(8.dp)
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(12.dp),
            verticalAlignment = Alignment.CenterVertically
        ) {
            Icon(
                imageVector = transaction.type.icon,
                contentDescription = transaction.type.name,
                tint = transaction.type.color,
                modifier = Modifier.size(24.dp)
            )
            
            Spacer(modifier = Modifier.width(12.dp))
            
            Column(
                modifier = Modifier.weight(1f)
            ) {
                Text(
                    text = transaction.type.displayName,
                    style = MaterialTheme.typography.bodyMedium,
                    color = Color.White
                )
                Text(
                    text = transaction.id.take(12) + "...",
                    style = MaterialTheme.typography.bodySmall,
                    color = Color.White.copy(alpha = 0.7f),
                    fontFamily = androidx.compose.ui.text.font.FontFamily.Monospace
                )
            }
            
            Column(
                horizontalAlignment = Alignment.End
            ) {
                Text(
                    text = "${if (transaction.type == ODIXPAYTransactionType.SENT) "-" else "+"}${String.format("%.6f", transaction.amount)} ${transaction.currency}",
                    style = MaterialTheme.typography.bodyMedium,
                    color = if (transaction.type == ODIXPAYTransactionType.SENT) USDTgColors.Error else USDTgColors.Success,
                    fontWeight = FontWeight.Medium
                )
                Text(
                    text = formatTime(transaction.timestamp),
                    style = MaterialTheme.typography.bodySmall,
                    color = Color.White.copy(alpha = 0.7f)
                )
            }
        }
    }
}

// Data Models
data class ODIXPAYTransaction(
    val id: String,
    val type: ODIXPAYTransactionType,
    val amount: Double,
    val currency: String,
    val timestamp: Long
)

enum class ODIXPAYTransactionType(
    val name: String,
    val displayName: String,
    val icon: ImageVector,
    val color: Color
) {
    SENT(
        "sent",
        "Sent Payment",
        Icons.Default.CallMade,
        USDTgColors.Error
    ),
    RECEIVED(
        "received",
        "Received Payment",
        Icons.Default.CallReceived,
        USDTgColors.Success
    ),
    MERCHANT(
        "merchant",
        "Merchant Payment",
        Icons.Default.Store,
        USDTgColors.Warning
    ),
    BILL_PAY(
        "bill_pay",
        "Bill Payment",
        Icons.Default.Receipt,
        USDTgColors.Info
    ),
    QUANTUM_SAFE(
        "quantum_safe",
        "Quantum-Safe Pay",
        Icons.Default.Security,
        USDTgColors.Analytics
    )
}

private fun formatTime(timestamp: Long): String {
    val now = System.currentTimeMillis()
    val diff = now - timestamp
    
    return when {
        diff < 60_000 -> "Just now"
        diff < 3600_000 -> "${diff / 60_000}m ago"
        diff < 86400_000 -> "${diff / 3600_000}h ago"
        else -> "${diff / 86400_000}d ago"
    }
}

@Preview(showBackground = true)
@Composable
fun ODIXPAYScreenPreview() {
    MaterialTheme {
        ODIXPAYScreen()
    }
}
