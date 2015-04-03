#include "mainwindow.h"
#include "grttextedit.h"
//#include "domparser.h"


#include <QtWidgets>

#include "newprowizard.h"
#include "newmodulewizard.h"
#include "editmodulewizard.h"

mainWindow::mainWindow(){
    createActions();
    createMenus();
    createWidgets();
    setMinimumSize(1000, 700);
    //setGeometry(0, 0, 1650, 1000);
    setWindowState(Qt::WindowMaximized);
    setCurrentFile("");
}

void mainWindow::closeEvent(QCloseEvent *event){
        event->accept();
}


void mainWindow::createActions(){

    newAction = new QAction("&New Top Module", this);
    newAction ->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newTopModule()));

    newModuleAction = new QAction("New Module", this);
    connect(newModuleAction, SIGNAL(triggered()), this, SLOT(newModule()));
    newModuleAction->setEnabled(true);

    openAction = new QAction("&Open...", this);
    openAction ->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction("&Save", this);
    saveAction ->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction("&Save as...", this);
    saveAsAction ->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    modifyAction = new QAction(tr("&Modify a Module"), this);
    modifyAction->setShortcut(tr("Ctrl+M"));
    connect(modifyAction, SIGNAL(triggered()), this, SLOT(parseOpen()));


    topModuleView = new QAction(tr("Top Module View"), this);

    moduleView = new QAction(tr("Module View"), this);

    newmodulewizard1 = new newmodulewizard;
    aboutGRTAction = new QAction(tr("About &GRT"), this);
    aboutGRTAction->setStatusTip(tr("Show the GRT about"));
    connect(aboutGRTAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(newmodulewizard1, SIGNAL(f(QString)), this, SLOT(loadFile(QString)));

}

void mainWindow::createWidgets(){
    textEdit = new grtTextEdit;
    setCentralWidget(textEdit);
}

bool mainWindow::loadFile(const QString &filename){
    if(!textEdit->readFile(filename))
        return false;
    //domParser parser(&moduleTreeWidget);
    //parser.readFile(filename);
    //std::cout << qPrintable(moduleTreeWidget.topLevelItem(0)->child(0)->text(0)) << std::endl;
    setCurrentFile(filename);
    return true;
}


void mainWindow::newTopModule(){
    newprowizard *newprowizard1 = new newprowizard;
    newprowizard1->show();
}

void mainWindow::newModule(){
    //newmodulewizard1 = new newmodulewizard;
    newmodulewizard1->show();

}

