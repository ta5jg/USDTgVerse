//
//  RealAssetRowView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-28.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

struct RealAssetRowView: View {
    let asset: WalletAsset
    @StateObject private var logoService = TokenLogoService()
    @StateObject private var priceService = TokenPriceService()
    @State private var logoImage: UIImage?
    
    var body: some View {
        HStack(spacing: 16) {
            // Token Logo
            tokenLogoView
            
            // Token Info
            VStack(alignment: .leading, spacing: 4) {
                HStack {
                    Text(asset.symbol)
                        .font(.headline)
                        .fontWeight(.semibold)
                        .foregroundColor(.primary)
                    
                    if asset.symbol.hasPrefix("USDTg") {
                        Image(systemName: "checkmark.seal.fill")
                            .foregroundColor(.green)
                            .font(.caption)
                    }
                }
                
                Text(asset.name)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            // Balance and Value
            VStack(alignment: .trailing, spacing: 4) {
                Text(asset.formattedBalance)
                    .font(.subheadline)
                    .fontWeight(.medium)
                    .foregroundColor(.primary)
                
                // Show individual token price, not total value
                Text("\(priceService.getFormattedPrice(for: asset.symbol)) each")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
        }
        .padding(.horizontal, 16)
        .padding(.vertical, 12)
        .background(
            RoundedRectangle(cornerRadius: 12)
                .fill(Color(.systemGray6))
        )
        .overlay(
            RoundedRectangle(cornerRadius: 12)
                .stroke(
                    asset.symbol.hasPrefix("USDTg") ? Color.green.opacity(0.3) : Color.clear,
                    lineWidth: 1
                )
        )
        .onAppear {
            loadTokenLogo()
        }
    }
    
    private var tokenLogoView: some View {
        ZStack {
            Circle()
                .fill(logoService.getLogoColor(for: asset.symbol))
                .frame(width: 40, height: 40)
            
            if let logoImage = logoImage {
                Image(uiImage: logoImage)
                    .resizable()
                    .aspectRatio(contentMode: .fit)
                    .frame(width: 32, height: 32)
                    .clipShape(Circle())
            } else {
                // Fallback to SF Symbols
                if asset.symbol.hasPrefix("USDTg") {
                    Text(asset.symbol == "USDTg" ? "USDTg" : 
                         asset.symbol == "USDTgV" ? "V" : "G")
                        .font(.system(size: 16, weight: .bold))
                        .foregroundColor(.white)
                } else {
                    Image(systemName: logoService.getLogo(for: asset.symbol))
                        .font(.system(size: 20, weight: .medium))
                        .foregroundColor(.white)
                }
            }
        }
    }
    
    private func loadTokenLogo() {
        let logoURL = priceService.getLogoURL(for: asset.symbol)
        guard !logoURL.isEmpty, let url = URL(string: logoURL) else { return }
        
        URLSession.shared.dataTask(with: url) { data, response, error in
            guard let data = data, let image = UIImage(data: data) else { return }
            
            DispatchQueue.main.async {
                self.logoImage = image
            }
        }.resume()
    }
}

// MARK: - Enhanced Asset Row with Price Change
struct EnhancedAssetRowView: View {
    let asset: WalletAsset
    let priceChange24h: Double
    @StateObject private var logoService = TokenLogoService()
    
    var body: some View {
        HStack(spacing: 16) {
            // Token Logo
            tokenLogoView
            
            // Token Info
            VStack(alignment: .leading, spacing: 4) {
                HStack {
                    Text(asset.symbol)
                        .font(.headline)
                        .fontWeight(.semibold)
                        .foregroundColor(.primary)
                    
                    if asset.symbol.hasPrefix("USDTg") {
                        Image(systemName: "checkmark.seal.fill")
                            .foregroundColor(.green)
                            .font(.caption)
                    }
                }
                
                Text(asset.name)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            // Balance and Value
            VStack(alignment: .trailing, spacing: 4) {
                Text(asset.formattedBalance)
                    .font(.subheadline)
                    .fontWeight(.medium)
                    .foregroundColor(.primary)
                
                Text(asset.formattedValue)
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                // 24h Price Change
                HStack(spacing: 4) {
                    Image(systemName: priceChange24h >= 0 ? "arrow.up.right" : "arrow.down.right")
                        .font(.caption2)
                        .foregroundColor(priceChange24h >= 0 ? .green : .red)
                    
                    Text(String(format: "%.2f%%", abs(priceChange24h)))
                        .font(.caption2)
                        .foregroundColor(priceChange24h >= 0 ? .green : .red)
                }
            }
        }
        .padding(.horizontal, 16)
        .padding(.vertical, 12)
        .background(
            RoundedRectangle(cornerRadius: 12)
                .fill(Color(.systemGray6))
        )
        .overlay(
            RoundedRectangle(cornerRadius: 12)
                .stroke(
                    asset.symbol.hasPrefix("USDTg") ? Color.green.opacity(0.3) : Color.clear,
                    lineWidth: 1
                )
        )
    }
    
    private var tokenLogoView: some View {
        ZStack {
            Circle()
                .fill(logoService.getLogoColor(for: asset.symbol))
                .frame(width: 40, height: 40)
            
            if asset.symbol.hasPrefix("USDTg") {
                // USDTgVerse Native Token Logos
                Text(asset.symbol == "USDTg" ? "USDTg" : 
                     asset.symbol == "USDTgV" ? "V" : "G")
                    .font(.system(size: 16, weight: .bold))
                    .foregroundColor(.white)
            } else {
                // External Token Logos
                Image(systemName: logoService.getLogo(for: asset.symbol))
                    .font(.system(size: 20, weight: .medium))
                    .foregroundColor(.white)
            }
        }
    }
}

// MARK: - Preview
struct RealAssetRowView_Previews: PreviewProvider {
    static var previews: some View {
        VStack(spacing: 8) {
            AssetRowView(asset: WalletAsset(
                symbol: "USDTg",
                name: "USDTgVerse Native Coin",
                balance: 1000.0,
                price: 1.00,
                logoURL: "usdtg_logo",
                change24h: 2.5,
                chain: "USDTgVerse",
                isNativeImage: true
            ))
            
            AssetRowView(asset: WalletAsset(
                symbol: "USDTgV",
                name: "USDTgVerse Utility Token",
                balance: 500.0,
                price: 0.50,
                logoURL: "usdt_logo",
                change24h: 1.8,
                chain: "USDTgVerse",
                isNativeImage: true
            ))
            
            AssetRowView(asset: WalletAsset(
                symbol: "BTC",
                name: "Bitcoin",
                balance: 0.025,
                price: 45000.0,
                logoURL: "https://assets.coingecko.com/coins/images/1/large/bitcoin.png",
                change24h: -1.2,
                chain: "Bitcoin",
                isNativeImage: false
            ))
        }
        .padding()
        .previewLayout(.sizeThatFits)
    }
}
