
#include <stdio.h>
#include <QStringList>
#include <QFile>
#include <QDebug>
#include <qtextcodec.h>

#include "j_filterdxfrw.h"
#include "rs_vector.h"
#include "rs_text.h"

J_filterdxfrw::J_filterdxfrw():DRW_Interface() {
}

J_filterdxfrw::~J_filterdxfrw() {    
}

bool J_filterdxfrw::fileImport(JNIEnv *env, jobject obj, const QString& file) {

    this->env = env;
    this->obj = obj;
    this->file = file;

    cls = env->GetObjectClass(obj);

    mid_addline = env->GetMethodID(cls, "callbackAddLine", "(DDDD)V");
    if (mid_addline == NULL) {
        qCritical("Couldn't get hold of java callback method callbackAddLine");
        return false;
    }

    mid_addcircle = env->GetMethodID(cls, "callbackAddCircle", "(DDD)V");
    if (mid_addcircle == NULL) {
        qCritical("Couldn't get hold of java callback method callbackAddCircle");
        return false;
    }

    mid_addblock = env->GetMethodID(cls, "callbackAddBlock", "(Ljava/lang/String;DD)V");
    if (mid_addblock == NULL) {
        qCritical("Couldn't get hold of java callback method callbackAddBlock");
        return false;
    }

    mid_endblock = env->GetMethodID(cls, "callbackEndBlock", "()V");
    if (mid_endblock == NULL) {
        qCritical("Couldn't get hold of java callback method callbackEndBlock");
        return false;
    }

    mid_addinsert = env->GetMethodID(cls, "callbackAddInsert", "(Ljava/lang/String;DDDDDDDII)V");
    if (mid_addinsert == NULL) {
        qCritical("Couldn't get hold of java callback method callbackAddInsert");
        return false;
    }

    mid_addhatch = env->GetMethodID(cls, "callbackAddHatch", "(IDDLjava/lang/String;)V");
    if (mid_addhatch == NULL) {
        qCritical("Couldn't get hold of java callback method callbackAddHatch debug");
        return false;
    }

    mid_addhatchloop = env->GetMethodID(cls, "callbackAddHatchLoop", "()V");
    if (mid_addhatchloop == NULL) {
        qCritical("Couldn't get hold of java callback method callbackAddHatchLoop");
        return false;
    }

    mid_addhatchloopcircle = env->GetMethodID(cls, "callbackAddHatchLoopCircle", "(DDD)V");
    if (mid_addhatchloopcircle == NULL) {
        qCritical("Couldn't get hold of java callback method callbackAddHatchLoopCircle");
        return false;
    }

    mid_addhatchdone = env->GetMethodID(cls, "callbackAddHatchDone", "()V");
    if (mid_addhatchdone == NULL) {
        qCritical("Couldn't get hold of java callback method callbackAddHatchDone");
        return false;
    }

    mid_addtext = env->GetMethodID(cls, "callbackAddText", "(DDDDDDIIILjava/lang/String;Ljava/lang/String;DI)V");
    if (mid_addtext == NULL) {
        qCritical("Couldn't get hold of java callback method callbackAddText");
        return false;
    }

    dxf = new dxfRW(QFile::encodeName(file));
    bool success = dxf->read(this, true);
    delete dxf;

    return success?true:false;

}

/**
 * Implementation of the method which handles layers.
 */
void J_filterdxfrw::addLayer(const DRW_Layer &data) {
    /*
    RS_DEBUG->print("RS_FilterDXF::addLayer");
    RS_DEBUG->print("  adding layer: %s", data.name.c_str());

    RS_DEBUG->print("RS_FilterDXF::addLayer: creating layer");

    QString name = QString::fromUtf8(data.name.c_str());
    if (name != "0" && graphic->findLayer(name)!=NULL) {
        return;
    }
    RS_Layer* layer = new RS_Layer(name);
    RS_DEBUG->print("RS_FilterDXF::addLayer: set pen");
    layer->setPen(attributesToPen(&data));

    RS_DEBUG->print("RS_FilterDXF::addLayer: flags");
    if (data.flags&0x01) {
        layer->freeze(true);
    }
    if (data.flags&0x04) {
        layer->lock(true);
    }
    //help layer doesn't appear in printing
    layer->setHelpLayer(! data.plotF);
    if (layer->isHelpLayer())
        RS_DEBUG->print(RS_Debug::D_WARNING, "RS_FilterDXF::addLayer: layer %s is help layer", layer->getName().toStdString().c_str());

    RS_DEBUG->print("RS_FilterDXF::addLayer: add layer to graphic");
    graphic->addLayer(layer);
    RS_DEBUG->print("RS_FilterDXF::addLayer: OK");
    */
}

/**
 * Implementation of the method which handles vports.
 */
void J_filterdxfrw::addVport(const DRW_Vport &data) {
    /*
    QString name = QString::fromStdString(data.name);
    if (name.toLower() == "*active") {
        data.grid == 1? graphic->setGridOn(true):graphic->setGridOn(false);
        graphic->setIsometricGrid(data.snapStyle);
        graphic->setCrosshairType( (RS2::CrosshairType)data.snapIsopair);
        RS_GraphicView *gv = graphic->getGraphicView();
        if (gv != NULL) {
            double width = data.height * data.ratio;
            double factorX= gv->getWidth() / width;
            double factorY= gv->getHeight() / data.height;
            if (factorX > factorY)
                factorX = factorY;
            int ox = gv->getWidth() -data.center.x*2*factorX;
            int oy = gv->getHeight() -data.center.y*2*factorX;
            gv->setOffset(ox, oy);
            gv->setFactor(factorX);
        }
    }
    */
}

/**
 * Implementation of the method which handles blocks.
 *
 * @todo Adding blocks to blocks (stack for currentContainer)
 */
