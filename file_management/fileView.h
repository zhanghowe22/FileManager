#ifndef CFILEVIEW_H
#define CFILEVIEW_H

#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

class CFileView : public QWidget {
    Q_OBJECT

  public:
    explicit CFileView(QWidget* parent = nullptr);

    void updateFileList(const QStringList& files);
    void showError(const QString& message);
    void showDownloadProgress(const QString& fileName, int progress);
    void updateConnectionStatus(const QString& status, bool isConnected);
  signals:
    void connectRequested(const QString& ip, quint16 port);
    void refreshRequested();
    void downloadRequested(const QString& fileName);
    void deleteRequested(const QString& fileName);

  private:
    QLineEdit* ipInput;
    QLineEdit* portInput;
    QPushButton* connectButton;
    QPushButton* refreshButton;
    QListWidget* fileList;
    QPushButton* downloadButton;
    QPushButton* deleteButton;
    QLabel* connectionStatusLabel;  // 显示连接状态

    void setupUI();
    void setupStyles();
};

#endif  // CFILEVIEW_H
