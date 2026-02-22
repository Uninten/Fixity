#ifndef T_RINGBUFFER_H
#define T_RINGBUFFER_H

#include <memory>
#include <QMutex>
#include <QByteArray>

class T_RingBuffer
{
public:
    explicit T_RingBuffer(size_t capacity) ;
    bool write(const char* data, int length);
    int read(char* data, int maxLength);
    // 获取可写入的空间大小
    int availableWriteSpace() const;
    // 获取可读取的数据大小
    int availableReadData() const;
    // 读取数据（但不移动读指针，用于查找分隔符）
    int peek(char* data, int maxLength);
    void skip(int bytes);
private:
    QMutex m_mutex;
    std::unique_ptr<char[]> m_buffer;
    int m_size;
    int m_readIndex;
    int m_writeIndex;
};

#endif // T_RINGBUFFER_H
