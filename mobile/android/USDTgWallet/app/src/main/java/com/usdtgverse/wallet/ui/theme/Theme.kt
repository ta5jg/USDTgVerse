package com.usdtgverse.wallet.ui.theme

import android.app.Activity
import android.os.Build
import androidx.compose.foundation.isSystemInDarkTheme
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.darkColorScheme
import androidx.compose.material3.dynamicDarkColorScheme
import androidx.compose.material3.dynamicLightColorScheme
import androidx.compose.material3.lightColorScheme
import androidx.compose.runtime.Composable
import androidx.compose.runtime.SideEffect
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.toArgb
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.LocalView
import androidx.core.view.WindowCompat

// USDTgVerse Brand Colors
val USDTgGreen = Color(0xFF4CAF50)
val USDTgDarkGreen = Color(0xFF2E7D32)
val USDTgLightGreen = Color(0xFF81C784)
val USDTgPurple = Color(0xFF673AB7)
val USDTgBlue = Color(0xFF2196F3)
val USDTgOrange = Color(0xFFFF9800)

private val DarkColorScheme = darkColorScheme(
    primary = USDTgGreen,
    secondary = USDTgPurple,
    tertiary = USDTgBlue,
    background = Color(0xFF121212),
    surface = Color(0xFF1E1E1E),
    onPrimary = Color.White,
    onSecondary = Color.White,
    onTertiary = Color.White,
    onBackground = Color.White,
    onSurface = Color.White,
    primaryContainer = USDTgDarkGreen,
    secondaryContainer = Color(0xFF4A148C),
    tertiaryContainer = Color(0xFF0D47A1),
    onPrimaryContainer = Color.White,
    onSecondaryContainer = Color.White,
    onTertiaryContainer = Color.White,
    surfaceVariant = Color(0xFF2A2A2A),
    onSurfaceVariant = Color(0xFFE0E0E0),
    outline = Color(0xFF616161),
    outlineVariant = Color(0xFF424242),
    scrim = Color.Black,
    inverseSurface = Color(0xFFE0E0E0),
    inverseOnSurface = Color(0xFF121212),
    inversePrimary = USDTgDarkGreen,
    surfaceDim = Color(0xFF0F0F0F),
    surfaceBright = Color(0xFF2A2A2A),
    surfaceContainerLowest = Color(0xFF0A0A0A),
    surfaceContainerLow = Color(0xFF1A1A1A),
    surfaceContainer = Color(0xFF1E1E1E),
    surfaceContainerHigh = Color(0xFF282828),
    surfaceContainerHighest = Color(0xFF323232)
)

private val LightColorScheme = lightColorScheme(
    primary = USDTgGreen,
    secondary = USDTgPurple,
    tertiary = USDTgBlue,
    background = Color(0xFFFFFBFE),
    surface = Color(0xFFFFFBFE),
    onPrimary = Color.White,
    onSecondary = Color.White,
    onTertiary = Color.White,
    onBackground = Color(0xFF1C1B1F),
    onSurface = Color(0xFF1C1B1F),
    primaryContainer = USDTgLightGreen,
    secondaryContainer = Color(0xFFE1BEE7),
    tertiaryContainer = Color(0xFFBBDEFB),
    onPrimaryContainer = Color(0xFF1B5E20),
    onSecondaryContainer = Color(0xFF4A148C),
    onTertiaryContainer = Color(0xFF0D47A1)
)

@Composable
fun USDTgWalletTheme(
    darkTheme: Boolean = isSystemInDarkTheme(),
    // Dynamic color is available on Android 12+
    dynamicColor: Boolean = true,
    content: @Composable () -> Unit
) {
    val colorScheme = when {
        dynamicColor && Build.VERSION.SDK_INT >= Build.VERSION_CODES.S -> {
            val context = LocalContext.current
            if (darkTheme) dynamicDarkColorScheme(context) else dynamicLightColorScheme(context)
        }

        darkTheme -> DarkColorScheme
        else -> LightColorScheme
    }
    
    val view = LocalView.current
    if (!view.isInEditMode) {
        SideEffect {
            val window = (view.context as Activity).window
            window.statusBarColor = colorScheme.primary.toArgb()
            WindowCompat.getInsetsController(window, view).isAppearanceLightStatusBars = darkTheme
        }
    }

    MaterialTheme(
        colorScheme = colorScheme,
        typography = Typography,
        content = content
    )
}

// Additional USDTgVerse specific colors
object USDTgColors {
    val Success = Color(0xFF4CAF50)
    val Warning = Color(0xFFFF9800)
    val Error = Color(0xFFF44336)
    val Info = Color(0xFF2196F3)
    
    // Network specific colors
    val Ethereum = Color(0xFF627EEA)
    val BNBChain = Color(0xFFF3BA2F)
    val Polygon = Color(0xFF8247E5)
    val Arbitrum = Color(0xFF28A0F0)
    val Tron = Color(0xFFFF060A)
    val Solana = Color(0xFF9945FF)
    val Avalanche = Color(0xFFE84142)
    val Optimism = Color(0xFFFF0420)
    
    // USDTgVerse PAY feature colors
    val CorporateCards = Color(0xFF2196F3)
    val ExpenseManagement = Color(0xFF4CAF50)
    val Treasury = Color(0xFF673AB7)
    val BillPay = Color(0xFFFF9800)
    val Invoice = Color(0xFFF44336)
    val Escrow = Color(0xFFFFEB3B)
    val Subscription = Color(0xFFE91E63)
    val BatchPayments = Color(0xFF00BCD4)
    val MerchantGateway = Color(0xFF3F51B5)
    val Analytics = Color(0xFF009688)
}

// Gradient definitions
object USDTgGradients {
    val PrimaryGradient = listOf(
        USDTgGreen,
        USDTgDarkGreen
    )
    
    val SecondaryGradient = listOf(
        USDTgPurple,
        USDTgBlue
    )
    
    val BackgroundGradient = listOf(
        Color(0xFF1A1A2E),
        Color(0xFF16213E),
        Color(0xFF0F3460)
    )
    
    val CardGradient = listOf(
        Color(0xFF2A2A2A),
        Color(0xFF1E1E1E)
    )
}
