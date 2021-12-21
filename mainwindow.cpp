#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_status(new QLabel)
{
    ui->setupUi(this);

    initUI();
    initPlot();
    initConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    //setWindowTitle(AppName);
    ui->statusbar->addWidget(m_status);
    setWindowFlags(Qt::WindowCloseButtonHint);
    resize(745, 510);
}

void MainWindow::initPlot()
{
    // 设置坐标范围、坐标名称
    ui->QCPlot->resize(width(), height());
    ui->QCPlot->xAxis->setLabel("水平 （m）");
    ui->QCPlot->yAxis->setLabel("垂直 （m）");

    // 设置画笔风格
    QPen drawPen;
    drawPen.setColor(Qt::green);
    drawPen.setWidth(4);
    drawPen.setStyle(Qt::PenStyle::DashDotDotLine);

    // 绘制散点
    QCPGraph * curGraph = ui->QCPlot->addGraph();
    curGraph->setPen(drawPen);
    curGraph->setLineStyle(QCPGraph::lsNone);
    curGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

    ui->QCPlot->xAxis->setVisible(true);
    ui->QCPlot->yAxis->setVisible(true);
}

void MainWindow::initConnect()
{
    connect(ui->QCPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(myMoveEvent(QMouseEvent*)));
}

void MainWindow::myMoveEvent(QMouseEvent* me)
{
    //获取鼠标坐标，相对父窗体坐标
    int x_pos = me->pos().x();
    int y_pos = me->pos().y();

    //鼠标坐标转化为CustomPlot内部坐标
    double x_val = ui->QCPlot->xAxis->pixelToCoord(x_pos);
    double y_val = ui->QCPlot->yAxis->pixelToCoord(y_pos);

    bool isShow = false;
    int index = 0;
    auto data = ui->QCPlot->graph(0)->data();
    for (int i = 0; i < ui->QCPlot->graph(0)->dataCount(); ++i){
        if (x_val <= data->at(i)->key + 3
                &&x_val > data->at(i)->key - 3
                && y_val <= data->at(i)->value + 3
                && y_val > data->at(i)->value - 3){
            index = i;
            isShow = true;
            break;
        }
    }

    if (isShow){
        QString strToolTip = QString("x: %1\ny: %2").arg(QString::number(data->at(index)->key, 10, 2),
                                                         QString::number(data->at(index)->value, 10, 2));
        QToolTip::showText(cursor().pos(), strToolTip, ui->QCPlot);
    }
    else
        QToolTip::hideText();
}

void MainWindow::on_Btn_SetHeight_clicked()
{
    auto num = ui->LE_Height->text().toDouble();
    ui->QCPlot->yAxis->setRange(-num, num);
    ui->QCPlot->replot();
}

void MainWindow::on_Btn_SetWidth_clicked()
{
    auto num = ui->LE_Width->text().toDouble();
    ui->QCPlot->xAxis->setRange(-num, num);
    ui->QCPlot->replot();
}

void MainWindow::on_Btn_LoadData_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("open a file."),"./",tr("file(*.txt);"));
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "Failed to open the DataFile!");
    }
    else {
        // 数据读取
        QStringList _list;
        QFile _file(fileName);
        if (_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            _file.seek(0);
            QTextStream _stream(&_file);
            while(!_file.atEnd()){
                _list = _stream.readAll().split("\n");
            }
        }
        _file.close();

        auto xlist = _list[0].split(",");
        auto ylist = _list[1].split(",");

        QVector<double> x, y;
        foreach(auto val, xlist)
            x << val.toDouble();
        foreach(auto val, ylist)
            y << val.toDouble();

//        x << -75 << -50 << -50 << 0 << 50 << 100 << 75;
//        y << -75 << -50 << -25 << 0 << 25 << 50 << 75;
        ui->QCPlot->graph(0)->setData(x, y);
        ui->QCPlot->replot();
    }
}

void MainWindow::on_Btn_OpenImg_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("open a file."),"./",tr("images(*.png *jpg *bmp);"));
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "Failed to open the Image!");
    }
    else {
        ui->QCPlot->setBackground(QPixmap(fileName));
        ui->QCPlot->setBackgroundScaledMode(Qt::IgnoreAspectRatio);
    }
}
