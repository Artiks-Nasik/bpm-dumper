#pragma once
#include "il2cpp_header.h"

namespace il2cpp
{
	void init( );
	Il2CppDomain* get_domain( );
	Il2CppAssembly** domain_get_assemblies( Il2CppDomain* domain, size_t* size );
	Il2CppClass* image_get_class( Il2CppImage* img, int idx );
	int image_get_class_count( Il2CppImage* img );
	bool class_is_enum( Il2CppClass* kls );
	bool class_is_valuetype( Il2CppClass* kls );
	FieldInfo* class_get_fields( Il2CppClass* kls, void** iter );
	MethodInfo* class_get_methods( Il2CppClass* kls, void** iter );
	int32_t field_get_flags( FieldInfo* fld );
	const char* type_get_name( Il2CppType* type );
	Il2CppClass* class_from_type( Il2CppType* type );
	Il2CppType* method_get_param( MethodInfo* method, int idx );
	const char* method_get_param_name( MethodInfo* method, int idx );
}

