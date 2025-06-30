#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

Atom::Atom(double x, double y, QString _element):QPointF(x, y){
    element = _element;
}
Atom::Atom(QPoint a):QPointF(a){
    setX(a.x());
    setY(a.y());
    element = "C";
}
Atom::Atom(QPointF a):QPointF(a){
    setX(a.x());
    setY(a.y());
    element = "C";
}
Atom::Atom(Atom& a):QPointF(a.x(), a.y()){
    setX(a.x());
    setY(a.y());
    element = a.element;
    bonds = a.bonds;
}
Atom::Atom(const Atom& a):QPointF(a.x(), a.y()){
    setX(a.x());
    setY(a.y());
    element = a.element;
    bonds = a.bonds;
}
Atom& Atom::operator=(Atom a){
    setX(a.x());
    setY(a.y());
    element = a.element;
    bonds = a.bonds;
    return *this;
}
Atom& Atom::operator=(QPoint a){
    setX(a.x());
    setY(a.y());
    element = "C";
    bonds.clear();
    return *this;
}

bool Atom::operator==(Atom& a){
    if (a.x() == x() && a.y() == y()) return 1;
    else return 0;
}

bool Atom::operator!=(Atom& a){
    return !((*this) == a);
}

Bond::Bond():QLineF(QPointF(0, 0), QPointF(0, 0)){
    atom1 = new Atom(0, 0);
    atom2 = new Atom(0, 0);
    level = 0;
}

Bond::Bond(Atom* a, Atom* b, int _level):QLineF(QPointF(a->x(), a->y()), QPointF(b->x(), b->y())){
    atom1 = a;
    atom2 = b;
    level = _level;
}

Bond::Bond(Bond& a):QLineF(QPointF(a.atom1->x(), a.atom1->y()), QPointF(a.atom2->x(), a.atom2->y())){
    atom1 = a.atom1;
    atom2 = a.atom2;
    level = a.level;
}

Bond::Bond(const Bond& a):QLineF(QPointF(a.atom1->x(), a.atom1->y()), QPointF(a.atom2->x(), a.atom2->y())){
    atom1 = a.atom1;
    atom2 = a.atom2;
    level = a.level;
}

Bond& Bond::operator=(const Bond& a){
    atom1 = a.atom1;
    atom2 = a.atom2;
    level = a.level;
    this->setP1(QPointF(atom1->x(), atom1->y()));
    this->setP2(QPointF(atom2->x(), atom2->y()));
    return *this;
}

Bond& Bond::operator+=(Atom a){
    *atom1 += a;
    *atom2 += a;
    this->setP1(*atom1);
    this->setP2(*atom2);
    return *this;
}

bool Bond::operator==(Bond& a){
    if (((*(a.atom1)) == (*atom1) && (*(a.atom2)) == (*atom2)) || ((*a.atom1) == (*atom2) && (*a.atom2) == (*atom1))) return 1;
    else return 0;
}

bool Bond::operator!=(Bond& a){
    return !((*this) == a);
}

void Bond::paint(QPainter& painter){
    this->setP1(*atom1);
    this->setP2(*atom2);
    int atomerasement = 0;
    if (atom1->element != "C" && atom2->element != "C"){
        QPointF center = this->center();
        QLineF unitVec = this->unitVector();
        int newLength = (this->length() - 16);
        QLineF tmp = QLineF(center - (unitVec.p2() - unitVec.p1()) * newLength/2, center + (unitVec.p2() - unitVec.p1()) * newLength/2);
        painter.drawLine(tmp);
        atomerasement = 16;
    }
    else if (atom2->element != "C"){
        QPointF center = this->center();
        QLineF unitVec = this->unitVector();
        int newLength = (this->length() - 16);
        QLineF tmp = QLineF(center - (unitVec.p2() - unitVec.p1()) * length()/2, center + (unitVec.p2() - unitVec.p1()) * newLength/2);
        painter.drawLine(tmp);
        atomerasement = 8;
    }
    else if (atom1->element != "C"){
        QPointF center = this->center();
        QLineF unitVec = this->unitVector();
        int newLength = (this->length() - 16);
        QLineF tmp = QLineF(center - (unitVec.p2() - unitVec.p1()) * newLength/2, center + (unitVec.p2() - unitVec.p1()) * length()/2);
        painter.drawLine(tmp);
        atomerasement = 8;
    }
    else{
        painter.drawLine(*atom1, *atom2);
    }
    if (level == 2){
        double offset = 8.66; // 双线间距
        QPointF direction = unitVector().p2() - unitVector().p1();
        QPointF normal(-direction.y(), direction.x()); // 垂直方向向量
        normal *= offset / QLineF(QPointF(0,0), normal).length(); // 标准化偏移
        QLineF tmp(p1() + normal, p2() + normal);

        if (atom1->bonds.size() == 2){ //处理双键中短的一段位于何处的问题
            Bond line = atom1->bonds[0];
            if (atom1->bonds[0] == (*this)){
                line = atom1->bonds[1];
            }
            if ((*line.atom1) != *atom1){
                QPointF normal2 = (line.unitVector().p1() - line.unitVector().p2());
                if (normal2.x() * normal.x() + normal2.y() * normal.y() < 0){
                    tmp = QLineF(p1() - normal, p2()- normal);
                }
            }
            else{
                QPointF normal2 = (line.unitVector().p2() - line.unitVector().p1());
                if (normal2.x() * normal.x() + normal2.y() * normal.y() < 0){
                    tmp = QLineF(p1() - normal, p2()- normal);
                }
            }
        }
        else if (atom2->bonds.size() == 2){
            Bond line = atom2->bonds[0];
            if (atom2->bonds[0] == (*this)) line = atom2->bonds[1];
            if ((*line.atom1) != *atom2){
                QPointF normal2 = (line.unitVector().p1() - line.unitVector().p2());
                if (normal2.x() * normal.x() + normal2.y() * normal.y() < 0){
                    tmp = QLineF(p1() - normal, p2()- normal);
                }
            }
            else{
                QPointF normal2 = (line.unitVector().p2() - line.unitVector().p1());
                if (normal2.x() * normal.x() + normal2.y() * normal.y() < 0){
                    tmp = QLineF(p1() - normal, p2()- normal);
                }
            }
        }
        QPointF center = tmp.center();
        QLineF unitVec = tmp.unitVector();
        int newLength = (tmp.length() - atomerasement) * 0.8;
        tmp = QLineF(center - (unitVec.p2() - unitVec.p1()) * newLength/2, center + (unitVec.p2() - unitVec.p1()) * newLength/2);
        painter.drawLine(tmp);
    }
    else if (level == 3){
        double offset = 5.0; // 双线间距
        QPointF direction = unitVector().p2() - unitVector().p1();
        QPointF normal(-direction.y(), direction.x()); // 垂直方向向量
        normal *= offset / QLineF(QPointF(0,0), normal).length(); // 标准化偏移

        QLineF line1(p1() + normal, p2() + normal);
        QLineF line2(p1() - normal, p2() - normal);

        painter.drawLine(line1);
        painter.drawLine(line2);
    }
}