void J_filterdxfrw::addBlock(const DRW_Block& data) {

     QString name = QString::fromUtf8(data.name.c_str());
     QString mid = name.mid(1,11);

     // Prevent special blocks (paper_space, model_space) from being added:
     if (mid.toLower() != "paper_space" && mid.toLower() != "model_space") {

         // data.name.c_str()
         /* Convert a C string to a Java String. */
        //2. char[] str = "To be or not to be.\n";
        //3. jstring jstr = (*env)->NewStringUTF(env, str);

         jstring jstr = env->NewStringUTF(data.name.c_str());

         env->CallVoidMethod(obj, mid_addblock,jstr,data.basePoint.x,data.basePoint.y);
     }


/*
    RS_DEBUG->print("RS_FilterDXF::addBlock");

    RS_DEBUG->print("  adding block: %s", data.name.c_str());
//TODO correct handle of model-space

    QString name = QString::fromUtf8(data.name.c_str());
    QString mid = name.mid(1,11);
// Prevent special blocks (paper_space, model_space) from being added:
    if (mid.toLower() != "paper_space" && mid.toLower() != "model_space") {

            RS_Vector bp(data.basePoint.x, data.basePoint.y);
            RS_Block* block =
                new RS_Block(graphic, RS_BlockData(name, bp, false ));
            //block->setFlags(flags);

            if (graphic->addBlock(block)) {
                currentContainer = block;
            }
    }
    */
}



/**
 * Implementation of the method which closes blocks.
 */
void J_filterdxfrw::endBlock() {

     env->CallVoidMethod(obj, mid_endblock);

    /*
    if (currentContainer->rtti() == RS2::EntityBlock) {
        RS_Block *bk = (RS_Block *)currentContainer;
        //remove unnamed blocks *D only if version != R12
        if (version!=1009) {
            if (bk->getName().startsWith("*D") )
                graphic->removeBlock(bk);
        }
    }
    currentContainer = graphic;
    */
}



/**
 * Implementation of the method which handles point entities.
 */
void J_filterdxfrw::addPoint(const DRW_Point& data) {
    /*
    RS_Vector v(data.basePoint.x, data.basePoint.y);

    RS_Point* entity = new RS_Point(currentContainer,
                                    RS_PointData(v));
    setEntityAttributes(entity, &data);

    currentContainer->addEntity(entity);
    */
}



/**
 * Implementation of the method which handles line entities.
 */
void J_filterdxfrw::addLine(const DRW_Line& data) {

    env->CallVoidMethod(obj, mid_addline, data.basePoint.x,data.basePoint.y,data.secPoint.x,data.secPoint.y);

    /*
    RS_DEBUG->print("RS_FilterDXF::addLine");

    RS_Vector v1(data.basePoint.x, data.basePoint.y);
    RS_Vector v2(data.secPoint.x, data.secPoint.y);

    RS_DEBUG->print("RS_FilterDXF::addLine: create line");

    if (currentContainer==NULL) {
        RS_DEBUG->print("RS_FilterDXF::addLine: currentContainer is NULL");
    }

    RS_Line* entity = new RS_Line(currentContainer,
                                  RS_LineData(v1, v2));
    RS_DEBUG->print("RS_FilterDXF::addLine: set attributes");
    setEntityAttributes(entity, &data);

    RS_DEBUG->print("RS_FilterDXF::addLine: add entity");

    currentContainer->addEntity(entity);

    RS_DEBUG->print("RS_FilterDXF::addLine: OK");
    */
}


/**
 * Implementation of the method which handles ray entities.
 */
void J_filterdxfrw::addRay(const DRW_Ray& data) {
    /*
    RS_DEBUG->print("RS_FilterDXF::addRay");

    RS_Vector v1(data.basePoint.x, data.basePoint.y);
    RS_Vector v2(data.basePoint.x+data.secPoint.x, data.basePoint.y+data.secPoint.y);

    RS_DEBUG->print("RS_FilterDXF::addRay: create line");

    if (currentContainer==NULL) {
        RS_DEBUG->print("RS_FilterDXF::addRay: currentContainer is NULL");
    }

    RS_Line* entity = new RS_Line(currentContainer,
                                  RS_LineData(v1, v2));
    RS_DEBUG->print("RS_FilterDXF::addRay: set attributes");
    setEntityAttributes(entity, &data);

    RS_DEBUG->print("RS_FilterDXF::addRay: add entity");

    currentContainer->addEntity(entity);

    RS_DEBUG->print("RS_FilterDXF::addRay: OK");
    */
}


/**
 * Implementation of the method which handles line entities.
 */
void J_filterdxfrw::addXline(const DRW_Xline& data) {
    /*
    RS_DEBUG->print("RS_FilterDXF::addXline");

    RS_Vector v1(data.basePoint.x, data.basePoint.y);
    RS_Vector v2(data.basePoint.x+data.secPoint.x, data.basePoint.y+data.secPoint.y);

    RS_DEBUG->print("RS_FilterDXF::addXline: create line");

    if (currentContainer==NULL) {
        RS_DEBUG->print("RS_FilterDXF::addXline: currentContainer is NULL");
    }

    RS_Line* entity = new RS_Line(currentContainer,
                                  RS_LineData(v1, v2));
    RS_DEBUG->print("RS_FilterDXF::addXline: set attributes");
    setEntityAttributes(entity, &data);

    RS_DEBUG->print("RS_FilterDXF::addXline: add entity");

    currentContainer->addEntity(entity);

    RS_DEBUG->print("RS_FilterDXF::addXline: OK");
    */
}



/**
 * Implementation of the method which handles circle entities.
 */
void J_filterdxfrw::addCircle(const DRW_Circle& data) {

    env->CallVoidMethod(obj, mid_addcircle, data.basePoint.x,data.basePoint.y,data.radious);

    /*
    RS_DEBUG->print("RS_FilterDXF::addCircle");

    RS_Vector v(data.basePoint.x, data.basePoint.y);
    RS_CircleData d(v, data.radious);
    RS_Circle* entity = new RS_Circle(currentContainer, d);
    setEntityAttributes(entity, &data);

    currentContainer->addEntity(entity);
    */
}



