/**
 *Copyright 2022 CA.
 */

#ifndef CAPILOT_IMAGE_FRAME_H
#define CAPILOT_IMAGE_FRAME_H

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>

namespace cav {
class CapilotImageFrame {
 public:
  CapilotImageFrame() {
    data_ = std::make_shared<std::vector<uint8_t> >();
    data_->resize(sizeof(int32_t) + sizeof(int64_t));
  }

  /**
   * @brief
   * @param[img] 图像起始地址
   * @param[len] 图像大小
   * @return
   */
  CapilotImageFrame(const void *img, size_t len) {
    data_ = std::make_shared<std::vector<uint8_t> >();
    data_->resize(sizeof(int32_t) + sizeof(int64_t) + len);
    ::memcpy(image_data(), img, len);
  }

  /**
   * @brief
   * @param[frame_id] 图像帧ID
   * @param[timestamp] 图像时间戳
   * @param[img] 图像起始地址
   * @param[len] 图像大小
   * @return
   */
  CapilotImageFrame(int32_t frame_id, int64_t timestamp, const void *img,
                    size_t len) {
    data_ = std::make_shared<std::vector<uint8_t> >();
    data_->resize(sizeof(int32_t) + sizeof(int64_t) + len);

    int32_t *id = frame_id_addr();
    *id = frame_id;
    int64_t *ts = timestamp_addr();
    *ts = timestamp;
    ::memcpy(image_data(), img, len);
  }

  /**
   * @brief 获取图像帧ID
   * @param
   * @return 图像帧ID
   */
  int32_t frame_id() { return *frame_id_addr(); }

  /**
   * @brief 设置图像帧ID
   * @param[frame_id] 图像帧ID
   * @return
   */
  void set_frame_id(int32_t frame_id) {
    int32_t *id = frame_id_addr();
    *id = frame_id;
  }

  /**
   * @brief 获取图像帧时间戳
   * @param
   * @return 图像帧时间戳
   */
  int64_t timestamp() { return *timestamp_addr(); }

  /**
   * @brief 设置图像帧时间戳
   * @param[timestamp] 图像帧时间戳
   * @return
   */
  void set_timestamp(int64_t timestamp) {
    int64_t *ts = timestamp_addr();
    *ts = timestamp;
  }

  /**
   * @brief 获取图像起始地址
   * @param
   * @return
   */
  void *image_data() {
    int64_t *ts_addr = timestamp_addr();
    return reinterpret_cast<void *>(ts_addr + 1);
  }

  /**
   * @brief 获取图像大小
   * @param
   * @return
   */
  size_t image_size() {
    return data_->size() - (sizeof(int32_t) + sizeof(int64_t));
  }

  /**
   * @brief 设置图像
   * @param[img] 图像起始地址
   * @param[len] 图像大小
   * @return
   */
  void set_image(const void *img, size_t len) {
    if (image_size() < len) {
      data_->resize(sizeof(int32_t) + sizeof(int64_t) + len);
    }
    ::memcpy(image_data(), img, len);
  }

  /**
   * @brief 序列化帧
   * @param
   * @return
   */
  bool encode_frame() {
    // data_中数据已经序列化
    return true;
  }

  /**
   * @brief 序列化帧
   * @param [addr] 序列化数据起始地址
   * @param [len] 序列化数据大小
   * @return
   */
  bool encode_frame(void *&addr, size_t &len) {
    addr = data_->data();
    len = data_->size();
    return true;
  }

  /**
   * @brief 反序列化帧
   * @param [addr] 待反序列化数据起始地址
   * @param [len] 待反序列化数据大小
   * @return 反序列化失败false， 成功true
   */
  bool decode_frame(void *addr, size_t len) {
    if (len < sizeof(int32_t) + sizeof(int64_t)) {
      return false;
    }

    len -= sizeof(int32_t);
    len -= sizeof(int64_t);
    if (image_size() < len) {
      data_->resize(sizeof(int32_t) + sizeof(int64_t) + len);
    }

    uint8_t *cur = (uint8_t *)addr;
    int32_t *id = frame_id_addr();
    *id = *((int32_t *)cur);
    cur += sizeof(int32_t);

    int64_t *ts = timestamp_addr();
    *ts = *((int64_t *)cur);
    cur += sizeof(int64_t);

    if (len > 0) {
      ::memcpy(image_data(), cur, len);
    }
    return true;
  }

