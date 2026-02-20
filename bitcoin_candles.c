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
    { 59800, 62000, 58500, 61500 }, // 2
    { 61500, 63000, 60000, 60200 }, // 3
    { 60200, 61000, 58000, 58500 }, // 4
    { 58500, 60000, 57500, 59500 }, // 5
    { 59500, 62500, 59000, 62000 }, // 6
    { 62000, 65500, 61000, 64800 }, // 7
    { 64800, 67200, 63500, 63900 }, // 8
    { 63900, 66000, 62800, 65700 }, // 9
    { 65700, 68000, 65000, 67500 }, // 10
    { 67500, 70000, 66500, 69000 }, // 11
    { 69000, 71500, 68000, 70800 }, // 12
    { 70800, 73000, 69500, 71200 }, // 13
    { 71200, 72000, 68000, 68500 }, // 14
    { 68500, 70000, 67000, 69500 }, // 15
    { 69500, 72000, 69000, 71800 }, // 16
    { 71800, 74000, 71000, 73500 }, // 17
    { 73500, 76000, 72500, 75000 }, // 18
    { 75000, 77500, 74000, 76800 }, // 19
    { 76800, 79000, 75500, 78000 }, // 20
    { 78000, 80500, 77000, 79500 }, // 21
    { 79500, 82000, 78500, 81000 }, // 22
    { 81000, 83500, 80000, 82500 }, // 23
    { 82500, 85000, 81500, 84000 }, // 24
    { 84000, 86500, 83000, 85500 }, // 25
    { 85500, 88000, 84500, 87000 }, // 26
    { 87000, 89500, 86000, 88500 }, // 27
    { 88500, 91000, 87500, 90000 }, // 28
    { 90000, 92500, 89000, 91500 }, // 29
    { 91500, 94000, 90500, 93000 }, // 30
    { 93000, 95500, 92000, 94500 }, // 31
    { 94500, 97000, 93500, 96000 }, // 32
    { 96000, 98500, 95000, 97500 }, // 33
    { 97500, 100000, 96500, 99000 }, // 34
    { 99000, 101500, 98000, 100500 }, // 35
    { 100500, 103000, 99500, 102000 }, // 36
    { 102000, 104500, 101000, 103500 }, // 37
    { 103500, 106000, 102500, 105000 }, // 38
    { 105000, 107500, 104000, 106500 }, // 39
    { 106500, 109000, 105500, 108000 }, // 40
    { 108000, 110500, 107000, 109500 }, // 41
    { 109500, 112000, 108500, 111000 }, // 42
    { 111000, 113500, 110000, 112500 }, // 43
    { 112500, 115000, 111500, 114000 }, // 44
    { 114000, 116500, 113000, 115500 }, // 45
    { 115500, 118000, 114500, 117000 }, // 46
    { 117000, 119500, 116000, 118500 }, // 47
    { 118500, 121000, 117500, 120000 }, // 48
    { 120000, 122500, 119000, 121500 }, // 49
    { 121500, 124000, 120500, 123000 }, // 50
    { 123000, 125500, 122000, 124500 }, // 51
    { 124500, 127000, 123500, 126000 }, // 52
    { 126000, 128500, 125000, 127500 }, // 53
    { 127500, 130000, 126500, 129000 }, // 54
    { 129000, 131500, 128000, 130500 }, // 55
    { 130500, 133000, 129500, 132000 }, // 56
    { 132000, 134500, 131000, 133500 }, // 57
    { 133500, 136000, 132500, 135000 }, // 58
    { 135000, 137500, 134000, 136500 }, // 59
    { 136500, 139000, 135500, 138000 }, // 60
    { 138000, 140500, 137000, 139500 }, // 61
    { 139500, 142000, 138500, 141000 }, // 62
    { 141000, 143500, 140000, 142500 }, // 63
    { 142500, 145000, 141500, 144000 }, // 64
    { 144000, 146500, 143000, 145500 }, // 65
    { 145500, 148000, 144500, 147000 }, // 66
    { 147000, 149500, 146000, 148500 }, // 67
    { 148500, 151000, 147500, 150000 }, // 68
    { 150000, 152500, 149000, 151500 }, // 69
    { 151500, 154000, 150500, 153000 }, // 70
    { 153000, 155500, 152000, 154500 }, // 71
    { 154500, 157000, 153500, 156000 }, // 72
    { 156000, 158500, 155000, 157500 }, // 73
    { 157500, 160000, 156500, 159000 }, // 74
    { 159000, 161500, 158000, 160500 }, // 75
    { 160500, 163000, 159500, 162000 }, // 76
    { 162000, 164500, 161000, 163500 }, // 77
    { 163500, 166000, 162500, 165000 }, // 78
    { 165000, 167500, 164000, 166500 }, // 79
    { 166500, 169000, 165500, 168000 }, // 80
    { 168000, 170500, 167000, 169500 }, // 81
    { 169500, 172000, 168500, 171000 }, // 82
    { 171000, 173500, 170000, 172500 }, // 83
    { 172500, 175000, 171500, 174000 }, // 84
    { 174000, 176500, 173000, 175500 }, // 85
    { 175500, 178000, 174500, 177000 }, // 86
    { 177000, 179500, 176000, 178500 }, // 87
    { 178500, 181000, 177500, 180000 }, // 88
    { 180000, 182500, 179000, 181500 }, // 89
    { 181500, 184000, 180500, 183000 }, // 90
    { 183000, 185500, 182000, 184500 }, // 91
    { 184500, 187000, 183500, 186000 }, // 92
    { 186000, 188500, 185000, 187500 }, // 93
    { 187500, 190000, 186500, 189000 }, // 94
    { 189000, 191500, 188000, 190500 }, // 95
    { 190500, 193000, 189500, 192000 }, // 96
    { 192000, 194500, 191000, 193500 }, // 97
    { 193500, 196000, 192500, 195000 }, // 98
    { 195000, 197500, 194000, 196500 }, // 99
    { 196500, 199000, 195500, 198000 }, // 100
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
