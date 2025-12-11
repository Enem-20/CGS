#ifndef SEGMENTMAP_H
#define SEGMENTMAP_H

#include <bitset>
#include <QObject>
#include <QList>

struct Region {
    size_t from;
    size_t to;
    size_t elementSizeBytedSize;
    size_t elementedSize = to - from;
    size_t bytedSize = elementSizeBytedSize * elementedSize;
};

class SegmentMap : public QObject
{
    Q_OBJECT
private: // static variables
    static constexpr size_t MASK_SIZE = 1024 * 1024 * 1024;
    static std::bitset<MASK_SIZE> _dataMask;
private:
    size_t _bitMaxSize;
public: // constructors
    explicit SegmentMap(size_t bitMaxSize = MASK_SIZE, QObject *parent = nullptr);
public: // functions
    size_t getMissingSegmentsCount() const;
    size_t getExistingSegmentsCount() const;

    QList<size_t> getMissingSegments() const;
    QList<size_t> getExistingsSegments() const;

    QList<Region> getMissingRegions(size_t typeSize) const;
    QList<Region> getExistingRegions(size_t typeSize) const;
signals:
    void segmentsMissingNotify(QList<size_t> missing) const;
    void regionsMissingNotify(QList<Region> missing) const;
public slots:
    void segmentWritten(size_t i);
    void segmentErased(size_t i);
    void setBitMaskSize(size_t size);
    size_t getBitMaskSize() const;
    void reset();
    void singleReset(size_t index);

    void notifyConnectsAboutMissingSegments() const;
    void notifyConnectsAboutMissingRegions(size_t typeSize) const;
};

#endif // SEGMENTMAP_H
