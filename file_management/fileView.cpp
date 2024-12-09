#include "fileView.h"

#include <QMessageBox>

CFileView::CFileView(QWidget* parent) :
    QWidget(parent), ipInput(new QLineEdit(this)), portInput(new QLineEdit(this)),
    connectButton(new QPushButton("连接 ", this)), refreshButton(new QPushButton("刷新文件列表 ", this)),
    fileList(new QListWidget(this)), downloadButton(new QPushButton("下载文件 ", this)),
    deleteButton(new QPushButton("删除文件 ", this)), connectionStatusLabel(new QLabel("未连接 ", this)) {
	// 设置UI布局
	setupUI();
	// 设置控件样式
	setupStyles();

	// 默认端口
	portInput->setText("6666");

	// 信号与槽连接
	connect(connectButton, &QPushButton::clicked, this, [this]() {
		QString ip = ipInput->text();
		quint16 port = portInput->text().toUShort();
		emit connectRequested(ip, port);
		});

	connect(refreshButton, &QPushButton::clicked, this, &CFileView::refreshRequested);

	connect(downloadButton, &QPushButton::clicked, this, [this]() {
		if (fileList->currentItem()) {
			emit downloadRequested(fileList->currentItem()->text());
		}
		else {
			QMessageBox::warning(this, "警告", "请选择一个文件进行下载 ");
		}
		});

	connect(deleteButton, &QPushButton::clicked, this, [this]() {
		if (fileList->currentItem()) {
			emit deleteRequested(fileList->currentItem()->text());
		}
		else {
			QMessageBox::warning(this, "警告", "请选择一个文件进行删除 ");
		}
		});
}

void CFileView::updateFileList(const QStringList& files) {
    fileList->clear();
    fileList->addItems(files);
}

void CFileView::showDownloadProgress(const QString& fileName, int progress) {
    QMessageBox::information(this, "下载进度", QString("文件 %1 下载完成 %2%").arg(fileName).arg(progress));
}

void CFileView::showError(const QString& message) { QMessageBox::critical(this, "错误", message); }

void CFileView::updateConnectionStatus(const QString& status, bool isConnected) {
    connectionStatusLabel->setText(status);
    connectionStatusLabel->setStyleSheet(isConnected ? "color: green; font-weight: bold;" :
                                                       "color: red; font-weight: bold;");
}

void CFileView::setupUI()
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setSpacing(12);  // 设置控件间的间距

	// IP 和端口输入框布局
	QHBoxLayout* connectionLayout = new QHBoxLayout();
	connectionLayout->addWidget(new QLabel("IP 地址:", this));
	connectionLayout->addWidget(ipInput);
	connectionLayout->addWidget(new QLabel("端口:", this));
	connectionLayout->addWidget(portInput);
	connectionLayout->addWidget(connectButton);
	layout->addLayout(connectionLayout);

	// 连接状态标签
	layout->addWidget(connectionStatusLabel);

	// 文件操作布局
	layout->addWidget(refreshButton);
	layout->addWidget(fileList);
	layout->addWidget(downloadButton);
	layout->addWidget(deleteButton);

	// 设置主布局
	setLayout(layout);
}

void CFileView::setupStyles()
{
	// 设置背景色
	setStyleSheet("background-color: #f4f4f4;");

	// 输入框样式
	ipInput->setStyleSheet("background-color: white; border-radius: 5px; padding: 8px;");
	portInput->setStyleSheet("background-color: white; border-radius: 5px; padding: 8px;");

	// 按钮样式
	connectButton->setStyleSheet("background-color: #5cb85c; color: white; border-radius: 5px; padding: 8px;");
	connectButton->setCursor(Qt::PointingHandCursor);

	refreshButton->setStyleSheet("background-color: #ffffff; border: 1px solid #ddd; border-radius: 5px; padding: 8px;");
	downloadButton->setStyleSheet("background-color: #5cb85c; color: white; border-radius: 5px; padding: 8px;");
	deleteButton->setStyleSheet("background-color: #d9534f; color: white; border-radius: 5px; padding: 8px;");
	fileList->setStyleSheet("background-color: white; border-radius: 5px; padding: 10px;");

	// 连接状态标签样式
	connectionStatusLabel->setStyleSheet("color: #5bc0de; font-weight: bold; font-size: 16px;");
}