/**
 * Implementation of the method which handles arc entities.
 *
 * @param angle1 Start angle in deg (!)
 * @param angle2 End angle in deg (!)
 */
void J_filterdxfrw::addArc(const DRW_Arc& data) {
    /*
    RS_DEBUG->print("RS_FilterDXF::addArc");
    RS_Vector v(data.basePoint.x, data.basePoint.y);
    RS_ArcData d(v, data.radious,
                 data.staangle/ARAD,
                 data.endangle/ARAD,
                 false);
    RS_Arc* entity = new RS_Arc(currentContainer, d);
    setEntityAttributes(entity, &data);

    currentContainer->addEntity(entity);
    */
}



/**
 * Implementation of the method which handles ellipse entities.
 *
 * @param angle1 Start angle in rad (!)
 * @param angle2 End angle in rad (!)
 */
void J_filterdxfrw::addEllipse(const DRW_Ellipse& data) {
    /*
    RS_DEBUG->print("J_filterdxfrw::addEllipse");

    RS_Vector v1(data.basePoint.x, data.basePoint.y);
    RS_Vector v2(data.secPoint.x, data.secPoint.y);
    double ang2 = data.endparam;
    if ( fabs(ang2- 6.28318530718) < 1.0e-10 && fabs(data.staparam) < 1.0e-10 )
        ang2 = 0.0;
    RS_EllipseData ed(v1, v2, data.ratio, data.staparam,
                                    ang2, false);
    RS_Ellipse* entity = new RS_Ellipse(currentContainer, ed);
    setEntityAttributes(entity, &data);

    currentContainer->addEntity(entity);
    */
}


/**
 * Implementation of the method which handles trace entities.
 */
void J_filterdxfrw::addTrace(const DRW_Trace& data) {
    /*
    RS_Solid* entity;
    RS_Vector v1(data.basePoint.x, data.basePoint.y);
    RS_Vector v2(data.secPoint.x, data.secPoint.y);
    RS_Vector v3(data.thirdPoint.x, data.thirdPoint.y);
    RS_Vector v4(data.fourPoint.x, data.fourPoint.y);
    if (v3 == v4)
        entity = new RS_Solid(currentContainer, RS_SolidData(v1, v2, v3));
    else
        entity = new RS_Solid(currentContainer, RS_SolidData(v1, v2, v3,v4));

    setEntityAttributes(entity, &data);
    currentContainer->addEntity(entity);
    */
}

/**
 * Implementation of the method which handles solid entities.
 */
void J_filterdxfrw::addSolid(const DRW_Solid& data) {
    addTrace(data);
}

/**
 * Implementation of the method which handles lightweight polyline entities.
 */
void J_filterdxfrw::addLWPolyline(const DRW_LWPolyline& data) {
    /*
    RS_DEBUG->print("J_filterdxfrw::addLWPolyline");
    RS_PolylineData d(RS_Vector(false),
                      RS_Vector(false),
                      data.flags&0x1);
    RS_Polyline *polyline = new RS_Polyline(currentContainer, d);
    setEntityAttributes(polyline, &data);

    QList< QPair<RS_Vector*, double> > verList;
    for (unsigned int i=0; i<data.vertlist.size(); i++) {
        DRW_Vertex2D *vert = data.vertlist.at(i);
        RS_Vector *v = new RS_Vector(vert->x, vert->y);
        verList.append(qMakePair(v, vert->bulge));
    }
    polyline->appendVertexs(verList);
    while (!verList.isEmpty())
         delete verList.takeFirst().first;

    currentContainer->addEntity(polyline);
    */
}


/**
 * Implementation of the method which handles polyline entities.
 */
void J_filterdxfrw::addPolyline(const DRW_Polyline& data) {
    /*
    RS_DEBUG->print("J_filterdxfrw::addPolyline");
    if ( data.flags&0x10)
        return; //the polyline is a polygon mesh, not handled

    if ( data.flags&0x40)
        return; //the polyline is a poliface mesh, TODO convert

    RS_PolylineData d(RS_Vector(false),
                      RS_Vector(false),
                      data.flags&0x1);
    RS_Polyline *polyline = new RS_Polyline(currentContainer, d);
    setEntityAttributes(polyline, &data);

    QList< QPair<RS_Vector*, double> > verList;
    for (unsigned int i=0; i<data.vertlist.size(); i++) {
        DRW_Vertex *vert = data.vertlist.at(i);
        RS_Vector *v = new RS_Vector(vert->basePoint.x, vert->basePoint.y);
        verList.append(qMakePair(v, vert->bulge));
    }
    polyline->appendVertexs(verList);
    while (!verList.isEmpty())
         delete verList.takeFirst().first;

    currentContainer->addEntity(polyline);
    */
}


/**
 * Implementation of the method which handles splines.
 */
void J_filterdxfrw::addSpline(const DRW_Spline* data) {
    /*
    RS_DEBUG->print("J_filterdxfrw::addSpline: degree: %d", data->degree);

        RS_Spline* spline;
        if (data->degree>=1 && data->degree<=3) {
        RS_SplineData d(data->degree, ((data->flags&0x1)==0x1));
        spline = new RS_Spline(currentContainer, d);
        setEntityAttributes(spline, data);

        currentContainer->addEntity(spline);
    } else {
        RS_DEBUG->print(RS_Debug::D_WARNING,
                        "RS_FilterDXF::addSpline: Invalid degree for spline: %d. "
                        "Accepted values are 1..3.", data->degree);
        return;
    }
    for (unsigned int i=0; i<data->controllist.size(); i++) {
        DRW_Coord *vert = data->controllist.at(i);
        RS_Vector v(vert->x, vert->y);
        spline->addControlPoint(v);
    }
    spline->update();
    */
}


