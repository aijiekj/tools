#ifndef TOOOLS_H
#define TOOOLS_H

#include <QWidget>

namespace Ui {
class PToools;
}

class PToools : public QWidget {
  Q_OBJECT

 public:
  explicit PToools(QWidget *parent = 0);
  ~PToools();

 private slots:
  void slot_OperDirBtn_Clicked();
  void slot_StartCompressedBtn_Clicked();

 private:
  void initSignalSlot();    //初始化信号与槽

 private:
  Ui::PToools *ui;
};

#endif // TOOOLS_H
