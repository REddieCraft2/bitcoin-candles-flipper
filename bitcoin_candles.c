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
    { 58000, 60500, 57000, 59800 },
    { 59800, 63000, 58000, 61500 },
    { 61500, 62000, 58500, 59000 },
    { 59000, 59500, 56000, 57200 },
    { 57200, 58000, 54500, 55000 },
    { 55000, 57500, 54000, 56800 },
    { 56800, 60000, 56500, 59500 },
    { 59500, 61000, 57000, 57800 },
    { 57800, 58500, 55000, 55500 },
    { 55500, 59000, 55000, 58500 },
    { 58500, 62000, 58000, 61800 },
    { 61800, 65000, 61000, 64500 },
    { 64500, 66000, 62000, 62800 },
    { 62800, 63500, 60000, 60500 },
    { 60500, 64000, 60000, 63500 },
    { 63500, 67000, 63000, 66500 },
    { 66500, 68000, 64000, 64800 },
    { 64800, 65500, 61500, 62000 },
    { 62000, 66000, 61500, 65500 },
    { 65500, 69000, 65000, 68500 },
    { 68500, 70000, 66000, 66800 },
    { 66800, 67500, 63500, 64000 },
    { 64000, 68000, 63500, 67500 },
    { 67500, 71000, 67000, 70500 },
    { 70500, 72000, 68000, 68800 },
    { 68800, 69500, 65500, 66000 },
    { 66000, 70000, 65500, 69500 },
    { 69500, 73000, 69000, 72500 },
    { 72500, 74000, 70000, 70800 },
    { 70800, 71500, 67500, 68000 },
    { 68000, 72000, 67500, 71500 },
    { 71500, 75000, 71000, 74500 },
    { 74500, 76000, 72000, 72800 },
    { 72800, 73500, 69500, 70000 },
    { 70000, 74000, 69500, 73500 },
    { 73500, 77000, 73000, 76500 },
    { 76500, 78000, 74000, 74800 },
    { 74800, 75500, 71500, 72000 },
    { 72000, 76000, 71500, 75500 },
    { 75500, 79000, 75000, 78500 },
    { 78500, 80000, 76000, 76800 },
    { 76800, 77500, 73500, 74000 },
    { 74000, 78000, 73500, 77500 },
    { 77500, 81000, 77000, 80500 },
    { 80500, 82000, 78000, 78800 },
    { 78800, 79500, 75500, 76000 },
    { 76000, 80000, 75500, 79500 },
    { 79500, 83000, 79000, 82500 },
    { 82500, 84000, 80000, 80800 },
    { 80800, 81500, 77500, 78000 },
    { 78000, 82000, 77500, 81500 },
    { 81500, 85000, 81000, 84500 },
    { 84500, 86000, 82000, 82800 },
    { 82800, 83500, 79500, 80000 },
    { 80000, 84000, 79500, 83500 },
    { 83500, 87000, 83000, 86500 },
    { 86500, 88000, 84000, 84800 },
    { 84800, 85500, 81500, 82000 },
    { 82000, 86000, 81500, 85500 },
    { 85500, 89000, 85000, 88500 },
    { 88500, 90000, 86000, 86800 },
    { 86800, 87500, 83500, 84000 },
    { 84000, 88000, 83500, 87500 },
    { 87500, 91000, 87000, 90500 },
    { 90500, 92000, 88000, 88800 },
    { 88800, 89500, 85500, 86000 },
    { 86000, 90000, 85500, 89500 },
    { 89500, 93000, 89000, 92500 },
    { 92500, 94000, 90000, 90800 },
    { 90800, 91500, 87500, 88000 },
    { 88000, 92000, 87500, 91500 },
    { 91500, 95000, 91000, 94500 },
    { 94500, 96000, 92000, 92800 },
    { 92800, 93500, 89500, 90000 },
    { 90000, 94000, 89500, 93500 },
    { 93500, 97000, 93000, 96500 },
    { 96500, 98000, 94000, 94800 },
    { 94800, 95500, 91500, 92000 },
    { 92000, 96000, 91500, 95500 },
    { 95500, 99000, 95000, 98500 },
    { 98500, 100000, 96000, 96800 },
    { 96800, 97500, 93500, 94000 },
    { 94000, 98000, 93500, 97500 },
    { 97500, 101000, 97000, 100500 },
    { 100500, 102000, 98000, 98800 },
    { 98800, 99500, 95500, 96000 },
    { 96000, 100000, 95500, 99500 },
    { 99500, 103000, 99000, 102500 },
    { 102500, 104000, 100000, 100800 },
    { 100800, 101500, 97500, 98000 },
    { 98000, 102000, 97500, 101500 },
    { 101500, 105000, 101000, 104500 },
    { 104500, 106000, 102000, 102800 },
    { 102800, 103500, 99500, 100000 },
    { 100000, 104000, 99500, 103500 },
    { 103500, 107000, 103000, 106500 },
    { 106500, 108000, 104000, 104800 },
    { 104800, 105500, 101500, 102000 },
    { 102000, 106000, 101500, 105500 },
    { 105500, 109000, 105000, 108500 },
};

