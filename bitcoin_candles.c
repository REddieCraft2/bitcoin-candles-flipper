#include <furi.h>
#include <gui/gui.h>
#include <input/input.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CANDLE_COUNT   100
#define MIN_VISIBLE    3
#define MAX_VISIBLE    20
#define CHART_X        2
#define CHART_Y        10
#define CHART_W        124
#define CHART_H        52

typedef struct {
    int open;
    int high;
    int low;
    int close;
} Candle;

static Candle candles[CANDLE_COUNT] = {
    { 58000, 60500, 57000, 59800 }, // 1
    { 59800, 63000, 58000, 61500 }, // 2
    { 61500, 62000, 58500, 59000 }, // 3
    { 59000, 59500, 56000, 57200 }, // 4
    { 57200, 58000, 54500, 55000 }, // 5
    { 55000, 57500, 54000, 56800 }, // 6
    { 56800, 60000, 56500, 59500 }, // 7
    { 59500, 61000, 57000, 57800 }, // 8
    { 57800, 58500, 55000, 55500 }, // 9
    { 55500, 59000, 55000, 58500 }, // 10
    { 58500, 62000, 58000, 61800 }, // 11
    { 61800, 65000, 61000, 64500 }, // 12
    { 64500, 66000, 62000, 62800 }, // 13
    { 62800, 63500, 60000, 60500 }, // 14
    { 60500, 64000, 60000, 63500 }, // 15
    { 63500, 67000, 63000, 66500 }, // 16
    { 66500, 68000, 64000, 64800 }, // 17
    { 64800, 65500, 61500, 62000 }, // 18
    { 62000, 66000, 61500, 65500 }, // 19
    { 65500, 69000, 65000, 68500 }, // 20
    { 68500, 70000, 66000, 66800 }, // 21
    { 66800, 67500, 63500, 64000 }, // 22
    { 64000, 68000, 63500, 67500 }, // 23
    { 67500, 71000, 67000, 70500 }, // 24
    { 70500, 72000, 68000, 68800 }, // 25
    { 68800, 69500, 65500, 66000 }, // 26
    { 66000, 70000, 65500, 69500 }, // 27
    { 69500, 73000, 69000, 72500 }, // 28
    { 72500, 74000, 70000, 70800 }, // 29
    { 70800, 71500, 67500, 68000 }, // 30
    { 68000, 72000, 67500, 71500 }, // 31
    { 71500, 75000, 71000, 74500 }, // 32
    { 74500, 76000, 72000, 72800 }, // 33
    { 72800, 73500, 69500, 70000 }, // 34
    { 70000, 74000, 69500, 73500 }, // 35
    { 73500, 77000, 73000, 76500 }, // 36
    { 76500, 78000, 74000, 74800 }, // 37
    { 74800, 75500, 71500, 72000 }, // 38
    { 72000, 76000, 71500, 75500 }, // 39
    { 75500, 79000, 75000, 78500 }, // 40
    { 78500, 80000, 76000, 76800 }, // 41
    { 76800, 77500, 73500, 74000 }, // 42
    { 74000, 78000, 73500, 77500 }, // 43
    { 77500, 81000, 77000, 80500 }, // 44
    { 80500, 82000, 78000, 78800 }, // 45
    { 78800, 79500, 75500, 76000 }, // 46
    { 76000, 80000, 75500, 79500 }, // 47
    { 79500, 83000, 79000, 82500 }, // 48
    { 82500, 84000, 80000, 80800 }, // 49
    { 80800, 81500, 77500, 78000 }, // 50
    { 78000, 82000, 77500, 81500 }, // 51
    { 81500, 85000, 81000, 84500 }, // 52
    { 84500, 86000, 82000, 82800 }, // 53
    { 82800, 83500, 79500, 80000 }, // 54
    { 80000, 84000, 79500, 83500 }, // 55
    { 83500, 87000, 83000, 86500 }, // 56
    { 86500, 88000, 84000, 84800 }, // 57
    { 84800, 85500, 81500, 82000 }, // 58
    { 82000, 86000, 81500, 85500 }, // 59
    { 85500, 89000, 85000, 88500 }, // 60
    { 88500, 90000, 86000, 86800 }, // 61
    { 86800, 87500, 83500, 84000 }, // 62
    { 84000, 88000, 83500, 87500 }, // 63
    { 87500, 91000, 87000, 90500 }, // 64
    { 90500, 92000, 88000, 88800 }, // 65
    { 88800, 89500, 85500, 86000 }, // 66
    { 86000, 90000, 85500, 89500 }, // 67
    { 89500, 93000, 89000, 92500 }, // 68
    { 92500, 94000, 90000, 90800 }, // 69
    { 90800, 91500, 87500, 88000 }, // 70
    { 88000, 92000, 87500, 91500 }, // 71
    { 91500, 95000, 91000, 94500 }, // 72
    { 94500, 96000, 92000, 92800 }, // 73
    { 92800, 93500, 89500, 90000 }, // 74
    { 90000, 94000, 89500, 93500 }, // 75
    { 93500, 97000, 93000, 96500 }, // 76
    { 96500, 98000, 94000, 94800 }, // 77
    { 94800, 95500, 91500, 92000 }, // 78
    { 92000, 96000, 91500, 95500 }, // 79
    { 95500, 99000, 95000, 98500 }, // 80
    { 98500, 100000, 96000, 96800 }, // 81
    { 96800, 97500, 93500, 94000 }, // 82
    { 94000, 98000, 93500, 97500 }, // 83
    { 97500, 101000, 97000, 100500 }, // 84
    { 100500, 102000, 98000, 98800 }, // 85
    { 98800, 99500, 95500, 96000 }, // 86
    { 96000, 100000, 95500, 99500 }, // 87
    { 99500, 103000, 99000, 102500 }, // 88
    { 102500, 104000, 100000, 100800 }, // 89
    { 100800, 101500, 97500, 98000 }, // 90
    { 98000, 102000, 97500, 101500 }, // 91
    { 101500, 105000, 101000, 104500 }, // 92
    { 104500, 106000, 102000, 102800 }, // 93
    { 102800, 103500, 99500, 100000 }, // 94
    { 100000, 104000, 99500, 103500 }, // 95
    { 103500, 107000, 103000, 106500 }, // 96
    { 106500, 108000, 104000, 104800 }, // 97
    { 104800, 105500, 101500, 102000 }, // 98
    { 102000, 106000, 101500, 105500 }, // 99
    { 105500, 109000, 105000, 108500 }, // 100
};

