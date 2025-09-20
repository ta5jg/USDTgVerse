//
//  InlineWalletSetup.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

// MARK: - Inline Wallet Setup (Embedded in existing project)
struct InlineWalletSetup: View {
    @State private var selectedOption = 0
    @State private var walletName = ""
    @State private var seedPhrase = ""
    @State private var isCreating = false
    @State private var showingMnemonic = false
    @State private var generatedMnemonic = ""
    @State private var showingSuccess = false
    @Binding var hasWallet: Bool
    
    var body: some View {
        VStack(spacing: 24) {
            // Header
            VStack(spacing: 16) {
                Image("usdtg_logo")
                    .resizable()
                    .aspectRatio(contentMode: .fit)
                    .frame(width: 80, height: 80)
                    .clipShape(Circle())
                    .overlay(
                        Circle()
                            .stroke(LinearGradient(gradient: Gradient(colors: [Color.green, Color.blue]), startPoint: .topLeading, endPoint: .bottomTrailing), lineWidth: 3)
                    )
                
                Text("Welcome to USDTgVerse")
                    .font(.title2)
                    .fontWeight(.bold)
                
                Text("Create or import your multi-chain wallet")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                    .multilineTextAlignment(.center)
            }
            
            // Options
            VStack(spacing: 16) {
                // Create New Wallet
                Button(action: { selectedOption = 0 }) {
                    HStack(spacing: 16) {
                        Image(systemName: "plus.circle.fill")
                            .font(.title2)
                            .foregroundColor(selectedOption == 0 ? .white : .blue)
                        
                        VStack(alignment: .leading, spacing: 4) {
                            Text("Create New Wallet")
                                .font(.headline)
                                .fontWeight(.semibold)
                            
                            Text("Generate a new wallet + Get 10 USDTg bonus!")
                                .font(.caption)
                                .opacity(0.8)
                        }
                        
                        Spacer()
                        
                        Image(systemName: selectedOption == 0 ? "checkmark.circle.fill" : "circle")
                            .foregroundColor(selectedOption == 0 ? .white : .secondary)
                    }
                    .foregroundColor(selectedOption == 0 ? .white : .primary)
                    .padding()
                    .background(selectedOption == 0 ? Color.blue : Color(.systemGray6))
                    .cornerRadius(12)
                }
                
                // Import Existing Wallet
                Button(action: { selectedOption = 1 }) {
                    HStack(spacing: 16) {
                        Image(systemName: "square.and.arrow.down.fill")
                            .font(.title2)
                            .foregroundColor(selectedOption == 1 ? .white : .green)
                        
                        VStack(alignment: .leading, spacing: 4) {
                            Text("Import Existing Wallet")
                                .font(.headline)
                                .fontWeight(.semibold)
                            
                            Text("Import with seed phrase")
                                .font(.caption)
                                .opacity(0.8)
                        }
                        
                        Spacer()
                        
                        Image(systemName: selectedOption == 1 ? "checkmark.circle.fill" : "circle")
                            .foregroundColor(selectedOption == 1 ? .white : .secondary)
                    }
                    .foregroundColor(selectedOption == 1 ? .white : .primary)
                    .padding()
                    .background(selectedOption == 1 ? Color.green : Color(.systemGray6))
                    .cornerRadius(12)
                }
            }
            
            // Input Fields
            VStack(spacing: 16) {
                // Wallet Name
                VStack(alignment: .leading, spacing: 8) {
                    Text("Wallet Name")
                        .font(.headline)
                        .fontWeight(.medium)
                    
                    TextField("My USDTgVerse Wallet", text: $walletName)
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                }
                
                // Import Field (only show if importing)
                if selectedOption == 1 {
                    VStack(alignment: .leading, spacing: 8) {
                        Text("Seed Phrase (12 or 24 words)")
                            .font(.headline)
                            .fontWeight(.medium)
                        
                        TextEditor(text: $seedPhrase)
                            .frame(height: 100)
                            .padding(8)
                            .background(Color(.systemGray6))
                            .cornerRadius(8)
                            .overlay(
                                RoundedRectangle(cornerRadius: 8)
                                    .stroke(Color(.systemGray4), lineWidth: 1)
                            )
                    }
                }
            }
            
            Spacer()
            
            // Action Button
            Button(action: handleWalletAction) {
                HStack {
                    if isCreating {
                        ProgressView()
                            .progressViewStyle(CircularProgressViewStyle(tint: .white))
                            .scaleEffect(0.8)
                    }
                    
                    Text(selectedOption == 0 ? "Create Wallet & Get 10 USDTg" : "Import Wallet")
                        .font(.headline)
                        .fontWeight(.semibold)
                }
                .foregroundColor(.white)
                .frame(maxWidth: .infinity)
                .padding()
                .background(
                    LinearGradient(
                        gradient: Gradient(colors: [selectedOption == 0 ? .blue : .green, (selectedOption == 0 ? .blue : .green).opacity(0.8)]),
                        startPoint: .topLeading,
                        endPoint: .bottomTrailing
                    )
                )
                .cornerRadius(12)
                .disabled(walletName.isEmpty || isCreating)
            }
        }
        .padding()
        .sheet(isPresented: $showingMnemonic) {
            InlineMnemonicView(mnemonic: generatedMnemonic, hasWallet: $hasWallet)
        }
        .sheet(isPresented: $showingSuccess) {
            InlineSuccessView(walletName: walletName, hasWallet: $hasWallet)
        }
    }
    
