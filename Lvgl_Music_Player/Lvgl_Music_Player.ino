/*******************************************************************************
 * LVGL Hello World
 * This is a simple example for LVGL - Light and Versatile Graphics Library
 * import from: https://github.com/lvgl/lv_demos.git
 *
 * Dependent libraries:
 * LVGL: https://github.com/lvgl/lvgl.git
 * Audio: https://github.com/schreibfaul1/ESP32-audioI2S.git
 * JPEGDEC: https://github.com/bitbank2/JPEGDEC.git
 *
 * LVGL Configuration file:
 * Copy your_arduino_path/libraries/lvgl/lv_conf_template.h
 * to your_arduino_path/libraries/lv_conf.h
 *
 * In lv_conf.h around line 15, enable config file:
 * #if 1 // Set it to "1" to enable content
 *
 * Then find and set:
 * #define LV_COLOR_DEPTH     16
 * #define LV_TICK_CUSTOM     1
 *
 * For SPI/parallel 8 display set color swap can be faster, parallel 16/RGB screen don't swap!
 * #define LV_COLOR_16_SWAP   1 // for SPI and parallel 8
 * #define LV_COLOR_16_SWAP   0 // for parallel 16 and RGB
 *
 * #define LV_FONT_FMT_TXT_LARGE 1
 ******************************************************************************/
#include <lvgl.h>
#include "ui.h"

#include <JPEGDEC.h>
JPEGDEC jpegdec;

#include <SD_MMC.h>
#include <Audio.h>
Audio audio;

// microSD card
#define SD_SCK 48
#define SD_MISO 41
#define SD_MOSI 47
#define SD_CS 42

// I2S
#define I2S_DOUT 40
#define I2S_BCLK 1
#define I2S_LRCK 2

/*******************************************************************************
 * Start of Arduino_GFX setting
 *
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: CS:  9, DC:  8, RST:  7, BL:  6, SCK: 13, MOSI: 11, MISO: 12
 * ESP32 various dev board     : CS:  5, DC: 27, RST: 33, BL: 22, SCK: 18, MOSI: 23, MISO: nil
 * ESP32-C3 various dev board  : CS:  7, DC:  2, RST:  1, BL:  3, SCK:  4, MOSI:  6, MISO: nil
 * ESP32-S2 various dev board  : CS: 34, DC: 38, RST: 33, BL: 21, SCK: 36, MOSI: 35, MISO: nil
 * ESP32-S3 various dev board  : CS: 40, DC: 41, RST: 42, BL: 48, SCK: 36, MOSI: 35, MISO: nil
 * ESP8266 various dev board   : CS: 15, DC:  4, RST:  2, BL:  5, SCK: 14, MOSI: 13, MISO: 12
 * Raspberry Pi Pico dev board : CS: 17, DC: 27, RST: 26, BL: 28, SCK: 18, MOSI: 19, MISO: 16
 * RTL8720 BW16 old patch core : CS: 18, DC: 17, RST:  2, BL: 23, SCK: 19, MOSI: 21, MISO: 20
 * RTL8720_BW16 Official core  : CS:  9, DC:  8, RST:  6, BL:  3, SCK: 10, MOSI: 12, MISO: 11
 * RTL8722 dev board           : CS: 18, DC: 17, RST: 22, BL: 23, SCK: 13, MOSI: 11, MISO: 12
 * RTL8722_mini dev board      : CS: 12, DC: 14, RST: 15, BL: 13, SCK: 11, MOSI:  9, MISO: 10
 * Seeeduino XIAO dev board    : CS:  3, DC:  2, RST:  1, BL:  0, SCK:  8, MOSI: 10, MISO:  9
 * Teensy 4.1 dev board        : CS: 39, DC: 41, RST: 40, BL: 22, SCK: 13, MOSI: 11, MISO: 12
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */

// ESP32S3-2.1-TP
#define GFX_BL 38
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 39 /* CS */,
    48 /* SCK */, 47 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
    4 /* R0 */, 5 /* R1 */, 6 /* R2 */, 7 /* R3 */, 15 /* R4 */,
    8 /* G0 */, 20 /* G1 */, 3 /* G2 */, 46 /* G3 */, 9 /* G4 */, 10 /* G5 */,
    11 /* B0 */, 12 /* B1 */, 13 /* B2 */, 14 /* B3 */, 0 /* B4 */,
    1 /* hsync_polarity */, 10 /* hsync_front_porch */, 8 /* hsync_pulse_width */, 50 /* hsync_back_porch */,
    1 /* vsync_polarity */, 10 /* vsync_front_porch */, 8 /* vsync_pulse_width */, 20 /* vsync_back_porch */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type5_init_operations, sizeof(st7701_type5_init_operations));
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;
static lv_color_t *cbuf;

static char song_list[10][255];
static int song_list_len = 0;
static int song_idx = 0;

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