Widget::Widget(QWidget *parent):QWidget(parent){
    m_isDrawing = false;
    LengthEnough = false;
    setMouseTracking(true);
    connect(timer, &QTimer::timeout, this, [&](){
        timer->stop();
        return ;
    });
}

Widget::~Widget(){

}

void Widget::mousePressEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        if (!timer->isActive()){
            timer->start(200);  // 启动200ms延迟
        }
        else{
            timer->stop();
            return ;
        }
    }
    if (DoubleClickFlag){
        return ;
    }
    if (MyButton::mode != 0){
        m_isDraging = 0;
        Draged_atoms.clear();
        Draged_bonds.clear();
        Draged_texts.clear();
        update();
    }
    if (MyButton::mode == 2 || MyButton::mode == 3 || MyButton::mode == 4){ //化学键
        if (e->button() == Qt::LeftButton) {
            setMouseTracking(false); //在拖拽过程中停止检测自由鼠标
            m_Atom = new Atom();
            m_Bond2 = new Bond();

            m_startPoint = new Atom(e->pos());
            int f = CalcAtomDistance(*m_startPoint);
            if (f != -1){
                m_startPoint = atoms[f];
                //qDebug() << *m_startPoint;
            }
            else{
                f = CalcBondDistance(*m_startPoint);
                if (f != -1){
                    m_Bond = &bonds[f];
                    m_isDrawing = false;
                    return ;
                }
            }
            m_isDrawing = true;
            LengthEnough = false;
            CanMove = true;
        }
    }
    else if (MyButton::mode == 5){ //输入
        if (e->button() == Qt::LeftButton){
            int f = CalcAtomDistance(Atom(e->pos()));
            if (f != -1){
                OutputPos = QPointF(*atoms[f]);
                OutputText = QInputDialog::getText(this, tr("输入"), tr("请输入:"), QLineEdit::Normal, tr("C"));
                if (OutputText.isEmpty()) return ; //输入为空
                Save();
                atoms[f]->element = OutputText;
                update();
                return ;
            }
            OutputPos = QPointF(e->pos());
            OutputText = QInputDialog::getText(this, tr("输入"), tr("请输入:"), QLineEdit::Normal, tr("C"));
            Save();
            texts.push_back({OutputPos, OutputText});
            update();
        }
    }
    else if (MyButton::mode == 1){ //橡皮
        if (e->button() == Qt::LeftButton){
            int f = CalcAtomDistance(e->pos());
            if (f != -1){
                Save();
                EraseAtom(f);
            }
            else{
                f = CalcBondDistance(e->pos());
                if (f != -1){
                    Save();
                    EraseBond(f);
                }
                else{
                    f = CalcTextDistance(e->pos());
                    if (f != -1){
                        Save();
                        EraseText(f);
                    }
                    else return ;
                }
            }
            update();
        }
    }
    else if (MyButton::mode == 0){ //鼠标拖拽
        setCursor(Qt::ClosedHandCursor);
        setMouseTracking(false); //在拖拽过程中停止检测自由鼠标
        if (m_isDraging && rect.contains(e->pos())){
            DragstartPoint = e->pos();
            Save(); //无论如何先存着
            DragingUseful = 0;
            return ;
        }
        int f = CalcAtomDistance(e->pos());
        m_startPoint = new Atom(e->pos());
        m_endPoint = new Atom(e->pos());
        m_isDraging = 0;
        DragingUseful = 0;
        DragstartPoint = QPointF(0, 0);
        Draged_atoms.clear();
        Draged_bonds.clear();
        Draged_texts.clear();
        rect = QRectF(QPointF(0, 0), QPointF(0, 0));
        if (f != -1){
            Draged_Atom = atoms[f];
            m_isDragingAtom = 1;
        }
        else{
            f = CalcBondDistance(e->pos());
            if (f != -1){
                Draged_Bond = &bonds[f];
                m_isDragingBond = 1;
            }
            else{
                f = CalcTextDistance(e->pos());
                if (f != -1){
                    Draged_TextId = f;
                    m_isDragingText = 1;
                }
                else{//证明此时在拖拽框选
                    m_isDraging = 1;
                    return ;
                }
            }
        }
        update();
        Save();
    }
    else if (MyButton::mode >=6 && MyButton::mode <=13){ //成环
        if (e->button() == Qt::LeftButton) {
            setMouseTracking(false); //在拖拽过程中停止检测自由鼠标
            m_Atom = new Atom();
            m_Bond2 = new Bond();
            m_startPoint = new Atom(e->pos());
            int f = CalcAtomDistance(*m_startPoint);
            if (f != -1){
                m_startPoint = atoms[f];
            }
            else{
                f = CalcBondDistance(*m_startPoint);
                if (f != -1){
                    base_bond = &bonds[f];
                    Ring_on_bond=true;
                    return ;
                }
            }
            m_isDrawingRing = true;
            LengthEnough = false;
            CanMove = true;
        }
    }
}

