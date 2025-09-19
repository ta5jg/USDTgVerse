package com.usdtgverse.wallet.ui.screens

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.LazyRow
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
import com.usdtgverse.wallet.data.model.WalletAsset
import com.usdtgverse.wallet.data.model.Transaction
import com.usdtgverse.wallet.data.model.TransactionType
import com.usdtgverse.wallet.ui.theme.USDTgColors
import com.usdtgverse.wallet.ui.theme.USDTgGradients
import com.usdtgverse.wallet.viewmodel.WalletViewModel

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun WalletScreen(
    viewModel: WalletViewModel = hiltViewModel(),
    onSendClick: () -> Unit = {},
    onReceiveClick: () -> Unit = {},
    onSwapClick: () -> Unit = {},
    onBridgeClick: () -> Unit = {}
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
            WalletHeader(
                walletAddress = uiState.walletAddress,
                isConnected = uiState.isConnected,
                blockHeight = uiState.blockHeight
            )
        }
        
        item {
            PortfolioCard(
                totalValue = uiState.totalPortfolioValue,
                usdtgBalance = uiState.usdtgBalance,
                dailyChange = uiState.dailyChangePercent
            )
        }
        
        item {
            QuickActionsRow(
                onSendClick = onSendClick,
                onReceiveClick = onReceiveClick,
                onSwapClick = onSwapClick,
                onBridgeClick = onBridgeClick
            )
        }
        
        item {
            Text(
                text = "Assets",
                style = MaterialTheme.typography.headlineSmall,
                color = Color.White,
                modifier = Modifier.padding(vertical = 8.dp)
            )
        }
        
        items(uiState.assets) { asset ->
            AssetCard(asset = asset)
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
fun WalletHeader(
    walletAddress: String,
    isConnected: Boolean,
    blockHeight: Long
) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(
            containerColor = Color.White.copy(alpha = 0.1f)
        ),
        shape = RoundedCornerShape(16.dp)
    ) {
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp)
        ) {
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                Column {
                    Text(
                        text = "USDTgVerse",
                        style = MaterialTheme.typography.headlineSmall,
                        color = USDTgColors.Success,
                        fontWeight = FontWeight.Bold
                    )
                    Text(
                        text = "Enterprise Blockchain",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                }
                
                Column(
                    horizontalAlignment = Alignment.End
                ) {
                    Row(
                        verticalAlignment = Alignment.CenterVertically
                    ) {
                        Box(
                            modifier = Modifier
                                .size(8.dp)
                                .clip(CircleShape)
                                .background(if (isConnected) USDTgColors.Success else USDTgColors.Error)
                        )
                        Spacer(modifier = Modifier.width(4.dp))
                        Text(
                            text = if (isConnected) "Connected" else "Disconnected",
                            style = MaterialTheme.typography.bodySmall,
                            color = Color.White.copy(alpha = 0.7f)
                        )
                    }
                    Text(
                        text = "Block #$blockHeight",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                }
            }
            
            Spacer(modifier = Modifier.height(8.dp))
            
            Text(
                text = walletAddress,
                style = MaterialTheme.typography.bodySmall,
                color = Color.White.copy(alpha = 0.8f),
                fontFamily = androidx.compose.ui.text.font.FontFamily.Monospace
            )
        }
    }
}

@Composable
fun PortfolioCard(
    totalValue: Double,
    usdtgBalance: Double,
    dailyChange: Double
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
                    brush = Brush.linearGradient(USDTgGradients.PrimaryGradient),
                    shape = RoundedCornerShape(20.dp)
                )
                .padding(20.dp)
        ) {
            Column(
                horizontalAlignment = Alignment.CenterHorizontally
            ) {
                Text(
                    text = "Total Portfolio Value",
                    style = MaterialTheme.typography.bodyMedium,
                    color = Color.White.copy(alpha = 0.8f)
                )
                
                Spacer(modifier = Modifier.height(8.dp))
                
                Text(
                    text = "$${String.format("%.2f", totalValue)}",
                    style = MaterialTheme.typography.displayMedium,
                    color = Color.White,
                    fontWeight = FontWeight.Bold
                )
                
                Spacer(modifier = Modifier.height(16.dp))
                
                Row(
                    modifier = Modifier.fillMaxWidth(),
                    horizontalArrangement = Arrangement.SpaceBetween
                ) {
                    Text(
                        text = "USDTg: ${String.format("%.2f", usdtgBalance)}",
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.White
                    )
                    
                    Text(
                        text = "${if (dailyChange >= 0) "+" else ""}${String.format("%.2f", dailyChange)}% (24h)",
                        style = MaterialTheme.typography.bodyMedium,
                        color = if (dailyChange >= 0) USDTgColors.Success else USDTgColors.Error
                    )
                }
            }
        }
    }
}

