package com.usdtgverse.wallet.data.model

data class WalletAsset(
    val id: String = "",
    val symbol: String,
    val name: String,
    val balance: Double,
    val totalValue: Double,
    val valueChange: Double,
    val logo: String? = null
)

