#include "grttextedit.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>

grtTextEdit::grtTextEdit(QWidget* parent):QTextEdit(parent)
{}


bool grtTextEdit::writeFile(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)){
            QMessageBox::warning(this, tr("GRT"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(file.fileName())
                                 .arg(file.errorString()));
            return false;
    }
    QTextStream out(&file);

    //QApplication::setOverrideCursor(Qt::WaitCursor);
    QString text;
    text = this->toPlainText();
    out << text;

    //QApplication::restoreOverrideCursor();
    file.close();
    return true;
}

bool grtTextEdit::readFile(const QString &fileName){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)){
            QMessageBox::warning(this, tr("GRT"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(file.fileName())
                                 .arg(file.errorString()));
            return false;
    }
    QTextStream in(&file);
    QString line;
    this->clear();

    //QApplication::setOverrideCursor(Qt::WaitCursor);

    while(!in.atEnd()){
        line = in.readLine();
        QString text = this->toPlainText() + line + '\n';
        this->setPlainText(text);
    }
    //QApplication::restoreOverrideCursor();
    file.close();
    return true;
}
