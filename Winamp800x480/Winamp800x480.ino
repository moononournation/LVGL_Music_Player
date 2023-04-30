/*******************************************************************************
 * LVGL Music Player
 * This is a simple example for LVGL - Light and Versatile Graphics Library
 * import from: https://github.com/lvgl/lv_demos.git
 *
 * TODO:
 * change Lyrics font colors
 * prev/next song
 * sound spectrum
 * debug
 *
 * Dependent libraries:
 * LVGL: https://github.com/lvgl/lvgl.git
 * JPEGDEC: https://github.com/bitbank2/JPEGDEC.git
 * TouchLib: https://github.com/mmMicky/TouchLib.git
 * Audio: https://github.com/moononournation/ESP32-audioI2S.git
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
 * #define LV_USE_FONT_COMPRESSED 1
 ******************************************************************************/
// ESP32_8048S070
// Cover size
#define MP3_COVER_IMG_W 508
#define MP3_COVER_IMG_H 200
// TOUCH
#define TOUCH_MODULES_GT911                    // GT911 / CST_SELF / CST_MUTUAL / ZTW622 / L58 / FT3267 / FT5x06
#define TOUCH_MODULE_ADDR GT911_SLAVE_ADDRESS1 // CTS328_SLAVE_ADDRESS / L58_SLAVE_ADDRESS / CTS826_SLAVE_ADDRESS / CTS820_SLAVE_ADDRESS / CTS816S_SLAVE_ADDRESS / FT3267_SLAVE_ADDRESS / FT5x06_ADDR / GT911_SLAVE_ADDRESS1 / GT911_SLAVE_ADDRESS2 / ZTW622_SLAVE1_ADDRESS / ZTW622_SLAVE2_ADDRESS
#define TOUCH_SCL 20
#define TOUCH_SDA 19
#define TOUCH_RES 38
#define TOUCH_INT 18
// microSD card
#define SD_SCK 12
#define SD_MISO 13
#define SD_MOSI 11
#define SD_CS 10
// I2S
#define I2S_DOUT 17
#define I2S_BCLK 0
#define I2S_LRCK 18

#include <lvgl.h>
#include "ui.h"

#include <JPEGDEC.h>
JPEGDEC jpegdec;

#include <Audio.h>
Audio audio;

#include <SD_MMC.h>

#include "touch.h"

/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */

// ESP32_8048S070
#define GFX_BL 2
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    41 /* DE */, 40 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    14 /* R0 */, 21 /* R1 */, 47 /* R2 */, 48 /* R3 */, 45 /* R4 */,
    9 /* G0 */, 46 /* G1 */, 3 /* G2 */, 8 /* G3 */, 16 /* G4 */, 1 /* G5 */,
    15 /* B0 */, 7 /* B1 */, 6 /* B2 */, 5 /* B3 */, 4 /* B4 */,
    0 /* hsync_polarity */, 180 /* hsync_front_porch */, 30 /* hsync_pulse_width */, 16 /* hsync_back_porch */,
    0 /* vsync_polarity */, 12 /* vsync_front_porch */, 13 /* vsync_pulse_width */, 10 /* vsync_back_porch */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    800 /* width */, 480 /* height */, rgbpanel);
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

#include "FFT.h"
#define CANVAS_FFT_WIDTH 75
#define CANVAS_FFT_HEIGHT 17
Arduino_Canvas *canvasFFT_gfx = new Arduino_Canvas(CANVAS_FFT_WIDTH /* width */, CANVAS_FFT_HEIGHT /* height */, NULL);
lv_obj_t *ui_CanvasFFT;

/* Change to your screen resolution */
static uint32_t screenWidth;
static uint32_t screenHeight;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;

static bool isPlaying = false;
static bool isSelectedSongChanged = false;
static int song_count = 0;
static char textBuf[6];
static uint32_t currentSongDuration = 0;
static uint32_t currentTimeProgress = 0;
static String playingStr;
static size_t coverImgFileSize = 0;
static uint8_t *coverImgFile;
static int16_t coverImgBitmapW;
static int16_t coverImgBitmapH;
static size_t coverImgBitmapSize = 0;
static uint8_t *coverImgBitmap;
static lv_img_dsc_t img_cover;
static size_t lyricsTextSize = 0;
static char *lyricsText;
static uint16_t syncTimeLyricsSec[100];
static uint8_t syncTimeLyricsLineIdx[100];
static uint16_t syncTimeLyricsCount = 0;

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

