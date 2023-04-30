/*
 * Extract from:
 * https://github.com/m5stack/M5Unified/blob/master/examples/Advanced/MP3_with_ESP8266Audio/MP3_with_ESP8266Audio.ino
 */
// #define FFT_SIZE 256
#define FFT_SIZE 128
class fft_t
{
  float _wr[FFT_SIZE + 1];
  float _wi[FFT_SIZE + 1];
  float _fr[FFT_SIZE + 1];
  float _fi[FFT_SIZE + 1];
  uint16_t _br[FFT_SIZE + 1];
  size_t _ie;

public:
  fft_t(void)
  {
#ifndef M_PI
#define M_PI 3.141592653
#endif
    _ie = logf((float)FFT_SIZE) / log(2.0) + 0.5;
    static constexpr float omega = 2.0f * M_PI / FFT_SIZE;
    static constexpr int s4 = FFT_SIZE / 4;
    static constexpr int s2 = FFT_SIZE / 2;
    for (int i = 1; i < s4; ++i)
    {
      float f = cosf(omega * i);
      _wi[s4 + i] = f;
      _wi[s4 - i] = f;
      _wr[i] = f;
      _wr[s2 - i] = -f;
    }
    _wi[s4] = _wr[0] = 1;

    size_t je = 1;
    _br[0] = 0;
    _br[1] = FFT_SIZE / 2;
    for (size_t i = 0; i < _ie - 1; ++i)
    {
      _br[je << 1] = _br[je] >> 1;
      je = je << 1;
      for (size_t j = 1; j < je; ++j)
      {
        _br[je + j] = _br[je] + _br[j];
      }
    }
  }

  void exec(const int16_t *in)
  {
    memset(_fi, 0, sizeof(_fi));
    for (size_t j = 0; j < FFT_SIZE / 2; ++j)
    {
      float basej = 0.25 * (1.0 - _wr[j]);
      size_t r = FFT_SIZE - j - 1;

      /// perform han window and stereo to mono convert.
      _fr[_br[j]] = basej * (in[j * 2] + in[j * 2 + 1]);
      _fr[_br[r]] = basej * (in[r * 2] + in[r * 2 + 1]);
    }

    size_t s = 1;
    size_t i = 0;
    do
    {
      size_t ke = s;
      s <<= 1;
      size_t je = FFT_SIZE / s;
      size_t j = 0;
      do
      {
        size_t k = 0;
        do
        {
          size_t l = s * j + k;
          size_t m = ke * (2 * j + 1) + k;
          size_t p = je * k;
          float Wxmr = _fr[m] * _wr[p] + _fi[m] * _wi[p];
          float Wxmi = _fi[m] * _wr[p] - _fr[m] * _wi[p];
          _fr[m] = _fr[l] - Wxmr;
          _fi[m] = _fi[l] - Wxmi;
          _fr[l] += Wxmr;
          _fi[l] += Wxmi;
        } while (++k < ke);
      } while (++j < je);
    } while (++i < _ie);
  }

  uint32_t get(size_t index)
  {
    return (index < FFT_SIZE / 2) ? (uint32_t)sqrtf(_fr[index] * _fr[index] + _fi[index] * _fi[index]) : 0u;
  }
};

static constexpr size_t WAVE_SIZE = (44100 / 30);
static fft_t fft;
static uint16_t prev_h[(FFT_SIZE / 2) + 1];
static uint16_t peak_y[(FFT_SIZE / 2) + 1];
static uint32_t raw_data[WAVE_SIZE];
static size_t raw_data_idx = 0;

#if (LV_COLOR_16_SWAP != 0)
static uint16_t level_color[] = {
    MSB_16(RGB565(0xcd, 0x19, 0x22)),
    MSB_16(RGB565(0xcd, 0x22, 0x10)),
    MSB_16(RGB565(0xcd, 0x2a, 0x10)),
    MSB_16(RGB565(0xd6, 0x5a, 0x00)),
    MSB_16(RGB565(0xd6, 0x66, 0x01)),
    MSB_16(RGB565(0xd6, 0x73, 0x01)),
    MSB_16(RGB565(0xc6, 0x7b, 0x09)),
    MSB_16(RGB565(0xde, 0xa5, 0x19)),
    MSB_16(RGB565(0xd6, 0xb5, 0x22)),
    MSB_16(RGB565(0xbc, 0xde, 0x2a)),
    MSB_16(RGB565(0x93, 0xde, 0x22)),
    MSB_16(RGB565(0x29, 0xce, 0x10)),
    MSB_16(RGB565(0x32, 0xbe, 0x10)),
    MSB_16(RGB565(0x37, 0xb6, 0x0e)),
    MSB_16(RGB565(0x31, 0x9c, 0x0a)),
    MSB_16(RGB565(0x28, 0x94, 0x01))};
#else
static uint16_t level_color[] = {
    RGB565(0xcd, 0x19, 0x22),
    RGB565(0xcd, 0x22, 0x10),
    RGB565(0xcd, 0x2a, 0x10),
    RGB565(0xd6, 0x5a, 0x00),
    RGB565(0xd6, 0x66, 0x01),
    RGB565(0xd6, 0x73, 0x01),
    RGB565(0xc6, 0x7b, 0x09),
    RGB565(0xde, 0xa5, 0x19),
    RGB565(0xd6, 0xb5, 0x22),
    RGB565(0xbc, 0xde, 0x2a),
    RGB565(0x93, 0xde, 0x22),
    RGB565(0x29, 0xce, 0x10),
    RGB565(0x32, 0xbe, 0x10),
    RGB565(0x37, 0xb6, 0x0e),
    RGB565(0x31, 0x9c, 0x0a),
    RGB565(0x28, 0x94, 0x01)};
#endif

static void init_peak_array()
{
  int i = (FFT_SIZE / 2) + 1;
  while (i--)
  {
    peak_y[i] = INT16_MAX - 1;
  }
}

static void draw_fft_level_meter(Arduino_GFX *meter_gfx)
{
  // size_t bw = meter_gfx->width() / 60;
  size_t bw = meter_gfx->width() / 19;
  if (bw < 3)
  {
    bw = 3;
  }
  int32_t fft_height = 17;
  int32_t fft_heightc = fft_height >> 2;
  size_t xe = meter_gfx->width() / bw;
  if (xe > (FFT_SIZE / 2))
  {
    xe = (FFT_SIZE / 2);
  }

  for (size_t bx = 0; bx <= xe; ++bx)
  {
    size_t x = bx * bw;
    int32_t f = fft.get(bx);
    // int32_t h = (f * fft_height) >> 18;
    int32_t h = (f * fft_height) >> 13;
    if (h >= fft_height)
    {
      h = fft_height - 1;
    }
    int32_t y = fft_height - h + 1;
    int32_t ph = prev_h[bx];
    if (h != ph)
    {
      prev_h[bx] = h;
      if (h < ph)
      {
        meter_gfx->fillRect(x, y - (ph - h), bw - 1, ph - h, BLACK);
      }
      while (y < fft_height)
      {
        meter_gfx->writeFastHLine(x, y, bw - 1, level_color[y - 1]);
        y++;
      }
    }
    int32_t py = peak_y[bx] + 1;
    if (py < y)
    {
      meter_gfx->writeFastHLine(x, py - 1, bw - 1, BLACK);
    }
    else
    {
      py = y - 1;
    }
    if (peak_y[bx] != py)
    {
      peak_y[bx] = py;
      meter_gfx->writeFastHLine(x, py, bw - 1, RGB565(0x96, 0x96, 0x96));
    }
  }
}
