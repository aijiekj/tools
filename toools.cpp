#include "toools.h"
#include "ui_toools.h"
#include "zipfiles.h"
#include <QFileDialog>
#include <QDebug>

PToools::PToools(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::PToools) {
  ui->setupUi(this);
  ui->statusTipLbl->setText("");
  //初始化信号与槽
  initSignalSlot();
}

PToools::~PToools() {
  delete ui;
}

void PToools::slot_OperDirBtn_Clicked() {
  QString dir = QFileDialog::getExistingDirectory(this,
                tr("请选择压缩文件夹"),
                ".",
                QFileDialog::ShowDirsOnly
                | QFileDialog::DontResolveSymlinks);

  if (!dir.isEmpty()) {
    qDebug() << "Dir Name:" << dir;
    ui->compressedDir_inputLdt->setText(dir);
    QDir dirPath(dir);

    ui->compressedFileNamLdt->setText(dirPath.dirName() + QString(".rar"));
  }
}

void PToools::slot_StartCompressedBtn_Clicked() {
  ui->statusTipLbl->setText("正在压缩文件,请稍候...");

  bool bRet = PZipFiles::commpressDir(ui->compressedDir_inputLdt->text(),
                                      ui->compressedFileNamLdt->text());

  if (bRet) {
    ui->statusTipLbl->setText("文件压缩成功,您可以进行后续操作了！");
  } else {
    ui->statusTipLbl->setText("文件压缩失败,您可以进行后续操作了！");
  }

  qDebug() << "Compressed Successfully!" << bRet;
}

void PToools::initSignalSlot() {
  connect(ui->openDirBtn, SIGNAL(clicked()),
          this, SLOT(slot_OperDirBtn_Clicked()));
  connect(ui->startCompressedBtn, SIGNAL(clicked()),
          this, SLOT(slot_StartCompressedBtn_Clicked()));
}
