#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
==========================================================
 Script:      add_headers.py
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-09-27
 Last Update: 2025-10-05
 Version:     1.1

 Description:
   Verilen dosya veya klasördeki kaynak dosyalara evrensel
   dosya başlığı (file header comment) ekler. Başlık; Created,
   Last Update, Author, Version, License, Description alanlarını
   içerir. Header idempotent çalışır (özel MARKER ile).
   macOS'ta gerçek oluşturulma zamanı (st_birthtime), değilse
   git ilk commit tarihi; o da yoksa mtime kullanılır.
   Shebang (#!...) ve Solidity SPDX satırları korunur.

 Usage:
   python3 add_headers.py <dosya_veya_klasör> [opsiyonlar]

 Options:
   --author "Ad Soyad"         Yazar adı (varsayılan: Irfan Gedik)
   --version "x.y.z"           Versiyon (varsayılan: 1.0)
   --license "MIT License"     Lisans bilgisi
   --description "Açıklama"    Dosya açıklaması (boşsa auto doldurulur)
   --auto-description          Dosya adına/uzantıya göre açıklamayı üret
   --include-exts .py .c .sol  Sadece belirtilen uzantılara uygula
   --exclude node_modules .git Belirtilen yolları atla
   --dry-run                   Deneme modu (dosya yazılmaz)
   --no-backup                 .bak yedeği alma
   --overwrite                 Önceden eklenmiş header’ı güncelle
   --with-hidden               Gizli klasör ve dosyaları da tara
   --with-usage                Header’ın üstüne Usage bloğunu da ekle
   --encoding utf-8            Dosya encoding (varsayılan: utf-8)
   --verbose                   Ayrıntılı çıktı

 Examples:
   # 1) Önce dene (yazmadan raporla)
   python3 add_headers.py ./project --dry-run --exclude node_modules .git dist build

   # 2) Gerçek yazım (yedek .bak alır)
   python3 add_headers.py ./project \
     --author "Irfan Gedik" \
     --version "1.0.0" \
     --license "MIT License" \
     --description "USDTg dApp ana modülleri" \
     --exclude node_modules .git dist build

   # 3) Sadece .py ve .sol dosyaları
   python3 add_headers.py . --include-exts .py .sol --exclude node_modules .git

   # 4) Usage bloğunu da ekle
   python3 add_headers.py . --with-usage

   # 5) Zaten eklenmiş header’ları da güncelle
   python3 add_headers.py . --overwrite

   # 6) Açıklamayı otomatik üret (dosya adına/uzantıya göre)
   python3 add_headers.py . --auto-description --exclude node_modules .git
==========================================================
"""

import os
import sys
import argparse
import datetime
import subprocess
import shlex

HEADER_MARKER_BEGIN = "=== FILE-HEADER-START ==="
HEADER_MARKER_END   = "=== FILE-HEADER-END ==="

USAGE_BLOCK = """
==========================================================
 Script:      add_headers.py
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.1

 Description:
   Bu script, verilen dosya veya klasördeki kaynak kodlarına
   evrensel dosya başlığı (file header comment) ekler.
   Header; Created, Last Update, Author, Version, License,
   Description gibi bilgileri içerir. Shebang/SPDX korunur.

 Usage:
   python3 add_headers.py <dosya_veya_klasör> [opsiyonlar]

 Options:
   --author "Ad Soyad"         Yazar adı (varsayılan: Irfan Gedik)
   --version "x.y.z"           Versiyon (varsayılan: 1.0)
   --license "MIT License"     Lisans bilgisi
   --description "Açıklama"    Dosya açıklaması
   --auto-description          Dosya adına/uzantıya göre açıklamayı üret
   --include-exts .py .c .sol  Sadece belirtilen uzantılara uygula
   --exclude node_modules .git Belirtilen yolları atla
   --dry-run                   Deneme modu (dosya yazılmaz)
   --no-backup                 .bak yedeği alma
   --overwrite                 Önceden eklenmiş header’ı güncelle
   --with-hidden               Gizli klasör ve dosyaları da tara
   --with-usage                Dosya başına bu Usage bloğunu da ekle
   --encoding utf-8            Dosya encoding (varsayılan: utf-8)
   --verbose                   Ayrıntılı çıktı
==========================================================
"""

# Uzantı -> (yorum_baslangıç, yorum_bitiş, tek_satır_mı)
COMMENT_STYLES = {
    ".c":    ("/*", "*/", False),
    ".h":    ("/*", "*/", False),
    ".cpp":  ("/*", "*/", False),
    ".hpp":  ("/*", "*/", False),
    ".java": ("/**", "*/", False),
    ".js":   ("/**", "*/", False),
    ".ts":   ("/**", "*/", False),
    ".jsx":  ("/**", "*/", False),
    ".tsx":  ("/**", "*/", False),
    ".py":   ('"""', '"""', False),
    ".sh":   ("#", "", True),
    ".bash": ("#", "", True),
    ".zsh":  ("#", "", True),
    ".sol":  ("//", "", True),
    ".go":   ("/**", "*/", False),
    ".rb":   ("#", "", True),
    ".swift":("/**", "*/", False),
    ".php":  ("/*", "*/", False),
    ".css":  ("/*", "*/", False),
    ".scss": ("/*", "*/", False),
    ".kt":   ("/**", "*/", False),
}

# Eklemek istemediğimiz/yorum desteklemeyen uzantılar:
SKIP_EXTS = {
    ".json", ".yml", ".yaml", ".toml", ".lock",
    ".png", ".jpg", ".jpeg", ".gif", ".webp", ".svg", ".ico",
    ".pdf", ".zip", ".gz", ".bz2", ".xz", ".7z",
    ".bin", ".o", ".so", ".dylib", ".a", ".class", ".jar",
}

def run_git_date(path):
    """Git ile dosyanın ilk commit tarihini (Creation) bul (repo içindeyse)."""
    try:
        cmd = f'git log --follow --diff-filter=A --date=iso-strict --format=%ad -- {shlex.quote(path)}'
        out = subprocess.check_output(
            cmd, shell=True, stderr=subprocess.DEVNULL, universal_newlines=True
        ).strip()
        if out:
            first_line = out.splitlines()[-1]  # en eski satır
            dt = datetime.datetime.fromisoformat(first_line.replace("Z", "+00:00"))
            return dt.astimezone(None).replace(tzinfo=None)
    except Exception:
        pass
    return None

def get_file_creation_date(path):
    """
    Öncelik:
    1) macOS: st_birthtime
    2) Git ilk commit tarihi (varsa)
    3) mtime (son değişiklik)
    """
    try:
        st = os.stat(path)
        if hasattr(st, "st_birthtime") and st.st_birthtime > 0:
            return datetime.datetime.fromtimestamp(st.st_birthtime)
    except Exception:
        pass

    git_dt = run_git_date(path)
    if git_dt:
        return git_dt

    try:
        st = os.stat(path)
        return datetime.datetime.fromtimestamp(st.st_mtime)
    except Exception:
        return datetime.datetime.today()

def choose_comment_style(ext):
    return COMMENT_STYLES.get(ext, ("/*", "*/", False))

def build_usage_block(ext):
    """Dil uzantısına göre Usage bloğunu uygun yorum formatına çevir."""
    start, end, is_single = choose_comment_style(ext)
    lines = USAGE_BLOCK.strip().splitlines()
    if is_single:
        prefix = start + " "
        return "\n".join(prefix + ln for ln in lines) + "\n\n"
    else:
        block = [start] + lines + [end]
        return "\n".join(block) + "\n\n"

def infer_description(path, ext):
    """
    Dosya adı, uzantı ve klasör adından heuristik bir açıklama üretir.
    Örn: main.c -> 'Giriş noktası (C)'
         app.js -> 'Uygulama başlangıç modülü (JavaScript)'
    """
    name = os.path.basename(path)
    stem, _ = os.path.splitext(name)
    folder = os.path.basename(os.path.dirname(path)).lower()

    lang = {
        ".c": "C", ".cpp": "C++", ".h": "C Header", ".hpp": "C++ Header",
        ".py": "Python", ".js": "JavaScript", ".ts": "TypeScript", ".jsx": "React (JSX)",
        ".tsx": "React (TSX)", ".java": "Java", ".go": "Go", ".rb": "Ruby",
        ".swift": "Swift", ".php": "PHP", ".css": "CSS", ".scss": "SCSS",
        ".kt": "Kotlin", ".sol": "Solidity"
    }.get(ext, ext.lstrip(".").upper() if ext else "Kod")

    s = stem.lower()

    rules = [
        ({"main", "entry", "bootstrap"},       f"Giriş noktası ({lang})"),
        ({"index"},                            f"Giriş/indeks modülü ({lang})"),
        ({"app", "application"},               f"Uygulama başlangıç modülü ({lang})"),
        ({"server", "srv"},                    f"HTTP sunucusu / REST API ({lang})"),
        ({"client", "cli"},                    f"İstemci arayüz/modül ({lang})"),
        ({"controller", "ctrl"},               f"Controller katmanı ({lang})"),
        ({"service", "svc"},                   f"Servis/iş mantığı ({lang})"),
        ({"model", "schema", "entity"},        f"Veri modeli/scheması ({lang})"),
        ({"route", "routes", "router"},        f"URL yönlendirme/route tanımları ({lang})"),
        ({"middleware", "mw"},                 f"Middleware katmanı ({lang})"),
        ({"util", "utils", "helper"},          f"Yardımcı/fonksiyonel araçlar ({lang})"),
        ({"config", "settings", "constants"},  f"Yapılandırma/sabitler ({lang})"),
        ({"env", "dotenv"},                    f"Çevresel değişken yükleme ({lang})"),
        ({"test", "tests", "spec", "specs"},   f"Birim/entegrasyon testleri ({lang})"),
        ({"build", "vite", "webpack", "rollup"}, f"Derleme/araç yapılandırması ({lang})"),
        ({"lint", "eslint", "flake8", "ruff"}, f"Lint/analiz kuralları ({lang})"),
        ({"wallet"},                           f"Cüzdan modülü ({lang})"),
        ({"token", "erc20", "trc20", "bep20"}, f"Token sözleşmesi/modülü ({lang})"),
        ({"price", "oracle", "feed"},          f"Fiyat/oracle beslemesi ({lang})"),
        ({"db", "database", "repo", "repository"}, f"Veritabanı erişimi/deposu ({lang})"),
        ({"auth", "jwt", "oauth"},             f"Kimlik doğrulama/otor. ({lang})"),
        ({"logger", "logging"},                f"Günlükleme/izleme ({lang})"),
        ({"queue", "worker", "job"},           f"İş kuyruğu/worker süreçleri ({lang})"),
    ]
    for keys, desc in rules:
        if any(k in s for k in keys):
            return desc

    folder_hints = {
        "controllers": f"Controller katmanı ({lang})",
        "services":    f"Servis/iş mantığı ({lang})",
        "models":      f"Veri modeli/scheması ({lang})",
        "routes":      f"URL yönlendirme/route tanımları ({lang})",
        "middleware":  f"Middleware katmanı ({lang})",
        "config":      f"Yapılandırma ({lang})",
        "tests":       f"Testler ({lang})",
        "scripts":     f"Komut/yardımcı script ({lang})",
        "utils":       f"Yardımcı araçlar ({lang})",
        "migrations":  f"Veritabanı migrasyonları ({lang})",
        "pricefeed":   f"Fiyat/oracle beslemesi ({lang})",
    }
    if folder in folder_hints:
        return folder_hints[folder]

    if ext == ".sol":
        if "factory" in s:
            return f"Sözleşme fabrikası ({lang})"
        if "proxy" in s or "clone" in s:
            return f"Proxy/Clones kalıbı ({lang})"
        if "mock" in s:
            return f"Test için mock sözleşme ({lang})"

    return f"{lang} modülü"

def build_header_text(file_name, author, created_dt, version, license_text, description, ext):
    created = created_dt.strftime("%d.%m.%Y")
    today   = datetime.datetime.today().strftime("%d.%m.%Y")
    start, end, is_single = choose_comment_style(ext)

    core_lines = [
        HEADER_MARKER_BEGIN,
        "==============================================",
        f" File:        {file_name}",
        f" Author:      {author}",
        f" Created:     {created}",
        f" Last Update: {today}",
        f" Version:     {version}",
        "",
        " Description:",
        f"   {description or '[Dosya açıklaması]'}",
        "",
        " License:",
        f"   {license_text}",
        "==============================================",
        HEADER_MARKER_END,
    ]

    if is_single:
        prefix = start + " "
        return "\n".join(prefix + ln for ln in core_lines) + "\n\n"
    else:
        block = [start] + core_lines + [end]
        return "\n".join(block) + "\n\n"

def detect_shebang_and_spdx(lines, ext):
    """Shebang (#!...) ve SPDX konumlarını bul. Döner: (shebang_end_idx, spdx_idx)."""
    shebang_end = -1
    spdx_idx = -1
    if lines and lines[0].startswith("#!"):
        shebang_end = 0
    if ext == ".sol":
        for i, ln in enumerate(lines[:10]):
            if "SPDX-License-Identifier:" in ln:
                spdx_idx = i
                break
    return shebang_end, spdx_idx

def already_has_header(content):
    return (HEADER_MARKER_BEGIN in content) and (HEADER_MARKER_END in content)

def should_skip(path, include_exts, exclude_globs):
    ext = os.path.splitext(path)[1].lower()
    if ext in SKIP_EXTS:
        return True
    if include_exts and ext not in include_exts:
        return True
    for pat in exclude_globs:
        if pat and pat in path:
            return True
    return False

def process_file(path, args):
    ext = os.path.splitext(path)[1].lower()

    if should_skip(path, args.include_exts, args.exclude):
        return False, "SKIP_FILTER"

    try:
        with open(path, "r", encoding=args.encoding, errors="replace") as f:
            content = f.read()
    except Exception as e:
        return False, f"READ_FAIL: {e}"

    already = already_has_header(content)
    if already and not args.overwrite:
        return False, "ALREADY"

    created_dt = get_file_creation_date(path)

    # Açıklama: öncelik manuel, yoksa otomatik
    effective_description = args.description
    if args.auto_description or effective_description in (None, "", "[Dosya açıklaması]"):
        effective_description = infer_description(path, ext)

    header = build_header_text(
        os.path.basename(path),
        args.author,
        created_dt,
        args.version,
        args.license,
        effective_description,
        ext,
    )

    if args.with_usage:
        header = build_usage_block(ext) + header

    lines = content.splitlines()
    shebang_end, spdx_idx = detect_shebang_and_spdx(lines, ext)

    insert_at = 0
    if spdx_idx != -1:
        insert_at = spdx_idx + 1
    if shebang_end != -1:
        insert_at = max(insert_at, shebang_end + 1)

    new_content = "\n".join(lines[:insert_at]) + ("\n" if insert_at > 0 else "")
    new_content += header
    new_content += "\n".join(lines[insert_at:]) + ("\n" if content.endswith("\n") else "")

    if args.dry_run:
        return True, "DRY_RUN"

    # Yedek
    try:
        if not args.no_backup:
            bak = path + ".bak"
            if not os.path.exists(bak):
                with open(bak, "w", encoding=args.encoding, errors="replace") as bf:
                    bf.write(content)
    except Exception as e:
        return False, f"BACKUP_FAIL: {e}"

    try:
        with open(path, "w", encoding=args.encoding, errors="replace") as f:
            f.write(new_content)
        return True, ("UPDATED" if already else "ADDED")
    except Exception as e:
        return False, f"WRITE_FAIL: {e}"

def walk_and_process(root, args):
    stats = {"added":0, "updated":0, "skipped":0, "failed":0, "dry":0, "already":0}
    for base, dirs, files in os.walk(root):
        if not args.with_hidden:
            dirs[:] = [d for d in dirs if not d.startswith(".")]
            files = [f for f in files if not f.startswith(".")]

        for name in files:
            path = os.path.join(base, name)
            ok, status = process_file(path, args)
            if status == "ADDED":
                stats["added"] += 1
                print(f"✅ ADDED   : {path}")
            elif status == "UPDATED":
                stats["updated"] += 1
                print(f"♻️  UPDATED : {path}")
            elif status == "ALREADY":
                stats["already"] += 1
                if args.verbose: print(f"⏭️  ALREADY : {path}")
            elif status == "DRY_RUN":
                stats["dry"] += 1
                print(f"👀 DRY-RUN : {path}")
            elif status == "SKIP_FILTER":
                stats["skipped"] += 1
                if args.verbose: print(f"⏭️  SKIP    : {path}")
            elif status.startswith(("READ_FAIL","WRITE_FAIL","BACKUP_FAIL")):
                stats["failed"] += 1
                print(f"❌ FAIL    : {path} -> {status}")
            else:
                stats["skipped"] += 1
                if args.verbose: print(f"⏭️  SKIP    : {path} -> {status}")
    return stats

def parse_args():
    p = argparse.ArgumentParser(
        description="Klasördeki dosyalara evrensel dosya başlığı (file header) ekler."
    )
    p.add_argument("path", help="Hedef dosya/klasör")
    p.add_argument("--author", default="Irfan Gedik")
    p.add_argument("--version", default="1.0")
    p.add_argument("--license", default="MIT License")
    p.add_argument("--description", default="[Dosya açıklaması]")
    p.add_argument("--auto-description", action="store_true",
                   help="Dosya adına, uzantısına ve klasör adına göre açıklamayı otomatik üret.")
    p.add_argument("--include-exts", nargs="*", default=[],
                   help="Sadece bu uzantılara uygula (örn: .py .c .sol)")
    p.add_argument("--exclude", nargs="*", default=[],
                   help="Yol içinde geçiyorsa atla (örn: node_modules build dist vendor .git)")
    p.add_argument("--dry-run", action="store_true", dest="dry_run",
                   help="Deneme modu: dosyaya yazmaz, sadece raporlar.")
    p.add_argument("--no-backup", action="store_true", help="*.bak oluşturmadan yaz.")
    p.add_argument("--overwrite", action="store_true",
                   help="Daha önce eklenmiş header’ı da güncelle (MARKER ile).")
    p.add_argument("--with-hidden", action="store_true", help="Gizli dosya/klasörleri de tara.")
    p.add_argument("--with-usage", action="store_true",
                   help="Header’ın üstüne Usage bloğunu da ekle.")
    p.add_argument("--encoding", default="utf-8")
    p.add_argument("--verbose", action="store_true")
    return p.parse_args()

def main():
    args = parse_args()
    target = os.path.abspath(args.path)

    if os.path.isdir(target):
        stats = walk_and_process(target, args)
    else:
        ok, status = process_file(target, args)
        stats = {"added":0,"updated":0,"skipped":0,"failed":0,"dry":0,"already":0}
        if status == "ADDED": stats["added"]=1
        elif status == "UPDATED": stats["updated"]=1
        elif status == "ALREADY": stats["already"]=1
        elif status == "DRY_RUN": stats["dry"]=1
        elif status.startswith(("READ_FAIL","WRITE_FAIL","BACKUP_FAIL")):
            stats["failed"]=1
        else:
            stats["skipped"]=1

    print("\n=== SUMMARY ===")
    for k in ["added","updated","already","dry","skipped","failed"]:
        print(f"{k:>8}: {stats[k]}")

if __name__ == "__main__":
    main()