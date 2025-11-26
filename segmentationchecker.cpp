#include "segmentationchecker.h"

bool SegmentationChecker::Segment::contains(uint32_t index) const {
    return (index >= offset) && (index < (offset + size));
}

void SegmentationChecker::Segment::resize(uint32_t newOffset, uint32_t newSize) {
    offset = newOffset;
    size = newSize;
}

SegmentationChecker::SegmentationChecker(QObject *parent)
    : QObject{parent}
{}

void SegmentationChecker::resize(uint32_t newSize) {
    if(newSize < _size) {

    }

}

void SegmentationChecker::use() {
    _usedSegments.clear();
    _freeSegments.clear();
    _usedSegments.push_back({_start, _size});
}

void SegmentationChecker::use(uint32_t offset, uint32_t size) {
    use(SegmentationChecker::Segment{offset, size});
}

void SegmentationChecker::use(Segment segment) {

}

void SegmentationChecker::free() {
    _usedSegments.clear();
    _freeSegments.clear();
    _freeSegments.push_back({_start, _size});
}

void SegmentationChecker::free(uint32_t offset, uint32_t size) {
    free(SegmentationChecker::Segment{offset, size});
}

void SegmentationChecker::free(Segment segment) {

}

uint32_t SegmentationChecker::getFreeSize() const {
    uint32_t size = 0;
    for (const Segment& seg : _freeSegments) {
        size += seg.size;
    }
    return size;
}

uint32_t SegmentationChecker::getUsedSize() const {
    uint32_t size = 0;
    for (const Segment& seg : _usedSegments) {
        size += seg.size;
    }
    return size;
}

const QList<SegmentationChecker::Segment>& SegmentationChecker::getFreeSegments() const {
    return _freeSegments;
}

const QList<SegmentationChecker::Segment>& SegmentationChecker::getUsedSegments() const {
    return _usedSegments;
}
