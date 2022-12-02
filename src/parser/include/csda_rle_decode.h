#ifndef CSDA_RLE_DECODE_H
#define CSDA_RLE_DECODE_H

#include <stdint.h>
#include <vector>
#include <opencv2/opencv.hpp>

namespace cav
{
  struct ConnectRegion;
  struct RLE
  {
    int y;
    int xstart, xend;
    int16_t length;
    int16_t value;
    ConnectRegion *region;
  };

  struct RLEList
  {
    // RLE *rle;   // abandon
    RLE irle;
    RLEList *next;      // pointer to next RLEList in region
    RLEList *line_next; // pointer to next RLEList in current line
                        // helps to construct region
  };

  struct RLELine
  {
    RLEList *head;
    RLEList *tail;
    RLELine() : head(nullptr), tail(nullptr)
    {
    }
  };

  struct ConnectRegion
  {
    cv::Rect boundbox;
    int nArea;

    RLEList *RLEhead;
    RLEList *RLEtail;

    RLELine *linePtr;

    ConnectRegion *pre;
    ConnectRegion *next;

    ConnectRegion() : boundbox(0, 0, 0, 0), nArea(0),
                      RLEhead(nullptr), RLEtail(nullptr),
                      linePtr(nullptr),
                      pre(nullptr), next(nullptr)
    {
    }
  };

  /**
   * @brief RLE解析器，用于将用于将RLE编码数据进行解析，生成unsigned char存储
   */
  class CRLECompressedImage
  {
  public:
    CRLECompressedImage(int valid_height = INT_MAX,
                        uint8_t free_label = 0)
    {
      data_ = NULL;
      inited_ = false;
      size_ = 0;
      max_size_ = 0;
      valid_height_ = valid_height;
      free_label_ = free_label;
      rle_list_buf_.resize(5000); // reserve buffer
      rle_list_num_ = 0;
      width_ = 0;
      height_ = 0;
      region_check_rows_ = 0;
      region_abnormal_thres_ = INT32_MAX;
      region_check_en_ = 0;
    }
    ~CRLECompressedImage();

    enum DecompressStatus
    {
      Decompress_OK = 0,
      Decompress_Failed = -1,
      Decompress_BuffTooSmall = -2
    };

    static int Decompress(const uint8_t *input_data,
                          int size, uint8_t *output_data, int &buf_len,
                          uint16_t align = 32);

    bool inited_;   // if RLEList inited
    int size_;      // RLE Serialize Size
    uint8_t *data_; // RLE Serialize buf
    int max_size_;  // RLE Serialize buf max size

    int valid_height_; // new : the max value of valid line
    uint8_t free_label_;
    int width_;
    int height_;
    std::vector<RLEList> rle_list_buf_;
    size_t rle_list_num_;
    static std::vector<std::vector<int>> valid_height_lut_;
    int region_abnormal_thres_;
    int region_check_rows_;
    int region_check_en_;
  };

}
#endif // CSDA_RLE_DECODE_H