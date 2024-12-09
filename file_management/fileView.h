#ifndef CFILEVIEW_H
#define CFILEVIEW_H
#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class CFileView : public QWidget {
    Q_OBJECT

  public:
    explicit CFileView(QWidget* parent = nullptr);

  signals:
    void downloadRequested(const QString& fileName);
    void deleteRequested(const QString& fileName);
    void refreshRequested();

  public slots:
    void updateFileList(const QStringList& files);
    void showDownloadProgress(const QString& fileName, int progress);
    void showError(const QString& message);

  private:
    QListWidget* fileList;
    QPushButton* downloadButton;
    QPushButton* deleteButton;
    QPushButton* refreshButton;
    QProgressBar* progressBar;
    QLabel* statusLabel;
};

#endif  // CFILEVIEW_H
