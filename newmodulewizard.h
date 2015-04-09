#ifndef NEWMODULEWIZARD_H
#define NEWMODULEWIZARD_H

#include <QWizard>
#include <QMap>
QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTextEdit;
class QTableWidget;
class QPushButton;
class QComboBox;
class QXmlStreamWriter;
QT_END_NAMESPACE

class sourceRegistersPage;

class newmodulewizard : public QWizard
{
    Q_OBJECT

public:
    enum rType{CONTROL, STATE, MODULE, CONFIRM, DEBUG};
    newmodulewizard(QWidget *parent = 0);
    void accept() Q_DECL_OVERRIDE;
private:
    sourceRegistersPage *sr;
    sourceRegistersPage *sk;
    QMap<QString, int> sr_control_R;
    QMap<QString, int> sr_state_R;
    QMap<QString, int> sr_modulec_R;
    QMap<QString, int> sk_control_R;
    QMap<QString, int> sk_state_R;
    QMap<QString, int> sk_modulec_R;
    QXmlStreamWriter *writer;
    void writeRegisterXml(QMap<QString, int> m, rType t);

signals:
    void f(QString s);

public slots:
    void saveRegisters(int ty);


    //use of this macro will result in an error if the override function doesn't do anything.
};

class moduleIntroPage : public QWizardPage
{
    Q_OBJECT

public:
    moduleIntroPage(QWidget *parent = 0);

private:
    QLabel *label;
};

class moduleInfoPage : public QWizardPage
{
    Q_OBJECT

public:
    moduleInfoPage(QWidget *parent = 0);

private:
    QLabel *moduleNameLabel;
//    QLabel *timescaleLabel;
    QLineEdit *moduleNameLineEdit;
//    QLineEdit *timescaleLineEdit;
};

class sourceRegistersPage : public QWizardPage
{
    Q_OBJECT

public:
    enum Type{SOURCE, SINK};
    sourceRegistersPage(Type type, QWidget *parent = 0);
    QTableWidget *controlTable;
    QTableWidget *stateTable;
    QTableWidget *moduleCommutationTable;

protected:
    bool validatePage() Q_DECL_OVERRIDE;
    Type t;

private: 
    QLabel *fifo;
    QLabel *controlRLabel;
    QLabel *stateRLabel;
    QLabel *moduleCommutationRLabel;
    QPushButton *addcontrol;
    QPushButton *deletecontrol;
    QPushButton *addstate;
    QPushButton *deletestate;
    QPushButton *addmoduleCommutation;
    QPushButton *deletemoduleCommutation;
    QComboBox *fifoLine;

signals:
    void addcontrolr(int which);
    void addstater(int which);
    void addmcommutationr(int which);
    void deletecontrolr(int which);
    void deletestater(int which);
    void deletecommutationr(int which);
    void saveme(int ty);

public slots:
    void addb();
    void deleteb();
};

class notesPage : public QWizardPage
{
    Q_OBJECT

public:
    notesPage(QWidget *parent = 0);

protected:
    //void initializePage() Q_DECL_OVERRIDE;

private:

    QLabel *noteslabel;
    QTextEdit *notesText;


};

//class ConclusionPage : public QWizardPage
//{
//    Q_OBJECT

//public:
//    ConclusionPage(QWidget *parent = 0);

//protected:
//    //void initializePage() Q_DECL_OVERRIDE;

//private:
//    QLabel *label;
//};

#endif
