/*****************************************************************************/
/*  ScriptDECC.cpp - javascript enabeled plugin for LibreCAD                 */
/*                                                                           */
/*  Copyright (C) 2013 Demas as, (halfdan.saetherskar@demas.no)              */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

#include <QDebug>
#include <sstream>
#include "scriptdecc.h"

ScriptDECC::ScriptDECC(QTextEdit *qTextEdit, CAD *cad):
    V8Base(qTextEdit), _cad(cad)
{
    initialize();
}

void ScriptDECC::setIncludePath(const string path)
{
    includePath = path;
}

string ScriptDECC::getIncludePath()
{
    return includePath;
}

bool ScriptDECC::initialize()
{
    HandleScope scope;

    Local<ObjectTemplate> libreCADObj = ObjectTemplate::New();
    libreCADObj->SetCallAsFunctionHandler(callAsFuncHandler, classPtrToExternal());

    Local<ObjectTemplate> globals = ObjectTemplate::New();
    globals->Set(static_cast<Handle<String> >(String::New("CAD")), libreCADObj, ReadOnly);

    V8Base::initialize(globals);

    return true;
}

Handle<Value> ScriptDECC::callAsFuncHandler(const Arguments &args)
{
    HandleScope scope;

    ScriptDECC *objPtr = externalToClassPtr<ScriptDECC>(args.Data());

    Local<ObjectTemplate> instanceTemplate = ObjectTemplate::New();
    instanceTemplate->SetInternalFieldCount(1);

    //Set version property
    Local<Number> tmpVersion = Number::New(0.1);
    instanceTemplate->Set(static_cast<Handle<String> >(String::New("version")), tmpVersion, ReadOnly);

    //Set newEntity property function
    Local<FunctionTemplate> tmpNewEntity = objPtr->makeStaticCallableFunc(newEntityHandler);
    instanceTemplate->Set(static_cast<Handle<String> >(String::New("newEntity")), tmpNewEntity, ReadOnly);

    //Set addEntity property function
    Local<FunctionTemplate> tmpAddEntity = objPtr->makeStaticCallableFunc(addEntityHandler);
    instanceTemplate->Set(static_cast<Handle<String> >(String::New("addEntity")), tmpAddEntity, ReadOnly);

    //Set getData property function
    Local<FunctionTemplate> tmpGetData = objPtr->makeStaticCallableFunc(getDataHandler);
    instanceTemplate->Set(static_cast<Handle<String> >(String::New("getData")), tmpGetData, ReadOnly);

    //Set updateData property function
    Local<FunctionTemplate> tmpUpdateData = objPtr->makeStaticCallableFunc(updateDataHandler);
    instanceTemplate->Set(static_cast<Handle<String> >(String::New("updateData")), tmpUpdateData, ReadOnly);

    //Set updateView property function
    Local<FunctionTemplate> tmpUpdateView = objPtr->makeStaticCallableFunc(updateViewHandler);
    instanceTemplate->Set(static_cast<Handle<String> >(String::New("updateView")), tmpUpdateView, ReadOnly);

    //Set setLayer property function
    Local<FunctionTemplate> tmpSetLayer = objPtr->makeStaticCallableFunc(setLayerHandler);
    instanceTemplate->Set(static_cast<Handle<String> >(String::New("setLayer")), tmpSetLayer, ReadOnly);

    //Set addLine property function
    Local<FunctionTemplate> tmpAddLine = objPtr->makeStaticCallableFunc(addLineHandler);
    instanceTemplate->Set(static_cast<Handle<String> >(String::New("addLine")), tmpAddLine, ReadOnly);

    //Set addText property function
    Local<FunctionTemplate> tmpAddText = objPtr->makeStaticCallableFunc(addTextHandler);
    instanceTemplate->Set(static_cast<Handle<String> >(String::New("addText")), tmpAddText, ReadOnly);

    //Set addInsert property function
    Local<FunctionTemplate> tmpAddInsert = objPtr->makeStaticCallableFunc(addInsertHandler);
    instanceTemplate->Set(static_cast<Handle<String> >(String::New("addInsert")), tmpAddInsert, ReadOnly);

    Local<Object> instance = instanceTemplate->NewInstance();
    instance->SetInternalField(0, Integer::New(objPtr->instanceList.size()));

    qDebug() <<"Internal field set to "<<instance->GetInternalField(0)->ToNumber()->Value()<<endl;
    objPtr->instanceList.push_back(Persistent<Object>::New(instance));

    return scope.Close(instance);
}