void mainWindow::parseOpen(){

    editModuleWizard *editmodulewizard = new editModuleWizard;
    editmodulewizard->show();

    connect(this, SIGNAL(parseComplete(QMap<QString,QString>)), editmodulewizard->page(0), SLOT(revealValues(QMap<QString, QString>)));
    connect(this , SIGNAL(parseRegisterComplete(QMap<QString,register_attr>, int)), editmodulewizard->page(1), SLOT(revealRegisters(QMap<QString, register_attr>, int)));
    connect(this , SIGNAL(parseRegisterComplete(QMap<QString,register_attr>, int)), editmodulewizard->page(2), SLOT(revealRegisters(QMap<QString, register_attr>, int)));

    QString input = textEdit->toPlainText();
    QXmlStreamReader *parser = new QXmlStreamReader(input);

    QString note = "";
    values.insert("note", note);

    while(!parser->atEnd() && !parser->hasError()){
        QString pname = parser->name().toString();
        /* Read the next token every time
         * Attention:According to QXmlStreamReader,
         * a token is one of these types: startelement, endelement, text,
         * startdocument or enddocument.
         * the parser moves forward one element every time the readNext() function is called.
         */
        QXmlStreamReader::TokenType token = parser->readNext();

        /* Nothing to be done for a startdocument element */
        if(token == QXmlStreamReader::StartDocument){
            continue;
        }

        /* When meeting a startelement document, parse into it! */
        if(token == QXmlStreamReader::StartElement){
            //pname = parser->name().toString();
            if(parser->name() ==  "module"){
                continue;
            }

            /* First Edition NEED TO BE MODIFIED!!!
             * timescale is no longer an attribute!!!
             * or to be DELETE!
             */
            if(parser->name() == "module_name"){

                QXmlStreamAttributes attributes = parser->attributes();
                QString timevalue = attributes.value("timescale").toString();
                values.insert("timescale", timevalue);

                parser->readNext();
                // module_name should be the same as the xml file name
                values.insert("module_name", parser->text().toString());

                emit parseComplete(values);
                continue;
            }

            if(parser->name() == "notes"){
                continue;
            }

            if(parser->name() == "note"){

                QString tmp = values.value("note");

                /* add all strings together */
                parser->readNext();
                tmp += parser->text().toString() + "\n";

                values.insert("note", tmp);

                emit parseComplete(values);

                continue;
            }

//            if(parser->name() == "FIFO_port_width"){
//                parser->readNext();
//                values.insert("FIFO_port_width", parser->text().toString());
//                continue;
//            }

            if(parser->name() == "source"){
                parser->readNext();
                //pname = parser->name().toString();
                /* Into while until meet </source> */
                while(parser->name() != "source"){
                    //pname = parser->name().toString();
                    parser->readNext();
                    //pname = parser->name().toString();
                    if(parser->name() == "control_regs"){
                        //pname = parser->name().toString();
                        token = parser->readNext();

                        /* Into while until meet </control_regs> */
                        while(parser->name() != "control_regs"){

                            /* Only "name" tag could emerge inside this */
                            if(token == QXmlStreamReader::StartElement){

                                QString registerName;
                                register_attr tmp;

                                tmp.setType("control_regs");
                                tmp.setWidth(parser->attributes().value("width").toString());
                                token = parser->readNext();
                                registerName = parser->text().toString();
                                sourceRegisterValues.insert(registerName, tmp);

                                continue;
                            }

                            token = parser->readNext();
                            continue;
                        }
                        continue;
                    }

                    /* Same as "control_regs, Just look above!" */
                    if(parser->name() == "state_regs"){
                        //pname = parser->name().toString();
                        token = parser->readNext();

                        while(parser->name() != "state_regs"){

                            if(token == QXmlStreamReader::StartElement){
                                QString registerName;
                                register_attr tmp;

                                tmp.setType("state_regs");
                                tmp.setWidth(parser->attributes().value("width").toString());
                                token = parser->readNext();
                                registerName = parser->text().toString();
                                sourceRegisterValues.insert(registerName, tmp);

                                continue;
                            }
                            token = parser->readNext();
                            continue;
                        }
                        continue;
                    }

                    if(parser->name() == "module_commutation_regs"){
                        //pname = parser->name().toString();
                        token = parser->readNext();
                        //pname = parser->name().toString();
                        while(parser->name() != "module_commutation_regs"){
                            //pname = parser->name().toString();
                            if(token == QXmlStreamReader::StartElement){
                                //pname = parser->name().toString();
                                QString registerName;
                                register_attr tmp;

                                tmp.setType("source_module_commutation_regs");
                                tmp.setWidth(parser->attributes().value("width").toString());
                                token = parser->readNext();
                                registerName = parser->text().toString();
                                sourceRegisterValues.insert(registerName, tmp);

                                continue;
                            }
                            token = parser->readNext();
                            continue;
                        }
                        continue;
                    }
                }

                /* Parse source completed */
                emit parseRegisterComplete(sourceRegisterValues, 0);//type 0 for source

                parser->readNext();
                continue;
            }

            if(parser->name() == "sink"){

                parser->readNext();

                while(parser->name() != "sink"){

                    parser->readNext();

                    if(parser->name() == "confirm_regs"){

                        token = parser->readNext();

                        while(parser->name() != "confirm_regs"){
                            if(token == QXmlStreamReader::StartElement){
                                QString registerName;
                                register_attr tmp;

                                tmp.setType("confirm_regs");
                                tmp.setWidth(parser->attributes().value("width").toString());
                                token = parser->readNext();
                                registerName = parser->text().toString();
                                sinkRegisterValues.insert(registerName, tmp);

                                continue;
                            }

                            token = parser->readNext();
                            continue;
                        }
                        continue;
                    }

                    if(parser->name() == "debug_regs"){
                        token = parser->readNext();
                        while(parser->name() != "debug_regs"){
                            if(token == QXmlStreamReader::StartElement){
                                QString registerName;
                                register_attr tmp;
                                tmp.setType("debug_regs");
                                tmp.setWidth(parser->attributes().value("width").toString());
                                token = parser->readNext();
                                registerName = parser->text().toString();
                                sinkRegisterValues.insert(registerName, tmp);
                                continue;
                            }
                            token = parser->readNext();
                            continue;
                        }
                        continue;
                    }

                    if(parser->name() == "module_commutation_regs"){
                        token = parser->readNext();
                        while(parser->name() != "module_commutation_regs"){
                            if(token == QXmlStreamReader::StartElement){
                                qDebug() << qPrintable(parser->name().toString());
                                QString registerName;
                                register_attr tmp;
                                tmp.setType("sink_module_commutation_regs");
                                tmp.setWidth(parser->attributes().value("width").toString());
                                token = parser->readNext();
                                registerName = parser->text().toString();
                                sinkRegisterValues.insert(registerName, tmp);
                                continue;
                            }
                            token = parser->readNext();
                            continue;
                        }
                        continue;
                    }
                }

                emit parseRegisterComplete(sinkRegisterValues, 1);//type 1 for sink
//                QMapIterator<QString, register_attr> i(registerValues);
//                while(i.hasNext()){
//                    i.next();
//                    qDebug() << "name: " << qPrintable(i.key());
//                    qDebug() << "type: " << qPrintable(i.value().getType());
//                    qDebug() << "width: " << qPrintable(i.value().getWidth());
//                }
                parser->readNext();
                continue;
            }
        }
    }
}
   // textEdit


