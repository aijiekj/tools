#include "toools.h"
#include <QApplication>
#include "zipfiles.h"
#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QMutex>

QString  cur_time =
  QDateTime::currentDateTime().toString("tools yyyy_MM_dd_hh_mm_ss");
QString txt_Name = "./log/" + cur_time + ".txt";
QFile *logFile = new QFile(txt_Name);

void MyLog(QtMsgType type , const QMessageLogContext &context ,
           const QString &msg) {
  static QMutex mutex;
  mutex.lock();

  QString log;

  switch (type) {
  case QtDebugMsg:
    log = QString("Debug: ");
    break;

  case QtWarningMsg:
    log = QString("Error: ");
    break;

  case QtCriticalMsg:
    log = QString("Critical Error: ");
    break;

  case QtFatalMsg:
    log = QString("Fatal Error: ");
    abort();
    break;

  default:
    log = QString("Unknow Msg Type : ");
    break;
  };

  log += QDateTime::currentDateTime().toString("yyyy年MM月dd日 hh:mm:ss.zzz, File: ")
         + QString(context.file) ;
  log += QString(", Line: ") + QString::number(context.line) +
         QString(" : %1\n").arg(msg);
  logFile->write(log.toStdString().c_str());
  logFile->flush();

  mutex.unlock();
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  QDir dir;
  dir.mkdir("log");
  logFile->open(QIODevice::ReadWrite | QIODevice::Text);
  qInstallMessageHandler(MyLog);

  if (argc == 2) {
    QString fileName = QString("./updateinfo.ini");
    QSettings *settings = new QSettings(fileName, QSettings::IniFormat);
    settings->setIniCodec("UTF8");

    int nRet = 0;
    if (QString(argv[1]) == QString("backup")) {
      QString sBackDir = "./";
      QString sBackToDir = QString("../software_bak/");
      QDir softwareBack(sBackDir);
      softwareBack.mkpath("../software_bak/");

      qDebug() << "back Dir:" << sBackDir;

      QString sOldVersion = settings->value("info/oldVersion").toString();
      qDebug() << sOldVersion;
      QString sBackFiles = sBackToDir + QString("/%1_%2.rar")
                           .arg(sOldVersion)
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh_mm"));
      qDebug() << "back start....";
      bool bRet = PZipFiles::commpressDir(sBackDir,
                                          sBackFiles);
      qDebug() << "back end....";
      if (!bRet) {
        nRet = -1;
      }
    } else if (QString(argv[1]) == QString("insttall")) {

      QString sDownFileZip = QCoreApplication::applicationDirPath() + "/" +
                             settings->value("info/downloadPath").toString();

      qDebug() << "Start Install...";
      QFile downFile(sDownFileZip);
      if (downFile.exists()) {

        qDebug() << sDownFileZip;
        PZipFiles::extractDir(
          sDownFileZip,
          QCoreApplication::applicationDirPath());

//        if (!bRet) {
//          nRet = -1;
//        }

        qDebug() << "End Install...";

      } else {
        qDebug() << "File Not Find!!";
        nRet = -1;
      }

      qDebug() << "Install States:" << nRet;
    }

    return nRet;
  }

  PToools w;
  w.show();

  return a.exec();
}
