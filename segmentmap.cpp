#include "segmentmap.h"

std::bitset<SegmentMap::MASK_SIZE> SegmentMap::_dataMask;

SegmentMap::SegmentMap(size_t bitMaxSize, QObject *parent)
    : QObject{parent}
    , _bitMaxSize(bitMaxSize)
{

}

size_t SegmentMap::getMissingSegmentsCount() const {
    size_t count = 0;
    for(size_t i = 0; i < _bitMaxSize; ++i) {
        if(!_dataMask.test(i)) ++count;
    }
    return count;
}

size_t SegmentMap::getExistingSegmentsCount() const {
    size_t count = 0;
    for(size_t i = 0; i < _bitMaxSize; ++i) {
        if(_dataMask.test(i)) ++count;
    }
    return count;
}

QList<size_t> SegmentMap::getMissingSegments() const {
    QList<size_t> result;
    for(size_t i = 0; i < _bitMaxSize; ++i) {
        if(!_dataMask.test(i)) result += i;
    }
    return result;
}

QList<size_t> SegmentMap::getExistingsSegments() const {
    QList<size_t> result;
    for(size_t i = 0; i < _bitMaxSize; ++i) {
        if(_dataMask.test(i)) result += i;
    }
    return result;
}

void SegmentMap::segmentWritten(size_t i) {
    _dataMask.set(i, true);
}

void SegmentMap::segmentErased(size_t i) {
    _dataMask.set(i, false);
}

void SegmentMap::setBitMaskSize(size_t size) {
    _bitMaxSize = size;
}

size_t SegmentMap::getBitMaskSize() const {
    return _bitMaxSize;
}

void SegmentMap::reset() {
    _dataMask.reset();
}

void SegmentMap::singleReset(size_t index) {
    _dataMask.reset(index);
}

QList<Region> SegmentMap::getMissingRegions(size_t typeSize) const {
    QList<Region> result;
    int startIndex = -1;

    for(size_t i = 0; i < _bitMaxSize; ++i) {
        if(_dataMask.test(i)) {
            if((i > 0) && !_dataMask.test(i-1)) {
                result += Region{static_cast<size_t>(startIndex), i-1, typeSize};
            }
        }
        else {
            if((i > 0) && _dataMask.test(i-1) || i == 0) {
                startIndex = i;
            }
        }
    }

    return result;
}

QList<Region> SegmentMap::getExistingRegions(size_t typeSize) const {
    QList<Region> result;
    int startIndex = -1;
    for(size_t i = 0; i < _bitMaxSize; ++i) {
        if(!_dataMask.test(i)) {
            if((i > 0) && _dataMask.test(i-1)) {
                result += Region{static_cast<size_t>(startIndex), i-1, typeSize};
            }
        }
        else {
            if((i > 0) && !_dataMask.test(i-1) || i == 0) {
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
