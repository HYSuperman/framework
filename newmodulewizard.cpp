#include "newmodulewizard.h"
#include <QtWidgets>
#include <QXmlStreamWriter>
#include <QFile>
#include <stdlib.h>

newmodulewizard::newmodulewizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new moduleIntroPage);
    addPage(new moduleInfoPage);
//    sourceRegistersPage *srp1 = new sourceRegistersPage(1);
    sr = new sourceRegistersPage(sourceRegistersPage::SOURCE);
    addPage(sr);
//    sourceRegistersPage *srp2 = new sourceRegistersPage(2);
    sk = new sourceRegistersPage(sourceRegistersPage::SINK);
    addPage(sk);
    addPage(new notesPage);

    connect(this->page(2), SIGNAL(saveme(int)), this, SLOT(saveRegisters(int)));
    connect(this->page(3), SIGNAL(saveme(int)), this, SLOT(saveRegisters(int)));

    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(tr("New Module Wizard"));
}

void newmodulewizard::accept(){
    QFile *file = new QFile("output.xml");
    writer = new QXmlStreamWriter(file);
    if(!file->open(QIODevice::Truncate | QIODevice::ReadWrite))
        return;
    writer->setAutoFormatting(true);
    writer->writeStartDocument();
    writer->writeStartElement("module");
    writer->writeStartElement("module_name");
    //writer->writeAttribute("timescale", "1ns/1ps");   Cut in the newer version
    writer->writeCharacters(field("modulename").toString());
    writer->writeEndElement();
    writer->writeStartElement("timescale");
    writer->writeCharacters("1ns/1ps");
    writer->writeEndElement();
    writer->writeStartElement("notes");
    QString noteString(field("notes").toString());
    QStringList notes = noteString.split("\n");
    for(int i = 0;i < notes.size();i++){
        writer->writeTextElement("note", notes.at(i));
    }
    writer->writeEndElement();
    writer->writeStartElement("source");
    writeRegisterXml(sr_control_R, CONTROL);
    writeRegisterXml(sr_state_R, STATE);
    writeRegisterXml(sr_modulec_R, MODULE);
    writer->writeTextElement("FIFO_port_width", field("sr_fifo_width").toString());
    writer->writeEndElement();
    writer->writeStartElement("sink");
    writeRegisterXml(sk_control_R, CONFIRM);
    writeRegisterXml(sk_state_R, DEBUG);
    writeRegisterXml(sk_modulec_R, MODULE);
    writer->writeTextElement("FIFO_port_width", field("sk_fifo_width").toString());
    writer->writeEndElement();
    writer->writeEndElement();
    writer->writeEndDocument();

    file->close();
    QString s("output.xml");
    emit f(s);

    QDialog::accept();
}

void newmodulewizard::writeRegisterXml(QMap<QString, int> m, rType t){
    if(t == CONTROL)
        writer->writeStartElement("control_regs");
    else if(t == STATE)
        writer->writeStartElement("state_regs");
    else if(t == MODULE)
        writer->writeStartElement("module_commutation_regs");
    else if(t == CONFIRM)
        writer->writeStartElement("confirm_regs");
    else if(t == DEBUG)
        writer->writeStartElement("debug_regs");

    QMapIterator<QString, int> i(m);
    while(i.hasNext()){
        i.next();
        writer->writeStartElement("name");
        char v[10];
        sprintf(v, "%d", i.value());
        QString val(v);
        writer->writeAttribute("width", val);
        writer->writeCharacters(i.key());
        writer->writeEndElement();
    }
    writer->writeEndElement();

}

