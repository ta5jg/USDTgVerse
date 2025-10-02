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
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.lifecycle.compose.collectAsStateWithLifecycle
import com.usdtgverse.wallet.ui.theme.USDTgColors
import com.usdtgverse.wallet.ui.theme.USDTgGradients
import com.usdtgverse.wallet.viewmodel.TradingViewModel

enum class TradingTab(val title: String) {
    TRADE("Trade"),
    POSITIONS("Positions"),
    ORDERS("Orders"),
    HISTORY("History")
}

enum class OrderType(val title: String) {
    MARKET("Market"),
    LIMIT("Limit"),
    STOP("Stop")
}

enum class OrderSide(val title: String, val color: Color) {
    BUY("Buy", USDTgColors.Success),
    SELL("Sell", USDTgColors.Error)
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun TradingScreen(
    viewModel: TradingViewModel = hiltViewModel(),
    onOrderPlaced: (TradingOrder) -> Unit = {}
) {
    val uiState by viewModel.uiState.collectAsStateWithLifecycle()
    var selectedTab by remember { mutableStateOf(TradingTab.TRADE) }
    
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
            TradingHeader(
                tradingBalance = uiState.tradingBalance,
                availableBalance = uiState.availableBalance,
                marginUsed = uiState.marginUsed,
                unrealizedPnL = uiState.unrealizedPnL
            )
        }
        
        item {
            TabSelector(
                selectedTab = selectedTab,
                onTabSelected = { selectedTab = it }
            )
        }
        
        when (selectedTab) {
            TradingTab.TRADE -> {
                item {
                    TradeView(
                        selectedSymbol = uiState.selectedSymbol,
                        currentPrice = uiState.currentPrice,
                        priceChange = uiState.priceChange,
                        symbols = uiState.availableSymbols,
                        onSymbolChanged = viewModel::updateSymbol,
                        onOrderPlaced = onOrderPlaced
                    )
                }
            }
            TradingTab.POSITIONS -> {
                items(uiState.positions) { position ->
                    PositionCard(
                        position = position,
                        onClosePosition = { viewModel.closePosition(position.id) }
                    )
                }
            }
            TradingTab.ORDERS -> {
                items(uiState.orders) { order ->
                    OrderCard(
                        order = order,
                        onCancelOrder = { viewModel.cancelOrder(order.id) }
                    )
                }
            }
            TradingTab.HISTORY -> {
                items(uiState.tradeHistory) { trade ->
                    HistoryCard(trade = trade)
                }
            }
        }
    }
}

