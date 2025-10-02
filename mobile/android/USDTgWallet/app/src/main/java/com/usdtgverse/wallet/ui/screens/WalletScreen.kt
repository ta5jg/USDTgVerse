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
    walletViewModel: WalletViewModel,
    onSendClick: () -> Unit = {},
    onReceiveClick: () -> Unit = {},
    onSwapClick: () -> Unit = {},
    onBridgeClick: () -> Unit = {}
) {
    val uiState by walletViewModel.uiState.collectAsStateWithLifecycle()
    
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
                walletAddress = "usdtg1234...abcd",
                isConnected = true,
                blockHeight = 1234567L
            )
        }
        
        item {
            PortfolioCard(
                totalValue = uiState.balance.toDoubleOrNull() ?: 0.0,
                usdtgBalance = 1250.00,
                dailyChange = 2.5
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
        
        items(uiState.tokens) { token ->
            AssetCard(token = token)
        }
        
        item {
            Text(
                text = "Recent Transactions",
                style = MaterialTheme.typography.headlineSmall,
                color = Color.White,
                modifier = Modifier.padding(vertical = 8.dp)
            )
        }
        
        items(uiState.transactions.take(5)) { transaction ->
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
                icon = Icons.Default.TransferWithinAStation,
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
fun AssetCard(token: com.usdtgverse.wallet.viewmodel.TokenData) {
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
                    text = token.symbol.take(2),
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
                    text = token.symbol,
                    style = MaterialTheme.typography.bodyLarge,
                    color = Color.White,
                    fontWeight = FontWeight.Medium
                )
                Text(
                    text = "USDTgVerse Token",
                    style = MaterialTheme.typography.bodySmall,
                    color = Color.White.copy(alpha = 0.7f)
                )
            }
            
            Column(
                horizontalAlignment = Alignment.End
            ) {
                Text(
                    text = token.balance,
                    style = MaterialTheme.typography.bodyLarge,
                    color = Color.White,
                    fontWeight = FontWeight.Medium
                )
                Text(
                    text = token.value,
                    style = MaterialTheme.typography.bodySmall,
                    color = Color.White.copy(alpha = 0.7f)
                )
            }
        }
    }
}

@Composable
fun TransactionCard(transaction: com.usdtgverse.wallet.viewmodel.TransactionData) {
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
                imageVector = if (transaction.type == "Receive") Icons.Default.CallReceived else Icons.Default.CallMade,
                contentDescription = transaction.type,
                tint = if (transaction.type == "Receive") USDTgColors.Success else USDTgColors.Error,
                modifier = Modifier.size(24.dp)
            )
            
            Spacer(modifier = Modifier.width(12.dp))
            
            Column(
                modifier = Modifier.weight(1f)
            ) {
                Text(
                    text = transaction.type,
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
                    text = transaction.amount,
                    style = MaterialTheme.typography.bodyMedium,
                    color = if (transaction.type == "Receive") USDTgColors.Success else USDTgColors.Error,
                    fontWeight = FontWeight.Medium
                )
                Text(
                    text = transaction.timestamp,
                    style = MaterialTheme.typography.bodySmall,
                    color = Color.White.copy(alpha = 0.7f)
                )
            }
        }
    }
}


// Preview is commented out due to ViewModel dependency
// @Preview(showBackground = true)
// @Composable
// fun WalletScreenPreview() {
//     MaterialTheme {
//         WalletScreen(hiltViewModel())
//     }
// }