/* Open a file (Mostly user defined xml file) and set the textEdit's text to the file's content*/
bool mainWindow::open(){
    QFileDialog openFileDialog(this);
    openFileDialog.setFileMode(QFileDialog::AnyFile);
    openFileDialog.setViewMode(QFileDialog::Detail);
    QStringList fileNames;
    QString filename;
    if(openFileDialog.exec()){
        fileNames = openFileDialog.selectedFiles();
    }
    if(fileNames.size() != 0){
        filename = fileNames.at(0).toLocal8Bit().constData();
    }
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)){
            QMessageBox::warning(this, tr("GRT"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(file.fileName())
                                 .arg(file.errorString()));
            return false;
    }
    QTextStream in(&file);
    QString line;
    textEdit->clear();

    //QApplication::setOverrideCursor(Qt::WaitCursor);

    while(!in.atEnd()){
        line = in.readLine();
        QString text = textEdit->toPlainText() + line + '\n';
        textEdit->setPlainText(text);
    }

    file.close();
    return true;
}

bool mainWindow::save(){
    if(curFile.isEmpty()){
        return saveAs();
    }else{
        return 0;//saveFile(curFile);
    }
}

bool mainWindow::saveAs(){
    return false;
}

void mainWindow::setEnableNewModule(){
    newModuleAction->setEnabled(true);
}



/*
 * create menus for the menu bar
 */
void mainWindow::createMenus(){
    projectMenu = menuBar()->addMenu(tr("&Project"));
    projectMenu->addAction(newAction);
    projectMenu->addAction(newModuleAction);
    projectMenu->addAction(openAction);
    projectMenu->addAction(saveAction);
    projectMenu->addAction(saveAsAction);
    separatorAction = projectMenu->addSeparator();
    projectMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(modifyAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutGRTAction);
}

void mainWindow::setCurrentFile(const QString &fileName){
    curFile = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if(!curFile.isEmpty()){
        shownName = strippedName(curFile);
    }
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("GRT")));
}

QString mainWindow::strippedName(const QString &fullFileName){
    return QFileInfo(fullFileName).fileName();
}

void mainWindow::about(){
    QMessageBox::about(this, tr("About GRT"),
                       tr("<h2>GRT 1.0</h2>"
                          "<p>Copyright &copy; 2014 RAW."
                          "<p>GRT IS COOL!!!</p>"));
}

mainWindow::~mainWindow()
{}