#define MAX_OBJS 50

typedef enum {
    TOOL_LINE,
    TOOL_HLINE,
    TOOL_BOX,
} DrawTool;

typedef enum {
    OBJ_LINE,
    OBJ_HLINE,
    OBJ_BOX,
} ObjType;

typedef struct {
    ObjType type;
    int price1;
    int price2;
    int cidx1;
    int cidx2;
} DrawnObj;

typedef enum {
    STATE_CHART,
    STATE_DRAW,
} AppStateMode;

typedef struct {
    AppStateMode mode;
    bool running;

    int offset;
    int visible;

    DrawTool tool;
    int cursor_x;
    int cursor_y;
    int start_price;
    int start_cidx;
    bool has_start;

    DrawnObj objs[MAX_OBJS];
    int obj_count;
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
    return CHART_Y + CHART_H - ((price - price_min) * CHART_H / (price_max - price_min));
}

static int y_to_price(int py, int price_min, int price_max) {
    int rel = CHART_Y + CHART_H - py;
    return price_min + (rel * (price_max - price_min)) / CHART_H;
}

static int cidx_to_x(int cidx1000, int offset, int visible) {
    int candle_w = (CHART_W - 4) / visible;
    if(candle_w < 1) candle_w = 1;
    int rel = cidx1000 - offset * 1000;
    return CHART_X + 2 + (rel * candle_w) / 1000 + candle_w / 2;
}

static int x_to_cidx(int px, int offset, int visible) {
    int candle_w = (CHART_W - 4) / visible;
    if(candle_w < 1) candle_w = 1;
    int rel_px = px - (CHART_X + 2 + candle_w / 2);
    return offset * 1000 + (rel_px * 1000) / candle_w;
}

