#pragma once
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace clog {

enum LEVLE {
  Debug,
  Info,
  Warning,
  Error,
};

class Logger {
 public:
  ~Logger() {
    if (ofstream_.is_open()) {
      ofstream_.close();
    }
  }

  inline void init_logger(LEVLE level) { level_ = level; }

  Logger &operator()(LEVLE level);
  Logger &operator<<(const std::string &data);
  Logger &operator<<(const std::wstring &data);
  Logger &operator<<(const int data);

 private:
  const std::string k_log_file_name = "device_manager.log";
  std::ofstream ofstream_;
  LEVLE level_ = Info;
  bool flag = false;
};

extern Logger g_logger_;

};  // namespace clog

#define qDebug                 \
  clog::g_logger_(clog::Debug) \
      << "[" << __FUNCTION__ << "][" << __FILE__ << ":" << __LINE__ << "]"
#define qInfo                 \
  clog::g_logger_(clog::Info) \
      << "[" << __FUNCTION__ << "][" << __FILE__ << ":" << __LINE__ << "]"
#define qWarning                 \
  clog::g_logger_(clog::Warning) \
      << "[" << __FUNCTION__ << "][" << __FILE__ << ":" << __LINE__ << "]"
#define qError                 \
  clog::g_logger_(clog::Error) \
      << "[" << __FUNCTION__ << "][" << __FILE__ << ":" << __LINE__ << "]"