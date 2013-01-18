/*****************************************************************************/
/*  decc.cpp - plugin for LibreCAD                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSettings>
#include <QMessageBox>
#include <QDoubleValidator>

#include "document_interface.h"
#include "decc.h"
#include "dialogdecc.h"

QString DECC::name() const
 {
     return (tr("dECC plugin"));
 }

PluginCapabilities DECC::getCapabilities() const
{
    PluginCapabilities pluginCapabilities;
    pluginCapabilities.menuEntryPoints
            << PluginMenuLocation("Help", tr("dECC plugin"));
    return pluginCapabilities;
}

void DECC::execComm(Document_Interface *doc,
                             QWidget *parent, QString cmd)
{
    Q_UNUSED(doc);
    DialogDECC pdt(parent,doc);
    int result =  pdt.exec();
    //if (result == QDialog::Accepted)
      //  pdt.procesAction(doc);
}



Q_EXPORT_PLUGIN2(decc, DECC);
