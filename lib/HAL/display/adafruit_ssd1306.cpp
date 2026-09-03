#include "tft.h"

#if defined(USE_ADAFRUIT_SSD1306)

#include <Arduino.h>

static constexpr float DEG_TO_RAD_F = 0.017453292519943295769f;

tft_display::tft_display(int16_t w, int16_t h)
    : _oled(w, h, &Wire, TFT_RST), _height(h), _width(w) {}

uint16_t tft_display::mono(uint32_t color) {
    return color == TFT_BLACK ? SSD1306_BLACK : SSD1306_WHITE;
}

void tft_display::flush() {
    if (_ready) _oled.display();
}

void tft_display::begin(uint32_t speed) {
    (void)speed;

    // Match the user's known-good sketch exactly: explicit ESP32-S3 I2C pins,
    // then Adafruit_SSD1306 with internal charge pump and address 0x3C.
    Wire.begin(TFT_SDA, TFT_SCL);
    Wire.setClock(400000);

    _ready = _oled.begin(SSD1306_SWITCHCAPVCC, TFT_ADDR);
    if (!_ready) return;

    _oled.clearDisplay();
    _oled.setRotation(_rotation);
    _oled.setTextSize(_textSize);
    _oled.setTextColor(mono(_textColor), mono(_textBgColor));
    _oled.setTextWrap(false);
    _width = _oled.width();
    _height = _oled.height();
    _oled.display();
}

void tft_display::init(uint8_t tc) {
    (void)tc;
    begin();
}

void tft_display::setRotation(uint8_t r) {
    _rotation = r & 3;
    if (_ready) {
        _oled.setRotation(_rotation);
        _width = _oled.width();
        _height = _oled.height();
        flush();
    }
}

void tft_display::drawPixelNoFlush(int32_t x, int32_t y, uint32_t color) {
    if (_ready) _oled.drawPixel(x, y, mono(color));
}

void tft_display::drawPixel(int32_t x, int32_t y, uint32_t color) {
    drawPixelNoFlush(x, y, color);
    flush();
}

void tft_display::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color) {
    if (!_ready) return;
    _oled.drawLine(x0, y0, x1, y1, mono(color));
    flush();
}

void tft_display::drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color) {
    if (!_ready) return;
    _oled.drawFastHLine(x, y, w, mono(color));
    flush();
}

void tft_display::drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color) {
    if (!_ready) return;
    _oled.drawFastVLine(x, y, h, mono(color));
    flush();
}

void tft_display::drawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color) {
    if (!_ready) return;
    _oled.drawRect(x, y, w, h, mono(color));
    flush();
}

void tft_display::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color) {
    if (!_ready) return;
    _oled.fillRect(x, y, w, h, mono(color));
    flush();
}

void tft_display::fillRectHGradient(
    int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2
) {
    (void)color2;
    fillRect(x, y, w, h, color1);
}

void tft_display::fillRectVGradient(
    int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2
) {
    (void)color2;
    fillRect(x, y, w, h, color1);
}

void tft_display::fillScreen(uint32_t color) {
    if (!_ready) return;
    _oled.fillScreen(mono(color));
    flush();
}

void tft_display::drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color) {
    if (!_ready) return;
    _oled.drawRoundRect(x, y, w, h, r, mono(color));
    flush();
}

void tft_display::fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color) {
    if (!_ready) return;
    _oled.fillRoundRect(x, y, w, h, r, mono(color));
    flush();
}

void tft_display::drawCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color) {
    if (!_ready) return;
    _oled.drawCircle(x0, y0, r, mono(color));
    flush();
}

void tft_display::fillCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color) {
    if (!_ready) return;
    _oled.fillCircle(x0, y0, r, mono(color));
    flush();
}

void tft_display::drawTriangle(
    int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color
) {
    if (!_ready) return;
    _oled.drawTriangle(x0, y0, x1, y1, x2, y2, mono(color));
    flush();
}

void tft_display::fillTriangle(
    int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color
) {
    if (!_ready) return;
    _oled.fillTriangle(x0, y0, x1, y1, x2, y2, mono(color));
    flush();
}