@Composable
fun TradingHeader(
    tradingBalance: Double,
    availableBalance: Double,
    marginUsed: Double,
    unrealizedPnL: Double
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
                            Color(0xFF1E3A8A), // Blue
                            Color(0xFF3730A3)  // Purple
                        )
                    ),
                    shape = RoundedCornerShape(20.dp)
                )
                .padding(20.dp)
        ) {
            Column(
                horizontalAlignment = Alignment.CenterHorizontally
            ) {
                // Trading Logo
                Box(
                    modifier = Modifier
                        .size(50.dp)
                        .clip(CircleShape)
                        .background(Color.White.copy(alpha = 0.2f)),
                    contentAlignment = Alignment.Center
                ) {
                    Icon(
                        imageVector = Icons.Default.TrendingUp,
                        contentDescription = "Trading",
                        tint = Color.White,
                        modifier = Modifier.size(24.dp)
                    )
                }
                
                Spacer(modifier = Modifier.height(12.dp))
                
                Text(
                    text = "Leveraged Trading",
                    style = MaterialTheme.typography.headlineSmall,
                    color = Color.White,
                    fontWeight = FontWeight.Bold
                )
                
                Spacer(modifier = Modifier.height(16.dp))
                
                // Balance Overview
                Row(
                    modifier = Modifier.fillMaxWidth(),
                    horizontalArrangement = Arrangement.SpaceBetween
                ) {
                    Column {
                        Text(
                            text = "Trading Balance",
                            style = MaterialTheme.typography.bodySmall,
                            color = Color.White.copy(alpha = 0.8f)
                        )
                        Text(
                            text = "$${String.format("%.2f", tradingBalance)}",
                            style = MaterialTheme.typography.titleLarge,
                            color = Color.White,
                            fontWeight = FontWeight.Bold
                        )
                    }
                    
                    Column(
                        horizontalAlignment = Alignment.End
                    ) {
                        Text(
                            text = "Unrealized PnL",
                            style = MaterialTheme.typography.bodySmall,
                            color = Color.White.copy(alpha = 0.8f)
                        )
                        Text(
                            text = "$${String.format("%.2f", unrealizedPnL)}",
                            style = MaterialTheme.typography.titleLarge,
                            color = if (unrealizedPnL >= 0) USDTgColors.Success else USDTgColors.Error,
                            fontWeight = FontWeight.Bold
                        )
                    }
                }
                
                Spacer(modifier = Modifier.height(12.dp))
                
                Row(
                    modifier = Modifier.fillMaxWidth(),
                    horizontalArrangement = Arrangement.SpaceBetween
                ) {
                    Column {
                        Text(
                            text = "Available",
                            style = MaterialTheme.typography.bodySmall,
                            color = Color.White.copy(alpha = 0.8f)
                        )
                        Text(
                            text = "$${String.format("%.2f", availableBalance)}",
                            style = MaterialTheme.typography.bodyMedium,
                            color = Color.White,
                            fontWeight = FontWeight.Medium
                        )
                    }
                    
                    Column(
                        horizontalAlignment = Alignment.End
                    ) {
                        Text(
                            text = "Margin Used",
                            style = MaterialTheme.typography.bodySmall,
                            color = Color.White.copy(alpha = 0.8f)
                        )
                        Text(
                            text = "$${String.format("%.2f", marginUsed)}",
                            style = MaterialTheme.typography.bodyMedium,
                            color = Color.White,
                            fontWeight = FontWeight.Medium
                        )
                    }
                }
            }
        }
    }
}

@Composable
fun TabSelector(
    selectedTab: TradingTab,
    onTabSelected: (TradingTab) -> Unit
) {
    LazyRow(
        horizontalArrangement = Arrangement.spacedBy(8.dp),
        contentPadding = PaddingValues(horizontal = 4.dp)
    ) {
        items(TradingTab.values()) { tab ->
            TabButton(
                tab = tab,
                isSelected = selectedTab == tab,
                onClick = { onTabSelected(tab) }
            )
        }
    }
}

@Composable
fun TabButton(
    tab: TradingTab,
    isSelected: Boolean,
    onClick: () -> Unit
) {
    Card(
        onClick = onClick,
        colors = CardDefaults.cardColors(
            containerColor = if (isSelected) USDTgColors.Primary else Color.White.copy(alpha = 0.1f)
        ),
        shape = RoundedCornerShape(20.dp)
    ) {
        Text(
            text = tab.title,
            style = MaterialTheme.typography.bodyMedium,
            color = if (isSelected) Color.White else Color.White.copy(alpha = 0.7f),
            fontWeight = if (isSelected) FontWeight.Medium else FontWeight.Normal,
            modifier = Modifier.padding(horizontal = 16.dp, vertical = 8.dp)
        )
    }
}

@Composable
fun TradeView(
    selectedSymbol: String,
    currentPrice: Double,
    priceChange: Double,
    symbols: List<String>,
    onSymbolChanged: (String) -> Unit,
    onOrderPlaced: (TradingOrder) -> Unit
) {
    var orderType by remember { mutableStateOf(OrderType.MARKET) }
    var orderSide by remember { mutableStateOf(OrderSide.BUY) }
    var quantity by remember { mutableStateOf("") }
    var price by remember { mutableStateOf("") }
    var leverage by remember { mutableStateOf(1f) }
    
    Column(
        verticalArrangement = Arrangement.spacedBy(16.dp)
    ) {
        // Symbol Selector
        SymbolSelector(
            selectedSymbol = selectedSymbol,
            symbols = symbols,
            onSymbolSelected = onSymbolChanged
        )
        
        // Price Display
        PriceDisplay(
            currentPrice = currentPrice,
            priceChange = priceChange
        )
        
        // Order Form
        OrderForm(
            orderType = orderType,
            onOrderTypeChanged = { orderType = it },
            quantity = quantity,
            onQuantityChanged = { quantity = it },
            price = price,
            onPriceChanged = { price = it },
            leverage = leverage,
            onLeverageChanged = { leverage = it }
        )
        
        // Order Buttons
        OrderButtons(
            orderSide = orderSide,
            onOrderSideChanged = { orderSide = it },
            onOrderPlaced = {
                val order = TradingOrder(
                    id = "order_${System.currentTimeMillis()}",
                    symbol = selectedSymbol,
                    side = orderSide,
                    type = orderType,
                    quantity = quantity.toDoubleOrNull() ?: 0.0,
                    price = price.toDoubleOrNull() ?: currentPrice,
                    leverage = leverage.toInt()
                )
                onOrderPlaced(order)
            }
        )
    }
}