/**
 * @brief ScriptDECC::newEntityHandler
 *
 * Precondition
 *
 *     args[0] = A string-representation of an DPI::ETYPE,
 *               as defined in the document_interface.h file.
 *
 * Postcondition
 *
 *     A new plugin entity is hopefully created, and the entity id
 *     is returned to the JavaScript code.
 *
 * Example of usage
 *
 *    var l = new CAD();
 *    var id = l.newEntity('LINE');
 *
 */
Handle<Value> ScriptDECC::newEntityHandler(const Arguments &args)
{
    HandleScope scope;

    Local<Object> thisObj = args.This();
    if(thisObj.IsEmpty())
        return v8::ThrowException(v8::String::New("Error getting hold of 'this'."));

    if(args.Length() != 1)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 1, got "<<args.Length()<<endl;
        return v8::ThrowException(v8::String::New(strstream.str().c_str()));
    }

    if (!args[0]->IsString())
        return v8::ThrowException(v8::String::New("The argument must be a string object."));

    String::Utf8Value val(Local<String>::Cast(args[0]));
    QString strArg(*val);

    ScriptDECC *objPtr = externalToClassPtr<ScriptDECC>(args.Data());

    int id = objPtr->_cad->newEntity(strArg);

    if (id > 0)
        return v8::Number::New(id);
    else
        return v8::ThrowException(v8::String::New("Entity type is not supported."));
}

/**
 * @brief ScriptDECC::getDataHandler
 *
 * Precondition
 *
 *     args[0] = A integer representing the 'Entity Id'
 *     args[1] = A string-representation of an DPI::ETYPE,
 *               as defined in the document_interface.h file.
 *
 * Postcondition
 *
 *    nop
 *
 * Example of usage
 *
 *    var l = new CAD();
 *    var id = l.newEntity('LINE');
 *    var layername = l.getData(id,'LAYER');
 *
 */
Handle<Value> ScriptDECC::getDataHandler(const Arguments &args)
{
    HandleScope scope;

    Local<Object> thisObj = args.This();
    if(thisObj.IsEmpty())
        return v8::ThrowException(v8::String::New("Error getting hold of 'this'."));

    if(args.Length() != 2)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 2, got "<<args.Length()<<endl;
        return v8::ThrowException(v8::String::New(strstream.str().c_str()));
    }

    if (!args[0]->IsInt32())
        return v8::ThrowException(v8::String::New("The first argument must be an integer object."));

    if (!args[1]->IsString())
        return v8::ThrowException(v8::String::New("The secound argument must be a string object."));


    int eid = args[0]->ToNumber()->Value();

    String::Utf8Value val(Local<String>::Cast(args[1]));
    QString strArg(*val);

    ScriptDECC *objPtr = externalToClassPtr<ScriptDECC>(args.Data());

    QString retval = objPtr->_cad->getData(eid,strArg);
    return v8::String::New(retval.toAscii());
}

/**
 * @brief ScriptDECC::updateDataHandler
 *
 * Precondition
 *
 *     args[0] = A integer representing the 'Entity Id'
 *     args[1] = A string-representation of an DPI::ETYPE,
 *               as defined in the document_interface.h file.
 *     args[2] = The new value.
 *
 * Postcondition
 *
 *    The entity data is updated.
 *
 * Example of usage
 *
 *    var l = new CAD();
 *    var id = l.newEntity('LINE');
 *    l.updateData(id,'STARTX',10);
 *    l.updateData(id,'LTYPE','DotLine');
 *
 */
Handle<Value> ScriptDECC::updateDataHandler(const Arguments &args)
{
    HandleScope scope;

    Local<Object> thisObj = args.This();
    if(thisObj.IsEmpty())
        return v8::ThrowException(v8::String::New("Error getting hold of 'this'."));

    if(args.Length() != 3)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 3, got "<<args.Length()<<endl;
        return v8::ThrowException(v8::String::New(strstream.str().c_str()));
    }

    if (!args[0]->IsInt32())
        return v8::ThrowException(v8::String::New("The first argument must be an integer object."));

    if (!args[1]->IsString())
        return v8::ThrowException(v8::String::New("The secound argument must be a string object."));

    int eid = args[0]->ToNumber()->Value();

    String::Utf8Value val(Local<String>::Cast(args[1]));
    QString strArg(*val);

    ScriptDECC *objPtr = externalToClassPtr<ScriptDECC>(args.Data());

    if (args[2]->IsNumber())
    {
        double dvalue = args[2]->ToNumber()->Value();
        objPtr->_cad->updateData(eid,strArg,dvalue);
    }
    else if (args[2]->IsString())
    {
        String::Utf8Value sval(Local<String>::Cast(args[2]));
        objPtr->_cad->updateData(eid,strArg,*sval);
    }
    else
    {
        return v8::ThrowException(v8::String::New("The third argument must be a number or a string."));
    }

    return v8::Undefined();
}