void tft_display::drawEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color) {
    if (!_ready || rx < 0 || ry < 0) return;
    uint16_t c = mono(color);
    for (int32_t deg = 0; deg < 360; ++deg) {
        float a = static_cast<float>(deg) * DEG_TO_RAD_F;
        int32_t x = x0 + static_cast<int32_t>(std::lround(rx * std::cos(a)));
        int32_t y = y0 + static_cast<int32_t>(std::lround(ry * std::sin(a)));
        _oled.drawPixel(x, y, c);
    }
    flush();
}

void tft_display::fillEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color) {
    if (!_ready || rx < 0 || ry < 0) return;
    uint16_t c = mono(color);
    if (ry == 0) {
        _oled.drawFastHLine(x0 - rx, y0, rx * 2 + 1, c);
    } else {
        for (int32_t y = -ry; y <= ry; ++y) {
            float yy = static_cast<float>(y) / static_cast<float>(ry);
            float inside = std::max(0.0f, 1.0f - yy * yy);
            int32_t dx = static_cast<int32_t>(std::lround(rx * std::sqrt(inside)));
            _oled.drawFastHLine(x0 - dx, y0 + y, dx * 2 + 1, c);
        }
    }
    flush();
}

void tft_display::drawArc(
    int32_t x, int32_t y, int32_t r, int32_t ir, uint32_t startAngle, uint32_t endAngle, uint32_t fg_color,
    uint32_t bg_color, bool smoothArc
) {
    (void)bg_color;
    (void)smoothArc;
    if (!_ready || r < 0) return;
    if (ir < 0) ir = 0;
    if (endAngle < startAngle) endAngle += 360;
    uint16_t c = mono(fg_color);
    for (uint32_t deg = startAngle; deg <= endAngle; ++deg) {
        float a = (static_cast<float>(deg) + 90.0f) * DEG_TO_RAD_F;
        int32_t x0 = x + static_cast<int32_t>(std::lround(ir * std::cos(a)));
        int32_t y0 = y + static_cast<int32_t>(std::lround(ir * std::sin(a)));
        int32_t x1 = x + static_cast<int32_t>(std::lround(r * std::cos(a)));
        int32_t y1 = y + static_cast<int32_t>(std::lround(r * std::sin(a)));
        _oled.drawLine(x0, y0, x1, y1, c);
    }
    flush();
}

void tft_display::drawWideLineNoFlush(float ax, float ay, float bx, float by, float wd, uint32_t color) {
    if (!_ready) return;
    float dx = bx - ax;
    float dy = by - ay;
    float len = std::sqrt(dx * dx + dy * dy);
    uint16_t c = mono(color);
    if (len == 0.0f) {
        _oled.drawPixel(static_cast<int32_t>(ax), static_cast<int32_t>(ay), c);
        return;
    }
    float nx = -dy / len;
    float ny = dx / len;
    int32_t half = static_cast<int32_t>(std::ceil(wd / 2.0f));
    for (int32_t i = -half; i <= half; ++i) {
        float ox = nx * static_cast<float>(i);
        float oy = ny * static_cast<float>(i);
        _oled.drawLine(
            static_cast<int32_t>(ax + ox), static_cast<int32_t>(ay + oy),
            static_cast<int32_t>(bx + ox), static_cast<int32_t>(by + oy), c
        );
    }
}

void tft_display::drawWideLine(
    float ax, float ay, float bx, float by, float wd, uint32_t fg_color, uint32_t bg_color
) {
    (void)bg_color;
    drawWideLineNoFlush(ax, ay, bx, by, wd, fg_color);
    flush();
}

void tft_display::drawXBitmap(
    int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color
) {
    if (!_ready || !bitmap) return;
    _oled.drawXBitmap(x, y, bitmap, w, h, mono(color));
    flush();
}

