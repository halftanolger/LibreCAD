/*****************************************************************************/
/*  CAD.h - javascript enabeled plugin for LibreCAD                          */
/*                                                                           */
/*  Copyright (C) 2013 Demas as, (halfdan.saetherskar@demas.no)              */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#ifndef CAD_H
#define CAD_H

#include <QString>
#include <QMap>
#include "document_interface.h"

class CAD
{
public:

    CAD(Document_Interface *doc);

    void addInsert(QString name, QPointF ins, QPointF scale, qreal rot);

    void addLine(double x1, double y1, double x2, double y2);
    void addText(QString text, QString style, QPointF *start, double height, double angle, int ha, int va );

    int newEntity(QString entityType);
    void addEntity(int eid);
    void updateView();
    void setLayer(QString name);
    QString getData(int, QString key);        

    template <typename T> void updateData(int eid, QString key, T val) {
            enum DPI::EDATA edata = QStringToPluginEntityData(key);
            if (edata == DPI::ETYPE) return;

            QHash<int,QVariant> data;
            data.insert(edata,val);

            entityMap.value(eid)->updateData(&data);
        }

private:

    enum DPI::ETYPE QStringToPluginEntityType(QString str);
    enum DPI::EDATA QStringToPluginEntityData(QString str);

    Document_Interface* _doc;
    QMap<int,Plug_Entity*> entityMap;
};

#endif // CAD_H
