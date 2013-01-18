/*****************************************************************************/
/*  ScriptDECC.h - javascript enabeled plugin for LibreCAD                   */
/*                                                                           */
/*  Copyright (C) 2013 Demas as, (halfdan.saetherskar@demas.no)              */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 2 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/
/*
 * Here I define my extension to the V8 JavaScript engine. So far I try to
 * support these LibreCAD plugin features:
 *
 * var cad = new CAD();
 *
 * var id = cad.newEntity('LINE');
 *
 */

#ifndef SCRIPTDECC_H
#define SCRIPTDECC_H

#include "v8base.h"
#include "cad.h"

class ScriptDECC : public V8Base
{

public:

    ScriptDECC(QTextEdit *qTextEdit, CAD *cad);
    void setIncludePath(const string path);
    string getIncludePath();
    bool initialize();

private:        

    // Prevent copying by hiding copyconstructor and
    // the copy assignment operator.
    ScriptDECC(const ScriptDECC &copy);
    ScriptDECC& operator=(const ScriptDECC&);

    static Handle<Value>callAsFuncHandler(const Arguments &args);    
    static Handle<Value>newEntityHandler(const Arguments &args);
    static Handle<Value>addEntityHandler(const Arguments &args);
    static Handle<Value>getDataHandler(const Arguments &args);
    static Handle<Value>updateDataHandler(const Arguments &args);
    static Handle<Value>updateViewHandler(const Arguments &args);
    static Handle<Value>setLayerHandler(const Arguments &args);
    static Handle<Value>addLineHandler(const Arguments &args);
    static Handle<Value>addTextHandler(const Arguments &args);

    CAD* _cad;
    string includePath;
    vector <Persistent<Object> > instanceList;

};

#endif // SCRIPTDECC_H
