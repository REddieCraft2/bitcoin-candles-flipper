#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// ============================================================
//  BETA: Bitcoin Candle Chart für Flipper Zero
//  Zeigt 4 Candles mit OHLC-Daten auf dem 128x64 Display
// ============================================================

#define CANDLE_COUNT 4
#define CHART_X      10   // Linker Rand
#define CHART_Y      8    // Oberer Rand
#define CHART_W      108  // Breite des Chart-Bereichs
#define CHART_H      48   // Höhe des Chart-Bereichs
#define CANDLE_W     10   // Breite eines Candle-Körpers
#define CANDLE_GAP   16   // Abstand zwischen Candles (Mitte zu Mitte)

// OHLC Candle Struktur
typedef struct {
    float open;
    float high;
    float low;
    float close;
    const char* label;
} Candle;

// ============================================================
//  HIER KANNST DU DIE WERTE SPÄTER ANPASSEN
// ============================================================
static Candle candles[CANDLE_COUNT] = {
    { .open = 62000, .high = 65500, .low = 61000, .close = 64800, .label = "T-3" },
    { .open = 64800, .high = 67200, .low = 63500, .close = 63900, .label = "T-2" },
    { .open = 63900, .high = 66000, .low = 62800, .close = 65700, .label = "T-1" },
    { .open = 65700, .high = 68000, .low = 65000, .close = 67500, .label = " T " },
};

typedef struct {
    bool running;
    int  selected;
} AppState;

static int price_to_y(float price, float price_min, float price_max) {
    if(price_max <= price_min) return CHART_Y + CHART_H / 2;
    float ratio = (price - price_min) / (price_max - price_min);
    return (int)(CHART_Y + CHART_H - ratio * CHART_H);
}

static void get_price_range(float* out_min, float* out_max) {
    float mn = candles[0].low;
    float mx = candles[0].high;
    for(int i = 1; i < CANDLE_COUNT; i++) {
        if(candles[i].low  < mn) mn = candles[i].low;
        if(candles[i].high > mx) mx = candles[i].high;
    }
    float pad = (mx - mn) * 0.05f;
    *out_min = mn - pad;
    *out_max = mx + pad;
}

static void draw_callback(Canvas* canvas, void* ctx) {
    AppState* state = (AppState*)ctx;

    float price_min, price_max;
    get_price_range(&price_min, &price_max);

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 7, "BTC/USD");

    canvas_draw_frame(canvas, CHART_X - 1, CHART_Y - 1, CHART_W + 2, CHART_H + 2);

    for(int i = 0; i < CANDLE_COUNT; i++) {
        Candle* c = &candles[i];
        bool bullish = (c->close >= c->open);

        int cx = CHART_X + 14 + i * CANDLE_GAP + (CANDLE_GAP / 2);

        int y_high  = price_to_y(c->high,  price_min, price_max);
        int y_low   = price_to_y(c->low,   price_min, price_max);
        int y_open  = price_to_y(c->open,  price_min, price_max);
        int y_close = price_to_y(c->close, price_min, price_max);

        int body_top    = bullish ? y_close : y_open;
        int body_bottom = bullish ? y_open  : y_close;
        int body_h      = body_bottom - body_top;
        if(body_h < 1) body_h = 1;

        canvas_draw_line(canvas, cx, y_high, cx, body_top);
        canvas_draw_line(canvas, cx, body_bottom, cx, y_low);

        int bx = cx - CANDLE_W / 2;
        if(bullish) {
            canvas_draw_frame(canvas, bx, body_top, CANDLE_W, body_h);
        } else {
            canvas_draw_box(canvas, bx, body_top, CANDLE_W, body_h);
        }

        if(i == state->selected) {
            canvas_draw_str(canvas, cx - 2, CHART_Y - 2, "^");
        }

        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, cx - 5, CHART_Y + CHART_H + 10, c->label);
    }

    Candle* sel = &candles[state->selected];
    char info[48];
    snprintf(info, sizeof(info), "O:%.1fk C:%.1fk", sel->open / 1000.0f, sel->close / 1000.0f);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 0, 63, info);
}

static void input_callback(InputEvent* event, void* ctx) {
    FuriMessageQueue* queue = (FuriMessageQueue*)ctx;
    furi_message_queue_put(queue, event, FuriWaitForever);
}

int32_t bitcoin_candles_app(void* p) {
    UNUSED(p);

    AppState state = { .running = true, .selected = 3 };

    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));

    ViewPort* viewport = view_port_alloc();
    view_port_draw_callback_set(viewport, draw_callback, &state);
    view_port_input_callback_set(viewport, input_callback, event_queue);

    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, viewport, GuiLayerFullscreen);

    InputEvent event;
    while(state.running) {
        if(furi_message_queue_get(event_queue, &event, 100) == FuriStatusOk) {
            if(event.type == InputTypePress || event.type == InputTypeRepeat) {
                switch(event.key) {
                case InputKeyLeft:
                    if(state.selected > 0) state.selected--;
                    break;
                case InputKeyRight:
                    if(state.selected < CANDLE_COUNT - 1) state.selected++;
                    break;
                case InputKeyBack:
                    state.running = false;
                    break;
                default:
                    break;
                }
            }
        }
        view_port_update(viewport);
    }

    gui_remove_view_port(gui, viewport);
    view_port_free(viewport);
    furi_message_queue_free(event_queue);
    furi_record_close(RECORD_GUI);

    return 0;
}
