#include "GarrysMod/Lua/Interface.h"
#include <pqxx/pqxx>
#include <pqxx/version>
#include <stdio.h>

#define GM_PSQL_VERSION 1.0

using namespace GarrysMod::Lua;

int connect( lua_State* state ){
    char* conninfo;
    
    if(LUA->IsType(1, GarrysMod::Lua::Type::STRING)
    && LUA->IsType(2, GarrysMod::Lua::Type::NIL)){
        //We are using Libpq strings to connect
        strcpy(conninfo, LUA->GetString(1));
    }else{
        //We are using a tuple to connect: dbname, user, password, host, port
        
        LUA->CheckType(1, GarrysMod::Lua::Type::STRING); //db
        LUA->CheckType(2, GarrysMod::Lua::Type::STRING); //user
        LUA->CheckType(3, GarrysMod::Lua::Type::STRING); //password
        
        //LUA->CheckType(4, GarrysMod::Lua::Type::STRING); //host
        //LUA->CheckType(5, GarrysMod::Lua::Type::NUMBER); //port
        
        std::string database = LUA->GetString(1);
        std::string user = LUA->GetString(2);
        std::string password = LUA->GetString(3);
        std::string host = "127.0.0.1";
        unsigned int port = 5432;
        unsigned int timeout = 10;
        
        if(LUA->IsType(4, GarrysMod::Lua::Type::STRING))
            host = LUA->GetString(4);
        
        if(LUA->IsType(5, GarrysMod::Lua::Type::NUMBER))
            port = (int)LUA->GetNumber(5);
            
        if(LUA->IsType(5, GarrysMod::Lua::Type::NUMBER))
            timeout = (int)LUA->GetNumber(6);
            
        snprintf(conninfo, sizeof(conninfo),
            "dbname=%s user=%s password=%s host=%s port=%i connect_timeout=%i",
            database, user, password, host, port, timeout
        );
    }
    
	return 1;
}


//
// Called when you module is opened
//
GMOD_MODULE_OPEN(){
	LUA->PushSpecial( GarrysMod::Lua::SPECIAL_GLOB );
        LUA->CreateTable();
            //Version
            LUA->CreateTable();
                //Push versions
                LUA->PushNumber(GM_PSQL_VERSION);
                LUA->SetField(-2, "module");
                
                LUA->PushNumber(std::stof("PQXX_VERSION_MAJOR + PQXX_VERSION_MINOR"));
                LUA->SetField(-2, "libpg");
    
            LUA->SetField(-2, "version");
            
            //Connection function
            LUA->PushCFunction(connect);
            LUA->SetField(-2, "connect");
        
        LUA->SetField(-2, "postgres");
    LUA->Pop();
	return 0;
}

//
// Called when your module is closed
//
GMOD_MODULE_CLOSE(){
	return 0;
}
