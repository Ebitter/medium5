#include "CppSQLite3.h"
#include <iostream>
#include <sys/timeb.h>
int main(int argc, char *argv[])
{
    const size_t INSERT_CNT = 100*10000;
    const int FEATURE_LEN = 1056;
    CppSQLite3DB dbHandle;

    unsigned char feature[FEATURE_LEN];
    for(int i=0; i<FEATURE_LEN;i++)
    {
        feature[i] = i%127;
    }
    try
    {
        dbHandle.open("./faceInfo.db");
        dbHandle.execDML("PRAGMA synchronous = OFF; ");
        dbHandle.execDML("PRAGMA journal_mode=WAL; ");
        std::cout<<"create faceInfo.db success!"<<std::endl;

        //创建表
        std::string strSql = "CREATE TABLE [snapface] ([camid] TEXT, [featureData] BLOB,[kfktimestamp] TEXT,[trackid] INTEGER, [statecode] INTEGER, [aligndata] TEXT,[sectionaldrawdata] TEXT,[float] REAL);";
        dbHandle.execDML(strSql.c_str());
        std::cout<<"create snapface table success!"<<std::endl;

        //创建索引
        //dbHandle.execDML("CREATE INDEX \"kfktimestamp\" on snapface (kfktimestamp ASC);");
        struct timeb tb;
        ftime(&tb);
        std::cout<<"start timestamp=" << tb.time<<std::endl;

        dbHandle.execDML("begin transaction;");
        strSql = "insert into snapface values(?,?,?,?,?,?,?,?);";
        CppSQLite3Statement stmt = dbHandle.compileStatement(strSql.c_str());
        for (int i = 0; i<INSERT_CNT; i++)
        {
            int pos = 1;
            stmt.bind(pos++, "camid");
            stmt.bind(pos++, feature, sizeof(feature));
            stmt.bind(pos++, "kfktimestamp");
            stmt.bind(pos++, 0);
            stmt.bind(pos++, 0);
            stmt.bind(pos++, "aligndata");
            stmt.bind(pos++, "sectionaldrawdata");
            stmt.bind(pos++, 3.1415926);

            stmt.execDML();
            stmt.reset();
        }
        dbHandle.execDML("commit transaction;");

        std::cout<<"insert features finish!"<<std::endl;
        struct timeb tend;
        ftime(&tend);
        std::cout<<"start timestamp=" << tend.time<<std::endl;
        std::cout<<"insert consume time=" << (tend.time*1000+tend.millitm - tb.time*1000-tb.millitm)/1000.0f << "S" <<std::endl;

    }
    catch (CppSQLite3Exception & e)
    {
        printf("%s", e.errorMessage());
        dbHandle.execDML("rollback transaction;");
    }
    catch (...)
    {
        printf("ERROR_LOCALFEATURE_SQLITE3_INSERT");
        dbHandle.execDML("rollback transaction;");
    }
}