/**
 * @brief ScriptDECC::addEntityHandler
 *
 * Precondition
 *
 *     args[0] = A integer representing the 'Entity Id'
 *
 * Postcondition
 *
 *     The plugin entity is added to the LibreCAD document.
 *
 * Example of usage
 *
 *    var l = new CAD();
 *    var id = l.newEntity('LINE');
 *    l.addEntity(id);
 *
 */
Handle<Value> ScriptDECC::addEntityHandler(const Arguments &args)
{
    HandleScope scope;

    Local<Object> thisObj = args.This();
    if(thisObj.IsEmpty())
        return v8::ThrowException(v8::String::New("Error getting hold of 'this'."));

    if(args.Length() != 1)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 1, got "<<args.Length()<<endl;
        return v8::ThrowException(v8::String::New(strstream.str().c_str()));
    }

    if (!args[0]->IsInt32())
        return v8::ThrowException(v8::String::New("The first argument must be an integer object."));

    int eid = args[0]->ToNumber()->Value();

    ScriptDECC *objPtr = externalToClassPtr<ScriptDECC>(args.Data());
    objPtr->_cad->addEntity(eid);

    return v8::Undefined();
}

/**
 * @brief ScriptDECC::updateViewHandler
 *
 * Precondition
 *
 *     nop
 *
 * Postcondition
 *
 *     The LibreCAD document view is updated.
 *
 * Example of usage
 *
 *    var l = new CAD();
 *    l.updateView();
 *
 */
Handle<Value> ScriptDECC::updateViewHandler(const Arguments &args)
{
    HandleScope scope;

    Local<Object> thisObj = args.This();
    if(thisObj.IsEmpty())
        return v8::ThrowException(v8::String::New("Error getting hold of 'this'."));

    ScriptDECC *objPtr = externalToClassPtr<ScriptDECC>(args.Data());
    objPtr->_cad->updateView();

    return v8::Undefined();
}

/**
 * @brief ScriptDECC::setLayerHandler
 *
 * Precondition
 *
 *     nop
 *
 * Postcondition
 *
 *     The LibreCAD document's active layer is changed.
 *
 * Example of usage
 *
 *    var l = new CAD();
 *    l.setLayer('my new layername');
 *
 */
Handle<Value> ScriptDECC::setLayerHandler(const Arguments &args)
{
    HandleScope scope;

    Local<Object> thisObj = args.This();
    if(thisObj.IsEmpty())
        return v8::ThrowException(v8::String::New("Error getting hold of 'this'."));

    if(args.Length() != 1)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 1, got "<<args.Length()<<endl;
        return v8::ThrowException(v8::String::New(strstream.str().c_str()));
    }

    if (!args[0]->IsString())
        return v8::ThrowException(v8::String::New("The first argument must be a string object."));

    String::Utf8Value val(Local<String>::Cast(args[0]));
    QString strArg(*val);

    ScriptDECC *objPtr = externalToClassPtr<ScriptDECC>(args.Data());
    objPtr->_cad->setLayer(strArg);

    return v8::Undefined();
}

/**
 * @brief ScriptDECC::addLineHandler
 *
 * Precondition
 *
 *     args[0] = A integer representing x1
 *     args[1] = A integer representing y1
 *     args[2] = A integer representing x2
 *     args[3] = A integer representing y2
 *
 * Postcondition
 *
 *    A line is added to the LibreCAD document.
 *
 * Example of usage
 *
 *    var l = new CAD();
 *    l.addLine(10,10,20,20);
 *
 */
Handle<Value> ScriptDECC::addLineHandler(const Arguments &args)
{
    HandleScope scope;

    Local<Object> thisObj = args.This();
    if(thisObj.IsEmpty())
        return v8::ThrowException(v8::String::New("Error getting hold of 'this'."));

    if(args.Length() != 4)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 4, got "<<args.Length()<<endl;
        return v8::ThrowException(v8::String::New(strstream.str().c_str()));
    }

    for (int i = 0;i < 4; i++)
        if (!args[i]->IsInt32())
            return v8::ThrowException(v8::String::New("All arguments must be integer objects."));

    double  x1 = args[0]->ToNumber()->Value();
    double  y1 = args[1]->ToNumber()->Value();
    double  x2 = args[2]->ToNumber()->Value();
    double  y2 = args[3]->ToNumber()->Value();

    ScriptDECC *objPtr = externalToClassPtr<ScriptDECC>(args.Data());
    objPtr->_cad->addLine(x1,y1,x2,y2);

    return v8::Undefined();
}



