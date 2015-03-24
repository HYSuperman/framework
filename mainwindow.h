#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
class grtTextEdit;
class newmodulewizard;
class register_attr;

class mainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit mainWindow(/*QWidget *parent = 0*/);
    ~mainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private:
    void createActions();
    void createMenus();
    void createWidgets();
    bool okToContinue();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QStringList recentFiles;
    QString curFile;
    grtTextEdit *textEdit;

    /* QMap used to store the parsed module_name, timescale, notes, include_name
     * for example
     * key: module_name value: abc
     * key: timescale value: 1ns/1ps
     */
    QMap<QString, QString> values;


    /* QMap used to store the parsed registes.
     * For example
     *                                          name:abc
     *                                         /
     * key: control_regs value: (register_attr)
     *                                         \
     *                                          width:32
     */
    QMap<QString, register_attr> registerValues;
    QMap<QString, register_attr> sourceRegisterValues;
    QMap<QString, register_attr> sinkRegisterValues;

    /*
     * menus
     */
    QMenu *projectMenu;
    QMenu *editMenu;
    QMenu *helpMenu;


    /*
     * project menu actions
     */
    QAction *newAction;
    QAction *newModuleAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction *separatorAction;

    /* edit menu actions
     *
     */
    QAction *modifyAction;

    /*
     * tools menu actions
     */
    QAction *topModuleView;
    QAction *moduleView;

    /*
     * help menu actions
     */
    QAction *aboutGRTAction;

    /*
     * dialogs
     */

    newmodulewizard *newmodulewizard1;

signals:
    void parseComplete(QMap<QString, QString> values);
    void parseRegisterComplete(QMap<QString, register_attr> registerValues, int type);
public slots:
    bool loadFile(const QString &filename);
    void setEnableNewModule();
    //bool readXmlFile(const QString &filename);

private slots:
    void newTopModule();
    void newModule();
    void parseOpen();
//    void parseSource();
//    void parseSink();
    bool open();
    bool save();
    bool saveAs();
    //void openRecentFile();
    void about();



};

/* class register_attr is used to store the parsed registers' attributes */
class register_attr{

public:
    register_attr(){}
    register_attr(QString type, QString width)
        :type(type), width(width){}

    void setType(QString type){this->type = type;}
    void setWidth(QString width){this->width = width;}

    QString getType() const{return type;}
    QString getWidth() const{return width;}

private:
    QString type;
    QString width;
};

#endif // mainWindow_H
