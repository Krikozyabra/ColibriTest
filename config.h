#ifndef CONFIG_H
#define CONFIG_H

#define REWRITE_FILES 0
#define MODIFY_FILES 1

#include <string>
#include <mutex>

class Config
{
public:
    bool getDeletingInputFilesFlag(){return this->deletingInputFilesFlag;}
    void setDeletingInputFilesFlag(bool flag){this->deletingInputFilesFlag = flag;}

    bool getOutputFilesMode(){return this->outputFilesMode;}
    void setOutputFilesMode(bool mode){this->outputFilesMode=mode;}

    void setKeyCode(std::string key){this->keyCode = key;}
    std::string getKeyCode(){
        std::lock_guard<std::mutex> lock(mutex_);
        return this->keyCode;}
    int getKeyCodeLen() {
        std::lock_guard<std::mutex> lock(mutex_);
        return this->keyCode.length();}

    int getNumberOfFiles(){return numberOfFiles;}
    void addFileInProcess() {
        std::lock_guard<std::mutex> lock(mutex_);
        numberOfFiles++;}
    void delFileInProcess() {
        std::lock_guard<std::mutex> lock(mutex_);
        numberOfFiles--;}

    // Singleton не должен принимать новое значение и не должен быть клонируемым
    Config(Config &other) = delete;
    void operator=(const Config &) = delete;

    static Config *GetInstace();


protected:
    Config(){}
    ~Config(){}

private:
    bool deletingInputFilesFlag;
    bool outputFilesMode;
    std::string keyCode;
    int numberOfFiles;

    // Для хранение единственного экземпляра и мьютекс для многопоточности
    static Config * pinstance_;
    static std::mutex mutex_;
};

#endif // CONFIG_H
