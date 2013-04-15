

#include <string.h>
#include <cstring>
#include <QDebug>
#include "Jlibdxfrw.h"

#include "j_filterdxfrw.h"

JNIEXPORT jboolean JNICALL Java_no_demas_dcad_dxf_Jlibdxfrw_read
(JNIEnv *env, jobject obj, jstring filename)
{
    const char *str = env->GetStringUTFChars(filename, 0);
    char cap[256];
    strcpy(cap, str);
    env->ReleaseStringUTFChars(filename, str);

    J_filterdxfrw *dxf = new J_filterdxfrw();
    bool status = dxf->fileImport(env,obj, QString(cap));
    delete dxf;

    return status;
}

int main()
{
    return 0;
}
