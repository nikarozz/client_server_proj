#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT 
private:
    Ui::MainWindow *ui;
    QTcpSocket* socket;
    QByteArray Data;
    quint16 nextBlockSize = 0;
    QString input;
public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void sockReadyRead();
    void sockDisc();
    quint16 port();
private slots: 
    void on_pushButton_clicked();
    void on_sendButton_clicked();
};
#endif // MAINWINDOW_H