QVector<Atom*> Widget::calculateRingVertices(Bond* baseBond, int ringSize, bool clockwise) {
    QVector<Atom*> vertices;
    if (!baseBond || ringSize < 3) return vertices;
    // 获取基准键的两个端点
    Atom* atomA = baseBond->atom1;
    Atom* atomB = baseBond->atom2;
    // 计算基准边的向量和长度
    QPointF vecAB(atomB->x() - atomA->x(), atomB->y() - atomA->y());
    double bondLength = sqrt(vecAB.x() * vecAB.x() + vecAB.y() * vecAB.y());
    // 计算中点
    QPointF midPoint(
        (atomA->x() + atomB->x()) / 2.0,
        (atomA->y() + atomB->y()) / 2.0
        );
    // 计算单位向量和垂直向量
    QPointF unitVec = vecAB / bondLength;
    QPointF perpVec(-unitVec.y(), unitVec.x());
    // 根据顺时针/逆时针调整方向
    if (!clockwise) {
        perpVec = -perpVec;
    }
    // 计算正多边形的几何参数
    double radius = bondLength / (2 * sin(M_PI / ringSize));
    double height = radius * cos(M_PI / ringSize);
    // 计算圆心位置（从中点垂直偏移）
    QPointF circleCenter = midPoint + perpVec * height;
    // 添加第二个顶点（基准边的B点）
    vertices.append(atomB);
    // 添加第一个顶点（基准边的A点）
    vertices.append(atomA);
    // 计算从圆心指向A点的向量，并计算其角度
    QPointF vecA = QPointF(atomA->x() - circleCenter.x(), atomA->y() - circleCenter.y());
    double startAngle = atan2(vecA.y(), vecA.x());
    // 计算角度步长（弧度）
    double angleStep = (clockwise ? -1 : 1) * (2 * M_PI / ringSize);
    // 生成其他顶点（跳过已添加的两个顶点）
    for (int i = 2; i < ringSize; i++) {
        double angle = startAngle + angleStep * (i - 1);
        QPointF newPos(
            circleCenter.x() + radius * cos(angle),
            circleCenter.y() + radius * sin(angle)
            );
        // 边界检查
        if (newPos.x() < 0 || newPos.y() < 0 ||
            newPos.x() > this->width() || newPos.y() > this->height()) {
            QMessageBox::warning(this, "Out of Bound", "Vertex out of canvas");
            return QVector<Atom*>();
        }
        vertices.append(new Atom(newPos));
    }
    return vertices;
}

void Widget::calculateRing(Bond a, bool reverse){
    int x=6;//环大小
    if( MyButton::mode >= 6 && MyButton::mode <= 9 ) x=MyButton::mode-3;
    ring_atoms=calculateRingVertices(&a, x, reverse);
    bool f=0;
    for(auto i=ring_atoms.begin();i!=ring_atoms.end();++i)
    {
        if(CalcAtomDistance(**i)==-1)f=1;
    }
    if(!f)
    {
        ring_atoms=calculateRingVertices(&a, x, !reverse);
    }
    ring_bonds.clear();
    for(int i=0;i<x;++i)
    {
        ring_bonds.append(new Bond(ring_atoms[i],ring_atoms[(i+1)%x],1));
    }

    return;
}

void Widget::saveRingToScene() {
    for(auto i=ring_bonds.begin();i!=ring_bonds.end();++i)
    {
        MyButton::bondlevel=(*i)->level;
        m_startPoint=(*i)->atom1;
        m_endPoint=(*i)->atom2;
        saveLineToScene();
    }
    return;
}

