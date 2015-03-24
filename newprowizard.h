#ifndef NEWPROWIZARD_H
#define NEWPROWIZARD_H

#include <QWizard>

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTextEdit;
QT_END_NAMESPACE

class newprowizard : public QWizard
{
    Q_OBJECT

public:
    newprowizard(QWidget *parent = 0);

    //void accept() Q_DECL_OVERRIDE;//use of this macro will result in an error if the override function doesn't do anything.
};

class IntroPage : public QWizardPage
{
    Q_OBJECT

public:
    IntroPage(QWidget *parent = 0);

private:
    QLabel *label;
};

class TopModuleInfoPage : public QWizardPage
{
    Q_OBJECT

public:
    TopModuleInfoPage(QWidget *parent = 0);

private:
    QLabel *topmoduleNameLabel;
    QLabel *timescaleLabel;
    QLabel *partitionnumberLabel;
    QLineEdit *topmoduleNameLineEdit;
    QLineEdit *timescaleLineEdit;
    QLineEdit *partitionnumberLineEdit;
};

class GlobalCsrsPage : public QWizardPage
{
    Q_OBJECT

public:
    GlobalCsrsPage(QWidget *parent = 0);

protected:
    //void initializePage() Q_DECL_OVERRIDE;

private:
    QLabel *macroNameLabel;
    QLabel *baseIncludeLabel;
    QLineEdit *macroNameLineEdit;
    QLineEdit *baseIncludeLineEdit;
};

class OutputFilesPage : public QWizardPage
{
    Q_OBJECT

public:
    OutputFilesPage(QWidget *parent = 0);

protected:
    //void initializePage() Q_DECL_OVERRIDE;

private:
    QLabel *outputDirLabel;
    QLabel *headerLabel;
    QLabel *implementationLabel;
    QLineEdit *outputDirLineEdit;
    QLineEdit *headerLineEdit;
    QLineEdit *implementationLineEdit;
};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);

protected:
    //void initializePage() Q_DECL_OVERRIDE;

private:
    QLabel *label;
};

#endif