static void draw_candles(Canvas* canvas, AppState* state, int price_min, int price_max) {
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

static void draw_objects(Canvas* canvas, AppState* state, int price_min, int price_max) {
    for(int i = 0; i < state->obj_count; i++) {
        DrawnObj* o = &state->objs[i];
        if(o->type == OBJ_LINE) {
            int x1 = cidx_to_x(o->cidx1, state->offset, state->visible);
            int x2 = cidx_to_x(o->cidx2, state->offset, state->visible);
            int y1 = price_to_y(o->price1, price_min, price_max);
            int y2 = price_to_y(o->price2, price_min, price_max);
            canvas_draw_line(canvas, x1, y1, x2, y2);
        } else if(o->type == OBJ_HLINE) {
            int y = price_to_y(o->price1, price_min, price_max);
            canvas_draw_line(canvas, CHART_X, y, CHART_X + CHART_W, y);
        } else if(o->type == OBJ_BOX) {
            int x_left   = cidx_to_x(o->cidx1, state->offset, state->visible);
            int y_top    = price_to_y(o->price1, price_min, price_max);
            int y_bottom = price_to_y(o->price2, price_min, price_max);
            if(y_top > y_bottom) { int tmp = y_top; y_top = y_bottom; y_bottom = tmp; }
            int box_h = y_bottom - y_top;
            if(box_h < 1) box_h = 1;
            if(x_left < CHART_X + CHART_W) {
                if(x_left < CHART_X) x_left = CHART_X;
                int box_w = (CHART_X + CHART_W) - x_left;
                canvas_draw_frame(canvas, x_left, y_top, box_w, box_h);
            }
        }
    }
}

static const char* tool_name(DrawTool t) {
    if(t == TOOL_LINE)  return "Linie";
    if(t == TOOL_HLINE) return "Horizontal";
    return "Box";
}

static void render(Canvas* canvas, AppState* state) {
    int price_min, price_max;
    get_price_range_window(state->offset, state->visible, &price_min, &price_max);

    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);

    if(state->mode == STATE_CHART) {
        canvas_draw_str(canvas, 2, 7, "BTC/USD");
        if(state->obj_count > 0) canvas_draw_str(canvas, 80, 7, "Back=Undo");
    } else {
        char label[32];
        const char* step = state->has_start ? "Ende" : "Start";
        if(state->tool == TOOL_HLINE)
            snprintf(label, sizeof(label), "%s: OK=setzen", tool_name(state->tool));
        else
            snprintf(label, sizeof(label), "%s: %s", tool_name(state->tool), step);
        canvas_draw_str(canvas, 2, 7, label);
    }

    canvas_draw_frame(canvas, CHART_X, CHART_Y, CHART_W, CHART_H);
    draw_candles(canvas, state, price_min, price_max);
    draw_objects(canvas, state, price_min, price_max);

    if(state->mode == STATE_DRAW) {
        if(state->has_start) {
            if(state->tool == TOOL_LINE) {
                int x1 = cidx_to_x(state->start_cidx, state->offset, state->visible);
                int y1 = price_to_y(state->start_price, price_min, price_max);
                canvas_draw_line(canvas, x1, y1, state->cursor_x, state->cursor_y);
            } else if(state->tool == TOOL_BOX) {
                int x_left   = cidx_to_x(state->start_cidx, state->offset, state->visible);
                int y_top    = price_to_y(state->start_price, price_min, price_max);
                int y_bottom = state->cursor_y;
                if(y_top > y_bottom) { int tmp = y_top; y_top = y_bottom; y_bottom = tmp; }
                int box_h = y_bottom - y_top;
                if(box_h < 1) box_h = 1;
                if(x_left < CHART_X + CHART_W) {
                    if(x_left < CHART_X) x_left = CHART_X;
                    int box_w = (CHART_X + CHART_W) - x_left;
                    canvas_draw_frame(canvas, x_left, y_top, box_w, box_h);
                }
            }
        }
        if(state->tool == TOOL_HLINE) {
            canvas_draw_line(canvas, CHART_X, state->cursor_y,
                                     CHART_X + CHART_W, state->cursor_y);
        }
        // Cursor-Kreuz
        canvas_draw_line(canvas, state->cursor_x - 4, state->cursor_y,
                                 state->cursor_x + 4, state->cursor_y);
        canvas_draw_line(canvas, state->cursor_x, state->cursor_y - 4,
                                 state->cursor_x, state->cursor_y + 4);
    }
}

static void draw_callback(Canvas* canvas, void* ctx) {
    render(canvas, (AppState*)ctx);
}

static void input_callback(InputEvent* event, void* ctx) {
    FuriMessageQueue* queue = (FuriMessageQueue*)ctx;
    furi_message_queue_put(queue, event, FuriWaitForever);
}

