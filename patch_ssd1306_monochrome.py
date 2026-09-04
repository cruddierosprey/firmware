from pathlib import Path
import re

Import("env")

# LovyanGFX intentionally applies Bayer dithering when converting RGB565 to
# a 1-bit OLED. Bruce's dark magenta theme therefore becomes a field of dots
# on SSD1306. For this custom board build, convert every non-black source
# colour to full white so the UI is true 1-bit black/white instead of dithered.
libdeps_root = Path(env.subst("$PROJECT_LIBDEPS_DIR")) / env.subst("$PIOENV")
candidates = list(libdeps_root.rglob("Panel_SSD1306.cpp"))

if not candidates:
    raise RuntimeError(f"Panel_SSD1306.cpp not found under {libdeps_root}")

patched = 0
for path in candidates:
    text = path.read_text(encoding="utf-8")
    pattern = re.compile(
        r"inline\s+static\s+uint32_t\s+to_gray\s*\(uint8_t\s+r,\s*uint8_t\s+g,\s*uint8_t\s+b\)\s*\{.*?\n\s*\}",
        re.S,
    )
    replacement = """inline static uint32_t to_gray(uint8_t r, uint8_t g, uint8_t b)\n  {\n    // Bruce SSD1306 custom build: strict monochrome, no Bayer dithering.\n    return (r | g | b) ? 255u : 0u;\n  }"""
    new_text, count = pattern.subn(replacement, text, count=1)
    if count:
        path.write_text(new_text, encoding="utf-8")
        print(f"[SSD1306-MONO] Disabled RGB->1bit dithering in {path}")
        patched += 1
    elif "return (r | g | b) ? 255u : 0u;" in text:
        print(f"[SSD1306-MONO] Already patched: {path}")
        patched += 1

if not patched:
    raise RuntimeError("LovyanGFX SSD1306 to_gray() function was not patched")
