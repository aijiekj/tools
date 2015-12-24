#ifndef PZIPFILES_H
#define PZIPFILES_H

#include "../thirdparty/quazip/quazip.h"

class PZipFiles {
 public:
  PZipFiles();

  static bool commpressDir(const QString &sDir,
                           const QString &sSaveFile);
  static QStringList extractDir(QString fileCompressed, QString dir);
  static QString extractFile(const QString &fileCompressed,
                             QString fileName, QString fileDest);
  static bool extractFile(QuaZip *zip,
                          QString fileName, QString fileDest);
  static bool removeFile(QStringList listFile);
};

#endif // PZIPFILES_H