void newmodulewizard::saveRegisters(int ty){
    if(ty == 0){
    for(int i = 0;i < sr->controlTable->rowCount();i++)
        for(int j = 0;j < sr->controlTable->columnCount();j++)
        {
            if(sr->controlTable->item(i, j) != 0 && sr->controlTable->item(i, j+1) != 0 && j == 0){
                if(!sr->controlTable->item(i, j)->text().isEmpty() && !sr->controlTable->item(i, j + 1)->text().isEmpty()){
                QString name = sr->controlTable->item(i, j)->text();
                int width = sr->controlTable->item(i, j+1)->text().toInt();
                sr_control_R.insert(name, width);
                }
            }
        }
    for(int i = 0;i < sr->stateTable->rowCount();i++)
        for(int j = 0;j < sr->stateTable->columnCount();j++)
        {
            if(sr->stateTable->item(i, j) != 0 && sr->stateTable->item(i, j+1) != 0 && j == 0){
                if(!sr->stateTable->item(i, j)->text().isEmpty() && !sr->stateTable->item(i, j + 1)->text().isEmpty()){
                QString name = sr->stateTable->item(i, j)->text();
                int width = sr->stateTable->item(i, j+1)->text().toInt();
                sr_state_R.insert(name, width);
                }
            }
        }
    for(int i = 0;i < sr->moduleCommutationTable->rowCount();i++)
        for(int j = 0;j < sr->moduleCommutationTable->columnCount();j++)
        {
            if(sr->moduleCommutationTable->item(i, j) != 0 && sr->moduleCommutationTable->item(i, j+1) != 0 && j == 0){
                if(!sr->moduleCommutationTable->item(i, j)->text().isEmpty() && !sr->moduleCommutationTable->item(i, j + 1)->text().isEmpty()){
                QString name = sr->moduleCommutationTable->item(i, j)->text();
                int width = sr->moduleCommutationTable->item(i, j+1)->text().toInt();
                sr_modulec_R.insert(name, width);
                }
            }
        }

//    QMapIterator<QString, int> i(sr_control_R);
//    while(i.hasNext()){
//        i.next();
//        qDebug() << qPrintable("Source Controls: " + i.key() + " ") << i.value();
//    }
//    QMapIterator<QString, int> j(sr_state_R);
//    while(j.hasNext()){
//        j.next();
//        qDebug() << qPrintable("Source States: " + j.key() + " ") << j.value();
//    }
//    QMapIterator<QString, int> k(sr_modulec_R);
//    while(k.hasNext()){
//        k.next();
//        qDebug() << qPrintable("Source Modules: " + k.key() + " ") << k.value();
//    }
    }
    else if(ty == 1){
        for(int i = 0;i < sk->controlTable->rowCount();i++)
            for(int j = 0;j < sk->controlTable->columnCount();j++)
            {
                if(sk->controlTable->item(i, j) != 0 && sk->controlTable->item(i, j+1) != 0 && j == 0){
                    if(!sk->controlTable->item(i, j)->text().isEmpty() && !sk->controlTable->item(i, j + 1)->text().isEmpty()){
                    QString name = sk->controlTable->item(i, j)->text();
                    int width = sk->controlTable->item(i, j+1)->text().toInt();
                    sk_control_R.insert(name, width);
                    }
                }
            }
        for(int i = 0;i < sk->stateTable->rowCount();i++)
            for(int j = 0;j < sk->stateTable->columnCount();j++)
            {
                if(sk->stateTable->item(i, j) != 0 && sk->stateTable->item(i, j+1) != 0 && j == 0){
                    if(!sk->stateTable->item(i, j)->text().isEmpty() && !sk->stateTable->item(i, j + 1)->text().isEmpty()){
                    QString name = sk->stateTable->item(i, j)->text();
                    int width = sk->stateTable->item(i, j+1)->text().toInt();
                    sk_state_R.insert(name, width);
                    }
                }
            }
        for(int i = 0;i < sk->moduleCommutationTable->rowCount();i++)
            for(int j = 0;j < sk->moduleCommutationTable->columnCount();j++)
            {
                if(sk->moduleCommutationTable->item(i, j) != 0 && sk->moduleCommutationTable->item(i, j+1) != 0 && j == 0){
                    if(!sk->moduleCommutationTable->item(i, j)->text().isEmpty() && !sk->moduleCommutationTable->item(i, j + 1)->text().isEmpty()){
                    QString name = sk->moduleCommutationTable->item(i, j)->text();
                    int width = sk->moduleCommutationTable->item(i, j+1)->text().toInt();
                    sk_modulec_R.insert(name, width);
                    }
                }
            }
//        QMapIterator<QString, int> i(sk_control_R);
//        while(i.hasNext()){
//            i.next();
//            qDebug() << qPrintable("Sink Controls: " + i.key() + " ") << i.value();
//        }
//        QMapIterator<QString, int> j(sk_state_R);
//        while(j.hasNext()){
//            j.next();
//            qDebug() << qPrintable("Sink States: " + j.key() + " ") << j.value();
//        }
//        QMapIterator<QString, int> k(sk_modulec_R);
//        while(k.hasNext()){
//            k.next();
//            qDebug() << qPrintable("Sink Modules: " + k.key() + " ") << k.value();
//        }

    }
}


moduleIntroPage::moduleIntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction Page"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(""));

    label = new QLabel(tr("This is new module wizard in GRT"));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}


