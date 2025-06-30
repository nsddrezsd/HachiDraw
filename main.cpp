#include "mainwindow.h"

#include <QApplication>
#include <QPushButton>
#include <QLocale>
#include <QRegularExpressionValidator>
#include <QLineEdit>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "HachiDraw_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    Widget *Window = new Widget;
    Window->setWindowTitle("HachiDraw");
    Window->resize(1200, 1000);
    Window->move(400, 0);
    ToolWidget *ToolWindow = new ToolWidget;
    ToolWindow->setParent(Window);
    ToolWindow->setWindowTitle("Tools");
    ToolWindow->move(400, 300);
    QGridLayout *gridLayout = new QGridLayout(ToolWindow);

    // MainWindow::Path = QCoreApplication::applicationDirPath();
    // qDebug() << MainWindow::Path;

    MyButton *btn[14];
    btn[0] = new MyButton;
    btn[0]->setIcon(QIcon(":/new/prefix1/images/0.png"));
    btn[1] = new MyButton;
    btn[1]->setIcon(QIcon(":/new/prefix1/images/1.png"));
    btn[2] = new MyButton;
    btn[2]->setIcon(QIcon(":/new/prefix1/images/clicked-2.png"));
    btn[3] = new MyButton;
    btn[3]->setIcon(QIcon(":/new/prefix1/images/3.png"));
    btn[4] = new MyButton;
    btn[4]->setIcon(QIcon(":/new/prefix1/images/4.png"));
    btn[5] = new MyButton;
    btn[5]->setIcon(QIcon(":/new/prefix1/images/5.png"));
    btn[6] = new MyButton;
    btn[6]->setIcon(QIcon(":/new/prefix1/images/6.png"));
    btn[7] = new MyButton;
    btn[7]->setIcon(QIcon(":/new/prefix1/images/7.png"));
    btn[8] = new MyButton;
    btn[8]->setIcon(QIcon(":/new/prefix1/images/8.png"));
    btn[9] = new MyButton;
    btn[9]->setIcon(QIcon(":/new/prefix1/images/9.png"));
    for (int i = 0; i < 10; ++i) {
        int row = i / 2;
        int col = i % 2;
        btn[i]->buttonmode = i;
        btn[i]->setIconSize(QSize(30, 30));
        ToolWindow->buildconnect(btn[i], Window);
        gridLayout->addWidget(btn[i], row, col);
    }
    ToolWidget::LastButton = btn[2];
    gridLayout->setContentsMargins(5, 5, 5, 5);
    gridLayout->setSpacing(5);
    ToolWindow->setLayout(gridLayout);
    ToolWindow->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    Window->setStyleSheet("background-color: white;");
    Window->show();
    ToolWindow->show();
    return a.exec();
}