/**
 * Implementation of the method which handles inserts.
 */
void J_filterdxfrw::addInsert(const DRW_Insert& data) {

    jstring jstr = env->NewStringUTF(data.name.c_str());

    env->CallVoidMethod(obj, mid_addinsert,jstr,
                        data.basePoint.x,
                        data.basePoint.y,
                        data.xscale,
                        data.yscale,
                        data.angle/ARAD,
                        data.colspace,
                        data.rowspace,
                        data.colcount,
                        data.rowcount);

    /*
    RS_DEBUG->print("RS_FilterDXF::addInsert");

    RS_Vector ip(data.basePoint.x, data.basePoint.y);
    RS_Vector sc(data.xscale, data.yscale);
    RS_Vector sp(data.colspace, data.rowspace);

    //cout << "Insert: " << name << " " << ip << " " << cols << "/" << rows << endl;

    RS_InsertData d( QString::fromUtf8(data.name.c_str()),
                    ip, sc, data.angle/ARAD,
                    data.colcount, data.rowcount,
                    sp, NULL, RS2::NoUpdate);
    RS_Insert* entity = new RS_Insert(currentContainer, d);
    setEntityAttributes(entity, &data);
    RS_DEBUG->print("  id: %d", entity->getId());
//    entity->update();
    currentContainer->addEntity(entity);
    */
}



/**
 * Implementation of the method which handles
 * multi texts (MTEXT).
 */
void J_filterdxfrw::addMText(const DRW_MText& data) {
    /*
    RS_DEBUG->print("RS_FilterDXF::addMText: %s", data.text.c_str());

    RS_MTextData::VAlign valign;
    RS_MTextData::HAlign halign;
    RS_MTextData::MTextDrawingDirection dir;
    RS_MTextData::MTextLineSpacingStyle lss;
    QString sty = QString::fromUtf8(data.style.c_str());
    sty=sty.toLower();

    if (data.textgen<=3) {
        valign=RS_MTextData::VATop;
    } else if (data.textgen<=6) {
        valign=RS_MTextData::VAMiddle;
    } else {
        valign=RS_MTextData::VABottom;
    }

    if (data.textgen%3==1) {
        halign=RS_MTextData::HALeft;
    } else if (data.textgen%3==2) {
        halign=RS_MTextData::HACenter;
    } else {
        halign=RS_MTextData::HARight;
    }

    if (data.alignH==1) {
        dir = RS_MTextData::LeftToRight;
    } else if (data.alignH==3) {
        dir = RS_MTextData::TopToBottom;
    } else {
        dir = RS_MTextData::ByStyle;
    }

    if (data.alignV==1) {
        lss = RS_MTextData::AtLeast;
    } else {
        lss = RS_MTextData::Exact;
    }

    QString mtext = toNativeString(QString::fromUtf8(data.text.c_str()));
    // use default style for the drawing:
    if (sty.isEmpty()) {
        // japanese, cyrillic:
        if (codePage=="ANSI_932" || codePage=="ANSI_1251") {
            sty = "Unicode";
        } else {
            sty = textStyle;
        }
    } else {
        sty = fontList.value(sty, sty);
    }

    RS_DEBUG->print("Text as unicode:");
    RS_DEBUG->printUnicode(mtext);
    double interlin = data.interlin;
    double angle = data.angle*M_PI/180.;
    RS_Vector ip = RS_Vector(data.basePoint.x, data.basePoint.y);

//Correct bad alignment of older dxflib or libdxfrw < 0.5.4
    if (oldMText) {
        interlin = data.interlin*0.96;
        if (valign == RS_MTextData::VABottom) {
            QStringList tl = mtext.split('\n', QString::SkipEmptyParts);
            if (!tl.isEmpty()) {
                QString txt = tl.at(tl.size()-1);
#ifdef  RS_VECTOR2D
                RS_TextData d(RS_Vector(0.,0.), RS_Vector(0.,0.),
#else
                RS_TextData d(RS_Vector(0.,0.,0.), RS_Vector(0.,0.,0.),
#endif

                              data.height, 1, RS_TextData::VABaseline, RS_TextData::HALeft,
                              RS_TextData::None, txt, sty, 0,
                              RS2::Update);
                RS_Text* entity = new RS_Text(NULL, d);
                double textTail = entity->getMin().y;
                delete entity;
                RS_Vector ot = RS_Vector(0.0,textTail).rotate(angle);
                ip.move(ot);
            }
        }
    }

    RS_MTextData d(ip, data.height, data.widthscale,
                  valign, halign,
                  dir, lss,
                  interlin,
                  mtext, sty, angle,
                  RS2::NoUpdate);
    RS_MText* entity = new RS_MText(currentContainer, d);

    setEntityAttributes(entity, &data);
    entity->update();
    currentContainer->addEntity(entity);
    */
}



/**
 * Implementation of the method which handles
 * texts (TEXT).
 */
