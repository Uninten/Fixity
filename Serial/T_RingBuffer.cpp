#include "T_RingBuffer.h"
#include <QDebug>
#include <memory>
#include <QMutex>

T_RingBuffer::T_RingBuffer(size_t capacity)
    : m_size(capacity),
      m_buffer(new char[capacity]),
      m_readIndex(0),
      m_writeIndex(0){}

bool T_RingBuffer::write(const char* data, int length)
{
    QMutexLocker locker(&m_mutex);
    if (length <= 0) return false;

    int availableSpace = availableWriteSpace();
    if (length > availableSpace) {
        // 缓冲区已满，这里简单丢弃数据，可根据需求调整策略
        return false;
    }

    int remaining = m_size - m_writeIndex;
    if (length <= remaining) {
        memcpy(m_buffer.get() + m_writeIndex, data, length);
        m_writeIndex += length;
        if (m_writeIndex == m_size) m_writeIndex = 0;
    } else {
        int firstPart = remaining;
        int secondPart = length - firstPart;
        memcpy(m_buffer.get() + m_writeIndex, data, firstPart);
        memcpy(m_buffer.get(), data + firstPart, secondPart);
        m_writeIndex = secondPart;
    }

    return true;
}

int T_RingBuffer::read(char* data, int maxLength)
{
    QMutexLocker locker(&m_mutex);
    if (maxLength <= 0) return 0;

    int availableData = availableReadData();
    if (maxLength > availableData) maxLength = availableData;

    int remaining = m_size - m_readIndex;
    if (maxLength <= remaining) {
        memcpy(data, m_buffer.get() + m_readIndex, maxLength);
        m_readIndex += maxLength;
        if (m_readIndex == m_size) m_readIndex = 0;
    } else {
        int firstPart = remaining;
        int secondPart = maxLength - firstPart;
        memcpy(data, m_buffer.get() + m_readIndex, firstPart);
        memcpy(data + firstPart, m_buffer.get(), secondPart);
        m_readIndex = secondPart;
    }

    return maxLength;
}

int T_RingBuffer::availableWriteSpace() const
{
    if (m_writeIndex >= m_readIndex) {
        return m_size - (m_writeIndex - m_readIndex);
    } else {
        return m_readIndex - m_writeIndex;
    }
}

int T_RingBuffer::availableReadData() const
{
    if (m_writeIndex >= m_readIndex) {
        return m_writeIndex - m_readIndex;
    } else {
        return m_size - (m_readIndex - m_writeIndex);
    }
}

int T_RingBuffer::peek(char *data, int maxLength)
{
    QMutexLocker locker(&m_mutex);
    if (maxLength <= 0) return 0;

    int availableData = availableReadData();
    if (maxLength > availableData) maxLength = availableData;

    int remaining = m_size - m_readIndex;
    if (maxLength <= remaining) {
        memcpy(data, m_buffer.get() + m_readIndex, maxLength);
    } else {
        int firstPart = remaining;
        int secondPart = maxLength - firstPart;
        memcpy(data, m_buffer.get() + m_readIndex, firstPart);
        memcpy(data + firstPart, m_buffer.get(), secondPart);
    }

    return maxLength;
}

void T_RingBuffer::skip(int bytes)
{
    QMutexLocker locker(&m_mutex);
    if (bytes <= 0) return;
    if (bytes > availableReadData()) return; // 避免越界

    m_readIndex = (m_readIndex + bytes) % m_size;
}