void tft_display::drawXBitmap(
    int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg
) {
    if (!_ready || !bitmap) return;
    for (int16_t j = 0; j < h; ++j) {
        for (int16_t i = 0; i < w; ++i) {
            uint8_t byte = bitmap[(j * ((w + 7) / 8)) + (i >> 3)];
            bool bit = byte & (0x80 >> (i & 7));
            _oled.drawPixel(x + i, y + j, mono(bit ? color : bg));
        }
    }
    flush();
}

void tft_display::pushImageNoFlush(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data) {
    if (!_ready || !data) return;
    for (int32_t row = 0; row < h; ++row) {
        for (int32_t col = 0; col < w; ++col) {
            uint16_t color = data[row * w + col];
            if (_swapBytes) color = static_cast<uint16_t>((color >> 8) | (color << 8));
            _oled.drawPixel(x + col, y + row, mono(color));
        }
    }
}

void tft_display::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data) {
    pushImageNoFlush(x, y, w, h, data);
    flush();
}

void tft_display::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data) {
    pushImage(x, y, w, h, const_cast<const uint16_t *>(data));
}

void tft_display::pushImage(
    int32_t x, int32_t y, int32_t w, int32_t h, uint8_t *data, bool bpp8, uint16_t *cmap
) {
    if (!_ready || !data || !bpp8 || !cmap) return;
    for (int32_t row = 0; row < h; ++row) {
        for (int32_t col = 0; col < w; ++col) {
            uint8_t idx = data[row * w + col];
            _oled.drawPixel(x + col, y + row, mono(cmap[idx]));
        }
    }
    flush();
}

void tft_display::pushImage(
    int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8, uint16_t *cmap
) {
    pushImage(x, y, w, h, const_cast<uint8_t *>(data), bpp8, cmap);
}

void tft_display::invertDisplay(bool i) {
    if (!_ready) return;
    _oled.invertDisplay(i);
}

void tft_display::sleep(bool value) {
    (void)value;
}

void tft_display::setSwapBytes(bool swap) { _swapBytes = swap; }
bool tft_display::getSwapBytes() const { return _swapBytes; }

