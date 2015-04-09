#ifndef EDITMODULEWIZARD_H
#define EDITMODULEWIZARD_H

#include <QWizard>
#include <QMap>
#include "mainwindow.h"
#include "newmodulewizard.h"
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTextEdit;
QT_END_NAMESPACE
class sourceRegistersPageEdit;

/*
 * editModuleWizard is a wizard for reading the existed module
 * and modify it and save the changes
 */
class editModuleWizard : public QWizard
{
    Q_OBJECT
public:
    editModuleWizard(QWidget *parent = 0);
    ~editModuleWizard();
private:
    sourceRegistersPageEdit *source;
    sourceRegistersPageEdit *sink;
};


/*
 * The first page of the modify page wizard
 */
class infoPage:public QWizardPage{
    Q_OBJECT
public:
    infoPage(QWidget *parent = 0);

    QLabel *moduleNameLabel;
    QLineEdit *moduleNameLineEdit;

    QLabel *timeScaleLabel;
    QLineEdit *timeScaleLineEdit;

    QLabel *noteLabel;
    QTextEdit *noteTextEdit;

private:

public slots:
    void revealValues(QMap<QString, QString> values);
};


class sourceRegistersPageEdit:public sourceRegistersPage{
    Q_OBJECT
public:
    sourceRegistersPageEdit(sourceRegistersPage::Type t, QWidget *parent = 0);

public slots:
    void revealRegisters(QMap<QString, register_attr> registerValues, int type);

private:
    void fillRegisters(QMapIterator<QString, register_attr> i, QTableWidget *table);
};

#endif // EDITMODULEWIZARD_H