#if (LV_COLOR_16_SWAP != 0)
  gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif

  lv_disp_flush_ready(disp);
}

// pixel drawing callback
static int jpegDrawCallback(JPEGDRAW *pDraw)
{
  // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  gfx_draw_bitmap_to_framebuffer(
    pDraw->pPixels, pDraw->iWidth, pDraw->iHeight,
    (uint16_t *)cbuf, pDraw->x, pDraw->y, 240, 240);

  return 1;
}

void setup()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("LVGL Hello World Demo");

#ifdef GFX_EXTRA_PRE_INIT
  GFX_EXTRA_PRE_INIT();
#endif

  // Init Display
  gfx->begin();
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  lv_init();

  screenWidth = gfx->width();
  screenHeight = gfx->height();
#ifdef ESP32
  disp_draw_buf = (lv_color_t *)heap_caps_malloc(sizeof(lv_color_t) * screenWidth * 32, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#else
  disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * screenWidth * 32);
#endif
  if (!disp_draw_buf)
  {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  }
  else
  {
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * 32);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /* Change the following line to your display resolution */
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* Initialize the (dummy) input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);

    /* Init SquareLine prepared UI */
    ui_init();

    cbuf = (lv_color_t *)malloc(240 * 240 * 2);
    // In ui.c, replace "ui_ImageCover = lv_gif_create(ui_Screen1);" to "ui_ImageCover = lv_canvas_create(ui_Screen1);"
    lv_canvas_set_buffer(ui_ImageCover, cbuf, 240, 240, LV_IMG_CF_TRUE_COLOR);

    Serial.println("Setup done");
  }

  pinMode(SD_CS /* CS */, OUTPUT);
  digitalWrite(SD_CS /* CS */, HIGH);
  SD_MMC.setPins(SD_SCK /* CLK */, SD_MOSI /* CMD/MOSI */, SD_MISO /* D0/MISO */);
  if (!SD_MMC.begin("/root", true))
  {
    Serial.println(F("ERROR: SD_MMC Mount Failed!"));
  }
  else
  {
    audio.setPinout(I2S_BCLK, I2S_LRCK, I2S_DOUT);
    audio.setVolume(4); // 0...21

    File root = SD_MMC.open("/");
    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            log_i("DIR: %s", file.name());
        } else {
          const char *filename = file.name();

          int8_t len = strlen(filename);
          const char *MP3_EXT = ".mp3";
          if (
            (filename[0] != '.')
            && (strcmp(MP3_EXT, &filename[len-4]) == 0)
          ) {
            log_i("Song file: %s, size: %d", filename, file.size());
            strcpy(song_list[song_list_len++], (char *)filename);
          }
        }
        file = root.openNextFile();
    }

    log_i("Play: %s", song_list[song_idx]);
    audio.connecttoFS(SD_MMC, song_list[song_idx++]);

    xTaskCreatePinnedToCore(Task_Audio, "Task_Audio", 10240, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
  }
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
}

void Task_Audio(void *pvParameters) // This is a task.
{
  while (1)
    audio.loop();
}

void audio_id3data(const char *info)
{ // id3 metadata
  Serial.print("id3data     ");
  Serial.println(info);
}
void audio_id3image(File &file, const size_t pos, const size_t size)
{
  Serial.printf("audio_id3image, pos: %d, size: %d\n", pos, size);
  uint8_t *coverImgBuf = (uint8_t *)malloc(size);
  file.seek(pos);
  file.read(coverImgBuf, size);
  Serial.printf("%c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c\n", coverImgBuf[0], coverImgBuf[1], coverImgBuf[2], coverImgBuf[3], coverImgBuf[4], coverImgBuf[5], coverImgBuf[6], coverImgBuf[7], coverImgBuf[8], coverImgBuf[9], coverImgBuf[10], coverImgBuf[11], coverImgBuf[12], coverImgBuf[13], coverImgBuf[14], coverImgBuf[15]);
  Serial.printf("%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", coverImgBuf[0], coverImgBuf[1], coverImgBuf[2], coverImgBuf[3], coverImgBuf[4], coverImgBuf[5], coverImgBuf[6], coverImgBuf[7], coverImgBuf[8], coverImgBuf[9], coverImgBuf[10], coverImgBuf[11], coverImgBuf[12], coverImgBuf[13], coverImgBuf[14], coverImgBuf[15]);

  jpegdec.openRAM(coverImgBuf + 14, size - 14, jpegDrawCallback);
  jpegdec.decode(0, 0, 0);
  jpegdec.close();

  lv_obj_invalidate(ui_ImageCover);
}
void audio_eof_mp3(const char *info)
{ // end of file
  Serial.print("eof_mp3     ");
  Serial.println(info);

  log_i("Play: %s", song_list[song_idx]);
  audio.connecttoFS(SD_MMC, song_list[song_idx++]);
  if (song_idx >= song_list_len)
  {
    song_idx = 0;
  }
}
