/*
==============================================
 File:        AirdropManager.kt
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-10-13
 Last Update: 2025-10-13
 Version:     1.0

 Description:
   USDTgVerse Android Airdrop Manager
   
   Manages 10 USDTg locked airdrop system:
   - Request airdrop for new wallets
   - Pay fees with locked airdrop
   - Verify purchases and unlock airdrop
   - Anti-fraud protection

 License:
   MIT License
==============================================
*/

package com.usdtgverse.wallet.data

import android.content.Context
import android.provider.Settings
import android.util.Log
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import org.json.JSONObject
import java.io.BufferedReader
import java.io.InputStreamReader
import java.io.OutputStreamWriter
import java.net.HttpURLConnection
import java.net.URL

class AirdropManager(private val context: Context) {
    
    companion object {
        private const val TAG = "AirdropManager"
        private const val AIRDROP_API_BASE = "http://10.0.2.2:3006" // Android emulator localhost
        private const val AIRDROP_AMOUNT = 10.0
        private const val MIN_PURCHASE_UNLOCK = 50.0
    }
    
    /**
     * Request 10 USDTg airdrop for new wallet (LOCKED - Fee only)
     */
    suspend fun requestAirdrop(walletAddress: String, userId: String): AirdropResult {
        return withContext(Dispatchers.IO) {
            try {
                val deviceId = Settings.Secure.getString(
                    context.contentResolver,
                    Settings.Secure.ANDROID_ID
                )
                
                val url = URL("$AIRDROP_API_BASE/api/airdrop/create")
                val connection = url.openConnection() as HttpURLConnection
                
                connection.apply {
                    requestMethod = "POST"
                    setRequestProperty("Content-Type", "application/json")
                    doOutput = true
                }
                
                val requestBody = JSONObject().apply {
                    put("wallet_address", walletAddress)
                    put("user_id", userId)
                    put("device_fingerprint", deviceId)
                    put("ip_address", "0.0.0.0")
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
                        val airdropId = json.getString("airdrop_id")
                        val amount = json.getDouble("amount")
                        val message = json.getString("message")
                        
                        Log.d(TAG, "✅ Airdrop received: $airdropId")
                        Log.d(TAG, "💰 Amount: $amount USDTg (LOCKED - Fee only)")
                        Log.d(TAG, "📝 $message")
                        
                        AirdropResult.Success(
                            airdropId = airdropId,
                            amount = amount,
                            message = message
                        )
                    } else {
                        val error = json.optString("error", "Unknown error")
                        Log.e(TAG, "❌ Airdrop failed: $error")
                        AirdropResult.Error(error)
                    }
                } else {
                    Log.e(TAG, "❌ HTTP Error: $responseCode")
                    AirdropResult.Error("HTTP Error: $responseCode")
                }
            } catch (e: Exception) {
                Log.e(TAG, "❌ Airdrop request failed", e)
                AirdropResult.Error(e.message ?: "Unknown error")
            }
        }
    }
    
    /**
     * Use locked airdrop to pay transaction fee
     */
    suspend fun payFeeWithAirdrop(walletAddress: String, feeAmount: Double): Boolean {
        return withContext(Dispatchers.IO) {
            try {
                val url = URL("$AIRDROP_API_BASE/api/airdrop/use-fee")
                val connection = url.openConnection() as HttpURLConnection
                
                connection.apply {
                    requestMethod = "POST"
                    setRequestProperty("Content-Type", "application/json")
                    doOutput = true
                }
                
                val requestBody = JSONObject().apply {
                    put("wallet_address", walletAddress)
                    put("fee_amount", feeAmount)
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
                        val remaining = json.getDouble("remaining")
                        Log.d(TAG, "✅ Fee paid from airdrop: $feeAmount USDTg")
                        Log.d(TAG, "💰 Remaining locked: $remaining USDTg")
                        true
                    } else {
                        Log.e(TAG, "❌ Fee payment failed")
                        false
                    }
                } else {
                    Log.e(TAG, "❌ HTTP Error: $responseCode")
                    false
                }
            } catch (e: Exception) {
                Log.e(TAG, "❌ Fee payment failed", e)
                false
            }
        }
    }
    
    /**
     * Verify purchase and unlock airdrop if threshold met
     */
    suspend fun verifyPurchaseAndUnlock(
        walletAddress: String,
        purchaseAmount: Double
    ): UnlockResult {
        return withContext(Dispatchers.IO) {
            try {
                val url = URL("$AIRDROP_API_BASE/api/airdrop/verify-purchase")
                val connection = url.openConnection() as HttpURLConnection
                
                connection.apply {
                    requestMethod = "POST"
                    setRequestProperty("Content-Type", "application/json")
                    doOutput = true
                }
                
                val requestBody = JSONObject().apply {
                    put("wallet_address", walletAddress)
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
                        val unlocked = json.getBoolean("unlocked")
                        val message = json.getString("message")
                        val remainingToUnlock = json.getDouble("remaining_to_unlock")
                        
                        if (unlocked) {
                            Log.d(TAG, "🔓 AIRDROP UNLOCKED!")
                            Log.d(TAG, "💰 10 USDTg now available for all transactions")
                            UnlockResult.Unlocked(message)
                        } else {
                            Log.d(TAG, "💰 Purchase recorded. Need $remainingToUnlock more USDTg to unlock")
                            UnlockResult.Pending(remainingToUnlock, message)
                        }
                    } else {
                        val error = json.optString("error", "Unknown error")
                        UnlockResult.Error(error)
                    }
                } else {
                    Log.e(TAG, "❌ HTTP Error: $responseCode")
                    UnlockResult.Error("HTTP Error: $responseCode")
                }
            } catch (e: Exception) {
                Log.e(TAG, "❌ Purchase verification failed", e)
                UnlockResult.Error(e.message ?: "Unknown error")
            }
        }
    }
    
    /**
     * Get airdrop status for wallet
     */
    suspend fun getAirdropStatus(walletAddress: String): AirdropStatus? {
        return withContext(Dispatchers.IO) {
            try {
                val url = URL("$AIRDROP_API_BASE/api/airdrop/status?wallet_address=$walletAddress")
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
                        AirdropStatus(
                            airdropId = json.getString("airdrop_id"),
                            walletAddress = json.getString("wallet_address"),
                            airdropAmount = json.getDouble("airdrop_amount"),
                            lockedAmount = json.getDouble("locked_amount"),
                            usedAmount = json.getDouble("used_amount"),
                            purchaseAmount = json.getDouble("purchase_amount"),
                            status = json.getString("status"),
                            unlocked = json.getBoolean("unlocked"),
                            feeUsageCount = json.getInt("fee_usage_count")
                        )
                    } else {
                        null
                    }
                } else {
                    null
                }
            } catch (e: Exception) {
                Log.e(TAG, "❌ Status check failed", e)
                null
            }
        }
    }
}

// Result classes
sealed class AirdropResult {
    data class Success(
        val airdropId: String,
        val amount: Double,
        val message: String
    ) : AirdropResult()
    
    data class Error(val message: String) : AirdropResult()
}

sealed class UnlockResult {
    data class Unlocked(val message: String) : UnlockResult()
    data class Pending(val remainingAmount: Double, val message: String) : UnlockResult()
    data class Error(val message: String) : UnlockResult()
}

data class AirdropStatus(
    val airdropId: String,
    val walletAddress: String,
    val airdropAmount: Double,
    val lockedAmount: Double,
    val usedAmount: Double,
    val purchaseAmount: Double,
    val status: String,
    val unlocked: Boolean,
    val feeUsageCount: Int
)