@Composable
fun SymbolSelector(
    selectedSymbol: String,
    symbols: List<String>,
    onSymbolSelected: (String) -> Unit
) {
    Column(
        verticalArrangement = Arrangement.spacedBy(8.dp)
    ) {
        Text(
            text = "Trading Pair",
            style = MaterialTheme.typography.headlineSmall,
            color = Color.White
        )
        
        LazyRow(
            horizontalArrangement = Arrangement.spacedBy(8.dp)
        ) {
            items(symbols) { symbol ->
                SymbolButton(
                    symbol = symbol,
                    isSelected = selectedSymbol == symbol,
                    onClick = { onSymbolSelected(symbol) }
                )
            }
        }
    }
}

@Composable
fun SymbolButton(
    symbol: String,
    isSelected: Boolean,
    onClick: () -> Unit
) {
    Card(
        onClick = onClick,
        colors = CardDefaults.cardColors(
            containerColor = if (isSelected) USDTgColors.Primary else Color.White.copy(alpha = 0.1f)
        ),
        shape = RoundedCornerShape(20.dp)
    ) {
        Text(
            text = symbol,
            style = MaterialTheme.typography.bodyMedium,
            color = if (isSelected) Color.White else Color.White.copy(alpha = 0.7f),
            fontWeight = if (isSelected) FontWeight.Medium else FontWeight.Normal,
            modifier = Modifier.padding(horizontal = 16.dp, vertical = 8.dp)
        )
    }
}

@Composable
fun PriceDisplay(
    currentPrice: Double,
    priceChange: Double
) {
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
            horizontalArrangement = Arrangement.SpaceBetween,
            verticalAlignment = Alignment.CenterVertically
        ) {
            Column {
                Text(
                    text = "Current Price",
                    style = MaterialTheme.typography.bodySmall,
                    color = Color.White.copy(alpha = 0.7f)
                )
                Text(
                    text = "$${String.format("%.6f", currentPrice)}",
                    style = MaterialTheme.typography.titleLarge,
                    color = Color.White,
                    fontWeight = FontWeight.Bold,
                    fontFamily = androidx.compose.ui.text.font.FontFamily.Monospace
                )
            }
            
            Column(
                horizontalAlignment = Alignment.End
            ) {
                Row(
                    verticalAlignment = Alignment.CenterVertically
                ) {
                    Icon(
                        imageVector = if (priceChange >= 0) Icons.Default.TrendingUp else Icons.Default.TrendingDown,
                        contentDescription = null,
                        tint = if (priceChange >= 0) USDTgColors.Success else USDTgColors.Error,
                        modifier = Modifier.size(16.dp)
                    )
                    Spacer(modifier = Modifier.width(4.dp))
                    Text(
                        text = "${if (priceChange >= 0) "+" else ""}${String.format("%.2f", priceChange * 100)}%",
                        style = MaterialTheme.typography.bodyMedium,
                        color = if (priceChange >= 0) USDTgColors.Success else USDTgColors.Error,
                        fontWeight = FontWeight.Medium
                    )
                }
            }
        }
    }
}

