#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QPoint>
#include <QLine>
#include <QVector>
#include <QPainter>
#include <QPen>
#include <QList>
#include <qDebug>
#include <QStack>
#include <QSet>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
class Widget;
class Atom;
class Bond;
class MyButton;
//class Event;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static QString Path;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

class Bond;

class Atom : public QPointF { //原子
public:
    QString element; //原子的代码
    QVector<Bond> bonds; //相连的键
    Atom(double x=0, double y=0, QString _element = "C");
    Atom(QPoint a);
    Atom(QPointF a);
    Atom(Atom& a);
    Atom(const Atom& a);
    Atom& operator=(Atom a);
    Atom& operator=(QPoint a);
    bool operator==(Atom& a);
    bool operator!=(Atom& a);
};

class Bond : public QLineF { //化学键
public:
    Atom* atom1;
    Atom* atom2;
    int level = 0;
    Bond();
    Bond(Atom* a, Atom* b, int _level);
    Bond(Bond& a);
    Bond(const Bond& a);
    Bond& operator=(const Bond& a);
    Bond& operator+=(Atom a);
    bool operator==(Bond& a);
    bool operator!=(Bond& a);
    void paint(QPainter& painter);
};

class Widget : public QWidget { //画布
    Q_OBJECT
public:
    QVector<Bond> bonds;
    QVector<Atom*> atoms;
    QVector<QPair<QPointF, QString> > texts;
    QStack<QVector<Bond> > BondRecords; //化学键历史记录
    QStack<QVector<Atom> > AtomRecords; //原子历史记录
    QStack<QVector<QPair<QPointF, QString> > > TextRecords; //文本历史记录
    Atom* m_startPoint = new Atom();
    Atom* m_endPoint = new Atom();
    Bond* m_Bond = new Bond(); //click的bond
    Atom* m_Atom = new Atom(); //吸附的atom
    Bond* m_Bond2 = new Bond(); //吸附的bond
    Atom* Draged_Atom = new Atom(); //拖拽的原子
    Bond* Draged_Bond = new Bond(); //拖拽的键
    int Draged_TextId = -1; //拖拽的文字
    QString OutputText = "";
    QPointF OutputPos = QPointF(0, 0);
    bool m_isDrawing; //判定是否正在绘画
    bool m_isDragingAtom = 0;
    bool m_isDragingBond = 0;
    bool m_isDragingText = 0;
    int m_isDraging = 0; //判定是否在拖拽框选
    bool DragingUseful = 0; //判定是否真的拖拽移动了
    bool LengthEnough; //判定长度是否达标
    bool CanMove; //防止吸附后跟着鼠标跑
    bool DoubleClickFlag = 0; //防止双击出现单击效果
    bool Ring_on_bond = 0; //环是由一根键生出的
    int AtomThreshold = 10;
    int BondThreshold = 8;
    int TextThreshold = 20;
    QTimer* timer = new QTimer(this);
    bool m_isDrawingRing = 0;//判定是否正在成环
    QVector<Bond*> ring_bonds;
    QVector<Atom*> ring_atoms;
    Bond* base_bond;
    QRectF rect; //被框选的位置
    QVector<Bond*> Draged_bonds; //多条被框选的键
    QVector<Atom*> Draged_atoms; //多个被框选的原子
    QVector<int> Draged_texts; //多个被框选的文本
    QPointF DragstartPoint; //拖拽的起始点
    bool saved = 1; //是否保存当前所有更改
    QString File = "Untitled";
    QString FilePath = "";

    Widget(QWidget *parent = nullptr);
    ~Widget();
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void keyPressEvent(QKeyEvent* e); //按下键盘Ctrl + Z
    void closeEvent(QCloseEvent* e);
    void Save(); //保存目前历史状态
    void undo(); //Ctrl+Z后的撤销
    void paintEvent(QPaintEvent* e);
    void saveLineToScene();
    void saveRingToScene();
    void calculateRing(Bond a, bool reverse = 0);
    int CalcAtomDistance(Atom pos); //计算原子距离以便判断是否吸附
    int CalcBondDistance(Atom pos);
    int CalcTextDistance(Atom pos);
    void EraseAtom(int id); //删去原子
    void EraseBondForAtom(int id, Atom* other); //在删去原子的过程中删去键
    void EraseBond(int id); //删去一根键
    void MarkItems();
    void ChangeItems(Atom endPoint);
    void savefile(); //保存文件
    void openfile(); //打开文件
    QVector<Atom*> calculateRingVertices(Bond* baseBond, int ringSize, bool clockwise);
};

class MyButton: public QPushButton { //各种模式
    Q_OBJECT
public:
    static int mode;
    int buttonmode;
    static int bondlevel;
    MyButton(QWidget *parent = nullptr);
    ~MyButton();
};

class ToolWidget : public QWidget {
    Q_OBJECT
public:
    static MyButton* LastButton;
    void buildconnect(MyButton* button, Widget* window);
public slots:
    void ChangeMode(int mode, MyButton* button){
        MyButton::mode = mode;
        if (mode == 3) MyButton::bondlevel = 2;
        else if (mode == 4) MyButton::bondlevel = 3;
        else if (mode == 2) MyButton::bondlevel = 1;

        QString s = ":/new/prefix1/images/";
        QString t = "";
        int m = LastButton->buttonmode;
        if (m == 0) t = "0";
        while(m>0){
            t = char(m%10 + 48) + t;
            m/=10;
        }
        s += t;
        s += ".png";
        LastButton->setIcon(QIcon(s));
        LastButton = button;
        s = ":/new/prefix1/images/clicked-";
        t = "";
        m = mode;
        if (mode == 0) t = "0";
        while(m>0){
            t = char(m%10 + 48) + t;
            m/=10;
        }
        s += t;
        s += ".png";
        button->setIcon(QIcon(s));
    }
};

#endif // MAINWINDOW_H
