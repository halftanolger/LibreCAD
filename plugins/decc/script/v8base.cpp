#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <QDebug>
#include <QTextEdit>
#include <boost/algorithm/string.hpp>
#include "v8base.h"

using namespace std;
using namespace v8;

V8Base::V8Base(QTextEdit *qTextEdit)
    :console(qTextEdit)
{
}

V8Base::~V8Base()
{
    context.Dispose();
}

void V8Base::initialize(Local<ObjectTemplate> passedGlobals)
{
    HandleScope scope;

    // An ObjectTemplate is used to create objects at runtime.
    if (passedGlobals.IsEmpty())
        passedGlobals = ObjectTemplate::New();

    passedGlobals->Set(static_cast<Handle<String>>(String::New("logInfo")), makeStaticCallableFunc(logInfoCallback), ReadOnly);
    passedGlobals->Set(static_cast<Handle<String>>(String::New("logDebug")), makeStaticCallableFunc(logDebugCallback), ReadOnly);
    passedGlobals->Set(static_cast<Handle<String>>(String::New("logError")), makeStaticCallableFunc(logErrorCallback), ReadOnly);
    passedGlobals->Set(static_cast<Handle<String>>(String::New("clearConsole")),makeStaticCallableFunc(clearConsoleCallback), ReadOnly);

    context = Context::New(0,passedGlobals);
}

Local<FunctionTemplate> V8Base::makeStaticCallableFunc(InvocationCallback func)
{
    HandleScope scope;
    Local<FunctionTemplate> funcTemplate = FunctionTemplate::New(func, classPtrToExternal());
    return scope.Close(funcTemplate);
}

Local<External> V8Base::classPtrToExternal()
{
    HandleScope scope;
    return scope.Close(External::New(reinterpret_cast<void *>(this)));
}

Handle<Value> V8Base::clearConsoleCallback(const Arguments &args)
{
    HandleScope scope;
    V8Base *objPtr = externalToClassPtr<V8Base>(args.Data());
    objPtr->clearConsole();
    return v8::Undefined();
}

Handle<Value> V8Base::logDebugCallback(const Arguments &args)
{
    HandleScope scope;
    V8Base *objPtr = externalToClassPtr<V8Base>(args.Data());
    if(args.Length() != 1)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 1, got "<<args.Length()<<endl;
        objPtr->logError(strstream.str());
    }
    else
    {
        String::Utf8Value val(Local<String>::Cast(args[0]));
        objPtr->logDebug(*val);
    }
    return v8::Null();
}

Handle<Value> V8Base::logInfoCallback(const Arguments &args)
{
    HandleScope scope;
    V8Base *objPtr = externalToClassPtr<V8Base>(args.Data());
    if(args.Length() != 1)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 1, got "<<args.Length()<<endl;
        objPtr->logError(strstream.str());
    }
    else
    {
        String::Utf8Value val(Local<String>::Cast(args[0]));
        objPtr->logInfo(*val);
    }
    return v8::Null();
}

Handle<Value> V8Base::logErrorCallback(const Arguments &args)
{
    HandleScope scope;
    V8Base *objPtr = externalToClassPtr<V8Base>(args.Data());
    if(args.Length() != 1)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 1, got "<<args.Length()<<endl;
        std::cerr << strstream.str();
    }
    else
    {
        String::Utf8Value val(Local<String>::Cast(args[0]));
        objPtr->logError(*val);
    }
    return v8::Null();
}

void V8Base::logDebug(const string &str)
{
    log(debug, str);
}

void V8Base::logInfo(const string &str)
{
    log(info, str);
}

void V8Base::logError(const string &str)
{
    log(error, str);
}

void V8Base::log(const LogType type, const string &str)
{
    qDebug() << str.c_str();

    QString msg;
    switch(type) {
    case debug:
        msg.append("<FONT color=green>");
        break;
    case info:
        msg.append("<FONT color=black>");
        break;
    case error:
        msg.append("<FONT color=red>");
        break;
    default:
        msg.append("<FONT color=black>");
    }
    msg.append(str.c_str());
    msg.append("</FONT>");

    console->append(msg);
}

void V8Base::clearConsole()
{
    console->clear();
}

bool V8Base::runScript(const string &scriptStr)
{
    HandleScope scope;
    Local<String> source = String::New(scriptStr.c_str(), scriptStr.length());
    if(source.IsEmpty())
    {
        logError("Failed to generate string source\n");
        return false;
    }

    Context::Scope contextScope(context);

    TryCatch tryCatch;

    Local<Script> script = Script::Compile(source);

    if(script.IsEmpty())
    {
        Local<Message> message = tryCatch.Message();
        if (!message.IsEmpty())
        {
            String::Utf8Value error(tryCatch.Exception());
            String::Utf8Value filename(message->GetScriptResourceName());
            int linenum = message->GetLineNumber();
            stringstream  ss;
            ss << "File:" << *filename << " Linenr.:" << linenum << " Error:" << *error;
            string msg = ss.str();
            logError(msg);
        } else
        {
            String::Utf8Value error(tryCatch.Exception());
            logError(*error);
        }
        return false;
    }

    Local<Value> result = script->Run();

    if(result.IsEmpty())
    {
        Local<Message> message = tryCatch.Message();
        if (!message.IsEmpty())
        {
            String::Utf8Value error(tryCatch.Exception());
            String::Utf8Value filename(message->GetScriptResourceName());
            int linenum = message->GetLineNumber();
            stringstream  ss;
            ss << "File:" << *filename << " Linenr.:" << linenum << " Error:" << *error;
            string msg = ss.str();
            logError(msg);
        } else
        {
            String::Utf8Value error(tryCatch.Exception());
            logError(*error);
        }
        return false;
    }

    return true;
}

