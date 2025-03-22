#include "il2cpp.hpp"

#include <Windows.h>

namespace il2cpp
{
	HMODULE game = nullptr;

	void init( )
	{
		game = GetModuleHandle( "GameAssembly.dll" );
	}

	Il2CppDomain* get_domain( )
	{
		return reinterpret_cast< Il2CppDomain * ( * )( ) >( GetProcAddress( game, "il2cpp_domain_get" ) )( );
	}	
	Il2CppAssembly** domain_get_assemblies( Il2CppDomain* domain, size_t* size )
	{
		return reinterpret_cast< Il2CppAssembly ** ( * )( Il2CppDomain*, size_t* ) >( GetProcAddress( game, "ab418c_wasting_your_life" ) )( domain, size );
	}
	Il2CppClass* image_get_class( Il2CppImage* img, int idx )
	{
		return reinterpret_cast< Il2CppClass * ( * )( Il2CppImage*, int ) >( GetProcAddress( game, "il2cpp_image_get_class" ) )( img, idx );
	}
	int image_get_class_count( Il2CppImage* img )
	{
		return reinterpret_cast< int ( * )( Il2CppImage* ) >( GetProcAddress( game, "il2cpp_image_get_class_count" ) )( img );
	}
	bool class_is_enum( Il2CppClass* kls )
	{
		return reinterpret_cast< bool ( * )( Il2CppClass* ) >( GetProcAddress( game, "il2cpp_class_is_enum" ) )( kls );
	}	
	bool class_is_valuetype( Il2CppClass* kls )
	{
		return reinterpret_cast< bool ( * )( Il2CppClass* ) >( GetProcAddress( game, "il2cpp_class_is_valuetype" ) )( kls );
	}
	FieldInfo* class_get_fields( Il2CppClass* kls, void** iter )
	{
		return reinterpret_cast< FieldInfo* ( * )( Il2CppClass*, void** ) >( GetProcAddress( game, "il2cpp_class_get_fields" ) )( kls, iter );
	}	
	MethodInfo * class_get_methods( Il2CppClass * kls, void** iter )
	{
		return reinterpret_cast< MethodInfo * ( * )( Il2CppClass*, void** ) >( GetProcAddress( game, "il2cpp_class_get_methods" ) )( kls, iter );
	}	
	int32_t field_get_flags( FieldInfo * fld )
	{
		return reinterpret_cast< int32_t( * )( FieldInfo* ) >( GetProcAddress( game, "il2cpp_field_get_flags" ) )( fld );
	}
	const char* type_get_name( Il2CppType* type )
	{
		return reinterpret_cast< char* ( * )( Il2CppType* ) >( GetProcAddress( game, "il2cpp_type_get_name" ) )( type );
	}
	Il2CppClass* class_from_type( Il2CppType* type )
	{
		return reinterpret_cast< Il2CppClass * ( * )( Il2CppType* ) >( GetProcAddress( game, "il2cpp_class_from_type" ) )( type );
	}
	Il2CppType* method_get_param( MethodInfo* method, int idx )
	{
		return reinterpret_cast< Il2CppType * ( * )( MethodInfo*, int ) >( GetProcAddress( game, "il2cpp_method_get_param" ) )( method, idx );
	}	
	const char* method_get_param_name( MethodInfo* method, int idx )
	{
		return reinterpret_cast< const char * ( * )( MethodInfo*, int ) >( GetProcAddress( game, "il2cpp_method_get_param_name" ) )( method, idx );
	}
}