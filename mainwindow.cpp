#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "procs.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    procs *p = openProcs();

    if (!p)
        ui->textEdit->setText("Error");

    proc pr;

    while (nextProc(p, pr)) {
        QString s;

        s += QString::number(pr.pid);
        s += " ";

        if (pr.st_size > 0) {
            for (status *st = pr.st; st->next != NULL; st = (status*) st->next) {
                s += "Key: ";
                s += st->kv.key;
                s += + " ";
                s += "Value: ";
                s += st->kv.value;
                s += "\n";
            }
        }

        ui->textEdit->append(s);
    }
}