/**
 * @brief ScriptDECC::addLineHandler
 *
 * Precondition
 *
 *     args[0] = A string representing the text to add.
 *     args[1] = A string representing the style to use.
 *     args[2] = A double representing x, start-point-coordinate
 *     args[3] = A double representing y, start-point-coordinate
 *     args[4] = A double representing the height
 *     args[5] = A double representing the angle
 *     args[6] = An integer representing DPI:HAlign (1,2,3)
 *     args[7] = An integer representing DPI:VAlign (1,2,3)
 *
 * Postcondition
 *
 *    A text is added to the LibreCAD document.
 *
 * Example of usage
 *
 *    var l = new CAD();
 *    l.addText('hello world','iso',10,10,20,20,1,1);
 *
 */
Handle<Value> ScriptDECC::addTextHandler(const Arguments &args)
{
    HandleScope scope;

    Local<Object> thisObj = args.This();
    if(thisObj.IsEmpty())
        return v8::ThrowException(v8::String::New("Error getting hold of 'this'."));

    if(args.Length() != 8)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 8, got "<<args.Length()<<endl;
        return v8::ThrowException(v8::String::New(strstream.str().c_str()));
    }

    if (!args[0]->IsString())
        return v8::ThrowException(v8::String::New("Arg 0 must be a string."));
    if (!args[1]->IsString())
        return v8::ThrowException(v8::String::New("Arg 1 must be a string."));
    for(int i = 2; i < 8; i++)
        if (!args[i]->IsNumber())
            return v8::ThrowException(v8::String::New("Arg 2-7 must be number-objects."));

    double  x = args[2]->ToNumber()->Value();
    double  y = args[3]->ToNumber()->Value();
    double  height = args[4]->ToNumber()->Value();
    double  angle = args[5]->ToNumber()->Value();
    int  ha = args[6]->ToNumber()->Value();
    int  va = args[7]->ToNumber()->Value();

    String::Utf8Value valTxt(Local<String>::Cast(args[0]));
    QString text(*valTxt);

    String::Utf8Value valStyle(Local<String>::Cast(args[1]));
    QString style(*valStyle);

    QPointF start(x,y);

    ScriptDECC *objPtr = externalToClassPtr<ScriptDECC>(args.Data());
    objPtr->_cad->addText(text, style, &start, height, angle,ha,va );

    return v8::Undefined();
}




/**
 * @brief ScriptDECC::addInsertHandler
 *
 * Precondition
 *
 *     args[0] = A string representing the name.
 *     args[1] = A double representing insert x
 *     args[2] = A double representing insert y
 *     args[3] = A double representing scale x
 *     args[4] = A double representing scale y
 *     args[5] = A double representing the rotation angle
 *
 * Postcondition
 *
 *    A dxf file is added to the LibreCAD document.
 *
 * Example of usage
 *
 *    var l = new CAD();
 *    l.addInsert('inntak',10,10,1,1,0);
 *
 */      
Handle<Value> ScriptDECC::addInsertHandler(const Arguments &args)
{
    HandleScope scope;

    Local<Object> thisObj = args.This();
    if(thisObj.IsEmpty())
        return v8::ThrowException(v8::String::New("Error getting hold of 'this'."));

    if(args.Length() != 6)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 6, got "<<args.Length()<<endl;
        return v8::ThrowException(v8::String::New(strstream.str().c_str()));
    }

    if (!args[0]->IsString())
        return v8::ThrowException(v8::String::New("Arg 0 must be a string."));    
    for(int i = 1; i < 5; i++)
        if (!args[i]->IsInt32())
            return v8::ThrowException(v8::String::New("Arg 1-5 must be number-objects."));

    double  x = args[1]->ToNumber()->Value();
    double  y = args[2]->ToNumber()->Value();
    double  scale_x = args[3]->ToNumber()->Value();
    double  scale_y = args[4]->ToNumber()->Value();
    double  rot_angle = args[5]->ToNumber()->Value();

    String::Utf8Value valName(Local<String>::Cast(args[0]));
    QString name(*valName);

    QPointF insert_point(x,y);
    QPointF scale_vector(scale_x,scale_y);

    ScriptDECC *objPtr = externalToClassPtr<ScriptDECC>(args.Data());
    objPtr->_cad->addInsert(name, insert_point, scale_vector, rot_angle);

    return v8::Undefined();
}











