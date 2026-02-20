#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CANDLE_COUNT 10
#define CHART_X      2
#define CHART_Y      10
#define CHART_W      124
#define CHART_H      52
#define CANDLE_W     7
#define CANDLE_GAP   12

typedef struct {
    int open;
    int high;
    int low;
    int close;
} Candle;

static Candle candles[CANDLE_COUNT] = {
    { .open = 58000, .high = 60500, .low = 57000, .close = 59800 },
    { .open = 59800, .high = 62000, .low = 58500, .close = 61500 },
    { .open = 61500, .high = 63000, .low = 60000, .close = 60200 },
    { .open = 60200, .high = 61000, .low = 58000, .close = 58500 },
    { .open = 58500, .high = 60000, .low = 57500, .close = 59500 },
    { .open = 59500, .high = 62500, .low = 59000, .close = 62000 },
    { .open = 62000, .high = 65500, .low = 61000, .close = 64800 },
    { .open = 64800, .high = 67200, .low = 63500, .close = 63900 },
    { .open = 63900, .high = 66000, .low = 62800, .close = 65700 },
    { .open = 65700, .high = 68000, .low = 65000, .close = 67500 },
};

typedef struct {
    bool running;
} AppState;

static int price_to_y(int price, int price_min, int price_max) {
    if(price_max <= price_min) return CHART_Y + CHART_H / 2;
    int ratio_num = (price - price_min) * CHART_H;
    int ratio_den = (price_max - price_min);
    return CHART_Y + CHART_H - (ratio_num / ratio_den);
}

static void get_price_range(int* out_min, int* out_max) {
    int mn = candles[0].low;
    int mx = candles[0].high;
    for(int i = 1; i < CANDLE_COUNT; i++) {
        if(candles[i].low  < mn) mn = candles[i].low;
        if(candles[i].high > mx) mx = candles[i].high;
    }
    int pad = (mx - mn) / 20;
    *out_min = mn - pad;
    *out_max = mx + pad;
}

static void draw_callback(Canvas* canvas, void* ctx) {
    UNUSED(ctx);

    int price_min, price_max;
    get_price_range(&price_min, &price_max);

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    // Titel oben links
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 7, "BTC/USD");

    // Chart Rahmen
    canvas_draw_frame(canvas, CHART_X, CHART_Y, CHART_W, CHART_H);

    // Candles zeichnen
    for(int i = 0; i < CANDLE_COUNT; i++) {
        Candle* c = &candles[i];
        bool bullish = (c->close >= c->open);

        int cx = CHART_X + 6 + i * CANDLE_GAP;

        int y_high  = price_to_y(c->high,  price_min, price_max);
        int y_low   = price_to_y(c->low,   price_min, price_max);
        int y_open  = price_to_y(c->open,  price_min, price_max);
        int y_close = price_to_y(c->close, price_min, price_max);

        int body_top    = bullish ? y_close : y_open;
        int body_bottom = bullish ? y_open  : y_close;
        int body_h      = body_bottom - body_top;
        if(body_h < 1) body_h = 1;

        // Wicks
        canvas_draw_line(canvas, cx, y_high, cx, body_top);
        canvas_draw_line(canvas, cx, body_bottom, cx, y_low);

        // Körper
        int bx = cx - CANDLE_W / 2;
        if(bullish) {
            canvas_draw_frame(canvas, bx, body_top, CANDLE_W, body_h);
        } else {
            canvas_draw_box(canvas, bx, body_top, CANDLE_W, body_h);
        }
    }
}

static void input_callback(InputEvent* event, void* ctx) {
    FuriMessageQueue* queue = (FuriMessageQueue*)ctx;
    furi_message_queue_put(queue, event, FuriWaitForever);
}

int32_t bitcoin_candles_app(void* p) {
    UNUSED(p);

    AppState state = { .running = true };

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
                if(event.key == InputKeyBack) {
                    state.running = false;
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
