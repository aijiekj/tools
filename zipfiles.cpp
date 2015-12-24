#include "zipfiles.h"
#include "../thirdparty/quazip/quazipfile.h"
#include "../thirdparty/quazip/JlCompress.h"

#include <QFileInfo>
#include <QDebug>

#ifdef Q_WS_WIN
#include "WinBase.h"
#endif

static bool copyData(QIODevice &inFile, QIODevice &outFile) {
  while (!inFile.atEnd()) {
    char buf[4096];
    qint64 readLen = inFile.read(buf, 4096);
    if (readLen <= 0) {
      return false;
    }
    if (outFile.write(buf, readLen) != readLen) {
      return false;
    }
  }
  return true;
}


PZipFiles::PZipFiles() {
}

bool PZipFiles::commpressDir(const QString &sDir, const QString &sSaveFile) {
  return JlCompress::compressDir(sSaveFile, sDir, true);
}

QStringList PZipFiles::extractDir(QString fileCompressed, QString dir) {

  return JlCompress::extractDir(fileCompressed, dir);
//  QuaZip zip(fileCompressed);
//  if (!zip.open(QuaZip::mdUnzip)) {
//    return QStringList();
//  }

//  QDir directory(dir);
//  QStringList extracted;
//  if (!zip.goToFirstFile()) {
//    return QStringList();
//  }

//  do {
//    QString name = zip.getCurrentFileName();
//    QString absFilePath = directory.absoluteFilePath(name);
//    if (!extractFile(&zip, "", absFilePath)) {
//      removeFile(extracted);
//      return QStringList();
//    }
//    extracted.append(absFilePath);

//    QFileInfo tmpFileInfo(absFilePath);
//    qDebug() << absFilePath;
//    qDebug() << tmpFileInfo.lastModified();

//  } while (zip.goToNextFile());

//  // Chiudo il file zip
//  zip.close();
//  if (zip.getZipError() != 0) {
//    removeFile(extracted);
//    return QStringList();
//  }

//  return extracted;
}

QString PZipFiles::extractFile(const QString &fileCompressed, QString fileName,
                               QString fileDest) {
  // Apro lo zip
  QuaZip zip(fileCompressed);
  if (!zip.open(QuaZip::mdUnzip)) {
    return QString();
  }

  // Estraggo il file
  if (fileDest.isEmpty()) {
    fileDest = fileName;
  }
  if (!extractFile(&zip, fileName, fileDest)) {
    return QString();
  }

  // Chiudo il file zip
  zip.close();
  if (zip.getZipError() != 0) {
    removeFile(QStringList(fileDest));
    return QString();
  }
  return QFileInfo(fileDest).absoluteFilePath();
}

bool PZipFiles::extractFile(QuaZip *zip, QString fileName, QString fileDest) {
  // zip: oggetto dove aggiungere il file
  // filename: nome del file reale
  // fileincompress: nome del file all'interno del file compresso

  // Controllo l'apertura dello zip
  if (!zip) {
    return false;
  }
  if (zip->getMode() != QuaZip::mdUnzip) {
    return false;
  }

  // Apro il file compresso
  if (!fileName.isEmpty()) {
    zip->setCurrentFile(fileName);
  }
  QuaZipFile inFile(zip);
  if (!inFile.open(QIODevice::ReadOnly) || inFile.getZipError() != UNZ_OK) {
    return false;
  }

  // Controllo esistenza cartella file risultato
  QDir curDir;
  if (!curDir.mkpath(QFileInfo(fileDest).absolutePath())) {
    return false;
  }

  QuaZipFileInfo info;
  if (!zip->getCurrentFileInfo(&info)) {
    return false;
  }
  if (fileDest.endsWith('/') && QFileInfo(fileDest).isDir()) {
    return QFile(fileDest).setPermissions(info.getPermissions());
  }

  // Apro il file risultato
  QFile outFile;
  outFile.setFileName(fileDest);
  if (!outFile.open(QIODevice::WriteOnly)) {
    return false;
  }

  // Copio i dati
  if (!copyData(inFile, outFile) || inFile.getZipError() != UNZ_OK) {
    outFile.close();
    removeFile(QStringList(fileDest));
    return false;
  }
  outFile.close();

  // Chiudo i file
  inFile.close();
  if (inFile.getZipError() != UNZ_OK) {
    removeFile(QStringList(fileDest));
    return false;
  }
#ifdef Q_WS_WIN
  //get file time
  QDateTime origin(QDate(1601, 1, 1), QTime(0, 0, 0, 0), Qt::UTC);
  qint64 _100nanosecs = 10000 * origin.msecsTo(info.dateTime.toUTC());
  // Pack _100nanosecs into the structure.
  FILETIME fileTime;
  fileTime.dwLowDateTime = _100nanosecs;
  fileTime.dwHighDateTime = (_100nanosecs >> 32);

  //switch file name
  QByteArray text = fileDest.toLocal8Bit();
  char *pFileName = new char[text.size() + 1];
  strcpy(pFileName, text.data());
  qDebug() << *pFileName;
  HANDLE hFile;
  //open Files
  hFile = CreateFileA(pFileName, GENERIC_READ | GENERIC_WRITE,
                      FILE_SHARE_READ | FILE_SHARE_WRITE,
                      NULL,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL,
                      NULL);
  if (hFile == INVALID_HANDLE_VALUE) {
    outFile.setPermissions(info.getPermissions());
    delete pFileName;
    pFileName = NULL;
  }
  //setTime arguments(file_hanld, create_time, modify_time)
  //note:如果只想设置修改时间则可以(hFile, NULL, NULL, &ft1)
  SetFileTime(hFile, &fileTime, &fileTime, &fileTime);
  CloseHandle(hFile);

  delete pFileName;
  pFileName = NULL;
#endif

  return outFile.setPermissions(info.getPermissions());
}

bool PZipFiles::removeFile(QStringList listFile) {
  bool ret = true;
  // Per ogni file
  for (int i = 0; i < listFile.count(); i++) {
    // Lo elimino
    ret = ret && QFile::remove(listFile.at(i));
  }
  return ret;
}
