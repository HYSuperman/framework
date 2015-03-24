#include "newprowizard.h"
#include <QtWidgets>

newprowizard::newprowizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new IntroPage);
    addPage(new TopModuleInfoPage);
//! [0]

    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/images/background.png"));

    setWindowTitle(tr("New Module Wizard"));
//! [2]
}

IntroPage::IntroPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Introduction Page"));
    setPixmap(QWizard::WatermarkPixmap, QPixmap(""));

    label = new QLabel(tr("This is new module in GRT"));
    label->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(label);
    setLayout(layout);
}


TopModuleInfoPage::TopModuleInfoPage(QWidget *parent)
    : QWizardPage(parent)
{
    //! [8]
        setTitle(tr("Top Module Information"));
        setSubTitle(tr("Specify basic information about the top module for which you "
                       "want to generate xml codes."));
        setPixmap(QWizard::LogoPixmap, QPixmap(""));

    //! [10]
        topmoduleNameLabel = new QLabel("Top Module Name:");
        topmoduleNameLineEdit = new QLineEdit;
        topmoduleNameLabel->setBuddy(topmoduleNameLineEdit);

        timescaleLabel = new QLabel(tr("Timescale:"));
        timescaleLineEdit = new QLineEdit;
        timescaleLabel->setBuddy(timescaleLineEdit);

        partitionnumberLabel = new QLabel(tr("Partition Number:"));
        partitionnumberLineEdit = new QLineEdit;
        partitionnumberLabel->setBuddy(partitionnumberLineEdit);

    //! [11] //! [12]
        registerField("topmodulename*", topmoduleNameLineEdit);
        registerField("timescale", timescaleLineEdit);
        registerField("partitionnumber", partitionnumberLineEdit);
    //! [11]

        QGridLayout *layout = new QGridLayout;
        layout->addWidget(topmoduleNameLabel, 0, 0);
        layout->addWidget(topmoduleNameLineEdit, 0, 1);
        layout->addWidget(timescaleLabel, 1, 0);
        layout->addWidget(timescaleLineEdit, 1, 1);
        layout->addWidget(partitionnumberLabel, 2, 0);
        layout->addWidget(partitionnumberLineEdit, 2,1);
        setLayout(layout);
}
