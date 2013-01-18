#ifndef DIALOGDECC_H
#define DIALOGDECC_H

#include <QDialog>
#include "script/scriptdecc.h"

namespace Ui {
class DialogDECC;
}

class DialogDECC : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogDECC(QWidget *parent,Document_Interface *doc);
    ~DialogDECC();

public slots:
    void runScript();
    
private:       
    void readSettings();
    void writeSettings();

    Ui::DialogDECC *ui;
    CAD *_cad;

};

#endif // DIALOGDECC_H
