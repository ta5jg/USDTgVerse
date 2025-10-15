/*
==============================================
 File:        BonusManager.kt
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-10-13
 Last Update: 2025-10-13
 Version:     1.0

 Description:
   USDTgVerse Android Bonus Manager
   
   Manages purchase bonus system:
   - 10,000 USDTg → +10 USDTg bonus
   - VIP tier system (Bronze to Diamond)
   - Automatic bonus distribution

 License:
   MIT License
==============================================
*/

package com.usdtgverse.wallet.data

import android.util.Log
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import org.json.JSONObject
import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.OutputStreamWriter
import java.net.HttpURLConnection
import java.net.URL

class BonusManager {
    
    companion object {
        private const val TAG = "BonusManager"
        private const val BONUS_API_BASE = "http://10.0.2.2:3007" // Android emulator localhost
        private const val MIN_PURCHASE_BONUS = 10000.0
    }
    
    /**
     * Check if purchase qualifies for bonus and create bonus record
     */
    suspend fun checkPurchaseBonus(
        walletAddress: String,
        userId: String,
        purchaseAmount: Double
    ): BonusResult {
        return withContext(Dispatchers.IO) {
            try {
                if (purchaseAmount < MIN_PURCHASE_BONUS) {
                    return@withContext BonusResult.NotQualified(
                        "Purchase amount too low. Minimum: $MIN_PURCHASE_BONUS USDTg"
                    )
                }
                
                val url = URL("$BONUS_API_BASE/api/bonus/create")
                val connection = url.openConnection() as HttpURLConnection
                
                connection.apply {
                    requestMethod = "POST"
                    setRequestProperty("Content-Type", "application/json")
                    doOutput = true
                }
                
                val requestBody = JSONObject().apply {
                    put("wallet_address", walletAddress)
                    put("user_id", userId)
                    put("purchase_amount", purchaseAmount)
                }
                
                OutputStreamWriter(connection.outputStream).use { writer ->
                    writer.write(requestBody.toString())
                    writer.flush()
                }
                
                val responseCode = connection.responseCode
                if (responseCode == HttpURLConnection.HTTP_OK) {
                    val response = BufferedReader(InputStreamReader(connection.inputStream))
                        .use { it.readText() }
                    
                    val json = JSONObject(response)
                    val success = json.getBoolean("success")
                    
                    if (success) {
                        val bonusId = json.getString("bonus_id")
                        val bonusAmount = json.getDouble("bonus_amount")
                        val tier = json.getString("tier")
                        val bonusPercent = json.getDouble("bonus_percent")
                        val message = json.getString("message")
                        
                        Log.d(TAG, "🎁 BONUS EARNED!")
                        Log.d(TAG, "💰 Amount: $bonusAmount USDTg")
                        Log.d(TAG, "🏆 Tier: $tier")
                        Log.d(TAG, "📝 $message")
                        
                        BonusResult.Success(
                            bonusId = bonusId,
                            bonusAmount = bonusAmount,
                            tier = tier,
                            bonusPercent = bonusPercent,
                            message = message
                        )
                    } else {
                        val error = json.optString("error", "Unknown error")
                        Log.e(TAG, "❌ Bonus failed: $error")
                        BonusResult.Error(error)
                    }
                } else {
                    Log.e(TAG, "❌ HTTP Error: $responseCode")
                    BonusResult.Error("HTTP Error: $responseCode")
                }
            } catch (e: Exception) {
                Log.e(TAG, "❌ Bonus check failed", e)
                BonusResult.Error(e.message ?: "Unknown error")
            }
        }
    }
    
