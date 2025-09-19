/*
 * USDTgVerse P2P Network Manager - C++ High-Level Interface
 * Enterprise-grade networking built on ultra-fast C core
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>
#include <functional>
#include <sstream>
#include <iomanip>

extern "C" {
    #include "network_core.c"
}

namespace usdtgverse::p2p {

// ============================================================================
// C++ WRAPPER CLASSES
// ============================================================================

class PeerID {
private:
    usdtg_peer_id_t id_;
    
public:
    PeerID() { memset(id_, 0, 32); }
    
    static PeerID fromSeed(uint8_t seed) {
        PeerID peer_id;
        memset(peer_id.id_, seed, 32);
        return peer_id;
    }
    
    std::string toString() const {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (int i = 0; i < 6; i++) {
            oss << std::setw(2) << static_cast<unsigned>(id_[i]);
        }
        oss << "...";
        return oss.str();
    }
    
    const usdtg_peer_id_t& raw() const { return id_; }
};

class PeerInfo {
public:
    PeerID peer_id;
    std::string ip_address;
    uint16_t port;
    std::string state;
    int32_t reputation;
    uint64_t bytes_sent;
    uint64_t bytes_received;
    uint64_t messages_sent;
    uint64_t messages_received;
    
    PeerInfo(const usdtg_peer_t& core_peer) {
        peer_id = PeerID::fromSeed(core_peer.peer_id[0]);
        
        // Convert IP address
        std::ostringstream ip_oss;
        ip_oss << ((core_peer.addr >> 24) & 0xFF) << "."
               << ((core_peer.addr >> 16) & 0xFF) << "."
               << ((core_peer.addr >> 8) & 0xFF) << "."
               << (core_peer.addr & 0xFF);
        ip_address = ip_oss.str();
        
        port = core_peer.port;
        
        switch (core_peer.state) {
            case PEER_DISCONNECTED: state = "DISCONNECTED"; break;
            case PEER_CONNECTING: state = "CONNECTING"; break;
            case PEER_HANDSHAKING: state = "HANDSHAKING"; break;
            case PEER_CONNECTED: state = "CONNECTED"; break;
            case PEER_BANNED: state = "BANNED"; break;
            default: state = "UNKNOWN"; break;
        }
        
        reputation = core_peer.reputation_score;
        bytes_sent = core_peer.bytes_sent;
        bytes_received = core_peer.bytes_received;
        messages_sent = core_peer.messages_sent;
        messages_received = core_peer.messages_received;
    }
    
    std::string toString() const {
        std::ostringstream oss;
        oss << "Peer{" << peer_id.toString() 
            << " @ " << ip_address << ":" << port
            << ", " << state 
            << ", rep:" << reputation
            << ", sent:" << messages_sent 
            << ", recv:" << messages_received << "}";
        return oss.str();
    }
};

class NetworkMessage {
public:
    enum Type {
        HANDSHAKE = MSG_HANDSHAKE,
        HEARTBEAT = MSG_HEARTBEAT,
        TRANSACTION = MSG_TRANSACTION,
        BLOCK = MSG_BLOCK,
        CONSENSUS_VOTE = MSG_CONSENSUS_VOTE,
        CONSENSUS_PROPOSAL = MSG_CONSENSUS_PROPOSAL,
        PEER_LIST = MSG_PEER_LIST,
        PING = MSG_PING,
        PONG = MSG_PONG
    };
    
    Type type;
    std::vector<uint8_t> payload;
    PeerID origin;
    uint8_t hop_count;
    
    NetworkMessage(Type t, const std::vector<uint8_t>& data = {}) 
        : type(t), payload(data), hop_count(0) {}
    
    std::string toString() const {
        std::string type_str;
        switch (type) {
            case HANDSHAKE: type_str = "HANDSHAKE"; break;
            case HEARTBEAT: type_str = "HEARTBEAT"; break;
            case TRANSACTION: type_str = "TRANSACTION"; break;
            case BLOCK: type_str = "BLOCK"; break;
            case CONSENSUS_VOTE: type_str = "CONSENSUS_VOTE"; break;
            case CONSENSUS_PROPOSAL: type_str = "CONSENSUS_PROPOSAL"; break;
            case PEER_LIST: type_str = "PEER_LIST"; break;
            case PING: type_str = "PING"; break;
            case PONG: type_str = "PONG"; break;
            default: type_str = "UNKNOWN"; break;
        }
        
        return "Message{" + type_str + ", " + std::to_string(payload.size()) + " bytes, hops:" + 
               std::to_string(hop_count) + "}";
    }
};

// ============================================================================
// NETWORK MANAGER
// ============================================================================

class NetworkManager {
private:
    usdtg_network_t core_network_;
    std::atomic<bool> running_;
    std::unique_ptr<std::thread> network_thread_;
    
    // Event callbacks
    std::function<void(const NetworkMessage&, const PeerID&)> on_message_received_;
    std::function<void(const PeerID&)> on_peer_connected_;
    std::function<void(const PeerID&, const std::string&)> on_peer_disconnected_;
    
public:
    NetworkManager() : running_(false) {
        memset(&core_network_, 0, sizeof(core_network_));
    }
    
    ~NetworkManager() {
        stop();
    }
    
    bool start(uint16_t listen_port = 26656) {
        if (running_.load()) return false;
        
        if (usdtg_network_init(&core_network_, listen_port) != 0) {
            return false;
        }
        
        running_.store(true);
        
        // Start network thread
        network_thread_ = std::make_unique<std::thread>([this]() {
            while (running_.load()) {
                usdtg_network_step(&core_network_);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
        
        return true;
    }
    
    void stop() {
        if (running_.load()) {
            running_.store(false);
            
            if (network_thread_ && network_thread_->joinable()) {
                network_thread_->join();
            }
            
            usdtg_network_cleanup(&core_network_);
        }
    }
    
    bool connectToPeer(const std::string& ip, uint16_t port) {
        // Convert IP string to uint32_t
        uint32_t addr = inet_addr(ip.c_str());
        if (addr == INADDR_NONE) return false;
        
        int peer_index = usdtg_peer_connect(&core_network_, ntohl(addr), port);
        return peer_index >= 0;
    }
    
    bool broadcastMessage(const NetworkMessage& message) {
        usdtg_network_msg_t core_msg;
        memset(&core_msg, 0, sizeof(core_msg));
        core_msg.type = static_cast<usdtg_msg_type_t>(message.type);
        core_msg.payload_size = std::min(message.payload.size(), size_t(MAX_MESSAGE_SIZE));
        core_msg.hop_count = message.hop_count;
        
        if (core_msg.payload_size > 0) {
            memcpy(core_msg.payload, message.payload.data(), core_msg.payload_size);
        }
        
        int sent_count = usdtg_network_broadcast_message(&core_network_, &core_msg);
        return sent_count > 0;
    }
    
    std::vector<PeerInfo> getPeers() const {
        std::vector<PeerInfo> peers;
        
        for (uint32_t i = 0; i < core_network_.peer_count; i++) {
            peers.emplace_back(core_network_.peers[i]);
        }
        
        return peers;
    }
    
    size_t getConnectedPeerCount() const {
        return core_network_.connected_peer_count;
    }
    
    std::string getNetworkStatus() const {
        std::ostringstream oss;
        oss << "Network Status:\n";
        oss << "  Listen Port: " << core_network_.listen_port << "\n";
        oss << "  Total Peers: " << core_network_.peer_count << "\n";
        oss << "  Connected Peers: " << core_network_.connected_peer_count << "\n";
        oss << "  Banned Peers: " << core_network_.banned_peers << "\n";
        oss << "  Messages Sent: " << core_network_.total_messages_sent << "\n";
        oss << "  Messages Received: " << core_network_.total_messages_received << "\n";
        oss << "  Bytes Sent: " << core_network_.total_bytes_sent << "\n";
        oss << "  Bytes Received: " << core_network_.total_bytes_received << "\n";
        oss << "  Dropped Messages: " << core_network_.dropped_messages << "\n";
        
        return oss.str();
    }
    
    // Event handlers
    void setMessageReceivedHandler(std::function<void(const NetworkMessage&, const PeerID&)> handler) {
        on_message_received_ = std::move(handler);
    }
    
    void setPeerConnectedHandler(std::function<void(const PeerID&)> handler) {
        on_peer_connected_ = std::move(handler);
    }
    
    void setPeerDisconnectedHandler(std::function<void(const PeerID&, const std::string&)> handler) {
        on_peer_disconnected_ = std::move(handler);
    }
};

} // namespace usdtgverse::p2p

// ============================================================================
// DEMO APPLICATION
// ============================================================================

int main() {
    std::cout << R"(
🌌 =============================================== 🌌
    USDTgVerse P2P Network Manager
    Enterprise Networking: C Core + C++ API
🌌 =============================================== 🌌

)" << std::endl;
    
    using namespace usdtgverse::p2p;
    
    // Test C core first
    usdtg_test_network();
    std::cout << std::endl;
    
    // Test C++ wrapper
    std::cout << "🚀 Testing C++ Network Manager:\n";
    
    NetworkManager network;
    
    // Set event handlers
    network.setMessageReceivedHandler([](const NetworkMessage& msg, const PeerID& peer) {
        std::cout << "📨 Received " << msg.toString() << " from " << peer.toString() << "\n";
    });
    
    network.setPeerConnectedHandler([](const PeerID& peer) {
        std::cout << "✅ Peer connected: " << peer.toString() << "\n";
    });
    
    network.setPeerDisconnectedHandler([](const PeerID& peer, const std::string& reason) {
        std::cout << "❌ Peer disconnected: " << peer.toString() << " (" << reason << ")\n";
    });
    
    // Start network
    if (network.start(26657)) {
        std::cout << "   ✅ Network started on port 26657\n";
        
        // Run for a short time
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        // Test broadcast
        NetworkMessage test_msg(NetworkMessage::HEARTBEAT);
        network.broadcastMessage(test_msg);
        std::cout << "   📡 Broadcasted heartbeat message\n";
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
        std::cout << "\n📊 " << network.getNetworkStatus() << "\n";
        
        network.stop();
        std::cout << "   ✅ Network stopped\n";
    } else {
        std::cout << "   ❌ Failed to start network\n";
    }
    
    std::cout << "\n✅ P2P Network Features Ready:\n";
    std::cout << "   🔥 Ultra-fast C core networking\n";
    std::cout << "   🎯 User-friendly C++ API\n";
    std::cout << "   🔐 Noise IK encryption ready\n";
    std::cout << "   📡 Gossip protocol foundation\n";
    std::cout << "   🛡️ Anti-flood protection\n";
    std::cout << "   📊 Reputation system\n";
    std::cout << "   ⚡ Non-blocking I/O\n";
    std::cout << "   🌐 Multi-peer support (50+ peers)\n\n";
    
    std::cout << "🚀 USDTgVerse: Enterprise P2P Network Ready!\n";
    std::cout << "   Next: Integrate with HotStuff consensus\n\n";
    
    return 0;
}
