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
/*
 * Mostly everything here comes from these links:
 * https://developers.google.com/v8/embed
 * https://www.homepluspower.info/2010/06/v8-javascript-engine-tutorial-part-1.html
 * http://www.codeproject.com/Articles/29109/Using-V8-Google-s-Chrome-JavaScript-Virtual-Machin
 *
 * This base-class contain some log-function, for logging to a console. I
 * also put som 'local magic' into the read-script-file function; the support
 * for 'include directive' and the 'hidden' call to a javascript main-function.
 *
 * By doing that, I can write javascript-programs looking like this:
 *
 * #include an-other-javascript-file.js
 *
 * function main() {
 *  logInfo("Hello World");
 *  var a = function-in-the-other-javascrip-file(1,2,3);
 * }
 *
 */

#ifndef V8BASE_H
#define V8BASE_H

#include <string>
#include <iostream>
#include <QTextEdit>
#include <v8.h>

using namespace std;
using namespace v8;

class V8Base
{
public:

    enum LogType {debug, info, error};

    virtual ~V8Base() = 0;

    virtual string readFile(const string &fileName);
    virtual bool runScript(const string &str);
    virtual void setIncludePath(const string path) = 0;
    virtual string getIncludePath() = 0;

protected:

    explicit V8Base(QTextEdit *qTextEdit = 0);

    void initialize(Local<ObjectTemplate> passedGlobals = Local<ObjectTemplate>());

    virtual void logDebug(const string &str);
    virtual void logInfo(const string &str);
    virtual void logError(const string &str);
    virtual void log(const LogType type, const string &str);
    virtual void clearConsole();

    static Handle<Value> clearConsoleCallback(const Arguments &args);
    static Handle<Value> logDebugCallback(const Arguments &args);
    static Handle<Value> logInfoCallback(const Arguments &args);
    static Handle<Value> logErrorCallback(const Arguments &args);

    Handle<Context> getContext() { return context; }
    Local<FunctionTemplate> makeStaticCallableFunc(InvocationCallback func);
    Local<External> classPtrToExternal();


    //Template fucntion: converts an 'External' to a V8Base pointer
    template <typename T>
    static T *externalToClassPtr(Local<Value> data)
    {
        if(data.IsEmpty())
            cout<<"Data empty"<<endl;
        else if(!data->IsExternal())
            cout<<"Data not external"<<endl;
        else
            return static_cast<T *>(External::Unwrap(data));

        //If function gets here, one of the checks above failed
        return 0;
    }

private:

    // A script file can contain '#include' statements.
    string includeFile(const string &fileName, string currentContent);
    vector<string> loadedIncludeFiles;

    // Prevent copying by hiding copyconstructor and
    // the copy assignment operator.
    V8Base(const V8Base &copy);
    V8Base& operator=(const V8Base&);

    // All log -functions goes to the console,
    // represented by a QTextEdit object.
    QTextEdit *console;

    // Out main context is persitent, i.e. 'heap allocated'. It all happends
    // in the initialize() -method. It will be de -allocated in ~VBase().
    Persistent<Context> context;

};

#endif // V8BASE_H