    /**
     * Calculate bonus for purchase amount
     */
    suspend fun calculateBonus(purchaseAmount: Double): BonusCalculation? {
        return withContext(Dispatchers.IO) {
            try {
                val url = URL("$BONUS_API_BASE/api/bonus/calculate?amount=$purchaseAmount")
                val connection = url.openConnection() as HttpURLConnection
                
                connection.apply {
                    requestMethod = "GET"
                    setRequestProperty("Content-Type", "application/json")
                }
                
                val responseCode = connection.responseCode
                if (responseCode == HttpURLConnection.HTTP_OK) {
                    val response = BufferedReader(InputStreamReader(connection.inputStream))
                        .use { it.readText() }
                    
                    val json = JSONObject(response)
                    val success = json.getBoolean("success")
                    
                    if (success) {
                        BonusCalculation(
                            purchaseAmount = json.getDouble("purchase_amount"),
                            bonusAmount = json.getDouble("bonus_amount"),
                            tier = json.getString("tier"),
                            bonusPercent = json.getDouble("bonus_percent"),
                            qualifies = json.getBoolean("qualifies"),
                            minimumPurchase = json.getDouble("minimum_purchase")
                        )
                    } else {
                        null
                    }
                } else {
                    null
                }
            } catch (e: Exception) {
                Log.e(TAG, "❌ Bonus calculation failed", e)
                null
            }
        }
    }
    
    /**
     * Get user bonus statistics
     */
    suspend fun getUserStats(walletAddress: String): UserBonusStats? {
        return withContext(Dispatchers.IO) {
            try {
                val url = URL("$BONUS_API_BASE/api/bonus/user-stats?wallet_address=$walletAddress")
                val connection = url.openConnection() as HttpURLConnection
                
                connection.apply {
                    requestMethod = "GET"
                    setRequestProperty("Content-Type", "application/json")
                }
                
                val responseCode = connection.responseCode
                if (responseCode == HttpURLConnection.HTTP_OK) {
                    val response = BufferedReader(InputStreamReader(connection.inputStream))
                        .use { it.readText() }
                    
                    val json = JSONObject(response)
                    val success = json.getBoolean("success")
                    
                    if (success) {
                        UserBonusStats(
                            walletAddress = json.getString("wallet_address"),
                            totalPurchases = json.getDouble("total_purchases"),
                            totalBonuses = json.getDouble("total_bonuses"),
                            bonusCount = json.getInt("bonus_count"),
                            highestTier = json.getString("highest_tier"),
                            isVip = json.getBoolean("is_vip")
                        )
                    } else {
                        null
                    }
                } else {
                    null
                }
            } catch (e: Exception) {
                Log.e(TAG, "❌ Stats fetch failed", e)
                null
            }
        }
    }
    
    /**
     * Get system bonus statistics
     */
    suspend fun getSystemStats(): SystemBonusStats? {
        return withContext(Dispatchers.IO) {
            try {
                val url = URL("$BONUS_API_BASE/api/bonus/system-stats")
                val connection = url.openConnection() as HttpURLConnection
                
                connection.apply {
                    requestMethod = "GET"
                    setRequestProperty("Content-Type", "application/json")
                }
                
                val responseCode = connection.responseCode
                if (responseCode == HttpURLConnection.HTTP_OK) {
                    val response = BufferedReader(InputStreamReader(connection.inputStream))
                        .use { it.readText() }
                    
                    val json = JSONObject(response)
                    val success = json.getBoolean("success")
                    
                    if (success) {
                        SystemBonusStats(
                            totalBonuses = json.getLong("total_bonuses"),
                            totalBonusValue = json.getDouble("total_bonus_value"),
                            totalPurchaseVolume = json.getDouble("total_purchase_volume"),
                            bonusesToday = json.getInt("bonuses_today")
                        )
                    } else {
                        null
                    }
                } else {
                    null
                }
            } catch (e: Exception) {
                Log.e(TAG, "❌ System stats fetch failed", e)
                null
            }
        }
    }
}

// Result classes
sealed class BonusResult {
    data class Success(
        val bonusId: String,
        val bonusAmount: Double,
        val tier: String,
        val bonusPercent: Double,
        val message: String
    ) : BonusResult()
    
    data class NotQualified(val message: String) : BonusResult()
    data class Error(val message: String) : BonusResult()
}

data class BonusCalculation(
    val purchaseAmount: Double,
    val bonusAmount: Double,
    val tier: String,
    val bonusPercent: Double,
    val qualifies: Boolean,
    val minimumPurchase: Double
)

data class UserBonusStats(
    val walletAddress: String,
    val totalPurchases: Double,
    val totalBonuses: Double,
    val bonusCount: Int,
    val highestTier: String,
    val isVip: Boolean
)

data class SystemBonusStats(
    val totalBonuses: Long,
    val totalBonusValue: Double,
    val totalPurchaseVolume: Double,
    val bonusesToday: Int
)