  /**
   * @brief 获取序列化后的数据起始地址
   * @param
   * @return
   */
  void *frame_data() { return reinterpret_cast<void *>(data_->data()); }

  /**
   * @brief 获取序列化后的数据大小
   * @param
   * @return
   */
  size_t frame_size() { return data_->size(); }

 private:
  int32_t *frame_id_addr() {
    return reinterpret_cast<int32_t *>(data_->data());
  }

  int64_t *timestamp_addr() {
    return reinterpret_cast<int64_t *>(data_->data() + sizeof(int32_t));
  }

  std::shared_ptr<std::vector<uint8_t> > data_;
};

class CapilotParsingFrame {
 public:
  CapilotParsingFrame() {
    data_ = std::make_shared<std::vector<uint8_t> >();
    data_->resize(sizeof(int32_t) + sizeof(int64_t));
  }

  /**
   * @brief 获取图像帧ID
   * @param
   * @return 图像帧ID
   */
  int32_t frame_id() { return *frame_id_addr(); }

  /**
   * @brief 设置图像帧ID
   * @param[frame_id] 图像帧ID
   * @return
   */
  void set_frame_id(int32_t frame_id) {
    int32_t *id = frame_id_addr();
    *id = frame_id;
  }

  /**
   * @brief 获取图像帧时间戳
   * @param
   * @return 图像帧时间戳
   */
  int64_t timestamp() { return *timestamp_addr(); }

  /**
   * @brief 设置图像帧时间戳
   * @param[timestamp] 图像帧时间戳
   * @return
   */
  void set_timestamp(int64_t timestamp) {
    int64_t *ts = timestamp_addr();
    *ts = timestamp;
  }

  /**
   * @brief 获取全景分割图像起始地址
   * @param
   * @return
   */
  void *parsing_data() {
    if (!parsing_) {
      return nullptr;
    }
    return reinterpret_cast<void *>(parsing_->data());
  }

  /**
   * @brief 获取全景分割图像大小
   * @param
   * @return
   */
  size_t parsing_size() {
    if (!parsing_) {
      return 0;
    }

    return parsing_->size();
  }

  /**
   * @brief 获取车道线分割图像起始地址
   * @param
   * @return
   */
  void *lane_parsing_data() {
    if (!lane_parsing_) {
      return nullptr;
    }
    return reinterpret_cast<void *>(lane_parsing_->data());
  }

  /**
   * @brief 获取车道线分割图像大小
   * @param
   * @return
   */
  size_t lane_parsing_size() {
    if (!lane_parsing_) {
      return 0;
    }

    return lane_parsing_->size();
  }

  /**
   * @brief 设置全景分割图像
   * @param[img] 图像起始地址
   * @param[len] 图像大小
   * @return
   */
  void set_parsing(const void *img, size_t len) {
    if (!parsing_) {
      parsing_ = std::make_shared<std::vector<uint8_t> >();
    }

    if (parsing_->size() != len) {
      parsing_->resize(len);
    }
    ::memcpy(parsing_->data(), img, len);
  }

  /**
   * @brief 设置车道线分割图像
   * @param[img] 图像起始地址
   * @param[len] 图像大小
   * @return
   */
  void set_lane_parsing(const void *img, size_t len) {
    if (!lane_parsing_) {
      lane_parsing_ = std::make_shared<std::vector<uint8_t> >();
    }

    if (lane_parsing_->size() != len) {
      lane_parsing_->resize(len);
    }
    ::memcpy(lane_parsing_->data(), img, len);
  }