moduleInfoPage::moduleInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
        setTitle(tr("Module Information"));
        setSubTitle(tr("Specify basic information about the module from which you"
                       "want to generate xml codes."));
        setPixmap(QWizard::LogoPixmap, QPixmap(""));

        moduleNameLabel = new QLabel("Module Name:");
        moduleNameLineEdit = new QLineEdit;
        moduleNameLabel->setBuddy(moduleNameLineEdit);

//        timescaleLabel = new QLabel(tr("Timescale:"));
//        timescaleLineEdit = new QLineEdit;
//        timescaleLabel->setBuddy(timescaleLineEdit);

        registerField("modulename*", moduleNameLineEdit);
//        registerField("timescale*", timescaleLineEdit);

        QGridLayout *layout = new QGridLayout;
        layout->addWidget(moduleNameLabel, 0, 0);
        layout->addWidget(moduleNameLineEdit, 0, 1);
//        layout->addWidget(timescaleLabel, 1, 0);
//        layout->addWidget(timescaleLineEdit, 1, 1);
        setLayout(layout);
}

sourceRegistersPage::sourceRegistersPage(Type type, QWidget *parent)
    : QWizardPage(parent)
{
    t = type;
    if(t == SOURCE){
    setTitle(tr("Source Register Information"));
    setSubTitle(tr("Source Registers"));
    }
    else if(t == SINK){
    setTitle(tr("Sink Register Information"));
    setSubTitle(tr("Sink Registers"));
    }
    setPixmap(QWizard::LogoPixmap, QPixmap(""));

    QStringList *hstrings = new QStringList();
    hstrings->append("Name");
    hstrings->append("Width");

    controlRLabel = new QLabel("Control Registers:");
    if(t == SINK)
        controlRLabel->setText("Confirm Registers:");
    addcontrol = new QPushButton("+");
    deletecontrol = new QPushButton("-");
    QVBoxLayout *clayout = new QVBoxLayout;
    clayout->addWidget(controlRLabel);
    clayout->addWidget(addcontrol);
    clayout->addWidget(deletecontrol);

    controlTable = new QTableWidget(3, 2, this);
    controlTable->setColumnWidth(1, 200);
    controlTable->setHorizontalHeaderLabels(*hstrings);
    //qDebug() << controlTable->columnWidth(1);
    controlRLabel->setBuddy(controlTable);

    stateRLabel = new QLabel("State Registers:");
    if(t == SINK)
        stateRLabel->setText("Debug Registers:");
    addstate = new QPushButton("+");
    deletestate = new QPushButton("-");
    QVBoxLayout *slayout = new QVBoxLayout;
    slayout->addWidget(stateRLabel);
    slayout->addWidget(addstate);
    slayout->addWidget(deletestate);
    stateTable = new QTableWidget(3, 2, this);
    stateTable->setColumnWidth(1, 200);
    stateTable->setHorizontalHeaderLabels(*hstrings);
    //qDebug() << stateTable->columnWidth(1);
    stateRLabel->setBuddy(stateTable);

    moduleCommutationRLabel = new QLabel("Module Commutation Registers:");
    addmoduleCommutation = new QPushButton("+");
    deletemoduleCommutation = new QPushButton("-");
    QVBoxLayout *mlayout = new QVBoxLayout;
    mlayout->addWidget(moduleCommutationRLabel);
    mlayout->addWidget(addmoduleCommutation);
    mlayout->addWidget(deletemoduleCommutation);
    moduleCommutationTable = new QTableWidget(3, 2, this);
    moduleCommutationTable->setColumnWidth(1, 200);
    moduleCommutationTable->setHorizontalHeaderLabels(*hstrings);
    //qDebug() << stateTable->columnWidth(1);
    moduleCommutationRLabel->setBuddy(moduleCommutationTable);

    fifo = new QLabel("FIFO Width: ");
    fifoLine = new QLineEdit;
    fifo->setBuddy(fifoLine);

    if(t == SOURCE)
        registerField("sr_fifo_width", fifoLine);
    else if(t == SINK)
        registerField("sk_fifo_width", fifoLine);

//    if(type == 1){
//        for(int i = 0;i < controlTable->rowCount();i++)
//            for(int j = 0;j < controlTable->columnCount();j++){
//                char nameid[5];
//                sprintf(nameid,"%d",i);
//                QString id(nameid);
//                QString rname = "sr_c_r" + id;
//                QTableWidgetItem *item = controlTable->item(i, j);
//                registerField(rname, item, "text", SIGNAL(textChanged()));
//            }
//    }
//    else if(type == 2){
//    }

    QGridLayout *layout = new QGridLayout;
    layout->addLayout(clayout, 0, 0, 1, 1);
    //layout->addWidget(controlRLabel, 0, 0);
    layout->addWidget(controlTable, 0, 1, 1, 2);
    layout->addLayout(slayout, 1, 0, 1, 1);
    //layout->addWidget(stateRLabel, 1, 0);
    layout->addWidget(stateTable, 1, 1, 1, 2);
    layout->addLayout(mlayout, 2, 0, 1, 1);
    //layout->addWidget(moduleCommutationRLabel, 2, 0);
    layout->addWidget(moduleCommutationTable, 2, 1, 1, 2);
//    layout->addWidget(timescaleLabel, 1, 0);
//    layout->addWidget(timescaleLineEdit, 1, 1);
    layout->addWidget(fifo, 3, 0);
    layout->addWidget(fifoLine, 3, 1, 1, 2);
    setLayout(layout);

//    if(wizard() == NULL){
//        qDebug("afdf");
//    }

    connect(addcontrol, SIGNAL(clicked()), this, SLOT(addb()));
    connect(addstate, SIGNAL(clicked()), this, SLOT(addb()));
    connect(addmoduleCommutation, SIGNAL(clicked()), this, SLOT(addb()));
    connect(deletecontrol, SIGNAL(clicked()), this, SLOT(deleteb()));
    connect(deletestate, SIGNAL(clicked()), this, SLOT(deleteb()));
    connect(deletemoduleCommutation, SIGNAL(clicked()), this, SLOT(deleteb()));

    connect(this, SIGNAL(addcontrolr(int)), controlTable, SLOT(insertRow(int)));
    connect(this, SIGNAL(addstater(int)), stateTable, SLOT(insertRow(int)));
    connect(this, SIGNAL(addmcommutationr(int)), moduleCommutationTable, SLOT(insertRow(int)));
    connect(this, SIGNAL(deletecontrolr(int)), controlTable, SLOT(removeRow(int)));
    connect(this, SIGNAL(deletestater(int)), stateTable, SLOT(removeRow(int)));
    connect(this, SIGNAL(deletecommutationr(int)), moduleCommutationTable, SLOT(removeRow(int)));
}

