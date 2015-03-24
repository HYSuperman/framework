#ifndef GRTTEXTEDIT_H
#define GRTTEXTEDIT_H

#include <QTextEdit>


class grtTextEdit:public QTextEdit
{
public:
    grtTextEdit(QWidget *parent = 0);
    bool writeFile(const QString &fileName);
    bool readFile(const QString &fileName);
};

#endif // GRTTEXTEDIT_H

