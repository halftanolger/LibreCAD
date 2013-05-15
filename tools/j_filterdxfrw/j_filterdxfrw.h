#ifndef J_FILTERDXFRW_H
#define J_FILTERDXFRW_H

#include <jni.h>
#include <QString>
#include "drw_interface.h"
#include "libdxfrw.h"
#include "j_filterdxfrw_global.h"

class J_FILTERDXFRWSHARED_EXPORT J_filterdxfrw : public DRW_Interface
{
public:
    J_filterdxfrw();
    ~J_filterdxfrw();

    bool fileImport(JNIEnv *env, jobject obj, const QString& file);

    // Methods from DRW_CreationInterface:
    virtual void addHeader(const DRW_Header* data);
    virtual void addLType(const DRW_LType& /*data*/){}
    virtual void addLayer(const DRW_Layer& data);
    virtual void addDimStyle(const DRW_Dimstyle& /*data*/){}
    virtual void addVport(const DRW_Vport& data);
    virtual void addTextStyle(const DRW_Textstyle& /*data*/){}
    virtual void addBlock(const DRW_Block& data);
    virtual void endBlock();
    virtual void addPoint(const DRW_Point& data);
    virtual void addLine(const DRW_Line& data);
    virtual void addRay(const DRW_Ray& data);
    virtual void addXline(const DRW_Xline& data);
    virtual void addCircle(const DRW_Circle& data);
    virtual void addArc(const DRW_Arc& data);
    virtual void addEllipse(const DRW_Ellipse& data);
    virtual void addLWPolyline(const DRW_LWPolyline& data);
    virtual void addText(const DRW_Text& data);
    virtual void addPolyline(const DRW_Polyline& data);
    virtual void addSpline(const DRW_Spline* data);
    virtual void addKnot(const DRW_Entity&) {}
    virtual void addInsert(const DRW_Insert& data);
    virtual void addTrace(const DRW_Trace& data);
    virtual void addSolid(const DRW_Solid& data);
    virtual void addMText(const DRW_MText& data);
    virtual void addDimAlign(const DRW_DimAligned *data);
    virtual void addDimLinear(const DRW_DimLinear *data);
    virtual void addDimRadial(const DRW_DimRadial *data);
    virtual void addDimDiametric(const DRW_DimDiametric *data);
    virtual void addDimAngular(const DRW_DimAngular *data);
    virtual void addDimAngular3P(const DRW_DimAngular3p *data);
    virtual void addDimOrdinate(const DRW_DimOrdinate *data);
    virtual void addLeader(const DRW_Leader *data);
    virtual void addHatch(const DRW_Hatch* data);
    virtual void addViewport(const DRW_Viewport& /*data*/){}
    virtual void addImage(const DRW_Image* data);
    virtual void linkImage(const DRW_ImageDef* data);

    virtual void add3dFace(const DRW_3Dface& /*data*/){}
    virtual void addComment(const char*){}


    virtual void writeHeader(DRW_Header& data);
    virtual void writeEntities();
    virtual void writeLTypes();
    virtual void writeLayers();
    virtual void writeTextstyles();
    virtual void writeVports();
    virtual void writeBlockRecords();
    virtual void writeBlocks();
    virtual void writeDimstyles();




private:
    QString file;
    dxfRW *dxf;

    JNIEnv *env;
    jobject obj; // Objektet vi blir kalt fra.
    jclass cls;  // Klassen til objektet vi blir kalt fra.

    jmethodID mid_addline;
    jmethodID mid_addcircle;
    jmethodID mid_addblock;
    jmethodID mid_endblock;
    jmethodID mid_addinsert;
    jmethodID mid_addhatch;
    jmethodID mid_addhatchloop;
    jmethodID mid_addhatchloopcircle;
    jmethodID mid_addhatchdone;
    jmethodID mid_addtext;

};

#endif // J_FILTERDXFRW_H
