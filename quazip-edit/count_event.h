/*
 *  count event
*/
#ifndef COUNT_EVENT_H
#define COUNT_EVENT_H

#include <QEvent>
#include "quazip_global.h"

const QEvent::Type _COUNT_EVENT_TYPE = (QEvent::Type)(QEvent::User + 1);

class QUAZIP_EXPORT FCountEvent: public QEvent {
 public:
  FCountEvent(qint64 nCurrent, qint64 nTotal,
              QObject *parent = 0);

  //current count
  qint64 currentCount();
  //total count
  qint64 totalCount();

 private:
  qint64    current_count_;
  qint64    total_count_;
  QObject    *_parent;                //父类
};

#endif //  COUNT_EVENT_H
