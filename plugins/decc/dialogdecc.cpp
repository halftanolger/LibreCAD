#include <sstream>
#include <boost/algorithm/string.hpp>
#include <QDebug>
#include <QSettings>
#include "dialogdecc.h"
#include "ui_dialogdecc.h"

DialogDECC::DialogDECC(QWidget *parent, Document_Interface *doc) :
    QDialog(parent),
    ui(new Ui::DialogDECC)
{
    ui->setupUi(this);

    connect(ui->pushButtonRunScript, SIGNAL(clicked()), this, SLOT(runScript()));

    _cad = new CAD(doc);

    readSettings();

}

DialogDECC::~DialogDECC()
{
    writeSettings();
    delete ui;
    delete _cad;
}

void DialogDECC::runScript()
{
    ScriptDECC script(ui->textEdit, _cad);
    script.setIncludePath("/home/halftan/include/");

    string file = script.readFile(ui->lineEdit->text().toStdString());

    bool debug = true;
    if (debug)
    {
        vector< string > lines;
        string s = file;
        vector<string>::iterator itr;
        int lineNr = 0;

        qDebug("Script start -------------------------------------------------------------------");
        lines.clear();
        boost::algorithm::split( lines, s, boost::algorithm::is_any_of("\n"), boost::algorithm::token_compress_on );
        lineNr = 1;
        for(itr = lines.begin(); itr != lines.end(); itr++)
        {
            lineNr++;
            stringstream ss;
            ss << lineNr << " " << *itr;
            qDebug(ss.str().c_str());
        }
        qDebug("Script ends --------------------------------------------------------------------");
    }


    script.runScript(file);


}


void DialogDECC::readSettings()
 {

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "decc_plugin");

    ui->lineEdit->setText(settings.value("filename","/tmp/test.js").toString());

    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(640,480)).toSize();
    resize(size);
    move(pos);
 }

void DialogDECC::writeSettings()
 {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LibreCAD", "decc_plugin");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("filename", ui->lineEdit->text());   
 }
