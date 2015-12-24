#include "count_event.h"

FCountEvent::FCountEvent(qint64 nCurrent, qint64 nTotal,
    QObject *parent) : QEvent(_COUNT_EVENT_TYPE) {
    current_count_ = nCurrent;
    total_count_ = nTotal;
    _parent = parent;
}

//current count
qint64 FCountEvent::currentCount(){
    return current_count_;
}

//total count
qint64 FCountEvent::totalCount(){
    return total_count_;
}
