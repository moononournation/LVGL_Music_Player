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
// ZX3D50CE02S/WT32-SC01 PLUS
// Cover size
#define MP3_COVER_IMG_W 178
#define MP3_COVER_IMG_H 100
// TOUCH
#define TOUCH_MODULES_FT5x06 // GT911 / CST_SELF / CST_MUTUAL / ZTW622 / L58 / FT3267 / FT5x06
#define TOUCH_SCL 5
#define TOUCH_SDA 6
#define TOUCH_RES -1
#define TOUCH_ADD 0x38
// microSD card
#define SD_SCK 39
#define SD_MISO 38
#define SD_MOSI 40
#define SD_CS 41
// I2S
#define I2S_DOUT 37
#define I2S_BCLK 36
#define I2S_LRCK 35

#include <lvgl.h>
#include "ui.h"

#include <JPEGDEC.h>
JPEGDEC jpegdec;

#include <Audio.h>
Audio audio;

#include <SD_MMC.h>

#include <TouchLib.h>
TouchLib touch(Wire, TOUCH_SDA, TOUCH_SCL, TOUCH_ADD);

/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */

// ZX3D50CE02S/WT32-SC01 PLUS
#define GFX_BL 45
Arduino_DataBus *bus = new Arduino_ESP32LCD8(
    0 /* DC */, GFX_NOT_DEFINED /* CS */, 47 /* WR */, GFX_NOT_DEFINED /* RD */,
    9 /* D0 */, 46 /* D1 */, 3 /* D2 */, 8 /* D3 */, 18 /* D4 */, 17 /* D5 */, 16 /* D6 */, 15 /* D7 */);
Arduino_GFX *gfx = new Arduino_ST7796(bus, 4 /* RST */, 1 /* rotation */, true /* IPS */);
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

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

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  if (touch.read())
  {
    data->state = LV_INDEV_STATE_PR;

    TP_Point t = touch.getPoint(0);
    /*Set the coordinates*/
    data->point.x = t.y;
    data->point.y = 320 - t.x;
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
      if (
          (filename[0] != '.') && (strcmp(MP3_EXT, &filename[len - 4]) == 0))
      {
        Serial.printf("Song file: %s, size: %d\n", filename, file.size());
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
}

void play_selected_song()
{
  isPlaying = false;
  char song_filename[256];
  lv_roller_get_selected_str(ui_RollerPlayList, song_filename, sizeof(song_filename));
  Serial.printf("Play: %s\n", song_filename);
  audio.connecttoFS(SD_MMC, song_filename);
  currentSongDuration = 0;
  currentTimeProgress = 0;
  syncTimeLyricsCount = 0;
  isPlaying = true;
  lv_img_set_src(ui_ImageCover, nullptr);
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

  // init touch
#if (TOUCH_RES > 0)
  pinMode(TOUCH_RES, OUTPUT);
  digitalWrite(TOUCH_RES, 0);
  delay(200);
  digitalWrite(TOUCH_RES, 1);
  delay(200);
#endif
  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  touch.init();

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

    lv_obj_add_event_cb(ui_ScaleVolume, volumeChanged, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_ScaleProgress, timeProgressChanged, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_set_style_text_font(ui_RollerLyrics, &ui_font_NotoSansCJKhk, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui_RollerLyrics, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_anim_time(ui_RollerLyrics, 2000, LV_STATE_DEFAULT);

    lv_obj_set_style_text_font(ui_RollerPlayList, &ui_font_NotoSansCJKhk, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui_RollerPlayList, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_event_cb(ui_RollerPlayList, playListChanged, LV_EVENT_VALUE_CHANGED, NULL);

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

    read_song_list();
    play_selected_song();

    xTaskCreatePinnedToCore(Task_Audio, "Task_Audio", 10240, NULL, 3, NULL, 0);
  }
}

void loop()
{
  lv_timer_handler(); /* let the GUI do its work */
  delay(1);
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
          sprintf(textBuf, "%02d:%02d", currentSongDuration / 60, currentSongDuration % 60);
          lv_label_set_text(ui_LabelDuration, textBuf);
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
          }
        }
      }
    }
    delay(1);
  }
}

void audio_id3data(const char *info)
{ // id3 metadata
  Serial.print("id3data     ");
  Serial.println(info);
}
void audio_id3image(File &file, const size_t pos, const size_t size)
{
  Serial.printf("audio_id3image, pos: %d, size: %d\n", pos, size);
  if (coverImgFileSize == 0)
  {
    coverImgFile = (uint8_t *)malloc(size);
    coverImgFileSize = size;
  }
  else if (size > coverImgFileSize)
  {
    coverImgFile = (uint8_t *)realloc(coverImgFile, size);
    coverImgFileSize = size;
  }
  if (coverImgFile)
  {
    file.seek(pos);
    file.read(coverImgFile, size);
    Serial.printf("%c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c, %c\n", coverImgFile[0], coverImgFile[1], coverImgFile[2], coverImgFile[3], coverImgFile[4], coverImgFile[5], coverImgFile[6], coverImgFile[7], coverImgFile[8], coverImgFile[9], coverImgFile[10], coverImgFile[11], coverImgFile[12], coverImgFile[13], coverImgFile[14], coverImgFile[15]);
    Serial.printf("%02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X, %02X\n", coverImgFile[0], coverImgFile[1], coverImgFile[2], coverImgFile[3], coverImgFile[4], coverImgFile[5], coverImgFile[6], coverImgFile[7], coverImgFile[8], coverImgFile[9], coverImgFile[10], coverImgFile[11], coverImgFile[12], coverImgFile[13], coverImgFile[14], coverImgFile[15]);

    jpegdec.openRAM(coverImgFile + 14, size - 14, jpegDrawCallback);

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
      }
      jpegdec.close();
    }
  }
}
void audio_id3lyrics(File &file, const size_t pos, const size_t size)
{
  Serial.printf("audio_id3lyrics, pos: %d, size: %d\n", pos, size);
  if (lyricsTextSize == 0)
  {
    lyricsText = (char *)malloc(size);
    lyricsTextSize = size;
  }
  else if (size > lyricsTextSize)
  {
    lyricsText = (char *)realloc(coverImgFile, size);
    lyricsTextSize = size;
  }
  if (lyricsText)
  {
    file.seek(pos);
    file.read((uint8_t *)lyricsText, size);
    audio.unicode2utf8(lyricsText, size);
    Serial.println(lyricsText);

    size_t idxA = 0;
    size_t idxB = 0;
    size_t lastNewLineIdxB = 0;
    uint8_t lyricsLines = 0;
    bool isSecTag;
    uint8_t currentMinute;
    uint16_t currentSec;
    bool seenColon;
    bool seenDecimal;
    while (idxA < size)
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
          Serial.printf("currentSec: %d, lyricsLines: %d\n", currentSec, lyricsLines);
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
  }
}
void audio_eof_mp3(const char *info)
{ // end of file
  Serial.print("eof_mp3     ");
  Serial.println(info);

  uint16_t selected_id = lv_roller_get_selected(ui_RollerPlayList);
  selected_id++;
  if (selected_id >= song_count)
  {
    selected_id = 0;
  }
  lv_roller_set_selected(ui_RollerPlayList, selected_id, LV_ANIM_ON);
  play_selected_song();
}
