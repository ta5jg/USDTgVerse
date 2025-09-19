#!/usr/bin/env python3
"""
USDTgVerse Frontend Server
Serves all frontend applications on localhost
"""

import http.server
import socketserver
import os
import sys
import threading
import time
import webbrowser
from pathlib import Path

class USDTgVerseHTTPHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory="/Users/irfangedik/USDTgVerse", **kwargs)
    
    def end_headers(self):
        # Add CORS headers for API access
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type')
        super().end_headers()
    
    def log_message(self, format, *args):
        # Custom logging format
        print(f"🌐 {self.address_string()} - {format % args}")

def start_server(port=8080):
    print(f"""
🌌 =============================================== 🌌
    USDTgVerse Frontend Server
    Professional Localhost Deployment
🌌 =============================================== 🌌

🚀 Starting frontend server on localhost:{port}...
""")
    
    try:
        with socketserver.TCPServer(("", port), USDTgVerseHTTPHandler) as httpd:
            print(f"✅ Server started successfully!")
            print(f"📡 Serving at: http://localhost:{port}")
            print("")
            
            print("🌐 Available Applications:")
            print(f"   🌟 Main Website:     http://localhost:{port}/website/")
            print(f"   🔍 USDTgScan:        http://localhost:{port}/explorer/")
            print(f"   💳 Transfer dApp:    http://localhost:{port}/dapps/usdtg-transfer/")
            print(f"   🔒 Security Audit:   http://localhost:{port}/audit/")
            print("")
            
            # Auto-open main website
            def open_browser():
                time.sleep(2)
                webbrowser.open(f'http://localhost:{port}/website/')
                print("🌟 Main website opened in browser!")
            
            browser_thread = threading.Thread(target=open_browser)
            browser_thread.daemon = True
            browser_thread.start()
            
            print("⏳ Server running... Press Ctrl+C to stop")
            print("")
            
            httpd.serve_forever()
            
    except KeyboardInterrupt:
        print("\n🛑 Shutting down frontend server...")
        print("✅ Frontend server stopped")
    except Exception as e:
        print(f"❌ Server error: {e}")
        return 1
    
    return 0

if __name__ == "__main__":
    port = 8080
    if len(sys.argv) > 1:
        try:
            port = int(sys.argv[1])
        except ValueError:
            print("❌ Invalid port number")
            sys.exit(1)
    
    sys.exit(start_server(port))