bool sourceRegistersPage::validatePage(){
    if(t == SOURCE)
        emit saveme(0);
    else if(t == SINK)
        emit saveme(1);
    return true;
}

void sourceRegistersPage::addb(){
    QObject *obj = sender();
    if(obj == addcontrol){
        QItemSelectionModel *s = controlTable->selectionModel();
        s->hasSelection();
        QModelIndexList l = s->selectedRows();
        emit addcontrolr(controlTable->rowCount());
    }
    else if(obj == addstate){
        emit addstater(stateTable->rowCount());
    }
    else if(obj == addmoduleCommutation){
        emit addmcommutationr(moduleCommutationTable->rowCount());
    }
}

void sourceRegistersPage::deleteb(){
    QObject *obj = sender();
    if(obj == deletecontrol){
        QItemSelectionModel *s = controlTable->selectionModel();
        s->hasSelection();
        QModelIndexList l = s->selectedRows();
        QListIterator<QModelIndex> i(l);
        while(i.hasNext()){
            int d = i.next().row();
            emit deletecontrolr(d);
        }
    }
    else if(obj == deletestate){
        QItemSelectionModel *s = stateTable->selectionModel();
        s->hasSelection();
        QModelIndexList l = s->selectedRows();
        QListIterator<QModelIndex> i(l);
        while(i.hasNext()){
            int d = i.next().row();
            emit deletestater(d);
        }
    }
    else if(obj == deletemoduleCommutation){
        QItemSelectionModel *s = moduleCommutationTable->selectionModel();
        s->hasSelection();
        QModelIndexList l = s->selectedRows();
        QListIterator<QModelIndex> i(l);
        while(i.hasNext()){
            int d = i.next().row();
            emit deletecommutationr(d);
        }
    }
}

notesPage::notesPage(QWidget *parent)
    : QWizardPage(parent){
    setTitle(tr("Comments"));
    noteslabel = new QLabel("Comments:");
    notesText = new QTextEdit;
    notesText->setText("Company:\nEngineer:\nCreate Date:\nDesign Name:\nModule Name:\n\
Project Name:\nTarget Devices:\nTool versions:\nDescription:\nTool versions:\nDependencies:\n\
Revision:\nAdditional Comments:");

    registerField("notes", notesText, "plainText", SIGNAL(textChanged()));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(noteslabel);
    layout->addWidget(notesText);
    setLayout(layout);
}