uint16_t tft_display::color565(uint8_t r, uint8_t g, uint8_t b) const {
    return static_cast<uint16_t>(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}

int16_t tft_display::textWidth(const String &s, uint8_t font) const {
    (void)font;
    return static_cast<int16_t>(s.length() * 6 * _textSize);
}

int16_t tft_display::textWidth(const char *s, uint8_t font) const {
    (void)font;
    return s ? static_cast<int16_t>(strlen(s) * 6 * _textSize) : 0;
}

void tft_display::setCursor(int16_t x, int16_t y) {
    if (_ready) _oled.setCursor(x, y);
}

int16_t tft_display::getCursorX() const { return _ready ? _oled.getCursorX() : 0; }
int16_t tft_display::getCursorY() const { return _ready ? _oled.getCursorY() : 0; }

void tft_display::setTextSize(uint8_t s) {
    _textSize = s ? s : 1;
    if (_ready) _oled.setTextSize(_textSize);
}

void tft_display::setTextColor(uint16_t c) {
    _textColor = c;
    if (_ready) _oled.setTextColor(mono(c));
}

void tft_display::setTextColor(uint16_t c, uint16_t b, bool bgfill) {
    (void)bgfill;
    _textColor = c;
    _textBgColor = b;
    if (_ready) _oled.setTextColor(mono(c), mono(b));
}

void tft_display::setTextDatum(uint8_t d) { _textDatum = d; }
uint8_t tft_display::getTextDatum() const { return _textDatum; }
void tft_display::setTextFont(uint8_t f) { _textFont = f; }

void tft_display::setTextWrap(bool wrapX, bool wrapY) {
    (void)wrapY;
    if (_ready) _oled.setTextWrap(wrapX);
}

int16_t tft_display::drawString(const String &string, int32_t x, int32_t y, uint8_t font) {
    (void)font;
    return drawAlignedString(string, x, y, _textDatum);
}

int16_t tft_display::drawCentreString(const String &string, int32_t x, int32_t y, uint8_t font) {
    (void)font;
    return drawAlignedString(string, x, y, TC_DATUM);
}

int16_t tft_display::drawRightString(const String &string, int32_t x, int32_t y, uint8_t font) {
    (void)font;
    return drawAlignedString(string, x, y, TR_DATUM);
}

size_t tft_display::write(uint8_t c) {
    if (!_ready) return 0;
    size_t n = _oled.write(c);
    flush();
    return n;
}

size_t tft_display::write(const uint8_t *buffer, size_t size) {
    if (!_ready || !buffer) return 0;
    size_t n = _oled.write(buffer, size);
    flush();
    return n;
}

size_t tft_display::println() {
    if (!_ready) return 0;
    size_t n = _oled.println();
    flush();
    return n;
}

size_t tft_display::printf(const char *fmt, ...) {
    if (!_ready || !fmt) return 0;
    va_list args;
    va_start(args, fmt);
    char stackBuf[128];
    int len = vsnprintf(stackBuf, sizeof(stackBuf), fmt, args);
    va_end(args);
    if (len < 0) return 0;

    size_t n = 0;
    if (static_cast<size_t>(len) < sizeof(stackBuf)) {
        n = _oled.print(stackBuf);
    } else {
        std::unique_ptr<char[]> buf(new char[len + 1]);
        va_start(args, fmt);
        vsnprintf(buf.get(), len + 1, fmt, args);
        va_end(args);
        n = _oled.print(buf.get());
    }
    flush();
    return n;
}

int16_t tft_display::width() const { return _ready ? _oled.width() : _width; }
int16_t tft_display::height() const { return _ready ? _oled.height() : _height; }
SPIClass &tft_display::getSPIinstance() const { return SPI; }
void tft_display::writecommand(uint8_t c) { (void)c; }
uint32_t tft_display::getTextColor() const { return _textColor; }
uint32_t tft_display::getTextBgColor() const { return _textBgColor; }
uint8_t tft_display::getTextSize() const { return _textSize; }
uint8_t tft_display::getRotation() const { return _rotation; }

int16_t tft_display::fontHeight(int16_t font) const {
    (void)font;
    return static_cast<int16_t>(_textSize * 8);
}

Adafruit_SSD1306 *tft_display::native() { return &_oled; }

int16_t tft_display::drawAlignedString(const String &s, int32_t x, int32_t y, uint8_t datum) {
    if (!_ready) return 0;
    int16_t w = static_cast<int16_t>(s.length() * 6 * _textSize);
    int16_t h = static_cast<int16_t>(8 * _textSize);
    int32_t cx = x;
    int32_t cy = y;
    switch (datum) {
        case TC_DATUM: cx -= w / 2; break;
        case TR_DATUM: cx -= w; break;
        case MC_DATUM: cx -= w / 2; cy -= h / 2; break;
        case MR_DATUM: cx -= w; cy -= h / 2; break;
        case BC_DATUM: cx -= w / 2; cy -= h; break;
        case BR_DATUM: cx -= w; cy -= h; break;
        case BL_DATUM: cy -= h; break;
        default: break;
    }
    _oled.setCursor(cx, cy);
    _oled.print(s);
    flush();
    return w;
}

// -----------------------------------------------------------------------------
// Sprite bridge. Uses Adafruit_GFX's 16-bit canvas so Bruce can keep its normal
// tft_sprite API. Colors are converted to monochrome only when pushed to OLED.
// -----------------------------------------------------------------------------

tft_sprite::tft_sprite(tft_display *parent) : _display(parent) {}
tft_sprite::~tft_sprite() { deleteSprite(); }

void *tft_sprite::createSprite(int16_t w, int16_t h, uint8_t frames) {
    (void)frames;
    deleteSprite();
    if (w <= 0 || h <= 0) return nullptr;
    _canvas = new GFXcanvas16(w, h);
    if (!_canvas || !_canvas->getBuffer()) {
        deleteSprite();
        return nullptr;
    }
    _canvas->setTextSize(_textSize);
    _canvas->setTextColor(static_cast<uint16_t>(_textColor), static_cast<uint16_t>(_textBgColor));
    return _canvas->getBuffer();
}

void tft_sprite::deleteSprite() {
    if (_canvas) {
        delete _canvas;
        _canvas = nullptr;
    }
}

void tft_sprite::fillScreen(uint32_t color) {
    if (_canvas) _canvas->fillScreen(static_cast<uint16_t>(color));
}

void tft_sprite::setColorDepth(uint8_t depth) { _colorDepth = depth; }

void tft_sprite::setCursor(int16_t x, int16_t y) {
    if (_canvas) _canvas->setCursor(x, y);
}

void tft_sprite::setTextColor(uint16_t c) {
    _textColor = c;
    if (_canvas) _canvas->setTextColor(c);
}

void tft_sprite::setTextColor(uint16_t c, uint16_t b) {
    _textColor = c;
    _textBgColor = b;
    if (_canvas) _canvas->setTextColor(c, b);
}

void tft_sprite::setTextSize(uint8_t s) {
    _textSize = s ? s : 1;
    if (_canvas) _canvas->setTextSize(_textSize);
}

void tft_sprite::setTextDatum(uint8_t d) { _textDatum = d; }

void tft_sprite::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color) {
    if (_canvas) _canvas->fillRect(x, y, w, h, static_cast<uint16_t>(color));
}