void J_filterdxfrw::addText(const DRW_Text& data) {





    //RS_DEBUG->print("J_filterdxfrw::addText");
    RS_Vector refPoint = RS_Vector(data.basePoint.x, data.basePoint.y);;
    RS_Vector secPoint = RS_Vector(data.secPoint.x, data.secPoint.y);;
    double angle = data.angle;

    if (data.alignV !=0 || data.alignH !=0 ||data.alignH ==DRW_Text::HMiddle){
        if (data.alignH !=DRW_Text::HAligned && data.alignH !=DRW_Text::HFit){
            secPoint = RS_Vector(data.basePoint.x, data.basePoint.y);
            refPoint = RS_Vector(data.secPoint.x, data.secPoint.y);
        }
    }

    RS_TextData::VAlign valign = (RS_TextData::VAlign)data.alignV;
    RS_TextData::HAlign halign = (RS_TextData::HAlign)data.alignH;
    RS_TextData::TextGeneration dir;
    QString sty = QString::fromUtf8(data.style.c_str());
    sty=sty.toLower();

    if (data.textgen==2) {
        dir = RS_TextData::Backward;
    } else if (data.textgen==4) {
        dir = RS_TextData::UpsideDown;
    } else {
        dir = RS_TextData::None;
    }

    QString mtext = toNativeString(QString::fromUtf8(data.text.c_str()));
    // use default style for the drawing:
    if (sty.isEmpty()) {
        // japanese, cyrillic:
        if (codePage=="ANSI_932" || codePage=="ANSI_1251") {
            sty = "Unicode";
        } else {
            sty = textStyle;
        }
    } else {
        sty = fontList.value(sty, sty);
    }

    //RS_DEBUG->print("Text as unicode:");
    //RS_DEBUG->printUnicode(mtext);

    //RS_TextData d(refPoint, secPoint, data.height, data.widthscale,
    //              valign, halign, dir,
    //              mtext, sty, angle*M_PI/180,
    //              RS2::NoUpdate);
    //RS_Text* entity = new RS_Text(currentContainer, d);

    //setEntityAttributes(entity, &data);
    //entity->update();
    //currentContainer->addEntity(entity);





    jstring jstrMText = env->NewStringUTF(mtext.toStdString().c_str());
    jstring jstrStyle = env->NewStringUTF(sty.toStdString().c_str());

    env->CallVoidMethod(obj, mid_addtext,
                        refPoint.x,refPoint.y,
                        secPoint.x,secPoint.y,
                        data.height, data.widthscale,
                        valign, halign, dir,jstrMText,jstrStyle,angle*M_PI/180,RS2::NoUpdate);



}



/**
 * Implementation of the method which handles
 * dimensions (DIMENSION).
 */
//RS_DimensionData J_filterdxfrw::convDimensionData(const  DRW_Dimension* data) {
/*
    DRW_Coord crd = data->getDefPoint();
    RS_Vector defP(crd.x, crd.y);
    crd = data->getTextPoint();
    RS_Vector midP(crd.x, crd.y);
    RS_MTextData::VAlign valign;
    RS_MTextData::HAlign halign;
    RS_MTextData::MTextLineSpacingStyle lss;
    QString sty = QString::fromUtf8(data->getStyle().c_str());

    QString t; //= data.text;

    // middlepoint of text can be 0/0 which is considered to be invalid (!):
    //  0/0 because older QCad versions save the middle of the text as 0/0
    //  althought they didn't suport saving of the middle of the text.
    if (fabs(crd.x)<1.0e-6 && fabs(crd.y)<1.0e-6) {
        midP = RS_Vector(false);
    }

    if (data->getAlign()<=3) {
        valign=RS_MTextData::VATop;
    } else if (data->getAlign()<=6) {
        valign=RS_MTextData::VAMiddle;
    } else {
        valign=RS_MTextData::VABottom;
    }

    if (data->getAlign()%3==1) {
        halign=RS_MTextData::HALeft;
    } else if (data->getAlign()%3==2) {
        halign=RS_MTextData::HACenter;
    } else {
        halign=RS_MTextData::HARight;
    }

    if (data->getTextLineStyle()==1) {
        lss = RS_MTextData::AtLeast;
    } else {
        lss = RS_MTextData::Exact;
    }

    t = QString::fromUtf8(data->getText().c_str());

    if (sty.isEmpty()) {
        sty = dimStyle;
    }

    RS_DEBUG->print("Text as unicode:");
    RS_DEBUG->printUnicode(t);

    // data needed to add the actual dimension entity
    return RS_DimensionData(defP, midP,
                            valign, halign,
                            lss,
                            data->getTextLineFactor(),
                            t, sty, data->getDir());
                            */
//}



/**
 * Implementation of the method which handles
 * aligned dimensions (DIMENSION).
 */
void J_filterdxfrw::addDimAlign(const DRW_DimAligned *data) {
    /*
    RS_DEBUG->print("J_filterdxfrw::addDimAligned");

    RS_DimensionData dimensionData = convDimensionData((DRW_Dimension*)data);

    RS_Vector ext1(data->getDef1Point().x, data->getDef1Point().y);
    RS_Vector ext2(data->getDef2Point().x, data->getDef2Point().y);

    RS_DimAlignedData d(ext1, ext2);

    RS_DimAligned* entity = new RS_DimAligned(currentContainer,
                            dimensionData, d);
    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);
    */
}



/**
 * Implementation of the method which handles
 * linear dimensions (DIMENSION).
 */
void J_filterdxfrw::addDimLinear(const DRW_DimLinear *data) {
    /*
    RS_DEBUG->print("J_filterdxfrw::addDimLinear");

    RS_DimensionData dimensionData = convDimensionData((DRW_Dimension*)data);

    RS_Vector dxt1(data->getDef1Point().x, data->getDef1Point().y);
    RS_Vector dxt2(data->getDef2Point().x, data->getDef2Point().y);

    RS_DimLinearData d(dxt1, dxt2, RS_Math::deg2rad(data->getAngle()),
                       RS_Math::deg2rad(data->getOblique()));

    RS_DimLinear* entity = new RS_DimLinear(currentContainer,
                                            dimensionData, d);
    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);
    */
}



/**
 * Implementation of the method which handles
 * radial dimensions (DIMENSION).
 */
void J_filterdxfrw::addDimRadial(const DRW_DimRadial* data) {
    /*
    RS_DEBUG->print("J_filterdxfrw::addDimRadial");

    RS_DimensionData dimensionData = convDimensionData((DRW_Dimension*)data);
    RS_Vector dp(data->getDiameterPoint().x, data->getDiameterPoint().y);

    RS_DimRadialData d(dp, data->getLeaderLength());

    RS_DimRadial* entity = new RS_DimRadial(currentContainer,
                                            dimensionData, d);

    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);
    */
}



