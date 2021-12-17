#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Btn_LoadData_clicked();
    void on_Btn_OpenImg_clicked();
    void on_Btn_SetHeight_clicked();
    void on_Btn_SetWidth_clicked();
    void myMoveEvent(QMouseEvent*);

private:
    void initUI();
    void initConnect();
    void initPlot();

    Ui::MainWindow *ui;
    QLabel *m_status = nullptr;
};
#endif // MAINWINDOW_H