void tft_sprite::drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color) {
    if (_canvas) _canvas->drawFastVLine(x, y, h, static_cast<uint16_t>(color));
}

void tft_sprite::fillCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color) {
    if (_canvas) _canvas->fillCircle(x0, y0, r, static_cast<uint16_t>(color));
}

void tft_sprite::drawCircle(int32_t x0, int32_t y0, int32_t r, uint32_t color) {
    if (_canvas) _canvas->drawCircle(x0, y0, r, static_cast<uint16_t>(color));
}

void tft_sprite::fillEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color) {
    if (!_canvas || rx < 0 || ry < 0) return;
    if (ry == 0) {
        _canvas->drawFastHLine(x0 - rx, y0, rx * 2 + 1, color);
        return;
    }
    for (int32_t y = -ry; y <= ry; ++y) {
        float yy = static_cast<float>(y) / static_cast<float>(ry);
        float inside = std::max(0.0f, 1.0f - yy * yy);
        int32_t dx = static_cast<int32_t>(std::lround(rx * std::sqrt(inside)));
        _canvas->drawFastHLine(x0 - dx, y0 + y, dx * 2 + 1, color);
    }
}

void tft_sprite::fillTriangle(
    int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color
) {
    if (_canvas) _canvas->fillTriangle(x0, y0, x1, y1, x2, y2, static_cast<uint16_t>(color));
}

void tft_sprite::pushSprite(int32_t x, int32_t y, uint32_t transparent) {
    if (!_canvas || !_display || !_display->_ready) return;
    const uint16_t *buffer = _canvas->getBuffer();
    if (!buffer) return;
    int16_t w = _canvas->width();
    int16_t h = _canvas->height();
    for (int32_t j = 0; j < h; ++j) {
        for (int32_t i = 0; i < w; ++i) {
            uint16_t color = buffer[static_cast<size_t>(j) * static_cast<size_t>(w) + static_cast<size_t>(i)];
            if (transparent == TFT_TRANSPARENT && color == static_cast<uint16_t>(transparent)) continue;
            _display->drawPixelNoFlush(x + i, y + j, color);
        }
    }
    _display->flush();
}

void tft_sprite::pushToSprite(tft_sprite *dest, int32_t x, int32_t y, uint32_t transparent) {
    if (!_canvas || !dest || !dest->_canvas) return;
    const uint16_t *src = _canvas->getBuffer();
    if (!src) return;
    int16_t w = _canvas->width();
    int16_t h = _canvas->height();
    for (int32_t j = 0; j < h; ++j) {
        for (int32_t i = 0; i < w; ++i) {
            uint16_t color = src[static_cast<size_t>(j) * static_cast<size_t>(w) + static_cast<size_t>(i)];
            if (color == static_cast<uint16_t>(transparent)) continue;
            dest->setPixel(x + i, y + j, color);
        }
    }
}

