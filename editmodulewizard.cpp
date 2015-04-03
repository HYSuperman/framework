#include "editmodulewizard.h"
//#include "newmodulewizard.h"
//#include "mainwindow.h"
#include <QtWidgets>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QTextEdit>
#include <QDebug>
#include <QTableWidgetItem>

editModuleWizard::editModuleWizard(QWidget *parent)
    :QWizard(parent)
{
    addPage(new infoPage);
    source = new sourceRegistersPageEdit(sourceRegistersPage::SOURCE);
    addPage(source);
    sink = new sourceRegistersPageEdit(sourceRegistersPage::SINK);
    addPage(sink);

    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(tr("Edit Module Wizard"));
}

editModuleWizard::~editModuleWizard()
{}

infoPage::infoPage(QWidget *parent)
    :QWizardPage(parent){
    setTitle(tr("Module Information"));
    setSubTitle(tr("Specify basic information about the module from which you"
                   "want to generate xml codes."));
    setPixmap(QWizard::LogoPixmap, QPixmap(""));

    moduleNameLabel = new QLabel("Module Name:");
    moduleNameLineEdit = new QLineEdit;
    moduleNameLabel->setBuddy(moduleNameLineEdit);

    timeScaleLabel = new QLabel("Time Scale: ");
    timeScaleLineEdit = new QLineEdit;
    timeScaleLabel->setBuddy(timeScaleLineEdit);

    noteLabel = new QLabel("Notes: ");
    noteTextEdit = new QTextEdit;
    noteLabel->setBuddy(noteTextEdit);
//        timescaleLabel = new QLabel(tr("Timescale:"));
//        timescaleLineEdit = new QLineEdit;
//        timescaleLabel->setBuddy(timescaleLineEdit);

    registerField("modulename*", moduleNameLineEdit);
//        registerField("timescale*", timescaleLineEdit);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(moduleNameLabel, 0, 0);
    layout->addWidget(moduleNameLineEdit, 0, 1);
    layout->addWidget(timeScaleLabel, 1, 0);
    layout->addWidget(timeScaleLineEdit, 1, 1);
    layout->addWidget(noteLabel, 2, 0);
    layout->addWidget(noteTextEdit, 2, 1);
    setLayout(layout);
}

void infoPage::revealValues(QMap<QString, QString> values){
    if(values.contains("module_name")){
        moduleNameLineEdit->setText(values.value("module_name"));
    }
    if(values.contains("timescale")){
        timeScaleLineEdit->setText(values.value("timescale"));
    }
    if(values.contains("note")){
        noteTextEdit->setText(values.value("note"));
    }
}


/*sourceRegistersPageEdit inherits sourceRegistersPage, this class is used to display the parsed xml and modify it */
sourceRegistersPageEdit::sourceRegistersPageEdit(sourceRegistersPage::Type t, QWidget *parent)
    :sourceRegistersPage(t, parent){
}

/* Type 0 for source and type 1 for sink */
void sourceRegistersPageEdit::revealRegisters(QMap<QString, register_attr> registerValues, int type){

    if(type == 0 && t == sourceRegistersPage::SINK)
        return;
    if(type == 1 && t == sourceRegistersPage::SOURCE)
        return;
    QMapIterator<QString, register_attr> i(registerValues);

    controlTable->setCurrentCell(0, 0);
    stateTable->setCurrentCell(0, 0);
    moduleCommutationTable->setCurrentCell(0, 0);

    while(i.hasNext()){
        i.next();
        if(i.value().getType() == "control_regs" || i.value().getType() == "confirm_regs"){
            fillRegisters(i, controlTable);
            continue;
        }
        if(i.value().getType() == "state_regs" || i.value().getType() == "debug_regs"){
            fillRegisters(i, stateTable);
            continue;
        }
        if(i.value().getType() == "source_module_commutation_regs" || i.value().getType() == "sink_module_commutation_regs"){
            fillRegisters(i, moduleCommutationTable);
            continue;
        }
    }

}

void sourceRegistersPageEdit::fillRegisters(QMapIterator<QString, register_attr> i, QTableWidget *table){
//    qDebug("Filling in register tables...");
//    qDebug() << qPrintable(i.key());
//    qDebug() << qPrintable(i.value().getWidth());
    QTableWidgetItem *tmp1 = new QTableWidgetItem(i.key());
    QTableWidgetItem *tmp2 = new QTableWidgetItem(i.value().getWidth());

    table->setItem(table->currentRow(), table->currentColumn(), tmp1);
    table->setItem(table->currentRow(), table->currentColumn()+1, tmp2);
    table->setCurrentCell(table->currentRow()+1, 0);
    //qDebug("current:");
    if(table->currentRow() == -1 && table->currentColumn() == -1){
        table->insertRow(table->rowCount());
        table->setCurrentCell(table->rowCount() - 1, 0);
    }
    //qDebug() << table->currentRow();
    //qDebug() << table->currentColumn();
}
