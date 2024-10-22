// chamówa ale działa

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_intr_alloc.h"
#include "driver/rmt_tx.h"
#include "led_strip_encoder.h"


#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_GPIO_NUM      0

#define EXAMPLE_LED_NUMBERS         18
#define EXAMPLE_CHASE_SPEED_MS      100

static const char *TAG = "dynia";

static uint8_t led_strip_pixels[EXAMPLE_LED_NUMBERS * 3];

#define  RX_BUF_SIZE 256

static char rx_hpcp_frame[ RX_BUF_SIZE + 2 ];
static char rx_hpcp_data[ RX_BUF_SIZE + 2 ];
static unsigned char rx_hpcp_frame_idx = 0;

static unsigned char rx_temp_data [RX_BUF_SIZE+2];

static bool hpcp_frame_update_mark = false;

static void uart_rx_task(void *arg) {
    while (1) {
        size_t data_len;
        uart_get_buffered_data_len(UART_NUM_0, &data_len);
        if (data_len != 0) {
            int count = uart_read_bytes(UART_NUM_0, rx_temp_data, 8, 10/portTICK_PERIOD_MS);
            if ( count > 0 ) {
                for (int n = 0; n < count; n++ ) {
                    if ( rx_temp_data[ n ] != 0x0D ) {
                        rx_hpcp_frame [ rx_hpcp_frame_idx ] = rx_temp_data[ n ];
                        rx_hpcp_frame_idx++;
                        rx_hpcp_frame [ rx_hpcp_frame_idx ] = 0x00;
                        if (rx_hpcp_frame_idx > RX_BUF_SIZE ) {
                            memset( rx_hpcp_frame, 0x00, sizeof(rx_hpcp_frame));
                        }
                    }
                    else {
                        strcpy (rx_hpcp_data, rx_hpcp_frame);
                        hpcp_frame_update_mark = true;
                        //ESP_LOG_BUFFER_CHAR(TAG, rx_hpcp_frame, strlen(rx_hpcp_frame));
                        printf("%s\n", rx_hpcp_data);
                        memset( rx_hpcp_frame, 0x00, sizeof(rx_hpcp_frame));
                        rx_hpcp_frame_idx = 0;
                    }
                }
            }
        }
        vTaskDelay(10/portTICK_PERIOD_MS);
    }
}

#define GREEN_PART  0
#define BLUE_PART   1
#define RED_PART    2

// ^LLLLRRRR.rrrgggbbb
// 0123456789012345678

int CharToDec(char c){
  if(c>='0' && c<='9') return c-'0';
  if(c>='a' && c<='f') return c-'a'+10;
  if(c>='A' && c<='F') return c-'A'+10;
  return 0;
}

unsigned short hex2dec( char *h ) {
  char *P;
  int p=1,d=0;
  for(P=h; *P; P++);
  for(P--;P>=h;P--){
    d = d + (CharToDec(*P) * p);
    p =  p*16;
  }
  return d;
}

void hpcp_frame_2_leds( char *pFrame, uint8_t *pLeds ) {
    if ( pFrame[0] != '^') {
        ESP_LOGI(TAG, "bad frame");
        return;
    }
    if ( strlen(pFrame) != 19 ) {
        ESP_LOGI(TAG, "bad frame");
        return;
    }
    // oczka
    for (int n = 0; n < 8; n++ ) {
        char color = pFrame[ n + 1 ];
        switch( color ) {
            case 'R':
                pLeds[ (10 + n) * 3 + GREEN_PART] = 0;
                pLeds[ (10 + n) * 3 + BLUE_PART] = 0;
                pLeds[ (10 + n) * 3 + RED_PART] = 30;
                break;
            case 'G':
                pLeds[ (10 + n) * 3 + GREEN_PART] = 30;
                pLeds[ (10 + n) * 3 + BLUE_PART] = 0;
                pLeds[ (10 + n) * 3 + RED_PART] = 0;
                break;
            case 'B':
                pLeds[ (10 + n) * 3 + GREEN_PART] = 0;
                pLeds[ (10 + n) * 3 + BLUE_PART] = 30;
                pLeds[ (10 + n) * 3 + RED_PART] = 0;
                break;
            default:
                pLeds[ (10 + n) * 3 + GREEN_PART] = 0;
                pLeds[ (10 + n) * 3 + BLUE_PART] = 0;
                pLeds[ (10 + n) * 3 + RED_PART] = 0;
        }
    }
    // reszta
    char temp [4];
    memcpy( temp, &pFrame[10], 3);
    temp[3] = 0;
    unsigned short rrr = hex2dec(temp);

    memcpy( temp, &pFrame[13], 3);
    temp[3] = 0;
    unsigned short ggg = hex2dec(temp);

    memcpy( temp, &pFrame[16], 3);
    temp[3] = 0;
    unsigned short bbb = hex2dec(temp);
    for (int r = 0; r < 10; r++) {
         pLeds[ r * 3 + GREEN_PART] = (ggg & (1<<r)) ? 10 : 0;
         pLeds[ r * 3 + BLUE_PART] = (bbb & (1<<r)) ? 10 : 0;
         pLeds[ r * 3 + RED_PART] = (rrr & (1<<r)) ? 10 : 0;
    }
}

void app_main(void) {

    ESP_LOGI(TAG, "Create RMT TX channel");
    rmt_channel_handle_t led_chan = NULL;
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
        .gpio_num = RMT_LED_STRIP_GPIO_NUM,
        .mem_block_symbols = 64, // increase the block size can make the LED less flickering
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
        .trans_queue_depth = 4, // set the number of transactions that can be pending in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));

    ESP_LOGI(TAG, "Install led strip encoder");
    rmt_encoder_handle_t led_encoder = NULL;
    led_strip_encoder_config_t encoder_config = {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));

    ESP_LOGI(TAG, "Enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(led_chan));

    ESP_LOGI(TAG, "Start LED rainbow chase");
    rmt_transmit_config_t tx_config = {
        .loop_count = 0, // no transfer loop
    };

    const uart_config_t uart_config = {
        .baud_rate = 2*1200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_0, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    memset (rx_hpcp_frame, 0x00, sizeof(rx_hpcp_frame));

    xTaskCreate(uart_rx_task, "uart_rx_task", 1024*2, NULL, 10, NULL);

    while (1) {
        if ( hpcp_frame_update_mark == true ) {
            hpcp_frame_update_mark = false;
            hpcp_frame_2_leds( rx_hpcp_data, led_strip_pixels );
            ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));
            ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
        }
        vTaskDelay(/*pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS)*/ 5/portTICK_PERIOD_MS );
   }
}
