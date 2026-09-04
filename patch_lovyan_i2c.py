from pathlib import Path

Import("env")

project_dir = Path(env.subst("$PROJECT_DIR"))
path = project_dir / "lib" / "HAL" / "display" / "lovyan.cpp"
text = path.read_text(encoding="utf-8")

start_marker = "#elif defined(LOVYAN_I2C_BUS)"
cfg_marker = "        cfg.i2c_port = TFT_I2C_PORT;"
start = text.find(start_marker)
cfg = text.find(cfg_marker, start)

if start < 0 or cfg < 0:
    raise RuntimeError("Bruce LovyanGFX I2C block was not found")

block = text[start:cfg]

# Bruce 1.16.1 has the #error on the #else side, so a fully configured I2C
# display fails to compile. Replace only this preprocessor block and leave the
# actual bus configuration code untouched.
if '#else\n#error "To use LOVYAN_I2C_BUS' in block:
    fixed = """#elif defined(LOVYAN_I2C_BUS)\n#if !defined(TFT_I2C_PORT) || !defined(TFT_I2C_WRITE) || !defined(TFT_I2C_READ) || !defined(TFT_SDA) ||      \\\n    !defined(TFT_SCL) || !defined(TFT_ADDR)\n#error \"To use LOVYAN_I2C_BUS, define TFT_I2C_PORT, TFT_I2C_WRITE, TFT_I2C_READ, TFT_SDA, TFT_SCL and TFT_ADDR\"\n#endif\n"""
    path.write_text(text[:start] + fixed + text[cfg:], encoding="utf-8")
    print("[SSD1306] Patched Bruce 1.16.1 LovyanGFX I2C preprocessor guard")
else:
    print("[SSD1306] LovyanGFX I2C guard already patched or changed; no edit needed")