@Composable
fun QuickActionsRow(
    onSendClick: () -> Unit,
    onReceiveClick: () -> Unit,
    onSwapClick: () -> Unit,
    onBridgeClick: () -> Unit
) {
    LazyRow(
        horizontalArrangement = Arrangement.spacedBy(12.dp),
        contentPadding = PaddingValues(horizontal = 4.dp)
    ) {
        item {
            QuickActionButton(
                icon = Icons.Default.Send,
                label = "Send",
                color = USDTgColors.Error,
                onClick = onSendClick
            )
        }
        
        item {
            QuickActionButton(
                icon = Icons.Default.CallReceived,
                label = "Receive",
                color = USDTgColors.Success,
                onClick = onReceiveClick
            )
        }
        
        item {
            QuickActionButton(
                icon = Icons.Default.SwapHoriz,
                label = "Swap",
                color = USDTgColors.Info,
                onClick = onSwapClick
            )
        }
        
        item {
            QuickActionButton(
                icon = Icons.Default.Bridge,
                label = "Bridge",
                color = USDTgColors.Analytics,
                onClick = onBridgeClick
            )
        }
    }
}

@Composable
fun QuickActionButton(
    icon: ImageVector,
    label: String,
    color: Color,
    onClick: () -> Unit
) {
    Card(
        onClick = onClick,
        modifier = Modifier.width(80.dp),
        colors = CardDefaults.cardColors(
            containerColor = Color.White.copy(alpha = 0.1f)
        ),
        shape = RoundedCornerShape(12.dp)
    ) {
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            horizontalAlignment = Alignment.CenterHorizontally
        ) {
            Icon(
                imageVector = icon,
                contentDescription = label,
                tint = color,
                modifier = Modifier.size(24.dp)
            )
            
            Spacer(modifier = Modifier.height(8.dp))
            
            Text(
                text = label,
                style = MaterialTheme.typography.bodySmall,
                color = Color.White,
                textAlign = TextAlign.Center
            )
        }
    }
}

@Composable
fun AssetCard(asset: WalletAsset) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(
            containerColor = Color.White.copy(alpha = 0.1f)
        ),
        shape = RoundedCornerShape(12.dp)
    ) {
        Row(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            verticalAlignment = Alignment.CenterVertically
        ) {
            // Asset icon
            Box(
                modifier = Modifier
                    .size(40.dp)
                    .clip(CircleShape)
                    .background(USDTgColors.Success),
                contentAlignment = Alignment.Center
            ) {
                Text(
                    text = asset.symbol.take(2),
                    style = MaterialTheme.typography.bodySmall,
                    color = Color.White,
                    fontWeight = FontWeight.Bold
                )
            }
            
            Spacer(modifier = Modifier.width(12.dp))
            
            Column(
                modifier = Modifier.weight(1f)
            ) {
                Text(
                    text = asset.symbol,
                    style = MaterialTheme.typography.bodyLarge,
                    color = Color.White,
                    fontWeight = FontWeight.Medium
                )
                Text(
                    text = asset.name,
                    style = MaterialTheme.typography.bodySmall,
                    color = Color.White.copy(alpha = 0.7f)
                )
            }
            
            Column(
                horizontalAlignment = Alignment.End
            ) {
                Text(
                    text = String.format("%.6f", asset.balance),
                    style = MaterialTheme.typography.bodyLarge,
                    color = Color.White,
                    fontWeight = FontWeight.Medium
                )
                Text(
                    text = "$${String.format("%.2f", asset.totalValue)}",
                    style = MaterialTheme.typography.bodySmall,
                    color = Color.White.copy(alpha = 0.7f)
                )
            }
        }
    }
}

@Composable
fun TransactionCard(transaction: Transaction) {
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
                    text = "${if (transaction.type == TransactionType.SENT) "-" else "+"}${String.format("%.6f", transaction.amount)} ${transaction.asset}",
                    style = MaterialTheme.typography.bodyMedium,
                    color = if (transaction.type == TransactionType.SENT) USDTgColors.Error else USDTgColors.Success,
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

// Extension properties for TransactionType
val TransactionType.icon: ImageVector
    get() = when (this) {
        TransactionType.SENT -> Icons.Default.CallMade
        TransactionType.RECEIVED -> Icons.Default.CallReceived
        TransactionType.BRIDGE -> Icons.Default.Bridge
        TransactionType.ODIXPAY -> Icons.Default.CreditCard
    }

val TransactionType.color: Color
    get() = when (this) {
        TransactionType.SENT -> USDTgColors.Error
        TransactionType.RECEIVED -> USDTgColors.Success
        TransactionType.BRIDGE -> USDTgColors.Info
        TransactionType.ODIXPAY -> USDTgColors.Analytics
    }

val TransactionType.displayName: String
    get() = when (this) {
        TransactionType.SENT -> "Sent Payment"
        TransactionType.RECEIVED -> "Received Payment"
        TransactionType.BRIDGE -> "Bridge Transfer"
        TransactionType.ODIXPAY -> "OdixPay Payment"
    }

@Preview(showBackground = true)
@Composable
fun WalletScreenPreview() {
    MaterialTheme {
        WalletScreen()
    }
}