    private func handleWalletAction() {
        isCreating = true
        
        if selectedOption == 0 {
            // Create new wallet
            createNewWallet()
        } else {
            // Import existing wallet
            importExistingWallet()
        }
    }
    
    private func createNewWallet() {
        // Generate mnemonic seed phrase
        generatedMnemonic = generateMnemonic()
        
        // Simulate wallet creation
        DispatchQueue.main.asyncAfter(deadline: .now() + 2) {
            isCreating = false
            showingMnemonic = true
        }
    }
    
    private func importExistingWallet() {
        // Save wallet info (simplified)
        UserDefaults.standard.set(true, forKey: "hasWallet")
        UserDefaults.standard.set(walletName, forKey: "walletName")
        UserDefaults.standard.set(seedPhrase, forKey: "walletMnemonic")
        
        DispatchQueue.main.asyncAfter(deadline: .now() + 2) {
            isCreating = false
            showingSuccess = true
        }
    }
    
    private func generateMnemonic() -> String {
        // BIP39 word list (simplified for demo)
        let words = [
            "abandon", "ability", "able", "about", "above", "absent", "absorb", "abstract",
            "absurd", "abuse", "access", "accident", "account", "accuse", "achieve", "acid",
            "acoustic", "acquire", "across", "act", "action", "actor", "actress", "actual",
            "adapt", "add", "addict", "address", "adjust", "admit", "adult", "advance",
            "advice", "aerobic", "affair", "afford", "afraid", "again", "agent", "agree",
            "ahead", "aim", "air", "airport", "aisle", "alarm", "album", "alcohol"
        ]
        
        // Generate 12 random words
        var mnemonic: [String] = []
        for _ in 0..<12 {
            let randomIndex = Int.random(in: 0..<words.count)
            mnemonic.append(words[randomIndex])
        }
        
        return mnemonic.joined(separator: " ")
    }
}

// MARK: - Inline Mnemonic View
struct InlineMnemonicView: View {
    let mnemonic: String
    @Environment(\.presentationMode) var presentationMode
    @State private var isConfirmed = false
    @Binding var hasWallet: Bool
    