void Widget::mouseMoveEvent(QMouseEvent *e) {
    if (!m_isDrawing){ //吸附上色
        Atom tmp = Atom(e->pos());
        int f = CalcAtomDistance(tmp);
        if (f != -1){
            m_Atom = atoms[f];
            update();
            //return ;
        }
        else {
            f = CalcBondDistance(tmp);
            if (f != -1){
                m_Bond2 = &bonds[f];
                update();
                //return ;
            }
            else {
                m_Bond2 = new Bond();
                m_Atom = new Atom(0, 0);
                update();
                //return ;
            }
        }
    }
    if (MyButton::mode == 2 || MyButton::mode == 3 || MyButton::mode == 4){
        if (m_isDrawing) {
            Atom m_endPoint1 = e->pos();
            int f = CalcAtomDistance(m_endPoint1);
            if (f != -1 && *atoms[f] != *m_startPoint){ //无论距离自动吸附
                m_endPoint = atoms[f];
                LengthEnough = true;
                CanMove = false;
                update();
                return ;
            }
            else if (CanMove == false){
                m_endPoint = new Atom(e->pos());
                CanMove = true;
            }
            double dx = abs(m_endPoint1.x() - m_startPoint->x());
            double dy = abs(m_endPoint1.y() - m_startPoint->y());
            double len = sqrt(dx * dx + dy * dy);
            if (len >= 50 && CanMove){ //若距离大于等于50则自动将直线更改为50后成键
                m_endPoint->setX(m_startPoint->x() + (m_endPoint1.x() - m_startPoint->x()) * 50 / len);
                m_endPoint->setY(m_startPoint->y() + (m_endPoint1.y() - m_startPoint->y()) * 50 / len);
                LengthEnough = true;
                update();
            }
            else if (len <= 30 && LengthEnough){ //若距离重新回到30及以下可认为撤销成键
                LengthEnough = false;
                update();
            }
        }
    }
    else if (MyButton::mode == 0){
        if (e->buttons() & Qt::LeftButton){
            Atom tmp = Atom(e->pos());
            if (m_isDragingAtom){
                Draged_Atom->setX(Draged_Atom->x() + tmp.x() - m_startPoint->x());
                Draged_Atom->setY(Draged_Atom->y() + tmp.y() - m_startPoint->y());
            }
            else if (m_isDragingBond){
                Atom* a = Draged_Bond->atom1, *b = Draged_Bond->atom2;
                a->setX(a->x() + tmp.x() - m_startPoint->x());
                a->setY(a->y() + tmp.y() - m_startPoint->y());
                b->setX(b->x() + tmp.x() - m_startPoint->x());
                b->setY(b->y() + tmp.y() - m_startPoint->y());
            }
            else if (m_isDragingText){
                texts[Draged_TextId].first.setX(texts[Draged_TextId].first.x() + tmp.x() - m_startPoint->x());
                texts[Draged_TextId].first.setY(texts[Draged_TextId].first.y() + tmp.y() - m_startPoint->y());
            }
            else if (m_isDraging == 1){
                m_endPoint = new Atom(tmp);
                MarkItems();
                ChangeRect();
                update();
                return ;
            }
            else if (m_isDraging == 2){
                ChangeItems(tmp);
                update();
                return ;
            }
            update();
            m_startPoint->setX(tmp.x());
            m_startPoint->setY(tmp.y());
        }
    }
    else if (MyButton::mode >= 6 && MyButton::mode <= 13){
        if (m_isDrawingRing) {
            Atom m_endPoint1 = e->pos();
            int f = CalcAtomDistance(m_endPoint1);
            if (f != -1 && *atoms[f] != *m_startPoint){ //无论距离自动吸附
                m_endPoint = atoms[f];
                base_bond=new Bond(m_startPoint,m_endPoint,1);
                calculateRing(*base_bond);
                LengthEnough = true;
                CanMove = false;
                update();
                return ;
            }
            else if (CanMove == false){
                m_endPoint = new Atom(e->pos());
                base_bond=new Bond(m_startPoint,m_endPoint,1);
                calculateRing(*base_bond);
                CanMove = true;
            }
            double dx = abs(m_endPoint1.x() - m_startPoint->x());
            double dy = abs(m_endPoint1.y() - m_startPoint->y());
            double len = sqrt(dx * dx + dy * dy);
            if (len >= 50 && CanMove){ //若距离大于等于50则自动将直线更改为50后成键
                m_endPoint->setX(m_startPoint->x() + (m_endPoint1.x() - m_startPoint->x()) * 50 / len);
                m_endPoint->setY(m_startPoint->y() + (m_endPoint1.y() - m_startPoint->y()) * 50 / len);
                LengthEnough = true;
                base_bond=new Bond(m_startPoint,m_endPoint,1);
                calculateRing(*base_bond);
                f = CalcAtomDistance(*ring_atoms[2]);
                if(f!=-1)
                {
                    base_bond=new Bond(m_startPoint,atoms[f],1);
                    calculateRing(*base_bond,1);
                    base_bond=new Bond(m_startPoint,ring_atoms[2],1);
                    calculateRing(*base_bond);
                }
                update();
            }
            else if (len <= 30 && LengthEnough){ //若距离重新回到30及以下可认为撤销成键
                delete base_bond;
                base_bond=nullptr;
                ring_bonds.clear();
                ring_atoms.clear();
                LengthEnough = false;
                update();
            }
        }
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *e) {
    if (MyButton::mode == 2 || MyButton::mode == 3 || MyButton::mode == 4){
        if (e->button() == Qt::LeftButton && m_isDrawing && LengthEnough) {
            m_isDrawing = false;
            LengthEnough = false;
            Save();
            saveLineToScene();
            update();
            setMouseTracking(true);
            return ;
        }
        if (e->button() == Qt::LeftButton && !m_isDrawing){ //增加键级
            int f = CalcBondDistance(*m_startPoint);
            if (f != -1){
                if (bonds[f] == (*m_Bond)){
                    Save();
                    if (MyButton::bondlevel == 1){
                        bonds[f].level = (bonds[f].level + 1) % 3;
                        if (bonds[f].level == 0) bonds[f].level = 3;
                    }
                    else bonds[f].level = MyButton::bondlevel;
                    m_Bond = new Bond();
                    update();
                    setMouseTracking(true);
                    return ;
                }
            }
        }
        m_isDrawing = false;
        LengthEnough = false;
        m_startPoint = new Atom();
        setMouseTracking(true);
    }
    else if (MyButton::mode == 0){
        setCursor(Qt::OpenHandCursor);
        if (!m_isDraging){
            m_isDragingAtom = 0;
            m_isDragingBond = 0;
            m_isDragingText = 0;
            m_startPoint = new Atom();
            m_endPoint = new Atom();
            Draged_Atom = new Atom();
            Draged_Bond = new Bond();
            Draged_TextId = -1;
        }
        else if (m_isDraging){
            if (Draged_bonds.empty() && Draged_atoms.empty() && Draged_texts.empty()){ //啥都没框到
                m_isDraging = 0;
                setMouseTracking(true);
                update();
                return ;
            }
            ModifyRect(Draged_atoms, Draged_texts); // 实际上是为了更新新的框坐标
            if (!DragingUseful && !BondRecords.empty()){
                BondRecords.pop();
                AtomRecords.pop();
                TextRecords.pop();
            }
            m_isDraging = 2;
            update();
            return ;
        }
        setMouseTracking(true);
        update();
    }
    else if (MyButton::mode >= 6 && MyButton::mode <= 13){
        if (e->button() == Qt::LeftButton && ((m_isDrawingRing && LengthEnough)|| Ring_on_bond )) {
            m_isDrawingRing = false;
            LengthEnough = false;
            Ring_on_bond = false;
            calculateRing(*base_bond);
            Save();
            saveRingToScene();
            update();
            setMouseTracking(true);
            return;
        }
        m_isDrawingRing = false;
        LengthEnough = false;
        Ring_on_bond = false;
        m_startPoint = new Atom();
        setMouseTracking(true);
        ring_atoms.clear();
        ring_bonds.clear();
    }
}

void Widget::mouseDoubleClickEvent(QMouseEvent* e){
    if (e->button() == Qt::LeftButton) {
        DoubleClickFlag = 1;
        if (timer->isActive()) timer->stop();
    }
    if (MyButton::mode != 1 && MyButton::mode != 5){
        if (e->button() == Qt::LeftButton) {
            int f = CalcAtomDistance(Atom(e->pos()));
            if (f != -1){
                OutputPos = QPointF(*atoms[f]);
                OutputText = QInputDialog::getText(this, tr("输入"), tr("请输入:"), QLineEdit::Normal, tr("C"));
                if (OutputText.isEmpty()) return ; //输入为空
                Save();
                atoms[f]->element = OutputText;
                update();
                DoubleClickFlag = 0;
                return ;
            }
        }
    }
    DoubleClickFlag = 0;
    return ;
}

void Widget::keyPressEvent(QKeyEvent* e){
    if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_Z){
        undo();
        m_isDraging = 0;
        Draged_bonds.clear();
        Draged_atoms.clear();
        Draged_texts.clear();
        update();
    }
    else if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_S){
        savefile();
    }
    else if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_O){
        openfile();
    }
    else if (e->key() == Qt::Key_Delete){
        DeleteItems();
        m_isDraging = 0;
        update();
    }
    else if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_C && MyButton::mode == 0) {
        Copy();
    }
    else if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_V) {
        Paste();
    }
    else{
        QWidget::keyPressEvent(e);
    }
}

