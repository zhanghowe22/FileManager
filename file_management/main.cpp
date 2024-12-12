#include <QApplication>

#include "fileController.h"
#include "fileModel.h"
#include "fileView.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    // 创建MVC各组件
    CFileModel* model = new CFileModel();
    CFileView* view = new CFileView();
    CFileController* controller = new CFileController(model, view);

    // 窗口标题
    view->setWindowTitle("文件管理v1.0 ");
    view->resize(800, 600);

	QFont font("Microsoft YaHei", 12);
    view->setFont(font);

    view->show();

    int result = a.exec();

    // 清理资源
    delete controller;
    delete view;
    delete model;

    return result;
}