typedef struct {
    bool running;
    int  offset;
    int  visible;
} AppState;

static void get_price_range_window(int offset, int visible, int* out_min, int* out_max) {
    int mn = candles[offset].low;
    int mx = candles[offset].high;
    for(int i = offset; i < offset + visible && i < CANDLE_COUNT; i++) {
        if(candles[i].low  < mn) mn = candles[i].low;
        if(candles[i].high > mx) mx = candles[i].high;
    }
    int pad = (mx - mn) / 20;
    if(pad < 1) pad = 1;
    *out_min = mn - pad;
    *out_max = mx + pad;
}

static int price_to_y(int price, int price_min, int price_max) {
    if(price_max <= price_min) return CHART_Y + CHART_H / 2;
    int ratio_num = (price - price_min) * CHART_H;
    int ratio_den = (price_max - price_min);
    return CHART_Y + CHART_H - (ratio_num / ratio_den);
}

static void draw_callback(Canvas* canvas, void* ctx) {
    AppState* state = (AppState*)ctx;

    int price_min, price_max;
    get_price_range_window(state->offset, state->visible, &price_min, &price_max);

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);

    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 7, "BTC/USD");

    canvas_draw_frame(canvas, CHART_X, CHART_Y, CHART_W, CHART_H);

    int candle_w = (CHART_W - 4) / state->visible;
    if(candle_w < 1) candle_w = 1;
    int body_w = candle_w - 2;
    if(body_w < 1) body_w = 1;

    for(int i = 0; i < state->visible; i++) {
        int idx = state->offset + i;
        if(idx >= CANDLE_COUNT) break;

        Candle* c = &candles[idx];
        bool bullish = (c->close >= c->open);

        int cx = CHART_X + 2 + i * candle_w + candle_w / 2;

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

        int bx = cx - body_w / 2;
        if(bullish) {
            canvas_draw_frame(canvas, bx, body_top, body_w, body_h);
        } else {
            canvas_draw_box(canvas, bx, body_top, body_w, body_h);
        }
    }
}

static void input_callback(InputEvent* event, void* ctx) {
    FuriMessageQueue* queue = (FuriMessageQueue*)ctx;
    furi_message_queue_put(queue, event, FuriWaitForever);
}

int32_t bitcoin_candles_app(void* p) {
    UNUSED(p);

    AppState state = {
        .running = true,
        .offset  = 90,
        .visible = 10,
    };

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
                    if(state.offset > 0) state.offset--;
                    break;
                case InputKeyRight:
                    if(state.offset + state.visible < CANDLE_COUNT) state.offset++;
                    break;
                case InputKeyUp:
                    if(state.visible < MAX_VISIBLE && state.offset + state.visible < CANDLE_COUNT)
                        state.visible++;
                    break;
                case InputKeyDown:
                    if(state.visible > MIN_VISIBLE) state.visible--;
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