void Widget::closeEvent(QCloseEvent* e){
    if (!saved){
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "保存对此文件所做的更改？",
            "要把改动保存到" + File + "中吗？",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
            );
        if (reply == QMessageBox::Yes) {
            savefile();
            e->accept();
        }
        else if (reply == QMessageBox::Cancel){
            e->ignore();
        }
        else{
            e->accept();
        }
    }
}

void Widget::undo(){
    if (AtomRecords.empty()) return ;
    QVector<Atom> tmpAtom = AtomRecords.top(); AtomRecords.pop();
    QVector<Bond> tmpBond = BondRecords.top(); BondRecords.pop();
    texts = TextRecords.top(); TextRecords.pop();
    atoms.clear();
    bonds.clear();
    for (int i=0;i<tmpAtom.size();i++){
        Atom* tmp = new Atom(tmpAtom[i]);
        atoms.push_back(tmp);
    }
    for (int i=0;i<tmpBond.size();i++){
        Bond tmp = tmpBond[i];
        Atom tmp1 = Atom(tmpBond[i].p1());
        Atom tmp2 = Atom(tmpBond[i].p2());
        int f1 = CalcAtomDistance(tmp1), f2 = CalcAtomDistance(tmp2);
        if (f1 != -1){
            tmp.atom1 = atoms[f1];
        }
        if (f2 != -1){
            tmp.atom2 = atoms[f2];
        }
        atoms[f1]->bonds.push_back(tmp);
        atoms[f2]->bonds.push_back(tmp);
        bonds.push_back(tmp);
    }
    update();
}

void Widget::Save(){
    BondRecords.push(bonds);
    TextRecords.push(texts);
    QVector<Atom> tmp;
    for (int i=0;i<atoms.size();i++){
        tmp.push_back(*atoms[i]);
    }
    AtomRecords.push_back(tmp);
    saved = 0;
    return ;
}

void Widget::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 2));
    painter.setFont(QFont("Arial", 12));
    for (int i=0;i<bonds.size();i++) {
        int f = 0;
        for (int j=0;j<Draged_bonds.size();j++){
            if (*Draged_bonds[j] == bonds[i]){
                f = 1;
                break;
            }
        }
        if (f || *m_Bond2 == bonds[i]) painter.setPen(QPen(Qt::red, 4));
        bonds[i].paint(painter);
        painter.setPen(QPen(Qt::black, 2));
    }
    for (int i=0;i<atoms.size();i++){
        int f = 0;
        for (int j=0;j<Draged_atoms.size();j++){
            if (*Draged_atoms[j] == *atoms[i]){
                f = 1;
                break;
            }
        }
        if (f || *m_Atom == *atoms[i]){
            painter.setPen(QPen(Qt::red, 8));
            if (atoms[i]->element == "C") painter.drawPoint(*atoms[i]);
            else painter.drawText(*atoms[i] - QPointF(6, -6), atoms[i]->element);
            painter.setPen(QPen(Qt::black, 2));
        }
        else if (atoms[i]->element != "C"){
            painter.drawText(*atoms[i] - QPointF(6, -6), atoms[i]->element);
        }
    }
    if (*m_startPoint != Atom(0, 0) && *m_endPoint != Atom(0, 0) && m_isDrawing){
        Bond newbond = Bond(m_startPoint, m_endPoint, MyButton::bondlevel);
        if (LengthEnough){
            newbond.paint(painter);
        }
    }
    if (m_isDrawingRing){
        if (LengthEnough){
            for(auto it =ring_bonds.begin();it!=ring_bonds.end();++it)
                (*it)->paint(painter);
        }
    }
    QFontMetrics fm(QFont("Arial", 12));
    for (int i=0;i<texts.size();i++){
        QRect textRect = fm.boundingRect(texts[i].second);
        QRect drawRect(texts[i].first.x() - textRect.width()/2,
                       texts[i].first.y() - textRect.height()/2,
                       textRect.width(),
                       textRect.height());
        int f = 0;
        for (int j=0;j<Draged_texts.size();j++){
            if (i == Draged_texts[j]){
                f = 1;
                break;
            }
        }
        if (f || Draged_TextId == i) painter.setPen(QPen(Qt::red, 8));
        painter.drawText(drawRect, Qt::AlignCenter, texts[i].second);
        painter.setPen(QPen(Qt::black, 2));
    }
    OutputText = "";
    OutputPos = QPointF(0, 0);
    //框选的虚线
    if (m_isDraging){
        QPen pen;
        pen.setStyle(Qt::DashLine);
        pen.setColor(Qt::black);
        pen.setWidth(1);
        painter.setPen(pen);
        QPointF tmp1 = rect.topLeft();
        QPointF tmp2 = rect.bottomRight();
        painter.drawLine(QLineF(tmp1, QPointF(tmp1.x(), tmp2.y())));
        painter.drawLine(QLineF(tmp1, QPointF(tmp2.x(), tmp1.y())));
        painter.drawLine(QLineF(tmp2, QPointF(tmp1.x(), tmp2.y())));
        painter.drawLine(QLineF(tmp2, QPointF(tmp2.x(), tmp1.y())));
    }
    //标题
    QString title = "HachiDraw - [" + File;
    if (!saved) title += "*";
    title += "]";
    this->setWindowTitle(title);
}

