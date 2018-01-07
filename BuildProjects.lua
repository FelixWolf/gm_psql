solution "gmsv_psql"

	language "C++"
	location ( "out/" .. os.get() .."/src/")
	flags { "Symbols", "NoEditAndContinue", "NoPCH", "StaticRuntime", "EnableSSE" }
	targetdir ( "lib/" .. os.get() .. "/" )
    
	includedirs { "./include/" }
	linkoptions{ "-lpqxx -lpq" }
    
	configurations
	{ 
		"Release"
	}
	
	configuration "Release"
		defines { "NDEBUG" }
		flags{ "Optimize", "FloatFast" }
	
	project "gmsv_psql"
		if os.get() == "windows" then
			defines{ "WIN32" }
		elseif os.get() == "linux" then
			defines{ "LINUX" }
		elseif os.get() == "macosx" then
			defines{ "MAC" }
		end
        
		defines { "GMMODULE" }
		files { "src/**.*", "./include/**.*" }
        targetname( "gmsv_psql_" .. os.get())
        targetdir("out/" .. os.get() .."/bin/")
		kind "SharedLib"
		
