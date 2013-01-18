/*****************************************************************************/
/*  CAD.cpp - javascript enabeled plugin for LibreCAD                        */
/*                                                                           */
/*  Copyright (C) 2013 Demas as, (halfdan.saetherskar@demas.no)              */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include <string>
#include <QDebug>
#include <QVariant>
#include "cad.h"
#include "document_interface.h"

CAD::CAD(Document_Interface *doc):_doc(doc)
{
}

void CAD::addLine(double x1, double y1, double x2, double y2)
{ 
    _doc->addLine(new QPointF(x1,y1),new QPointF(x2,y2));
}

void CAD::addText(QString text, QString style, QPointF *start, double height, double angle, int ha, int va )
{

    DPI::HAlign halign = DPI::HAlignLeft;
    if (ha == 1)
        halign = DPI::HAlignLeft;
    else if (ha == 2)
        halign = DPI::HAlignCenter;
    else if (ha == 3)
        halign = DPI::HAlignRight;

    DPI::VAlign valign = DPI::VAlignBottom;
    if (va == 1)
        valign = DPI::VAlignTop;
    if (va == 2)
        valign = DPI::VAlignMiddle;
    if (va == 3)
        valign = DPI::VAlignBottom;

    _doc->addMText(text, style,start,height,angle,halign,valign);

}


int CAD::newEntity(QString entityType)
{
    enum DPI::ETYPE type = QStringToPluginEntityType(entityType);

    if (type == DPI::UNKNOWN)
        return 0;

    Plug_Entity *pe = _doc->newEntity(type);
    QHash<int,QVariant> data;
    pe->getData(&data);

    int eid = (data.take(DPI::EID)).toInt();
    entityMap.insert(eid,pe);

    return eid;
}

QString CAD::getData(int eid, QString entityData)
{
    enum DPI::EDATA edata = QStringToPluginEntityData(entityData);

    if (edata == DPI::ETYPE)
        return "";

    Plug_Entity *pe = entityMap.value(eid);

    QHash<int,QVariant> data;
    pe->getData(&data);

    QString val = (data.value(edata)).toString();

    return val;
}

void CAD::addEntity(int eid)
{
    Plug_Entity *pe = entityMap.value(eid);
    _doc->addEntity(pe);
}

void CAD::updateView()
{
    _doc->updateView();
}

void CAD::setLayer(QString name)
{
    _doc->setLayer(name);
}

enum DPI::EDATA CAD::QStringToPluginEntityData(QString entityData) {

    if (entityData.compare("LAYER") == 0)       return DPI::LAYER;
    if (entityData.compare("TEXTCONTENT") == 0) return DPI::TEXTCONTENT;
    if (entityData.compare("BLKNAME") == 0)     return DPI::BLKNAME;
    if (entityData.compare("EID") == 0)         return DPI::EID;
    if (entityData.compare("LTYPE") == 0)       return DPI::LTYPE;
    if (entityData.compare("TXTSTYLE") == 0)    return DPI::TXTSTYLE;
    if (entityData.compare("LAYER") == 0)       return DPI::LAYER;
    if (entityData.compare("STARTX") == 0)      return DPI::STARTX;
    if (entityData.compare("ENDX") == 0)        return DPI::ENDX;
    if (entityData.compare("VVECTORX") == 0)    return DPI::VVECTORX;
    if (entityData.compare("SIZEU") == 0)       return DPI::SIZEU;
    if (entityData.compare("STARTY") == 0)      return DPI::STARTY;
    if (entityData.compare("ENDY") == 0)        return DPI::ENDY;
    if (entityData.compare("VVECTORY") == 0)    return DPI::VVECTORY;
    if (entityData.compare("SIZEV") == 0)       return DPI::SIZEV;
    if (entityData.compare("STARTZ") == 0)      return DPI::STARTZ;
    if (entityData.compare("ENDZ") == 0)        return DPI::ENDZ;
    if (entityData.compare("LWIDTH") == 0)      return DPI::LWIDTH;
    if (entityData.compare("RADIUS") == 0)      return DPI::RADIUS;
    if (entityData.compare("HEIGHT") == 0)      return DPI::HEIGHT;
    if (entityData.compare("XSCALE") == 0)      return DPI::XSCALE;
    if (entityData.compare("YSCALE") == 0)      return DPI::YSCALE;
    if (entityData.compare("ZSCALE") == 0)      return DPI::ZSCALE;
    if (entityData.compare("LTSCALE") == 0)     return DPI::LTSCALE;
    if (entityData.compare("STARTANGLE") == 0)  return DPI::STARTANGLE;
    if (entityData.compare("ENDANGLE") == 0)    return DPI::ENDANGLE;
    if (entityData.compare("COLOR") == 0)       return DPI::COLOR;
    if (entityData.compare("CLOSEPOLY") == 0)   return DPI::CLOSEPOLY;
    if (entityData.compare("TXTALIGNH") == 0)   return DPI::TXTALIGNH;
    if (entityData.compare("TXTALIGNV") == 0)   return DPI::TXTALIGNV;

    return DPI::ETYPE; //Interpret this as 'unknown'?
}

enum DPI::ETYPE CAD::QStringToPluginEntityType(QString entityType) {

    if (entityType.compare("POINT") == 0)  return DPI::POINT;
    if (entityType.compare("LINE") == 0)   return DPI::LINE;
    if (entityType.compare("CIRCLE") == 0) return DPI::CIRCLE;
    if (entityType.compare("TEXT") == 0) return DPI::TEXT;

    /*
    ARC,
    ELLIPSE,
    IMAGE,
    OVERLAYBOX,
    SOLID,/end atomicEntity, start entityContainer
    MTEXT,
    TEXT,
    INSERT,
    POLYLINE,
    SPLINE,
    HATCH,
    DIMLEADER,
    DIMALIGNED,
    DIMLINEAR,
    DIMRADIAL,
    DIMDIAMETRIC,
    DIMANGULAR,
    UNKNOWN */

    return DPI::UNKNOWN;
}