void force_refresh_screen()
{
  lv_obj_invalidate(ui_Screen1);
  lv_timer_handler(); /* let the GUI do its work */
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch_has_signal())
  {
    if (touch_touched())
    {
      data->state = LV_INDEV_STATE_PR;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
    }
    else if (touch_released())
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
}

void volumeChanged(lv_event_t *e)
{
  int16_t volume = lv_slider_get_value(ui_ScaleVolume);
  // int16_t volume = lv_arc_get_value(ui_ScaleVolume);
  audio.setVolume(volume);
}

void timeProgressChanged(lv_event_t *e)
{
  int16_t selectedTime = lv_slider_get_value(ui_ScaleProgress);
  audio.setAudioPlayPosition(selectedTime);
}

void playListChanged(lv_event_t *e)
{
  isSelectedSongChanged = true;
}

// pixel drawing callback
static int jpegDrawCallback(JPEGDRAW *pDraw)
{
  // Serial.printf("Draw pos = %d,%d. size = %d x %d\n", pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  gfx_draw_bitmap_to_framebuffer(
      pDraw->pPixels, pDraw->iWidth, pDraw->iHeight,
      (uint16_t *)coverImgBitmap, pDraw->x, pDraw->y, coverImgBitmapW, coverImgBitmapH);

  return 1;
}

void read_song_list()
{
  String stringSongList = "";

  File root = SD_MMC.open("/");
  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.printf("DIR: %s\n", file.name());
    }
    else
    {
      const char *filename = file.name();

      int8_t len = strlen(filename);
      const char *MP3_EXT = ".mp3";
      if ((filename[0] != '.') && (strcmp(MP3_EXT, &filename[len - 4]) == 0))
      {
        // Serial.printf("Song file: %s, size: %d\n", filename, file.size());
        if (song_count > 0)
        {
          stringSongList += '\n';
        }
        stringSongList += filename;
        song_count++;
      }
    }
    file = root.openNextFile();
  }
  lv_roller_set_options(ui_RollerPlayList, stringSongList.c_str(), LV_ROLLER_MODE_INFINITE);

  force_refresh_screen();
}

