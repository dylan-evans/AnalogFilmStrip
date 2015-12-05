#include <pebble.h>

static Window *window;
static Layer *film1;
static AppTimer *timer;

#define ANIM_TIMER 50
#define SCROLL_INC 2


static void film1_update(Layer *layer, GContext *ctx)
{
    static int sprocket_pos = 5;
    static int frame_pos = -168;

    GRect bounds = layer_get_bounds(layer);

    /* Draw the sprocket holes */

    graphics_context_set_fill_color(ctx, GColorWhite);

    for(int y = sprocket_pos - 24; y < 168; y += 24) {
        graphics_fill_rect(ctx, GRect(3, y, 12, 8), 1, GCornersAll);
        graphics_fill_rect(ctx, GRect(129, y, 12, 8), 1, GCornersAll);
    }

    sprocket_pos = (sprocket_pos + SCROLL_INC) % 24;

    /* Draw the frames */
    graphics_context_set_fill_color(ctx, GColorBulgarianRose);

    graphics_fill_rect(ctx, GRect(18, -256 + frame_pos, 108, 128), 4, GCornersAll);
    graphics_fill_rect(ctx, GRect(18, -113 + frame_pos, 108, 128), 4, GCornersAll);
    graphics_fill_rect(ctx, GRect(18, 30 + frame_pos, 108, 128), 4, GCornersAll);

    frame_pos += SCROLL_INC;
    if(frame_pos > 0) frame_pos %= 143;
}

static void tick_handler(struct tm *tick, TimeUnits units)
{
}

static void app_timer_handler(void *data)
{
    layer_mark_dirty(window_get_root_layer(window));
    timer = app_timer_register(ANIM_TIMER, app_timer_handler, (void *)film1);
}

static void window_load(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    film1 = layer_create(bounds);

    window_set_background_color(window, GColorWindsorTan);
 
    layer_set_update_proc(film1, film1_update);

    layer_add_child(window_layer, film1);
    
    timer = app_timer_register(ANIM_TIMER, app_timer_handler, (void *)film1);
}

static void window_unload(Window *window)
{

}

static void init(void)
{
    window = window_create();
    window_set_window_handlers(window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
    const bool animated = true;
    window_stack_push(window, animated);
    //tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