int16_t tft_sprite::width() const { return _canvas ? _canvas->width() : 0; }
int16_t tft_sprite::height() const { return _canvas ? _canvas->height() : 0; }

int16_t tft_sprite::fontHeight(int16_t font) const {
    (void)font;
    return static_cast<int16_t>(_textSize * 8);
}

void tft_sprite::drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color) {
    if (_canvas) _canvas->drawLine(x0, y0, x1, y1, static_cast<uint16_t>(color));
}

void tft_sprite::drawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color) {
    if (_canvas) _canvas->drawRect(x, y, w, h, static_cast<uint16_t>(color));
}

void tft_sprite::drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color) {
    if (_canvas) _canvas->drawRoundRect(x, y, w, h, r, static_cast<uint16_t>(color));
}

void tft_sprite::fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color) {
    if (_canvas) _canvas->fillRoundRect(x, y, w, h, r, static_cast<uint16_t>(color));
}

void tft_sprite::drawPixel(int32_t x, int32_t y, uint32_t color) {
    setPixel(x, y, static_cast<uint16_t>(color));
}

void tft_sprite::drawXBitmap(
    int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, uint16_t color, uint16_t bg
) {
    if (!_canvas || !bitmap) return;
    for (int16_t j = 0; j < h; ++j) {
        for (int16_t i = 0; i < w; ++i) {
            uint8_t byte = bitmap[(j * ((w + 7) / 8)) + (i >> 3)];
            bool bit = byte & (0x80 >> (i & 7));
            _canvas->drawPixel(x + i, y + j, bit ? color : bg);
        }
    }
}

void tft_sprite::pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data) {
    if (!_canvas || !data) return;
    for (int32_t row = 0; row < h; ++row) {
        for (int32_t col = 0; col < w; ++col) setPixel(x + col, y + row, data[row * w + col]);
    }
}

void tft_sprite::pushImage(
    int32_t x, int32_t y, int32_t w, int32_t h, uint8_t *data, bool bpp8, uint16_t *cmap
) {
    if (!_canvas || !data || !bpp8 || !cmap) return;
    for (int32_t row = 0; row < h; ++row) {
        for (int32_t col = 0; col < w; ++col) {
            uint8_t idx = data[row * w + col];
            setPixel(x + col, y + row, cmap[idx]);
        }
    }
}

void tft_sprite::pushImage(
    int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8, uint16_t *cmap
) {
    pushImage(x, y, w, h, const_cast<uint8_t *>(data), bpp8, cmap);
}

void tft_sprite::fillRectHGradient(
    int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2
) {
    (void)color2;
    fillRect(x, y, w, h, color1);
}

void tft_sprite::fillRectVGradient(
    int16_t x, int16_t y, int16_t w, int16_t h, uint32_t color1, uint32_t color2
) {
    (void)color2;
    fillRect(x, y, w, h, color1);
}

int16_t tft_sprite::drawString(const String &string, int32_t x, int32_t y, uint8_t font) {
    (void)font;
    if (!_canvas) return 0;
    int16_t w = static_cast<int16_t>(string.length() * 6 * _textSize);
    int16_t h = static_cast<int16_t>(8 * _textSize);
    int32_t cx = x;
    int32_t cy = y;
    switch (_textDatum) {
        case TC_DATUM: cx -= w / 2; break;
        case TR_DATUM: cx -= w; break;
        case MC_DATUM: cx -= w / 2; cy -= h / 2; break;
        case MR_DATUM: cx -= w; cy -= h / 2; break;
        case BC_DATUM: cx -= w / 2; cy -= h; break;
        case BR_DATUM: cx -= w; cy -= h; break;
        case BL_DATUM: cy -= h; break;
        default: break;
    }
    _canvas->setCursor(cx, cy);
    _canvas->print(string);
    return w;
}

void tft_sprite::setPixel(int32_t x, int32_t y, uint16_t color) {
    if (_canvas) _canvas->drawPixel(x, y, color);
}

#endif // USE_ADAFRUIT_SSD1306