/**
 * Implementation of the method which handles
 * diametric dimensions (DIMENSION).
 */
void J_filterdxfrw::addDimDiametric(const DRW_DimDiametric* data) {
    /*
    RS_DEBUG->print("J_filterdxfrw::addDimDiametric");

    RS_DimensionData dimensionData = convDimensionData((DRW_Dimension*)data);
    RS_Vector dp(data->getDiameter1Point().x, data->getDiameter1Point().y);

    RS_DimDiametricData d(dp, data->getLeaderLength());

    RS_DimDiametric* entity = new RS_DimDiametric(currentContainer,
                              dimensionData, d);

    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);
    */
}



/**
 * Implementation of the method which handles
 * angular dimensions (DIMENSION).
 */
void J_filterdxfrw::addDimAngular(const DRW_DimAngular* data) {
    /*
    RS_DEBUG->print("J_filterdxfrw::addDimAngular");

    RS_DimensionData dimensionData = convDimensionData(data);
    RS_Vector dp1(data->getFirstLine1().x, data->getFirstLine1().y);
    RS_Vector dp2(data->getFirstLine2().x, data->getFirstLine2().y);
    RS_Vector dp3(data->getSecondLine1().x, data->getSecondLine1().y);
    RS_Vector dp4(data->getDimPoint().x, data->getDimPoint().y);

    RS_DimAngularData d(dp1, dp2, dp3, dp4);

    RS_DimAngular* entity = new RS_DimAngular(currentContainer,
                            dimensionData, d);

    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);
    */
}



/**
 * Implementation of the method which handles
 * angular dimensions (DIMENSION).
 */
void J_filterdxfrw::addDimAngular3P(const DRW_DimAngular3p* data) {
    /*
    RS_DEBUG->print("J_filterdxfrw::addDimAngular3P");

    RS_DimensionData dimensionData = convDimensionData(data);
    RS_Vector dp1(data->getFirstLine().x, data->getFirstLine().y);
    RS_Vector dp2(data->getSecondLine().x, data->getSecondLine().y);
    RS_Vector dp3(data->getVertexPoint().x, data->getVertexPoint().y);
    RS_Vector dp4 = dimensionData.definitionPoint;
    dimensionData.definitionPoint = RS_Vector(data->getVertexPoint().x, data->getVertexPoint().y);

    RS_DimAngularData d(dp1, dp2, dp3, dp4);

    RS_DimAngular* entity = new RS_DimAngular(currentContainer,
                            dimensionData, d);

    setEntityAttributes(entity, data);
    entity->update();
    currentContainer->addEntity(entity);
    */
}



void J_filterdxfrw::addDimOrdinate(const DRW_DimOrdinate* data) {

}


/**
 * Implementation of the method which handles leader entities.
 */
void J_filterdxfrw::addLeader(const DRW_Leader *data) {
    /*
    RS_DEBUG->print("J_filterdxfrw::addDimLeader");
    RS_LeaderData d(data->arrow!=0);
    RS_Leader* leader = new RS_Leader(currentContainer, d);
    setEntityAttributes(leader, data);

    currentContainer->addEntity(leader);

    for (unsigned int i=0; i<data->vertexlist.size(); i++) {
        DRW_Coord *vert = data->vertexlist.at(i);
        RS_Vector v(vert->x, vert->y);
        leader->addVertex(v);
    }*/
}



/**
 * Implementation of the method which handles hatch entities.
 */