@Composable
fun OrderForm(
    orderType: OrderType,
    onOrderTypeChanged: (OrderType) -> Unit,
    quantity: String,
    onQuantityChanged: (String) -> Unit,
    price: String,
    onPriceChanged: (String) -> Unit,
    leverage: Float,
    onLeverageChanged: (Float) -> Unit
) {
    Column(
        verticalArrangement = Arrangement.spacedBy(16.dp)
    ) {
        // Order Type
        Column(
            verticalArrangement = Arrangement.spacedBy(8.dp)
        ) {
            Text(
                text = "Order Type",
                style = MaterialTheme.typography.headlineSmall,
                color = Color.White
            )
            
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.spacedBy(8.dp)
            ) {
                OrderType.values().forEach { type ->
                    Card(
                        onClick = { onOrderTypeChanged(type) },
                        colors = CardDefaults.cardColors(
                            containerColor = if (orderType == type) USDTgColors.Primary else Color.White.copy(alpha = 0.1f)
                        ),
                        shape = RoundedCornerShape(8.dp),
                        modifier = Modifier.weight(1f)
                    ) {
                        Text(
                            text = type.title,
                            style = MaterialTheme.typography.bodyMedium,
                            color = if (orderType == type) Color.White else Color.White.copy(alpha = 0.7f),
                            textAlign = TextAlign.Center,
                            modifier = Modifier.padding(vertical = 8.dp)
                        )
                    }
                }
            }
        }
        
        // Quantity
        Column(
            verticalArrangement = Arrangement.spacedBy(8.dp)
        ) {
            Text(
                text = "Quantity",
                style = MaterialTheme.typography.headlineSmall,
                color = Color.White
            )
            
            OutlinedTextField(
                value = quantity,
                onValueChange = onQuantityChanged,
                placeholder = { Text("0.00", color = Color.White.copy(alpha = 0.5f)) },
                keyboardOptions = androidx.compose.foundation.text.KeyboardOptions(
                    keyboardType = androidx.compose.ui.text.input.KeyboardType.Decimal
                ),
                colors = OutlinedTextFieldDefaults.colors(
                    focusedTextColor = Color.White,
                    unfocusedTextColor = Color.White,
                    focusedBorderColor = USDTgColors.Primary,
                    unfocusedBorderColor = Color.White.copy(alpha = 0.3f)
                ),
                modifier = Modifier.fillMaxWidth()
            )
        }
        
        // Price (for limit/stop orders)
        if (orderType != OrderType.MARKET) {
            Column(
                verticalArrangement = Arrangement.spacedBy(8.dp)
            ) {
                Text(
                    text = "Price",
                    style = MaterialTheme.typography.headlineSmall,
                    color = Color.White
                )
                
                OutlinedTextField(
                    value = price,
                    onValueChange = onPriceChanged,
                    placeholder = { Text("0.00", color = Color.White.copy(alpha = 0.5f)) },
                    keyboardOptions = androidx.compose.foundation.text.KeyboardOptions(
                        keyboardType = androidx.compose.ui.text.input.KeyboardType.Decimal
                    ),
                    colors = OutlinedTextFieldDefaults.colors(
                        focusedTextColor = Color.White,
                        unfocusedTextColor = Color.White,
                        focusedBorderColor = USDTgColors.Primary,
                        unfocusedBorderColor = Color.White.copy(alpha = 0.3f)
                    ),
                    modifier = Modifier.fillMaxWidth()
                )
            }
        }
        
        // Leverage
        Column(
            verticalArrangement = Arrangement.spacedBy(8.dp)
        ) {
            Text(
                text = "Leverage: ${leverage.toInt()}x",
                style = MaterialTheme.typography.headlineSmall,
                color = Color.White
            )
            
            Slider(
                value = leverage,
                onValueChange = onLeverageChanged,
                valueRange = 1f..100f,
                steps = 98,
                colors = SliderDefaults.colors(
                    thumbColor = USDTgColors.Primary,
                    activeTrackColor = USDTgColors.Primary,
                    inactiveTrackColor = Color.White.copy(alpha = 0.3f)
                )
            )
        }
    }
}

@Composable
fun OrderButtons(
    orderSide: OrderSide,
    onOrderSideChanged: (OrderSide) -> Unit,
    onOrderPlaced: () -> Unit
) {
    Row(
        modifier = Modifier.fillMaxWidth(),
        horizontalArrangement = Arrangement.spacedBy(16.dp)
    ) {
        Button(
            onClick = {
                onOrderSideChanged(OrderSide.BUY)
                onOrderPlaced()
            },
            colors = ButtonDefaults.buttonColors(
                containerColor = USDTgColors.Success
            ),
            modifier = Modifier.weight(1f)
        ) {
            Text(
                text = "Buy",
                style = MaterialTheme.typography.headlineSmall,
                color = Color.White,
                fontWeight = FontWeight.Bold
            )
        }
        
        Button(
            onClick = {
                onOrderSideChanged(OrderSide.SELL)
                onOrderPlaced()
            },
            colors = ButtonDefaults.buttonColors(
                containerColor = USDTgColors.Error
            ),
            modifier = Modifier.weight(1f)
        ) {
            Text(
                text = "Sell",
                style = MaterialTheme.typography.headlineSmall,
                color = Color.White,
                fontWeight = FontWeight.Bold
            )
        }
    }
}