void Widget::ChangeRect(){
    if (m_startPoint->x() < m_endPoint->x() && m_startPoint->y() < m_endPoint->y()){
        rect = QRectF(*m_startPoint, *m_endPoint);
    }
    else if (m_startPoint->x() < m_endPoint->x() && m_startPoint->y() > m_endPoint->y()){
        rect = QRectF(QPointF(m_startPoint->x(), m_endPoint->y()), QPointF(m_endPoint->x(), m_startPoint->y()));
    }
    else if (m_startPoint->x() > m_endPoint->x() && m_startPoint->y() < m_endPoint->y()){
        rect = QRectF(QPointF(m_endPoint->x(), m_startPoint->y()), QPointF(m_startPoint->x(), m_endPoint->y()));
    }
    else{
        rect = QRectF(*m_endPoint, *m_startPoint);
    }
}

void Widget::ModifyRect(QVector<Atom*> _atoms, QVector<int> _texts){
    // 计算新粘贴内容的边界
    QPointF minPos(INT_MAX, INT_MAX);
    QPointF maxPos(INT_MIN, INT_MIN);
    // 检查原子位置
    for (Atom* atom : _atoms) {
        minPos.setX(qMin(minPos.x(), atom->x()));
        minPos.setY(qMin(minPos.y(), atom->y()));
        maxPos.setX(qMax(maxPos.x(), atom->x()));
        maxPos.setY(qMax(maxPos.y(), atom->y()));
    }
    // 检查文本位置
    for (int i = 0; i < _texts.size(); i ++) {
        minPos.setX(qMin(minPos.x(), texts[_texts[i]].first.x()));
        minPos.setY(qMin(minPos.y(), texts[_texts[i]].first.y()));
        maxPos.setX(qMax(maxPos.x(), texts[_texts[i]].first.x()));
        maxPos.setY(qMax(maxPos.y(), texts[_texts[i]].first.y()));
    }
    // 设置选择框
    m_startPoint = new Atom(minPos - QPointF(20, 20)); // 添加一些边距
    m_endPoint = new Atom(maxPos + QPointF(20, 20));
    ChangeRect(); // 更新选择的内容
}

void Widget::MarkItems(){
    Draged_atoms.clear();
    Draged_bonds.clear();
    Draged_texts.clear();
    ChangeRect();
    for (int i=0;i<atoms.size();i++){
        if (rect.contains(*atoms[i])){
            Draged_atoms.push_back(atoms[i]);
        }
    }
    for (int i=0;i<bonds.size();i++){
        if (rect.contains(*bonds[i].atom1) && rect.contains(*bonds[i].atom2)){
            Draged_bonds.push_back(&bonds[i]);
        }
    }
    for (int i=0;i<texts.size();i++){
        if (rect.contains(texts[i].first)){
            Draged_texts.push_back(i);
        }
    }
    return ;
}

void Widget::DeleteItems(){
    QVector<Atom> tmp1;
    QVector<Bond> tmp2;
    QVector<QPair<QPointF, QString> > tmp3;
    for (int i=0;i<Draged_atoms.size();i++){
        tmp1.push_back(*Draged_atoms[i]);
    }
    for (int i=0;i<Draged_bonds.size();i++){
        tmp2.push_back(*Draged_bonds[i]);
    }
    for (int i=0;i<Draged_texts.size();i++){
        tmp3.push_back(texts[Draged_texts[i]]);
    }
    for (int i=0;i<tmp1.size();i++){
        for (int j=0;j<atoms.size();j++){
            if (tmp1[i] == *atoms[j]){
                EraseAtom(j);
                break;
            }
        }
    }
    for (int i=0;i<tmp2.size();i++){
        for (int j=0;j<bonds.size();j++){
            if (tmp2[i] == bonds[j]){
                EraseBond(j);
                break;
            }
        }
    }
    for (int i=0;i<tmp3.size();i++){
        for (int j=0;j<texts.size();j++){
            if (tmp3[i] == texts[j]){
                EraseText(j);
                break;
            }
        }
    }
    rect = QRectF(QPointF(0, 0), QPointF(0, 0));
    return ;
}

void Widget::ChangeItems(Atom endPoint){
    DragingUseful = 1;
    Atom delta = endPoint - DragstartPoint;
    DragstartPoint = endPoint;
    *m_startPoint += delta;
    *m_endPoint += delta;
    ChangeRect();
    for (int i=0;i<Draged_atoms.size();i++){
        *Draged_atoms[i] += delta;
    }
    //想想看，为什么bond不需要加delta呢？
    //宝宝 我觉得是因为bond是链接原子地址，不需要记录位置
    for (int i=0;i<Draged_texts.size();i++){
        texts[Draged_texts[i]].first += delta;
    }
}

void Widget::saveLineToScene() {
    Bond newbond = Bond(m_startPoint, m_endPoint, MyButton::bondlevel);
    int f=1;
    for (int i=0;i<atoms.size();i++){
        if (*atoms[i] == *m_startPoint){
            atoms[i]->bonds.push_back(newbond);
            m_startPoint = atoms[i];
            f=0;
            break;
        }
    }
    if (f){
        m_startPoint->bonds.push_back(newbond);
        atoms.push_back(m_startPoint);
    }
    newbond = Bond(m_endPoint, m_startPoint, MyButton::bondlevel);
    f=1;
    for (int i=0;i<atoms.size();i++){
        if (*atoms[i] == *m_endPoint){
            atoms[i]->bonds.push_back(newbond);
            m_endPoint = atoms[i];
            f=0;
            break;
        }
    }
    if (f){
        m_endPoint->bonds.push_back(newbond);
        atoms.push_back(m_endPoint);
    }
    newbond = Bond(m_startPoint, m_endPoint, MyButton::bondlevel);
    bonds.push_back(newbond);
    m_startPoint = new Atom();
    m_endPoint = new Atom();
}

int Widget::CalcAtomDistance(Atom pos){
    int ans = -1;
    double mn = AtomThreshold;
    for (int i=0;i<atoms.size();i++){
        if (*atoms[i] == *m_startPoint/* || *atoms[i] == *m_endPoint*/) continue;
        double dx = abs(atoms[i]->x() - pos.x());
        double dy = abs(atoms[i]->y() - pos.y());
        double len = sqrt(dx*dx + dy*dy);
        if (len < mn){
            mn = len;
            ans = i;
        }
    }
    return ans;
}

