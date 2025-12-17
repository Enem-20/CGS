#include "segmentmap.h"

SegmentMap::SegmentMap(size_t size, QObject *parent)
    : QObject{parent}
    , _dataMask(size)
{

}

size_t SegmentMap::size() const {
    return _dataMask.size(); 
}

void SegmentMap::resize(size_t size) {
    _dataMask.resize(size);
}

size_t SegmentMap::getMissingSegmentsCount() const {
    size_t count = 0;
    for (size_t i = 0; i < _dataMask.size(); ++i) {
        if (!_dataMask.testBit(i)) ++count;
    }
    return count;
}

size_t SegmentMap::getExistingSegmentsCount() const {
    size_t count = 0;
    for (size_t i = 0; i < _dataMask.size(); ++i) {
        if (_dataMask.testBit(i)) ++count;
    }
    return count;
}

QList<size_t> SegmentMap::getMissingSegments() const {
    QList<size_t> result;
    for (size_t i = 0; i < _dataMask.size(); ++i) {
        if (!_dataMask.testBit(i)) result += i;
    }
    return result;
}

QList<size_t> SegmentMap::getExistingSegments() const {
    QList<size_t> result;
    for (size_t i = 0; i < _dataMask.size(); ++i) {
        if (_dataMask.testBit(i)) result += i;
    }
    return result;
}

void SegmentMap::segmentWritten(size_t i) {
    _dataMask.setBit(i, true);
}

void SegmentMap::segmentErased(size_t i) {
    _dataMask.setBit(i, false);
}

void SegmentMap::reset() {
    _dataMask.clear();
}

void SegmentMap::singleReset(size_t index) {
    segmentErased(index);
}

QList<Region> SegmentMap::getMissingRegions(size_t typeSize) const {
    QList<Region> result;
    int startIndex = -1;

    for (size_t i = 0; i < _dataMask.size(); ++i) {
        if (_dataMask.testBit(i)) {
            if ((i > 0) && !_dataMask.testBit(i-1)) {
                result += Region{static_cast<size_t>(startIndex), i-1, typeSize};
            }
        }
        else {
            if ((i > 0) && _dataMask.testBit(i-1) || i == 0) {
                startIndex = i;
            }
        }
    }

    return result;
}

QList<Region> SegmentMap::getExistingRegions(size_t typeSize) const {
    QList<Region> result;
    int startIndex = -1;
    for (size_t i = 0; i < _dataMask.size(); ++i) {
        if (!_dataMask.testBit(i)) {
            if ((i > 0) && _dataMask.testBit(i-1)) {
                result += Region{static_cast<size_t>(startIndex), i-1, typeSize};
            }
        }
        else {
            if ((i > 0) && !_dataMask.testBit(i-1) || i == 0) {
                startIndex = i;
            }
        }
    }

    return result;
}

void SegmentMap::notifyConnectsAboutMissingSegments() const {
    emit segmentsMissingNotify(getMissingSegments());
}

void SegmentMap::notifyConnectsAboutMissingRegions(size_t typeSize) const {
    emit regionsMissingNotify(getMissingRegions(typeSize));
}
