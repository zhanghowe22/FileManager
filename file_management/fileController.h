#ifndef CFILECONTROLLER_H
#define CFILECONTROLLER_H

#include <QObject>

#include "fileModel.h"
#include "fileView.h"

class CFileController : public QObject {
    Q_OBJECT
  public:
    explicit CFileController(CFileModel* model, CFileView* view, QObject* parent = nullptr);

  private slots:
    void onRequestFileList();
    void onDownloadFile(const QString& fileName);
    void onDeleteFile(const QString& fileName);
    void onFileDownloaded(const QString& fileName, const QByteArray& data);
    void onFileDeleted(const QString& fileName);

  private:
    CFileModel* model;
    CFileView* view;
};

#endif  // CFILECONTROLLER_H