    var body: some View {
        NavigationView {
            VStack(spacing: 24) {
                // Warning
                VStack(spacing: 16) {
                    Image(systemName: "exclamationmark.triangle.fill")
                        .font(.system(size: 48))
                        .foregroundColor(.orange)
                    
                    Text("IMPORTANT: Save Your Seed Phrase")
                        .font(.title2)
                        .fontWeight(.bold)
                        .multilineTextAlignment(.center)
                    
                    Text("Write down these 12 words in order. This is the ONLY way to recover your wallet.")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                }
                
                // Mnemonic Grid
                let words = mnemonic.components(separatedBy: " ")
                LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 2), spacing: 12) {
                    ForEach(Array(words.enumerated()), id: \.offset) { index, word in
                        HStack {
                            Text("\(index + 1).")
                                .font(.caption)
                                .foregroundColor(.secondary)
                                .frame(width: 20, alignment: .leading)
                            
                            Text(word)
                                .font(.headline)
                                .fontWeight(.medium)
                            
                            Spacer()
                        }
                        .padding()
                        .background(Color(.systemGray6))
                        .cornerRadius(8)
                    }
                }
                
                // Confirmation
                Button(action: { isConfirmed.toggle() }) {
                    HStack(spacing: 12) {
                        Image(systemName: isConfirmed ? "checkmark.square.fill" : "square")
                            .foregroundColor(isConfirmed ? .green : .secondary)
                        
                        Text("I have safely written down my seed phrase")
                            .font(.subheadline)
                    }
                }
                
                Spacer()
                
                // Continue Button
                Button(action: {
                    // Save wallet and close
                    UserDefaults.standard.set(true, forKey: "hasWallet")
                    UserDefaults.standard.set("My Wallet", forKey: "walletName")
                    UserDefaults.standard.set(mnemonic, forKey: "walletMnemonic")
                    
                    hasWallet = true
                    presentationMode.wrappedValue.dismiss()
                }) {
                    Text("Continue to Wallet & Get 10 USDTg")
                        .font(.headline)
                        .fontWeight(.semibold)
                        .foregroundColor(.white)
                        .frame(maxWidth: .infinity)
                        .padding()
                        .background(isConfirmed ? Color.green : Color.gray)
                        .cornerRadius(12)
                }
                .disabled(!isConfirmed)
            }
            .padding()
            .navigationTitle("Seed Phrase")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Close") {
                presentationMode.wrappedValue.dismiss()
            })
        }
    }
}

// MARK: - Inline Success View
struct InlineSuccessView: View {
    let walletName: String
    @Environment(\.presentationMode) var presentationMode
    @Binding var hasWallet: Bool
    
    var body: some View {
        NavigationView {
            VStack(spacing: 32) {
                Spacer()
                
                // Success Animation
                VStack(spacing: 24) {
                    Image(systemName: "checkmark.circle.fill")
                        .font(.system(size: 80))
                        .foregroundColor(.green)
                    
                    Text("Wallet Ready!")
                        .font(.title)
                        .fontWeight(.bold)
                        .multilineTextAlignment(.center)
                    
                    Text("Your wallet '\(walletName)' is ready to use")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                }
                
                // Bonus Info
                VStack(spacing: 16) {
                    HStack(spacing: 12) {
                        Image(systemName: "gift.fill")
                            .font(.title2)
                            .foregroundColor(.blue)
                        
                        VStack(alignment: .leading, spacing: 4) {
                            Text("Welcome Bonus!")
                                .font(.headline)
                                .fontWeight(.semibold)
                            
                            Text("You'll receive 10 USDTg as welcome gift")
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        Spacer()
                    }
                    .padding()
                    .background(Color.blue.opacity(0.1))
                    .cornerRadius(12)
                }
                
                Spacer()
                
                // Continue Button
                Button(action: {
                    hasWallet = true
                    presentationMode.wrappedValue.dismiss()
                }) {
                    Text("Start Using Wallet")
                        .font(.headline)
                        .fontWeight(.semibold)
                        .foregroundColor(.white)
                        .frame(maxWidth: .infinity)
                        .padding()
                        .background(Color.green)
                        .cornerRadius(12)
                }
            }
            .padding()
            .navigationTitle("Success")
            .navigationBarTitleDisplayMode(.inline)
        }
    }
}

struct InlineWalletSetup_Previews: PreviewProvider {
    static var previews: some View {
        InlineWalletSetup(hasWallet: .constant(false))
    }
}
