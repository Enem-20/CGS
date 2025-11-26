#ifndef SEGMENTATIONCHECKER_H
#define SEGMENTATIONCHECKER_H

#include <QObject>

class SegmentationChecker : public QObject {
    Q_OBJECT
public:
    struct Segment {
        uint32_t offset = 0;
        uint32_t size = 0;
        bool used = false;

        uint32_t getEnd() const;
        bool contains(uint32_t index) const;
        bool contains(Segment segment) const;
        bool containsEnd(uint32_t index) const;
        void resize(uint32_t newOffset, uint32_t newSize);
        void free();
        void use();
    };

    void resize(uint32_t size);
    void offset(int32_t offset);
    void offsetWithWrap(int32_t offset);

    void use();
    void use(uint32_t offset, uint32_t size);
    void use(Segment segment);
    void free();
    void free(Segment segment);
    void free(uint32_t offset, uint32_t size);

    uint32_t getFreeSize() const;
    uint32_t getUsedSize() const;
    QList<Segment> getFreeSegments() const;
    QList<Segment> getUsedSegments() const;

    explicit SegmentationChecker(QObject *parent = nullptr);

signals:

private:
    uint32_t _size = 0;
    QList<Segment> _segments;
};

#endif // SEGMENTATIONCHECKER_H
