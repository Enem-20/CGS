#include "segmentationchecker.h"

uint32_t SegmentationChecker::Segment::getEnd() const {
    return offset + size;
}

bool SegmentationChecker::Segment::contains(uint32_t index) const {
    return (index >= offset) && (index < (offset + size));
}

bool SegmentationChecker::Segment::contains(Segment segment) const {
    return contains(segment.offset) && containsEnd(segment.getEnd());
}

bool SegmentationChecker::Segment::containsEnd(uint32_t index) const {
    return getEnd() >= index;
}

void SegmentationChecker::Segment::resize(uint32_t newOffset, uint32_t newSize) {
    offset = newOffset;
    size = newSize;
}

void SegmentationChecker::Segment::free() {
    used = false;
}

void SegmentationChecker::Segment::use() {
    used = true;
}

SegmentationChecker::SegmentationChecker(QObject *parent)
    : QObject{parent}
{}

void SegmentationChecker::resize(uint32_t newSize) {
    _segments.clear();
    _size = newSize;
}

void SegmentationChecker::use() {
    _segments.clear();
    _segments.push_back({0, _size});
    _segments[0].use();
}

void SegmentationChecker::use(uint32_t offset, uint32_t size) {
    use(SegmentationChecker::Segment{offset, size});
}

void SegmentationChecker::use(Segment segment) {
    if (segment.offset >= _size) return;

    // if(segment.getEnd() >= _)size) {

    // }

    for (int32_t i = 0; i < _segments.size(); i++) {
        if (!_segments[i].contains(segment.offset)) continue;

        if (_segments[i].containsEnd(segment.getEnd())) {
            if(_segments[i].used) break;

            Segment firstSegment;
            firstSegment.offset = _segments[i].offset;
            firstSegment.size = segment.offset - _segments[i].offset;

            Segment secondSegment = segment;
            secondSegment.used = true;

            Segment thirdSegment;
            thirdSegment.offset = secondSegment.getEnd();
            thirdSegment.size = _segments[i].getEnd() - thirdSegment.offset;

            _segments[i] = firstSegment;
            _segments.insert(i + 1, thirdSegment);
            _segments.insert(i + 1, secondSegment);

            break;
        }
        else {
            for (int32_t j = i + 1; j < _segments.size(); j++) {

            }
        }
    }

    while(int32_t i = 0) {
        if (_segments[i].size == 0) {
            _segments.erase(_segments.begin() + i);
            continue;
        }
        if (++i >= _segments.size()) {
            break;
        }
    }
}

void SegmentationChecker::free() {
    _segments.clear();
    _segments.push_back({0, _size});
}

void SegmentationChecker::free(uint32_t offset, uint32_t size) {
    free(SegmentationChecker::Segment{offset, size});
}

void SegmentationChecker::free(Segment segment) {

}

uint32_t SegmentationChecker::getFreeSize() const {
    uint32_t size = 0;
    for (const Segment& seg : _segments) {
        if(!seg.used) size += seg.size;
    }
    return size;
}

uint32_t SegmentationChecker::getUsedSize() const {
    uint32_t size = 0;
    for (const Segment& seg : _segments) {
        if (seg.used) size += seg.size;
    }
    return size;
}

QList<SegmentationChecker::Segment> SegmentationChecker::getFreeSegments() const {
    QList<Segment> segments;
    for (const Segment& seg : _segments) {
        if (!seg.used) segments.push_back(seg);
    }
    return segments;
}

QList<SegmentationChecker::Segment> SegmentationChecker::getUsedSegments() const {
    QList<Segment> segments;
    for (const Segment& seg : _segments) {
        if (seg.used) segments.push_back(seg);
    }
    return segments;}