@Composable
fun PositionCard(
    position: TradingPosition,
    onClosePosition: () -> Unit
) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(
            containerColor = Color.White.copy(alpha = 0.1f)
        ),
        shape = RoundedCornerShape(12.dp)
    ) {
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                Text(
                    text = position.symbol,
                    style = MaterialTheme.typography.headlineSmall,
                    color = Color.White,
                    fontWeight = FontWeight.Bold
                )
                
                Card(
                    colors = CardDefaults.cardColors(
                        containerColor = position.side.color
                    ),
                    shape = RoundedCornerShape(6.dp)
                ) {
                    Text(
                        text = position.side.title,
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White,
                        fontWeight = FontWeight.Medium,
                        modifier = Modifier.padding(horizontal = 8.dp, vertical = 4.dp)
                    )
                }
            }
            
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Column {
                    Text(
                        text = "Quantity",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                    Text(
                        text = String.format("%.0f", position.quantity),
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.White,
                        fontWeight = FontWeight.Medium
                    )
                }
                
                Column(
                    horizontalAlignment = Alignment.CenterHorizontally
                ) {
                    Text(
                        text = "Entry Price",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                    Text(
                        text = "$${String.format("%.6f", position.entryPrice)}",
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.White,
                        fontWeight = FontWeight.Medium
                    )
                }
                
                Column(
                    horizontalAlignment = Alignment.End
                ) {
                    Text(
                        text = "Current Price",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                    Text(
                        text = "$${String.format("%.6f", position.currentPrice)}",
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.White,
                        fontWeight = FontWeight.Medium
                    )
                }
            }
            
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                Column {
                    Text(
                        text = "Leverage",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                    Text(
                        text = "${position.leverage}x",
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.White,
                        fontWeight = FontWeight.Medium
                    )
                }
                
                Column(
                    horizontalAlignment = Alignment.End
                ) {
                    Text(
                        text = "PnL",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                    Text(
                        text = "$${String.format("%.2f", position.pnl)}",
                        style = MaterialTheme.typography.bodyMedium,
                        color = if (position.pnl >= 0) USDTgColors.Success else USDTgColors.Error,
                        fontWeight = FontWeight.Bold
                    )
                }
            }
            
            Button(
                onClick = onClosePosition,
                colors = ButtonDefaults.buttonColors(
                    containerColor = USDTgColors.Warning
                ),
                modifier = Modifier.fillMaxWidth()
            ) {
                Text(
                    text = "Close Position",
                    style = MaterialTheme.typography.bodyMedium,
                    color = Color.White,
                    fontWeight = FontWeight.Medium
                )
            }
        }
    }
}

@Composable
fun OrderCard(
    order: TradingOrder,
    onCancelOrder: () -> Unit
) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(
            containerColor = Color.White.copy(alpha = 0.1f)
        ),
        shape = RoundedCornerShape(12.dp)
    ) {
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                Text(
                    text = order.symbol,
                    style = MaterialTheme.typography.headlineSmall,
                    color = Color.White,
                    fontWeight = FontWeight.Bold
                )
                
                Card(
                    colors = CardDefaults.cardColors(
                        containerColor = order.side.color
                    ),
                    shape = RoundedCornerShape(6.dp)
                ) {
                    Text(
                        text = order.side.title,
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White,
                        fontWeight = FontWeight.Medium,
                        modifier = Modifier.padding(horizontal = 8.dp, vertical = 4.dp)
                    )
                }
            }
            
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Column {
                    Text(
                        text = "Quantity",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                    Text(
                        text = String.format("%.0f", order.quantity),
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.White,
                        fontWeight = FontWeight.Medium
                    )
                }
                
                Column(
                    horizontalAlignment = Alignment.CenterHorizontally
                ) {
                    Text(
                        text = "Price",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                    Text(
                        text = "$${String.format("%.6f", order.price)}",
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.White,
                        fontWeight = FontWeight.Medium
                    )
                }
                
                Column(
                    horizontalAlignment = Alignment.End
                ) {
                    Text(
                        text = "Type",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                    Text(
                        text = order.type.title,
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.White,
                        fontWeight = FontWeight.Medium
                    )
                }
            }
            
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                Text(
                    text = "Status: Pending",
                    style = MaterialTheme.typography.bodyMedium,
                    color = Color.White.copy(alpha = 0.7f)
                )
                
                Button(
                    onClick = onCancelOrder,
                    colors = ButtonDefaults.buttonColors(
                        containerColor = USDTgColors.Error
                    )
                ) {
                    Text(
                        text = "Cancel",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White,
                        fontWeight = FontWeight.Medium
                    )
                }
            }
        }
    }
}

