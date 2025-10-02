package com.usdtgverse.wallet.data.model

enum class TransactionType {
    SENT,
    RECEIVED,
    BRIDGE,
    ODIXPAY
}

data class Transaction(
    val id: String,
    val type: TransactionType,
    val amount: Double,
    val asset: String,
    val timestamp: Long,
    val fee: Double,
    val confirmation: Int,
    val status: String
)

