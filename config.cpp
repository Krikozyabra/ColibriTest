#include "config.h"

//Инициализация статических переменных
Config* Config::pinstance_{nullptr};
std::mutex Config::mutex_;

// При первой попытки создается новый и единственный экземпляр и возвращается
// а мьютекс блокируется
Config *Config::GetInstace(){
    std::lock_guard<std::mutex> lock(mutex_);
    if (pinstance_ == nullptr){
        pinstance_ = new Config();
    }
    return pinstance_;
}