void J_filterdxfrw::addHatch(const DRW_Hatch *data) {


    double cx = 0.0,cy = 0.0,cr = 0.0;


    env->CallVoidMethod(obj, mid_addhatch, data->solid,data->scale,data->angle,env->NewStringUTF(data->name.c_str()));
    if (env->ExceptionCheck()) {
         qCritical("J_filterdxfrw::addHatch: Exception at env->CallVoidMethod(obj, mid_addhatch, ...);");
        return;
    }

    /*
    RS_DEBUG->print("RS_FilterDXF::addHatch()");
    RS_Hatch* hatch;
    RS_EntityContainer* hatchLoop;

    hatch = new RS_Hatch(currentContainer,
                         RS_HatchData(data->solid, data->scale, data->angle,
                                      QString::fromUtf8(data->name.c_str())));
    setEntityAttributes(hatch, data);
    currentContainer->appendEntity(hatch);
*/
    for (unsigned int i=0; i < data->looplist.size(); i++) {
        DRW_HatchLoop *loop = data->looplist.at(i);
        if ((loop->type & 32) == 32) continue;
        //hatchLoop = new RS_EntityContainer(hatch);
        //hatchLoop->setLayer(NULL);
        //hatch->addEntity(hatchLoop);


        env->CallVoidMethod(obj, mid_addhatchloop);
        if (env->ExceptionCheck()) {
             qCritical("J_filterdxfrw::addHatch: Exception at env->CallVoidMethod(obj, mid_addhatchloop);");
            return;
        }


        //RS_Entity* e = NULL;
        if ((loop->type & 2) == 2){   //polyline, convert to lines & arcs

            /*
            DRW_LWPolyline *pline = (DRW_LWPolyline *)loop->objlist.at(0);
            RS_Polyline *polyline = new RS_Polyline(NULL,
                    RS_PolylineData(RS_Vector(false), RS_Vector(false), pline->flags) );
            for (unsigned int j=0; j < pline->vertlist.size(); j++) {
                    DRW_Vertex2D *vert = pline->vertlist.at(j);
                    polyline->addVertex(RS_Vector(vert->x, vert->y), vert->bulge);
            }
            for (RS_Entity* e=polyline->firstEntity(); e!=NULL;
                    e=polyline->nextEntity()) {
                RS_Entity* tmp = e->clone();
                tmp->reparent(hatchLoop);
                tmp->setLayer(NULL);
                hatchLoop->addEntity(tmp);
            }
            delete polyline;
            */
        } else {
            for (unsigned int j=0; j<loop->objlist.size(); j++) {
                //e = NULL;
                DRW_Entity *ent = loop->objlist.at(j);
                switch (ent->eType) {
                case DRW::LINE: {
                    /*
                    DRW_Line *e2 = (DRW_Line *)ent;
                    e = new RS_Line(hatchLoop,
                                    RS_LineData(RS_Vector(e2->basePoint.x, e2->basePoint.y),
                                                RS_Vector(e2->secPoint.x, e2->secPoint.y)));
                    */
                    break;
                }
                case DRW::ARC: {
                    DRW_Arc *e2 = (DRW_Arc *)ent;
                    if (e2->isccw && e2->staangle<1.0e-6 && e2->endangle>360-1.0e-6) {

                        //e = new RS_Circle(hatchLoop,
                        //                  RS_CircleData(RS_Vector(e2->basePoint.x, e2->basePoint.y),
                        //                                e2->radious));


                        env->CallVoidMethod(obj, mid_addhatchloopcircle,e2->basePoint.x, e2->basePoint.y,e2->radious);
                        if (env->ExceptionCheck()) {
                             qCritical("J_filterdxfrw::addHatch: Exception at env->CallVoidMethod(obj, mid_addhatchloopcircle,x,y,r);");
                            return;
                        }

                        cx = e2->basePoint.x;
                        cy = e2->basePoint.y;
                        cr =e2->radious;


                    } else {
                        /*
                        if (e2->isccw) {
                            e = new RS_Arc(hatchLoop,
                                        RS_ArcData(RS_Vector(e2->basePoint.x, e2->basePoint.y), e2->radious,
                                                   RS_Math::correctAngle(RS_Math::deg2rad(e2->staangle)),
                                                   RS_Math::correctAngle(RS_Math::deg2rad(e2->endangle)),
                                                   false));
                        } else {
                            e = new RS_Arc(hatchLoop,
                                        RS_ArcData(RS_Vector(e2->basePoint.x, e2->basePoint.y), e2->radious,
                                                   RS_Math::correctAngle(2*M_PI-RS_Math::deg2rad(e2->staangle)),
                                                   RS_Math::correctAngle(2*M_PI-RS_Math::deg2rad(e2->endangle)),
                                                   true));
                        }
                        */
                    }
                    break;
                }
                case DRW::ELLIPSE: {
                    /*
                    DRW_Ellipse *e2 = (DRW_Ellipse *)ent;
                    double ang1 = e2->staparam;
                    double ang2 = e2->endparam;
                    if ( fabs(ang2 - 6.28318530718) < 1.0e-10 && fabs(ang1) < 1.0e-10 )
                        ang2 = 0.0;
                    else { //convert angle to parameter
                        ang1 = atan(tan(ang1)/e2->ratio);
                        ang2 = atan(tan(ang2)/e2->ratio);
                        if (ang1 < 0){//quadrant 2 & 4
                            ang1 +=M_PI;
                            if (e2->staparam > M_PI) //quadrant 4
                                ang1 +=M_PI;
                        } else if (e2->staparam > M_PI){//3 quadrant
                            ang1 +=M_PI;
                        }
                        if (ang2 < 0){//quadrant 2 & 4
                            ang2 +=M_PI;
                            if (e2->endparam > M_PI) //quadrant 4
                                ang2 +=M_PI;
                        } else if (e2->endparam > M_PI){//3 quadrant
                            ang2 +=M_PI;
                        }
                    }
                    e = new RS_Ellipse(hatchLoop,
                                       RS_EllipseData(RS_Vector(e2->basePoint.x, e2->basePoint.y),
                                                      RS_Vector(e2->secPoint.x, e2->secPoint.y),
                                                      e2->ratio, ang1, ang2, !e2->isccw));
                                                      */
                    break;
                }
                default:
                    break;
                }
                /*
                if (e!=NULL) {
                    e->setLayer(NULL);
                    hatchLoop->addEntity(e);
                }*/
            }
        }

    }


    env->CallVoidMethod(obj, mid_addhatchdone);
    if (env->ExceptionCheck()) {
         qCritical("J_filterdxfrw::addHatch: Exception at env->CallVoidMethod(obj, mid_addhatchdone);");
        return;
    }


    /*
    RS_DEBUG->print("hatch->update()");
    if (hatch->validate()) {
        hatch->update();
    } else {
        graphic->removeEntity(hatch);
        RS_DEBUG->print(RS_Debug::D_ERROR,
                    "J_filterdxfrw::endEntity(): updating hatch failed: invalid hatch area");
    }*/

/*
    env->CallVoidMethod(obj, mid_addhatch, data->solid,data->scale,data->angle,env->NewStringUTF(data->name.c_str()),cx,cy,cr);
    if (env->ExceptionCheck()) {
         qCritical("J_filterdxfrw::addHatch: Exception at env->CallVoidMethod(obj, mid_addhatch, ...);");
        return;
    }
*/


}


/**
 * Implementation of the method which handles image entities.
 */
void J_filterdxfrw::addImage(const DRW_Image *data) {/*
    RS_DEBUG->print("RS_FilterDXF::addImage");

    RS_Vector ip(data->basePoint.x, data->basePoint.y);
    RS_Vector uv(data->secPoint.x, data->secPoint.y);
    RS_Vector vv(data->vx, data->vy);
    RS_Vector size(data->sizeu, data->sizev);

    RS_Image* image = new RS_Image( currentContainer,
            RS_ImageData(QString(data->ref.c_str()).toInt(NULL, 16),
                         ip, uv, vv, size,
                         QString(""), data->brightness,
                         data->contrast, data->fade));

    setEntityAttributes(image, data);
    currentContainer->appendEntity(image);*/
}



