#include <pebble.h>

static Window *window;

static TextLayer *gridpower_layer;
static TextLayer *dailyenery_layer;
static TextLayer *montlyenergy_layer;
static TextLayer *yearlyenergy_layer;
static TextLayer *totalenergy_layer;

static AppSync sync;
static uint8_t sync_buffer[256];

enum SolarKey {
  RELOAD = 0x0,              // TUPLE_NTEGER
  SOLAR_GRIDPOWER = 0x1,     // TUPLE_CSTRING
  SOLAR_DAILYENERGY = 0x2,   // TUPLE_CSTRING
  SOLAR_MONTHLYENERGY = 0x3, // TUPLE_CSTRING
  SOLAR_YEARLYENERGY = 0x4,  // TUPLE_CSTRING
  SOLAR_TOTALENERGY = 0x5    // TUPLE_CSTRING
};


static void sync_error_callback(DictionaryResult dict_error, AppMessageResult app_message_error, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "App Message Sync Error: %d", app_message_error);
}

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
    case SOLAR_GRIDPOWER:      
      text_layer_set_text(gridpower_layer, new_tuple->value->cstring);
    break;
    case SOLAR_DAILYENERGY:
      text_layer_set_text(dailyenery_layer, new_tuple->value->cstring);
    break;
    case SOLAR_MONTHLYENERGY:
      text_layer_set_text(montlyenergy_layer, new_tuple->value->cstring);
    break;
    case SOLAR_YEARLYENERGY:
      text_layer_set_text(yearlyenergy_layer, new_tuple->value->cstring);
    break;
    case SOLAR_TOTALENERGY:
      text_layer_set_text(totalenergy_layer, new_tuple->value->cstring);
    break;    
  };
}

static void send_cmd(int reload) {
  Tuplet value = TupletInteger(0, reload);

  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (iter == NULL) {
    return;
  }

  dict_write_tuplet(iter, &value);
  dict_write_end(iter);

  app_message_outbox_send();
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {

}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {

}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  send_cmd(1); //RELOAD
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  
  gridpower_layer = text_layer_create(GRect(0, 10, 144, 68));
  text_layer_set_text_color(gridpower_layer, GColorBlack);
  text_layer_set_background_color(gridpower_layer, GColorClear);
  text_layer_set_font(gridpower_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(gridpower_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(gridpower_layer));
  
  
  dailyenery_layer = text_layer_create(GRect(0, 30, 144, 68));
  text_layer_set_text_color(dailyenery_layer, GColorBlack);
  text_layer_set_background_color(dailyenery_layer, GColorClear);
  text_layer_set_font(dailyenery_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(dailyenery_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(dailyenery_layer));  
  
  montlyenergy_layer = text_layer_create(GRect(0, 50, 144, 68));
  text_layer_set_text_color(montlyenergy_layer, GColorBlack);
  text_layer_set_background_color(montlyenergy_layer, GColorClear);
  text_layer_set_font(montlyenergy_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(montlyenergy_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(montlyenergy_layer));
  
  yearlyenergy_layer = text_layer_create(GRect(0, 70, 144, 68));
  text_layer_set_text_color(yearlyenergy_layer, GColorBlack);
  text_layer_set_background_color(yearlyenergy_layer, GColorClear);
  text_layer_set_font(yearlyenergy_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(yearlyenergy_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(yearlyenergy_layer));
  
  totalenergy_layer = text_layer_create(GRect(0, 90, 144, 68));
  text_layer_set_text_color(totalenergy_layer, GColorBlack);
  text_layer_set_background_color(totalenergy_layer, GColorClear);
  text_layer_set_font(totalenergy_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(totalenergy_layer, GTextAlignmentLeft);
  layer_add_child(window_layer, text_layer_get_layer(totalenergy_layer));
  
  Tuplet initial_values[] = {
    TupletCString(SOLAR_GRIDPOWER, "Power:"),
    TupletCString(SOLAR_DAILYENERGY, "Daily:"),
    TupletCString(SOLAR_MONTHLYENERGY, "Monthly:"),
    TupletCString(SOLAR_YEARLYENERGY, "Yearly:"),
    TupletCString(SOLAR_TOTALENERGY, "Total:"),
  };  
  
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, sync_error_callback, NULL);
  
  send_cmd(0);
  
}

static void window_unload(Window *window) {
  app_sync_deinit(&sync);

  text_layer_destroy(gridpower_layer);
  text_layer_destroy(dailyenery_layer);
  text_layer_destroy(montlyenergy_layer);
  text_layer_destroy(yearlyenergy_layer);
  text_layer_destroy(totalenergy_layer);
}

void handle_init(void) {
  window = window_create();
  window_set_background_color(window,GColorWhite);
  
  window_set_click_config_provider(window, click_config_provider);
  
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload
  });

  const int inbound_size = 128;
  const int outbound_size = 128;
  app_message_open(inbound_size, outbound_size);
  
  window_stack_push(window, true);
}

void handle_deinit(void) {
  window_destroy(window);
}

int main(void) {
  handle_init();
  app_event_loop();
  handle_deinit();
}