void cleanup_value()
{
  isPlaying = false;
  currentSongDuration = 0;
  currentTimeProgress = 0;
  syncTimeLyricsCount = 0;
  playingStr = "";
  syncTimeLyricsCount = 0;
  init_peak_array();
  canvasFFT_gfx->fillScreen(BLACK);

  lv_obj_add_flag(ui_ImageCover, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(ui_RollerLyrics, LV_OBJ_FLAG_HIDDEN);
}

void play_selected_song()
{
  cleanup_value();
  char song_filename[256];
  lv_roller_get_selected_str(ui_RollerPlayList, song_filename, sizeof(song_filename));
  Serial.printf("Play: %s\n", song_filename);
  audio.connecttoFS(SD_MMC, song_filename);
  isPlaying = true;
}

void prevSong(lv_event_t *e)
{
  int16_t selected_id = lv_roller_get_selected(ui_RollerPlayList);
  selected_id--;
  if (selected_id < 0)
  {
    selected_id = song_count - 1;
  }
  lv_roller_set_selected(ui_RollerPlayList, selected_id, LV_ANIM_ON);
  play_selected_song();
}

void playSong(lv_event_t *e)
{
  if (isPlaying)
  {
    audio.pauseResume();
  }
  else
  {
    play_selected_song();
  }
}

void pauseSong(lv_event_t *e)
{
  audio.pauseResume();
}

void stopSong(lv_event_t *e)
{
  audio.stopSong();
  cleanup_value();
}

void nextSong(lv_event_t *e)
{
  uint16_t selected_id = lv_roller_get_selected(ui_RollerPlayList);
  selected_id++;
  if (selected_id >= song_count)
  {
    selected_id = 0;
  }
  lv_roller_set_selected(ui_RollerPlayList, selected_id, LV_ANIM_ON);
  play_selected_song();
}

void setup()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("LVGL Music Player");

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

  // Init touch device
  touch_init(gfx->width(), gfx->height(), gfx->getRotation());

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

    /* Initialize the input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    /* Init SquareLine prepared UI */
    ui_init();

    lv_obj_set_style_anim_speed(ui_LabelPlaying, 5, LV_STATE_DEFAULT);
    lv_obj_clear_flag(ui_RollerPlayList, LV_OBJ_FLAG_SCROLL_MOMENTUM);
    lv_obj_set_style_anim_time(ui_RollerPlayList, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_anim_time(ui_RollerLyrics, 0, LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_ScaleVolume, volumeChanged, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_ScaleProgress, timeProgressChanged, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_ButtonPrev, prevSong, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_ButtonPlay, playSong, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_ButtonPause, pauseSong, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_ButtonStop, stopSong, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_ButtonNext, nextSong, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_RollerPlayList, playListChanged, LV_EVENT_VALUE_CHANGED, NULL);

    canvasFFT_gfx->begin();
    ui_CanvasFFT = lv_canvas_create(ui_Screen1);
    lv_canvas_set_buffer(ui_CanvasFFT, (lv_color_t *)canvasFFT_gfx->getFramebuffer(), CANVAS_FFT_WIDTH, CANVAS_FFT_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    lv_obj_set_width(ui_CanvasFFT, CANVAS_FFT_WIDTH);
    lv_obj_set_height(ui_CanvasFFT, CANVAS_FFT_HEIGHT);
    lv_obj_set_x(ui_CanvasFFT, -339);
    lv_obj_set_y(ui_CanvasFFT, -190);
    lv_obj_set_align(ui_CanvasFFT, LV_ALIGN_CENTER);

    Serial.println("Setup done");
  }

  pinMode(SD_CS /* CS */, OUTPUT);
  digitalWrite(SD_CS /* CS */, HIGH);
  SD_MMC.setPins(SD_SCK /* CLK */, SD_MOSI /* CMD/MOSI */, SD_MISO /* D0/MISO */);
  if (!SD_MMC.begin("/root", true /* mode1bit */, false /* format_if_mount_failed */, SDMMC_FREQ_DEFAULT))
  {
    Serial.println(F("ERROR: SD_MMC Mount Failed!"));
  }
  else
  {
    audio.setPinout(I2S_BCLK, I2S_LRCK, I2S_DOUT);
    volumeChanged(nullptr);

    read_song_list();
    play_selected_song();

    xTaskCreatePinnedToCore(Task_Audio, "Task_Audio", 10240, NULL, 3, NULL, 0);
  }
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(5);
  if (isSelectedSongChanged)
  {
    isSelectedSongChanged = false;
    play_selected_song();
  }
}

void Task_Audio(void *pvParameters) // This is a task.
{
  while (true)
  {
    if (isPlaying)
    {
      audio.loop();
      if (currentSongDuration == 0)
      {
        currentSongDuration = audio.getAudioFileDuration();
        if (currentSongDuration > 0)
        {
          // Serial.printf("currentSongDuration: %d\n", currentSongDuration);
          lv_slider_set_range(ui_ScaleProgress, 0, currentSongDuration);
          // sprintf(textBuf, "%02d:%02d", currentSongDuration / 60, currentSongDuration % 60);
          // lv_label_set_text(ui_LabelDuration, textBuf);
        }
      }
      uint32_t currentTime = audio.getAudioCurrentTime();
      if (currentTime != currentTimeProgress)
      {
        currentTimeProgress = currentTime;
        // Serial.printf("currentTime: %d\n", currentTime);
        lv_slider_set_value(ui_ScaleProgress, currentTimeProgress, LV_ANIM_ON);
        sprintf(textBuf, "%02d:%02d", currentTimeProgress / 60, currentTimeProgress % 60);
        lv_label_set_text(ui_LabelProgress, textBuf);
        for (int i = 0; i < syncTimeLyricsCount; ++i)
        {
          if (syncTimeLyricsSec[i] == currentTime)
          {
            lv_roller_set_selected(ui_RollerLyrics, syncTimeLyricsLineIdx[i], LV_ANIM_ON);
            break;
          }
        }

        uint32_t kbps = audio.getBitRate(true);
        sprintf(textBuf, "%d", kbps / 1000);
        lv_label_set_text(ui_LabelKbps, textBuf);

        uint32_t khz = audio.getSampleRate();
        sprintf(textBuf, "%d", khz / 1000);
        lv_label_set_text(ui_LabelKHz, textBuf);
      }
    }
    delay(1);
  }
}

void audio_id3data(const char *info)
{ // id3 metadata
  Serial.print("id3data     ");
  Serial.println(info);
  if (playingStr.length() > 0)
  {
    playingStr += " ";
  }
  playingStr += info;
  lv_label_set_text(ui_LabelPlaying, playingStr.c_str());
}
void audio_id3image(File &file, const size_t pos, const size_t len)
{
  Serial.printf("audio_id3image, pos: %d, size: %d\n", pos, len);
  if (coverImgFileSize == 0)
  {
    coverImgFile = (uint8_t *)malloc(len);
    coverImgFileSize = len;
  }
  else if (len > coverImgFileSize)
  {
    coverImgFile = (uint8_t *)realloc(coverImgFile, len);
    coverImgFileSize = len;
  }
  if (coverImgFile)
  {
    file.seek(pos);
    file.read(coverImgFile, len);
    Serial.printf("%c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c\n", coverImgFile[0], coverImgFile[1], coverImgFile[2], coverImgFile[3], coverImgFile[4], coverImgFile[5], coverImgFile[6], coverImgFile[7], coverImgFile[8], coverImgFile[9], coverImgFile[10], coverImgFile[11], coverImgFile[12], coverImgFile[13], coverImgFile[14], coverImgFile[15], coverImgFile[16], coverImgFile[17], coverImgFile[18], coverImgFile[19], coverImgFile[20], coverImgFile[21], coverImgFile[22], coverImgFile[23], coverImgFile[24], coverImgFile[25], coverImgFile[26], coverImgFile[27], coverImgFile[28], coverImgFile[29], coverImgFile[30], coverImgFile[31]);
    Serial.printf("%02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X\n", coverImgFile[0], coverImgFile[1], coverImgFile[2], coverImgFile[3], coverImgFile[4], coverImgFile[5], coverImgFile[6], coverImgFile[7], coverImgFile[8], coverImgFile[9], coverImgFile[10], coverImgFile[11], coverImgFile[12], coverImgFile[13], coverImgFile[14], coverImgFile[15], coverImgFile[16], coverImgFile[17], coverImgFile[18], coverImgFile[19], coverImgFile[20], coverImgFile[21], coverImgFile[22], coverImgFile[23], coverImgFile[24], coverImgFile[25], coverImgFile[26], coverImgFile[27], coverImgFile[28], coverImgFile[29], coverImgFile[30], coverImgFile[31]);

    size_t idx = 11;
    // seek JPEG header
    while ((idx < len) && ((coverImgFile[idx++] != 0xFF) || (coverImgFile[idx] != 0xD8)))
      ;
    --idx;
    Serial.printf("idx: %d\n", idx);
    jpegdec.openRAM(coverImgFile + idx, len - idx, jpegDrawCallback);

    int scale = 0;
    coverImgBitmapW = jpegdec.getWidth();
    coverImgBitmapH = jpegdec.getHeight();

    if ((coverImgBitmapW >= (MP3_COVER_IMG_W * 8)) || (coverImgBitmapH >= (MP3_COVER_IMG_H * 8)))
    {
      scale = JPEG_SCALE_EIGHTH;
      coverImgBitmapW >>= 3;
      coverImgBitmapH >>= 3;
    }
    else if ((coverImgBitmapW >= (MP3_COVER_IMG_W * 4)) || (coverImgBitmapH >= (MP3_COVER_IMG_H * 4)))
    {
      scale = JPEG_SCALE_QUARTER;
      coverImgBitmapW >>= 2;
      coverImgBitmapH >>= 2;
    }
    else if ((coverImgBitmapW >= (MP3_COVER_IMG_W * 2)) || (coverImgBitmapH >= (MP3_COVER_IMG_H * 2)))
    {
      scale = JPEG_SCALE_HALF;
      coverImgBitmapW >>= 1;
      coverImgBitmapH >>= 1;
    }
    Serial.printf("coverImgBitmapW: %d, coverImgBitmapH: %d\n", coverImgBitmapW, coverImgBitmapH);

    if (
        (coverImgBitmapW > 0) && (coverImgBitmapH > 0))
    {
      size_t imgBitmapSize = coverImgBitmapW * coverImgBitmapH * 2;
      if (coverImgBitmapSize == 0)
      {
        coverImgBitmap = (uint8_t *)malloc(imgBitmapSize);
        coverImgBitmapSize = imgBitmapSize;
      }
      else if (coverImgBitmapSize < imgBitmapSize)
      {
        coverImgBitmap = (uint8_t *)realloc(coverImgBitmap, imgBitmapSize);
        coverImgBitmapSize = imgBitmapSize;
      }
      if (coverImgBitmap)
      {
#if (LV_COLOR_16_SWAP != 0)
        jpegdec.setPixelType(RGB565_BIG_ENDIAN);
#endif
        jpegdec.decode(0, 0, scale);

        img_cover.header.cf = LV_IMG_CF_TRUE_COLOR;
        img_cover.header.w = coverImgBitmapW;
        img_cover.header.h = coverImgBitmapH;
        img_cover.header.always_zero = 0;
        img_cover.data_size = imgBitmapSize;
        img_cover.data = coverImgBitmap;

        uint16_t zW = MP3_COVER_IMG_W * 256 / coverImgBitmapW;
        uint16_t zH = MP3_COVER_IMG_H * 256 / coverImgBitmapH;
        Serial.printf("zW: %d, zH: %d\n", zW, zH);
        lv_img_set_zoom(ui_ImageCover, (zW < zH) ? zW : zH);
        lv_img_set_src(ui_ImageCover, &img_cover);
        lv_obj_clear_flag(ui_ImageCover, LV_OBJ_FLAG_HIDDEN);

        force_refresh_screen();
      }
      jpegdec.close();
    }
  }
}
void audio_id3lyrics(File &file, const size_t pos, const size_t len)
{
  Serial.printf("audio_id3lyrics, pos: %d, size: %d\n", pos, len);
  size_t len2 = len << 1;
  if (lyricsTextSize == 0)
  {
    lyricsText = (char *)malloc(len2);
    lyricsTextSize = len2;
  }
  else if (len2 > lyricsTextSize)
  {
    lyricsText = (char *)realloc(lyricsText, len2);
    lyricsTextSize = len2;
  }
  if (lyricsText)
  {
    file.seek(pos);
    file.read((uint8_t *)lyricsText, len);
    audio.unicode2utf8(lyricsText, len);
    // Serial.println(lyricsText);

    size_t idxA = 0;
    size_t idxB = 0;
    size_t lastNewLineIdxB = 0;
    uint8_t lyricsLines = 0;
    bool isSecTag;
    uint8_t currentMinute;
    uint16_t currentSec;
    bool seenColon;
    bool seenDecimal;
    while (
        (idxA < len2) && (lyricsText[idxA]))
    {
      if (lyricsText[idxA] == '[')
      {
        char c = lyricsText[++idxA];
        if ((c >= '0') && (c <= '9'))
        {
          isSecTag = true;
        }
        else
        {
          isSecTag = false;
        }
        currentMinute = 0;
        currentSec = 0;
        seenColon = false;
        seenDecimal = false;
        while (lyricsText[idxA] != ']')
        {
          c = lyricsText[idxA++];
          if (isSecTag)
          {
            if ((c >= '0') && (c <= '9'))
            {
              if (!seenColon)
              {
                currentMinute = (currentMinute * 10) + (c - '0');
              }
              else if (!seenDecimal)
              {
                currentSec = (currentSec * 10) + (c - '0');
              }
            }
            else if (c == ':')
            {
              seenColon = true;
            }
            else if (c == '.')
            {
              seenDecimal = true;
            }
          }
        }
        currentSec += currentMinute * 60;
        if (currentSec > 0)
        {
          // Serial.printf("currentSec: %d, lyricsLines: %d\n", currentSec, lyricsLines);
          syncTimeLyricsSec[syncTimeLyricsCount] = currentSec;
          syncTimeLyricsLineIdx[syncTimeLyricsCount] = lyricsLines;
          ++syncTimeLyricsCount;
        }
        ++idxA;
      }
      else
      {
        if (lyricsText[idxA] != '\r')
        {
          if (lyricsText[idxA] == '\n')
          {
            if ((idxB - lastNewLineIdxB) > 1)
            {
              lastNewLineIdxB = idxB;
              lyricsText[idxB++] = lyricsText[idxA];
              ++lyricsLines;
            }
            else
            {
              if (isSecTag)
              {
                --syncTimeLyricsCount; // risky
              }
            }
          }
          else
          {
            lyricsText[idxB++] = lyricsText[idxA];
          }
        }
        ++idxA;
      }
    }
    if (lyricsText[idxB - 1] == '\n')
    {
      lyricsText[idxB - 1] = 0;
    }
    else
    {
      lyricsText[idxB] = 0;
      ++lyricsLines;
    }

    lv_roller_set_options(ui_RollerLyrics, lyricsText, LV_ROLLER_MODE_NORMAL);
    lv_obj_clear_flag(ui_RollerLyrics, LV_OBJ_FLAG_HIDDEN);

    force_refresh_screen();
  }
}
void audio_eof_mp3(const char *info)
{ // end of file
  Serial.print("eof_mp3     ");
  Serial.println(info);

  nextSong(nullptr);
}
void audio_process_i2s(uint32_t *sample, bool *continueI2S)
{
  raw_data[raw_data_idx++] = *sample;
  if (raw_data_idx >= WAVE_SIZE)
  {
    fft.exec((int16_t *)raw_data);
    draw_fft_level_meter(canvasFFT_gfx);
    lv_obj_invalidate(ui_CanvasFFT);
    raw_data_idx = 0;
  }
  *continueI2S = true;
}