int Widget::CalcBondDistance(Atom pos){
    int ans = -1;
    double mn = BondThreshold;
    for (int i=0;i<bonds.size();i++){
        Atom a1 = *bonds[i].atom1, a2 = *bonds[i].atom2;
        double d = abs((a2.x() - a1.x()) * (a1.y() - pos.y()) - (a1.x() - pos.x()) * (a2.y() - a1.y()));
        d /= sqrt((a2.x() - a1.x()) * (a2.x() - a1.x()) + (a2.y() - a1.y()) * (a2.y() - a1.y()));
        double d1 = sqrt((a1.x() - pos.x()) * (a1.x() - pos.x()) + (a1.y() - pos.y()) * (a1.y() - pos.y()));
        double d2 = sqrt((a2.x() - pos.x()) * (a2.x() - pos.x()) + (a2.y() - pos.y()) * (a2.y() - pos.y()));
        double deltathreshold = sqrt(bonds[i].length() * bonds[i].length() + BondThreshold * BondThreshold) - BondThreshold;
        //神秘threshold，不妨猜猜看怎么得到的
        if (d < mn && abs(d1 - d2) < deltathreshold){
            mn = d;
            ans = i;
        }
    }
    return ans;
}

int Widget::CalcTextDistance(Atom pos){
    int ans = -1;
    double mn = TextThreshold;
    for (int i=0;i<texts.size();i++){
        double dx = abs(texts[i].first.x() - pos.x());
        double dy = abs(texts[i].first.y() - pos.y());
        double len = sqrt(dx*dx + dy*dy);
        if (len < mn){
            mn = len;
            ans = i;
        }
    }
    return ans;
}

void Widget::EraseAtom(int id){
    Atom* tmp = atoms[id];
    auto iter = atoms.begin() + id;
    atoms.erase(iter);
    for (int i=0;i<bonds.size();){
        if (*bonds[i].atom1 == *tmp){
            EraseBondForAtom(i, bonds[i].atom2);
        }
        else if (*bonds[i].atom2 == *tmp){
            EraseBondForAtom(i, bonds[i].atom1);
        }
        else i++;
    }
    return ;
}

void Widget::EraseBondForAtom(int id, Atom* other){
    Bond tmp = bonds[id];
    auto iter = bonds.begin() + id;
    bonds.erase(iter);
    for (int i=0;i<other->bonds.size();i++){
        if (other->bonds[i] == tmp){
            auto iter = other->bonds.begin() + i;
            other->bonds.erase(iter);
            break;
        }
    }
    if (other->bonds.size() == 0){
        for (int i=0;i<atoms.size();i++){
            if (*atoms[i] == *other){
                //events.top().ErasedAtom.push_front(*atoms[i]);
                EraseAtom(i);
                break;
            }
        }
    }
    return ;
}

void Widget::EraseBond(int id){
    if (bonds[id].level > 1){
        bonds[id].level -= 1;
    }
    else{
        Bond tmp = bonds[id];
        auto iter = bonds.begin() + id;
        bonds.erase(iter);
        for (int i=0;i<atoms.size();){
            if (*atoms[i] == *tmp.atom1 || *atoms[i] == *tmp.atom2){
                for (int j=0;j<atoms[i]->bonds.size();j++){
                    if (atoms[i]->bonds[j] == tmp){
                        atoms[i]->bonds.erase(atoms[i]->bonds.begin() + j);
                        break;
                    }
                }
                if (atoms[i]->bonds.size() == 0){
                    //events.top().ErasedAtom.push_front(*atoms[i]);
                    EraseAtom(i);
                }
                else i++;
            }
            else i++;
        }
    }
}

void Widget::EraseText(int id){
    texts.erase(texts.begin() + id);
}

void Widget::savefile(){
    if (FilePath == ""){ //没有保存位置的
        FilePath = QFileDialog::getSaveFileName(this, "保存文件", "C:\\Users\\PC\\Desktop\\untitled", "项目文件(*.hachi)");
    }
    QFile file(FilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QTextStream out(&file);
        out << atoms.size() << Qt::endl;
        for(int i=0;i<atoms.size();i++){
            out << atoms[i]->rx() << " " << atoms[i]->ry() << " " << atoms[i]->element << Qt::endl;
        }
        out << bonds.size() << Qt::endl;
        for(int i=0;i<bonds.size();i++){
            int id1 = 0, id2 = 0;
            for (int j=0;j<atoms.size();j++){
                if (*(bonds[i].atom1) == *atoms[j]){
                    id1 = j;
                }
                if (*(bonds[i].atom2) == *atoms[j]){
                    id2 = j;
                }
            }
            out << id1 << " " << id2 << " " << bonds[i].level << Qt::endl;
        }
        out << texts.size() << Qt::endl;
        for(int i=0;i<texts.size();i++){
            out << texts[i].first.rx() << " " << texts[i].first.ry() << " " << texts[i].second << Qt::endl;
        }
        saved = 1;
        update();
        file.close();
    }
    else if (FilePath != ""){
        QMessageBox::critical(this, tr("错误"), tr("文件保存错误!"));
    }
}

void Widget::openfile(){
    if (!saved){
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "保存对此文件所做的更改？",
            "要把改动保存到" + File + "中吗？",
            QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
            );
        if (reply == QMessageBox::Yes) {
            savefile();
        }
        else if (reply == QMessageBox::Cancel){
            return ;
        }
    }
    QString filepath = QFileDialog::getOpenFileName(this, "打开文件", "C:\\Users\\PC\\Desktop", "项目文件(*.hachi)");
    QFile file(filepath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        int atomnum, bondnum, textnum;
        in >> atomnum;
        atoms.clear();
        for (int i=0;i<atomnum;i++){
            double x, y;
            QString element;
            in >> x >> y >> element;
            atoms.push_back(new Atom(x, y, element));
        }
        in >> bondnum;
        bonds.clear();
        for (int i=0;i<bondnum;i++){
            int x, y, level;
            in >> x >> y >> level;
            Bond tmp = Bond(atoms[x], atoms[y], level);
            bonds.push_back(tmp);
            atoms[x]->bonds.push_back(tmp);
            atoms[y]->bonds.push_back(tmp);
        }
        in >> textnum;
        texts.clear();
        for (int i=0;i<textnum;i++){
            double x, y;
            QString text;
            in >> x >> y >> text;
            texts.push_back(QPair(QPointF(x, y), text));
        }
        File = "";
        int i = filepath.length() - 1;
        while(i >= 0 && filepath[i] != '/'){
            File = filepath[i] + File;
            i --;
        }
        FilePath = filepath;
        saved = 1;
        update();
        file.close();
    }
    else if (filepath != ""){
        QMessageBox::critical(this, tr("错误"), tr("文件打开错误!"));
    }
}

