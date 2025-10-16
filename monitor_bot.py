#!/usr/bin/env python3
"""
USDTgVerse Social Media Bot Monitoring Script
"""

import json
import os
from datetime import datetime, timedelta

def check_bot_health():
    """Bot sağlığını kontrol et"""
    analytics_file = "analytics/social_media_analytics.json"
    
    if not os.path.exists(analytics_file):
        print("❌ Analytics file not found")
        return False
    
    with open(analytics_file, 'r') as f:
        data = json.load(f)
    
    # Son 24 saatteki post sayısını kontrol et
    today = datetime.now().strftime("%Y-%m-%d")
    yesterday = (datetime.now() - timedelta(days=1)).strftime("%Y-%m-%d")
    
    if today in data.get("daily_stats", {}):
        today_posts = sum(platform.get("posts", 0) for platform in data["daily_stats"][today].values())
        print(f"📊 Today's posts: {today_posts}")
        
        if today_posts == 0:
            print("⚠️  No posts today - bot might be down")
            return False
    
    print("✅ Bot is healthy")
    return True

def generate_report():
    """Günlük rapor oluştur"""
    analytics_file = "analytics/social_media_analytics.json"
    
    if not os.path.exists(analytics_file):
        print("❌ Analytics file not found")
        return
    
    with open(analytics_file, 'r') as f:
        data = json.load(f)
    
    print("\n📊 USDTgVerse Social Media Report")
    print("=" * 50)
    print(f"Total Posts: {data.get('total_posts', 0)}")
    print(f"Total Reach: {data.get('total_reach', 0)}")
    print(f"Total Engagement: {data.get('total_engagement', 0)}")
    
    # Platform breakdown
    if "daily_stats" in data:
        today = datetime.now().strftime("%Y-%m-%d")
        if today in data["daily_stats"]:
            print(f"\nToday's Stats ({today}):")
            for platform, stats in data["daily_stats"][today].items():
                print(f"  {platform.upper()}: {stats.get('posts', 0)} posts")

if __name__ == "__main__":
    print("🔍 Checking bot health...")
    check_bot_health()
    generate_report()