/**
 * Implementation of the method which links image entities to image files.
 */
void J_filterdxfrw::linkImage(const DRW_ImageDef *data) {/*
    RS_DEBUG->print("J_filterdxfrw::linkImage");

    int handle = QString(data->handle.c_str()).toInt(NULL, 16);
    QString sfile(QString::fromUtf8(data->name.c_str()));
    QFileInfo fiDxf(file);
    QFileInfo fiBitmap(sfile);

    // try to find the image file:

    // first: absolute path:
    if (!fiBitmap.exists()) {
        RS_DEBUG->print("File %s doesn't exist.",
                        (const char*)QFile::encodeName(sfile));
        // try relative path:
        QString f1 = fiDxf.absolutePath() + "/" + sfile;
        if (QFileInfo(f1).exists()) {
            sfile = f1;
        } else {
            RS_DEBUG->print("File %s doesn't exist.", (const char*)QFile::encodeName(f1));
            // try drawing path:
            QString f2 = fiDxf.absolutePath() + "/" + fiBitmap.fileName();
            if (QFileInfo(f2).exists()) {
                sfile = f2;
            } else {
                RS_DEBUG->print("File %s doesn't exist.", (const char*)QFile::encodeName(f2));
            }
        }
    }

    // Also link images in subcontainers (e.g. inserts):
    for (RS_Entity* e=graphic->firstEntity(RS2::ResolveNone);
            e!=NULL; e=graphic->nextEntity(RS2::ResolveNone)) {
        if (e->rtti()==RS2::EntityImage) {
            RS_Image* img = (RS_Image*)e;
            if (img->getHandle()==handle) {
                img->setFile(sfile);
                RS_DEBUG->print("image found: %s", (const char*)QFile::encodeName(img->getFile()));
                img->update();
            }
        }
    }

    // update images in blocks:
    for (uint i=0; i<graphic->countBlocks(); ++i) {
        RS_Block* b = graphic->blockAt(i);
        for (RS_Entity* e=b->firstEntity(RS2::ResolveNone);
                e!=NULL; e=b->nextEntity(RS2::ResolveNone)) {
            if (e->rtti()==RS2::EntityImage) {
                RS_Image* img = (RS_Image*)e;
                if (img->getHandle()==handle) {
                    img->setFile(sfile);
                    RS_DEBUG->print("image in block found: %s",
                                    (const char*)QFile::encodeName(img->getFile()));
                    img->update();
                }
            }
        }
    }
    RS_DEBUG->print("linking image: OK");*/
}




using std::map;
/**
 * Sets the header variables from the DXF file.
 */
void J_filterdxfrw::addHeader(const DRW_Header* data){/*
    RS_Graphic* container = NULL;
    if (currentContainer->rtti()==RS2::EntityGraphic) {
        container = (RS_Graphic*)currentContainer;
    } else return;

    map<std::string,DRW_Variant *>::const_iterator it;
    for ( it=data->vars.begin() ; it != data->vars.end(); it++ ){
        QString key = QString::fromStdString((*it).first);
        DRW_Variant *var = (*it).second;
        switch (var->type) {
        case DRW_Variant::COORD:
            container->addVariable(key,
#ifdef  RS_VECTOR2D
            RS_Vector(var->content.v->x, var->content.v->y), var->code);
#else
            RS_Vector(var->content.v->x, var->content.v->y, var->content.v->z), var->code);
#endif
            break;
        case DRW_Variant::STRING:
            container->addVariable(key, QString::fromUtf8(var->content.s->c_str()), var->code);
            break;
        case DRW_Variant::INTEGER:
            container->addVariable(key, var->content.i, var->code);
            break;
        case DRW_Variant::DOUBLE:
            container->addVariable(key, var->content.d, var->code);
            break;
        default:
            break;
        }

    }
    codePage = graphic->getVariableString("$DWGCODEPAGE", "ANSI_1252");
    textStyle = graphic->getVariableString("$TEXTSTYLE", "Standard");
    dimStyle = graphic->getVariableString("$DIMSTYLE", "Standard");

    QString acadver = versionStr = graphic->getVariableString("$ACADVER", "");
    acadver.replace(QRegExp("[a-zA-Z]"), "");
    bool ok;
    version=acadver.toInt(&ok);
    if (!ok) { version = 1021;}

    //detect if dxf lib are a old dxflib or libdxfrw<0.5.4 (used to correct mtext alignment)
    oldMText = false;
    QStringList comm = QString::fromStdString(data->getComments()).split('\n',QString::SkipEmptyParts);
    for (int i = 0; i < comm.size(); ++i) {
        QStringList comstr = comm.at(i).split(' ',QString::SkipEmptyParts);
        if (!comstr.isEmpty() && comstr.at(0) == "dxflib") {
            oldMText = true;
            break;
        } else if (comstr.size()>1 && comstr.at(0) == "dxfrw"){
            QStringList libversionstr = comstr.at(1).split('.',QString::SkipEmptyParts);
            if (libversionstr.size()<3) break;
            int libRelease = (libversionstr.at(1)+ libversionstr.at(2)).toInt();
            if (libversionstr.at(0)=="0" && libRelease < 54){
                oldMText = true;
                break;
            }
        }
    }*/
}


 void J_filterdxfrw::writeHeader(DRW_Header& data){}
 void J_filterdxfrw::writeEntities(){}
 void J_filterdxfrw::writeLTypes(){}
 void J_filterdxfrw::writeLayers(){}
 void J_filterdxfrw::writeTextstyles(){}
 void J_filterdxfrw::writeVports(){}
 void J_filterdxfrw::writeBlockRecords(){}
 void J_filterdxfrw::writeBlocks(){}
 void J_filterdxfrw::writeDimstyles(){}



// EOF