string V8Base::includeFile(const string& incFileName, string currentContent)
{
    // Only append include-files which have not already been loaded.
    vector<string>::iterator loadedFilesItr;
    for(loadedFilesItr = loadedIncludeFiles.begin(); loadedFilesItr != loadedIncludeFiles.end(); loadedFilesItr++)
        if (!incFileName.compare(*loadedFilesItr))
            return string();

    loadedIncludeFiles.push_back(incFileName);

    string fileName(getIncludePath() + incFileName);

    ifstream inFile(fileName.c_str());
    if(!inFile.is_open())
    {
        logError("Failed to open file ");
        logError(fileName);
        return string();
    }
    inFile.seekg(0, ios_base::end);

    const int strLen = static_cast<streamoff>(inFile.tellg());
    const int bufSize = strLen+1;

    char *buf = new char[bufSize];
    inFile.seekg(0, ios_base::beg);
    inFile.read(buf, strLen);
    if(inFile.fail() && !inFile.eof())
    {
        logError("Failed to read file ");
        logError(fileName);
        inFile.close();
        return string();
    }

    inFile.close();

    string theScript(buf, strLen);
    string INCLUDE("#include ");

    vector< string > lines;
    boost::algorithm::split( lines, theScript, boost::algorithm::is_any_of("\n"), boost::algorithm::token_compress_on );
    string s;
    vector<string>::iterator itr;
    int lineNr = 0;
    for(itr = lines.begin(); itr != lines.end(); itr++)
    {
        lineNr++;
        if (strncmp((string(*itr)).c_str(),INCLUDE.c_str(),strlen(INCLUDE.c_str())) == 0)
        {
            string tmpstring(*itr);
            size_t pos = strlen(INCLUDE.c_str());
            string includeFileName = tmpstring.substr(pos);
            boost::trim(includeFileName);

            if (includeFileName.length() == 0)
            {
                ostringstream strstream;
                strstream<<"Error: #include ?, in file '" << fileName << "' at line "<<lineNr<<endl;
                logError(strstream.str().c_str());
                return string();
            }
            else
            {
                s.append(includeFile(includeFileName, currentContent));
            }
        }
        else
        {
            s.append(*itr);
            s.append("\n");
        }

    }
    currentContent.append(s);
    return currentContent;
}


string V8Base::readFile(const string &fileName)
{
    ifstream inFile(fileName.c_str());
    if(!inFile.is_open())
    {
        logError("Failed to open file ");
        logError(fileName);
        logError("\n");
        return string();
    }
    inFile.seekg(0, ios_base::end);

    const int strLen = static_cast<streamoff>(inFile.tellg());
    const int bufSize = strLen+1;

    char *buf = new char[bufSize];
    inFile.seekg(0, ios_base::beg);
    inFile.read(buf, strLen);
    if(inFile.fail() && !inFile.eof())
    {
        logError("Failed to read file\n");
        inFile.close();
        return string();
    }

    inFile.close();

    string theScript(buf, strLen);

    // Do some simple 'preprosessing'; replace #include 'something' with the
    // appropriate 'something', and append a call to the main() -funtion at the
    // end of the script.

    loadedIncludeFiles.clear();

    string MAIN("main();\n");
    string INCLUDE("#include ");

    vector< string > lines;
    boost::algorithm::split( lines, theScript, boost::algorithm::is_any_of("\n"), boost::algorithm::token_compress_on );
    string s;
    vector<string>::iterator itr;
    int lineNr = 0;
    for(itr = lines.begin(); itr != lines.end(); itr++)
    {
        lineNr++;
        if (strncmp((string(*itr)).c_str(),INCLUDE.c_str(),strlen(INCLUDE.c_str())) == 0)
        {
            string tmpstring(*itr);
            size_t pos = strlen(INCLUDE.c_str());
            string includeFileName = tmpstring.substr(pos);
            boost::trim(includeFileName);

            if (includeFileName.length() == 0)
            {
                ostringstream strstream;
                strstream<<"Error: #include ?, at line "<<lineNr<<endl;
                logError(strstream.str().c_str());
                return "";
            }
            else
            {
                string cc;
                s.append(includeFile(includeFileName, cc));
            }
        }
        else
        {
            s.append(*itr);
            s.append("\n");
        }
    }

    s.append(MAIN);
    return s;
}