int32_t bitcoin_candles_app(void* p) {
    UNUSED(p);

    AppState* state = malloc(sizeof(AppState));
    memset(state, 0, sizeof(AppState));
    state->mode      = STATE_CHART;
    state->running   = true;
    state->offset    = 90;
    state->visible   = 10;
    state->cursor_x  = CHART_X + CHART_W / 2;
    state->cursor_y  = CHART_Y + CHART_H / 2;
    state->tool      = TOOL_LINE;
    state->has_start = false;
    state->obj_count = 0;

    FuriMessageQueue* event_queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    ViewPort* viewport = view_port_alloc();
    view_port_draw_callback_set(viewport, draw_callback, state);
    view_port_input_callback_set(viewport, input_callback, event_queue);
    Gui* gui = furi_record_open(RECORD_GUI);
    gui_add_view_port(gui, viewport, GuiLayerFullscreen);

    InputEvent event;
    while(state->running) {
        if(furi_message_queue_get(event_queue, &event, 100) == FuriStatusOk) {

            // ── CHART-MODUS ──────────────────────────────────────────
            if(state->mode == STATE_CHART) {
                if(event.type == InputTypePress || event.type == InputTypeRepeat) {
                    if(event.key == InputKeyLeft) {
                        if(state->offset > 0) state->offset--;
                    } else if(event.key == InputKeyRight) {
                        if(state->offset + state->visible < CANDLE_COUNT) state->offset++;
                    } else if(event.key == InputKeyUp) {
                        if(state->visible > MIN_VISIBLE) state->visible--;
                    } else if(event.key == InputKeyDown) {
                        if(state->visible < MAX_VISIBLE &&
                           state->offset + state->visible < CANDLE_COUNT)
                            state->visible++;
                    }
                }
                if(event.type == InputTypeShort) {
                    if(event.key == InputKeyOk) {
                        state->mode      = STATE_DRAW;
                        state->cursor_x  = CHART_X + CHART_W / 2;
                        state->cursor_y  = CHART_Y + CHART_H / 2;
                        state->has_start = false;
                    } else if(event.key == InputKeyBack) {
                        if(state->obj_count > 0) state->obj_count--;
                    }
                }
                if(event.type == InputTypeLong && event.key == InputKeyBack) {
                    state->running = false;
                }

            // ── DRAW-MODUS ───────────────────────────────────────────
            } else {

                // Tool wechseln: nur bei InputTypeLong, einmalig
                if(event.type == InputTypeLong && event.key == InputKeyOk) {
                    state->tool = (DrawTool)((state->tool + 1) % 3);
                    state->has_start = false;

                // Cursor bewegen: Press + Repeat, Schritt = 3px
                } else if(event.type == InputTypePress || event.type == InputTypeRepeat) {
                    if(event.key == InputKeyLeft) {
                        state->cursor_x -= 3;
                        if(state->cursor_x < CHART_X + 1) state->cursor_x = CHART_X + 1;
                    } else if(event.key == InputKeyRight) {
                        state->cursor_x += 3;
                        if(state->cursor_x > CHART_X + CHART_W - 1) state->cursor_x = CHART_X + CHART_W - 1;
                    } else if(event.key == InputKeyUp) {
                        state->cursor_y -= 3;
                        if(state->cursor_y < CHART_Y + 1) state->cursor_y = CHART_Y + 1;
                    } else if(event.key == InputKeyDown) {
                        state->cursor_y += 3;
                        if(state->cursor_y > CHART_Y + CHART_H - 1) state->cursor_y = CHART_Y + CHART_H - 1;
                    }

                // Punkt setzen / Zurück: nur InputTypeShort (einmalig nach Loslassen)
                } else if(event.type == InputTypeShort) {
                    if(event.key == InputKeyOk) {
                        int price_min, price_max;
                        get_price_range_window(state->offset, state->visible,
                                               &price_min, &price_max);
                        int cur_price = y_to_price(state->cursor_y, price_min, price_max);
                        int cur_cidx  = x_to_cidx(state->cursor_x, state->offset, state->visible);

                        if(state->tool == TOOL_HLINE) {
                            if(state->obj_count < MAX_OBJS) {
                                state->objs[state->obj_count].type   = OBJ_HLINE;
                                state->objs[state->obj_count].price1 = cur_price;
                                state->obj_count++;
                            }
                        } else if(!state->has_start) {
                            state->start_price = cur_price;
                            state->start_cidx  = cur_cidx;
                            state->has_start   = true;
                        } else {
                            if(state->obj_count < MAX_OBJS) {
                                DrawnObj* o = &state->objs[state->obj_count];
                                if(state->tool == TOOL_LINE) {
                                    o->type   = OBJ_LINE;
                                    o->price1 = state->start_price;
                                    o->cidx1  = state->start_cidx;
                                    o->price2 = cur_price;
                                    o->cidx2  = cur_cidx;
                                } else { // TOOL_BOX
                                    o->type   = OBJ_BOX;
                                    o->price1 = state->start_price;
                                    o->price2 = cur_price;
                                    o->cidx1  = state->start_cidx;
                                }
                                state->obj_count++;
                            }
                            state->has_start = false;
                        }

                    } else if(event.key == InputKeyBack) {
                        if(state->has_start) {
                            state->has_start = false;
                        } else {
                            state->mode = STATE_CHART;
                        }
                    }
                }
            }
        }
        view_port_update(viewport);
    }

    gui_remove_view_port(gui, viewport);
    view_port_free(viewport);
    furi_message_queue_free(event_queue);
    furi_record_close(RECORD_GUI);
    free(state);
    return 0;
}