  /**
   * @brief 序列化帧
   * @param
   * @return
   */
  bool encode_frame() {
    /**
     * |frameId|timestamp|len|parsing_payload|len|lane_parsing_payload|
     * | 4bit  |  8bit   |4bit|    len       |4bit|     len           |
     */
    data_->resize(sizeof(int32_t) + sizeof(int64_t) + sizeof(int32_t) +
                  parsing_size() + sizeof(int32_t) + lane_parsing_size());
    uint8_t *cur = data_->data() + sizeof(int32_t) + sizeof(int64_t);
    int32_t parsing_len = parsing_size();
    ::memcpy(cur, &parsing_len, sizeof(int32_t));
    cur += sizeof(int32_t);
    if (parsing_len > 0) {
      ::memcpy(cur, parsing_data(), parsing_len);
      cur += parsing_len;
    }

    int32_t lane_parsing_len = lane_parsing_size();
    ::memcpy(cur, &lane_parsing_len, sizeof(int32_t));
    cur += sizeof(int32_t);
    if (lane_parsing_len > 0) {
      ::memcpy(cur, lane_parsing_data(), lane_parsing_len);
      cur += lane_parsing_len;
    }

    return true;
  }

  /**
   * @brief 序列化帧
   * @param [addr] 序列化数据起始地址
   * @param [len] 序列化数据大小
   * @return
   */
  bool encode_frame(void *&addr, size_t &len) {
    encode_frame();
    addr = data_->data();
    len = data_->size();
    return true;
  }

  /**
   * @brief 反序列化帧
   * @param [addr] 待反序列化数据起始地址
   * @param [len] 待反序列化数据大小
   * @return 反序列化失败false， 成功true
   */
  bool decode_frame(void *addr, size_t len) {
    if (len < sizeof(int32_t) + sizeof(int64_t)) {
      return false;
    }

    len -= sizeof(int32_t);
    len -= sizeof(int64_t);
    uint8_t *cur = (uint8_t *)addr;
    int32_t *id = frame_id_addr();
    *id = *((int32_t *)cur);
    cur += sizeof(int32_t);

    int64_t *ts = timestamp_addr();
    *ts = *((int64_t *)cur);
    cur += sizeof(int64_t);

    if (len < sizeof(int32_t)) {
      return false;
    }
    int32_t parsing_len = *((int32_t *)cur);
    len -= sizeof(int32_t);
    cur += sizeof(int32_t);
    if (len < (size_t)parsing_len) {
      return false;
    }

    if (parsing_len > 0) {
      set_parsing(cur, parsing_len);
      len -= parsing_len;
      cur += parsing_len;
    }

    if (len < sizeof(int32_t)) {
      return false;
    }
    int32_t lane_parsing_len = *((int32_t *)cur);
    len -= sizeof(int32_t);
    cur += sizeof(int32_t);
    if (len < (size_t)lane_parsing_len) {
      return false;
    }

    if (lane_parsing_len > 0) {
      set_lane_parsing(cur, lane_parsing_len);
      len -= lane_parsing_len;
      cur += lane_parsing_len;
    }

    return true;
  }

  /**
   * @brief 获取序列化后的数据起始地址
   * @param
   * @return
   */
  void *frame_data() { return reinterpret_cast<void *>(data_->data()); }

  /**
   * @brief 获取序列化后的数据大小
   * @param
   * @return
   */
  size_t frame_size() { return data_->size(); }

 private:
  int32_t *frame_id_addr() {
    return reinterpret_cast<int32_t *>(data_->data());
  }

  int64_t *timestamp_addr() {
    return reinterpret_cast<int64_t *>(data_->data() + sizeof(int32_t));
  }

  std::shared_ptr<std::vector<uint8_t> > data_;
  std::shared_ptr<std::vector<uint8_t> > parsing_;
  std::shared_ptr<std::vector<uint8_t> > lane_parsing_;
};
}  // namespace capilot
#endif