void Widget::Copy(){
    // 复制功能
    if (!Draged_atoms.empty() || !Draged_bonds.empty() || !Draged_texts.empty()) {
        // 清空之前的复制内容
        qDeleteAll(copiedAtoms);
        copiedAtoms.clear();
        copiedBonds.clear();
        copiedTexts.clear();
        // 保存复制位置（框选的左上角）
        copyPosition = rect.topLeft();
        // 复制选中的原子
        for (Atom* atom : Draged_atoms) {
            copiedAtoms.append(new Atom(*atom));
        }
        // 复制选中的键
        for (Bond* bond : Draged_bonds) {
            // 需要找到对应的复制后的原子
            Atom* newAtom1 = nullptr;
            Atom* newAtom2 = nullptr;
            for (int i = 0; i < Draged_atoms.size(); i++) {
                if (*Draged_atoms[i] == *bond->atom1) {
                    newAtom1 = copiedAtoms[i];
                }
                if (*Draged_atoms[i] == *bond->atom2) {
                    newAtom2 = copiedAtoms[i];
                }
            }
            if (newAtom1 && newAtom2) {
                copiedBonds.append(Bond(newAtom1, newAtom2, bond->level));
            }
        }
        // 复制选中的文本
        for (int textId : Draged_texts) {
            copiedTexts.append(texts[textId]);
        }
        // 清空已框选数组
        Draged_atoms.clear();
        Draged_bonds.clear();
        Draged_texts.clear();
        update();
    }
}

void Widget::Paste(){
    // 粘贴功能
    if (!copiedAtoms.empty() || !copiedTexts.empty()) {
        // 计算粘贴位置（原位置右下角固定距离）
        const QPointF pasteOffset(30, 30);  // 固定偏移量
        QPointF pastePosition = copyPosition + pasteOffset;
        // 检查是否会超出画板
        bool outOfBound = false;
        QRectF widgetRect(0, 0, width(), height());
        // 检查所有原子和文本是否会超出边界
        for (Atom* atom : copiedAtoms) {
            QPointF newPos = pastePosition + (*atom - copyPosition);
            if (!widgetRect.contains(newPos)) {
                outOfBound = true;
                break;
            }
        }
        for (const auto& text : copiedTexts) {
            QPointF newPos = pastePosition + (text.first - copyPosition);
            if (!widgetRect.contains(newPos)) {
                outOfBound = true;
                break;
            }
        }
        if (outOfBound) {
            QMessageBox::warning(this, "警告", "粘贴内容将超出画板边界");
            return;
        }
        // 执行粘贴操作
        Save();
        // 计算偏移量
        QPointF offset = pastePosition - copyPosition;
        // 粘贴原子
        QVector<Atom*> newAtoms;
        for (Atom* atom : copiedAtoms) {
            Atom* newAtom = new Atom(*atom + offset);
            newAtoms.append(newAtom);
            atoms.append(newAtom);
        }
        // 粘贴键
        for (const Bond& bond : copiedBonds) {
            // 找到对应的新原子
            Atom* newAtom1 = nullptr;
            Atom* newAtom2 = nullptr;
            for (int i = 0; i < copiedAtoms.size(); i++) {
                if (*copiedAtoms[i] == *bond.atom1) {
                    newAtom1 = newAtoms[i];
                }
                if (*copiedAtoms[i] == *bond.atom2) {
                    newAtom2 = newAtoms[i];
                }
            }
            if (newAtom1 && newAtom2) {
                Bond newBond(newAtom1, newAtom2, bond.level);
                bonds.append(newBond);
                newAtom1->bonds.append(newBond);
                newAtom2->bonds.append(newBond);
            }
        }
        // 粘贴文本
        for (const auto& text : copiedTexts) {
            QPair<QPointF, QString> newText(text.first + offset, text.second);
            texts.append(newText);
        }
        // 将框选部分替换为刚粘贴的部分
        Draged_atoms = newAtoms;
        Draged_bonds.clear();
        for (int i = bonds.size() - copiedBonds.size(); i < bonds.size(); i++) {
            Draged_bonds.append(&bonds[i]);
        }
        Draged_texts.clear();
        for (int i = texts.size() - copiedTexts.size(); i < texts.size(); i++) {
            Draged_texts.append(i);
        }
        if (!newAtoms.empty() || !copiedTexts.empty()) {
            ModifyRect(newAtoms, Draged_texts);
            // 更新粘贴位置，以便下次粘贴时使用
            copyPosition = pastePosition;
        }
        //int tmp = m_isDraging;
        m_isDraging = 2;
        update();
        //m_isDraging = tmp;
    }
}

MyButton::MyButton(QWidget *parent):QPushButton(parent) {

}

MyButton::~MyButton(){

}

void ToolWidget::buildconnect(MyButton* button, Widget* window){
    connect(button, &QPushButton::clicked, this, [=](){
        ChangeMode(button->buttonmode, button);
        if (button->buttonmode == 0) window->setCursor(Qt::OpenHandCursor);
        else if (button->buttonmode == 5) window->setCursor(Qt::IBeamCursor);
        else {
            window->setCursor(Qt::ArrowCursor);
        }
    });
}

int MyButton::bondlevel = 1;
int MyButton::mode = 2;
QString MainWindow::Path = "";
MyButton* ToolWidget::LastButton = nullptr;
// QVector<Bond> Widget::bonds;
// QVector<Atom*> Widget::atoms;