@Composable
fun HistoryCard(trade: TradingHistory) {
    Card(
        modifier = Modifier.fillMaxWidth(),
        colors = CardDefaults.cardColors(
            containerColor = Color.White.copy(alpha = 0.1f)
        ),
        shape = RoundedCornerShape(12.dp)
    ) {
        Column(
            modifier = Modifier
                .fillMaxWidth()
                .padding(16.dp),
            verticalArrangement = Arrangement.spacedBy(12.dp)
        ) {
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween,
                verticalAlignment = Alignment.CenterVertically
            ) {
                Text(
                    text = trade.symbol,
                    style = MaterialTheme.typography.headlineSmall,
                    color = Color.White,
                    fontWeight = FontWeight.Bold
                )
                
                Card(
                    colors = CardDefaults.cardColors(
                        containerColor = trade.side.color
                    ),
                    shape = RoundedCornerShape(6.dp)
                ) {
                    Text(
                        text = trade.side.title,
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White,
                        fontWeight = FontWeight.Medium,
                        modifier = Modifier.padding(horizontal = 8.dp, vertical = 4.dp)
                    )
                }
            }
            
            Row(
                modifier = Modifier.fillMaxWidth(),
                horizontalArrangement = Arrangement.SpaceBetween
            ) {
                Column {
                    Text(
                        text = "Quantity",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                    Text(
                        text = String.format("%.0f", trade.quantity),
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.White,
                        fontWeight = FontWeight.Medium
                    )
                }
                
                Column(
                    horizontalAlignment = Alignment.CenterHorizontally
                ) {
                    Text(
                        text = "Price",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                    Text(
                        text = "$${String.format("%.6f", trade.price)}",
                        style = MaterialTheme.typography.bodyMedium,
                        color = Color.White,
                        fontWeight = FontWeight.Medium
                    )
                }
                
                Column(
                    horizontalAlignment = Alignment.End
                ) {
                    Text(
                        text = "PnL",
                        style = MaterialTheme.typography.bodySmall,
                        color = Color.White.copy(alpha = 0.7f)
                    )
                    Text(
                        text = "$${String.format("%.2f", trade.pnl)}",
                        style = MaterialTheme.typography.bodyMedium,
                        color = if (trade.pnl >= 0) USDTgColors.Success else USDTgColors.Error,
                        fontWeight = FontWeight.Bold
                    )
                }
            }
            
            Text(
                text = formatTime(trade.timestamp),
                style = MaterialTheme.typography.bodySmall,
                color = Color.White.copy(alpha = 0.7f)
            )
        }
    }
}

// Data Models
data class TradingPosition(
    val id: String,
    val symbol: String,
    val side: OrderSide,
    val quantity: Double,
    val entryPrice: Double,
    val currentPrice: Double,
    val leverage: Int,
    val pnl: Double
)

data class TradingOrder(
    val id: String,
    val symbol: String,
    val side: OrderSide,
    val type: OrderType,
    val quantity: Double,
    val price: Double,
    val leverage: Int
)

data class TradingHistory(
    val id: String,
    val symbol: String,
    val side: OrderSide,
    val quantity: Double,
    val price: Double,
    val timestamp: Long,
    val pnl: Double
)

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
fun TradingScreenPreview() {
    MaterialTheme {
        TradingScreen()
    }
}
