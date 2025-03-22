#include "dumper.hpp"

#include "il2cpp.hpp"
#include "il2cpp_table_defs.hpp"
#include "file.hpp"
#include "normalize.hpp"

#include <mutex>

#include <iostream>
#include <filesystem>
#include <cctype>

#include <shlobj.h>
#include <objbase.h>

#pragma comment(lib,"Shell32")
#pragma comment(lib,"Ole32")

namespace dumper
{
	dump_status status = dump_status::NONE;
	Il2CppDomain* domain = nullptr;
	std::string dump_dir = "";

	std::once_flag flag;

	std::filesystem::path get_desktop_path( )
	{
		wchar_t* p;
		if ( S_OK != SHGetKnownFolderPath( FOLDERID_Desktop, 0, NULL, &p ) ) return "";
		std::filesystem::path result = p;
		CoTaskMemFree( p );
		return result;
	}

	void init( )
	{
		domain = il2cpp::get_domain( );
		dump_dir = get_desktop_path( ).string( );
	}

	std::string tname( std::string old, Il2CppType* tp )
	{
		if ( old.find( "`" ) != std::string::npos )
		{
			return il2cpp::type_get_name( tp );
		}
		return old;
	}

	std::string dump_field( FieldInfo* field )
	{
		if ( !field )
			return "";
		std::stringstream output;
		
		output << "\t";
		auto attrs = il2cpp::field_get_flags( field );
		auto access = attrs & FIELD_ATTRIBUTE_FIELD_ACCESS_MASK;
		switch ( access )
		{
			case FIELD_ATTRIBUTE_PRIVATE:
				output << "private ";
				break;
			case FIELD_ATTRIBUTE_PUBLIC:
				output << "public ";
				break;
			case FIELD_ATTRIBUTE_FAMILY:
				output << "protected ";
				break;
			case FIELD_ATTRIBUTE_FAM_AND_ASSEM:
				output << "internal ";
				break;
			case FIELD_ATTRIBUTE_FAM_OR_ASSEM:
				output << "protected internal ";
				break;
		}
		if ( attrs & FIELD_ATTRIBUTE_LITERAL )
		{
			output << "const ";
		}
		else
		{
			if ( attrs & FIELD_ATTRIBUTE_STATIC )
			{
				output << "static ";
			}
			if ( attrs & FIELD_ATTRIBUTE_INIT_ONLY )
			{
				output << "readonly ";
			}
		}
		output << tname(normal::str( il2cpp::class_from_type( field->type )->name ), field->type) << " ";
		output << field->name << "; // " << "0x" << std::uppercase << std::hex << field->offset << std::endl;
		
		return output.str( );
	}

	std::string dump_method( MethodInfo* method )
	{
		if ( !method )
			return "";
		std::stringstream output;
		
		const char* name = method->name;
		std::string return_name = tname(normal::str( il2cpp::class_from_type( method->return_type )->name), method->return_type);
		uint8_t parameters_count = method->parameters_count;
		Il2CppType** parameters = method->parameters;
		static auto base = (std::uintptr_t)GetModuleHandle( "GameAssembly.dll" );
		auto method_ptr = method->methodPointer;
		auto method_rva = (uintptr_t)method_ptr - base;

		output << "\t//RVA: 0x" << std::uppercase << std::hex << method_rva << std::endl;

		output << "\t" << return_name  << " " << name << "(";
		for ( int i = 0; i < parameters_count; i++ )
		{
			auto param = parameters[ i ];
			if ( !param )
				continue;
			
			auto param_type = tname(normal::str( il2cpp::class_from_type( param )->name ), param);
			auto param_name = il2cpp::method_get_param_name( method, i );

			output << param_type << " " << param_name;
			if ( i != parameters_count - 1 )
			{
				output << ", ";
			}
		}
		output << ") {}" << std::endl << std::endl;
		
		return output.str( );
	}

	dump_status dump( )
	{
		std::call_once( flag, init );

		std::stringstream strstream;
		File file( dump_dir + "\\dump.cs", "w" );
		if ( !file.ok( ) )
		{
			file.close( );
			return ERRORR;
		}
		
		size_t size_of_assemblies;
		Il2CppAssembly** assemblies = il2cpp::domain_get_assemblies( domain, &size_of_assemblies );

		for ( size_t i{ 0 }; i < size_of_assemblies; i++ )
		{
			Il2CppAssembly* assembly = assemblies[ i ];
			if ( !assembly )
				continue;
			strstream << "// Image " << i << ": " << assembly->image->name << std::endl;
		}
		strstream << std::endl;

		for ( size_t i{ 0 }; i < size_of_assemblies; i++ )
		{
			Il2CppAssembly* assembly = assemblies[ i ];
			if ( !assembly )
				continue;
			Il2CppImage* image = assembly->image;
			if ( !image )
				continue;

			int class_count = il2cpp::image_get_class_count( image );

			for ( size_t class_idx{ 0 }; class_idx < class_count; class_idx++ )
			{
				Il2CppClass* klass = il2cpp::image_get_class( image, class_idx );
				if ( !klass )
					continue;
				bool is_valuetype = il2cpp::class_is_valuetype( klass );
				bool is_enum = il2cpp::class_is_enum( klass );
				auto flags = klass->flags;
				auto parent = klass->parent;
				strstream << "// Namespace: " << klass->namespaze << std::endl;
				auto visibility = flags & TYPE_ATTRIBUTE_VISIBILITY_MASK;
				switch ( visibility ) {
					case TYPE_ATTRIBUTE_PUBLIC:
					case TYPE_ATTRIBUTE_NESTED_PUBLIC:
						strstream << "public ";
						break;
					case TYPE_ATTRIBUTE_NOT_PUBLIC:
					case TYPE_ATTRIBUTE_NESTED_FAM_AND_ASSEM:
					case TYPE_ATTRIBUTE_NESTED_ASSEMBLY:
						strstream << "internal ";
						break;
					case TYPE_ATTRIBUTE_NESTED_PRIVATE:
						strstream << "private ";
						break;
					case TYPE_ATTRIBUTE_NESTED_FAMILY:
						strstream << "protected ";
						break;
					case TYPE_ATTRIBUTE_NESTED_FAM_OR_ASSEM:
						strstream << "protected internal ";
						break;
				}

				if ( flags & TYPE_ATTRIBUTE_ABSTRACT && flags & TYPE_ATTRIBUTE_SEALED ) {
					strstream << "static ";
				}
				else if ( !( flags & TYPE_ATTRIBUTE_INTERFACE ) && flags & TYPE_ATTRIBUTE_ABSTRACT ) {
					strstream << "abstract ";
				}
				else if ( !is_valuetype && !is_enum && flags & TYPE_ATTRIBUTE_SEALED ) {
					strstream << "sealed ";
				}
				if ( flags & TYPE_ATTRIBUTE_INTERFACE ) {
					strstream << "interface ";
				}
				else if ( is_enum ) {
					strstream << "enum ";
				}
				else if ( is_valuetype ) {
					strstream << "struct ";
				}
				else {
					strstream << "class ";
				}

				strstream << klass->name << " " << std::string(parent ? ( std::string(": ") + parent->name ) : "" ) << std::endl;
				strstream << "{" << std::endl;
				strstream << "\t//fields" << std::endl;
				void* iter = nullptr;
				while ( auto field = il2cpp::class_get_fields( klass, &iter ) )
				{
					strstream << dump_field( field );
				}


				strstream << "\n\t//methods" << std::endl << std::endl;
				void* iter2 = nullptr;
				while ( auto method = il2cpp::class_get_methods( klass, &iter2 ) )
				{
					strstream << dump_method( method );
				}

				
				strstream << "}" << std::endl << std::endl;
			}
		}

		file.write(strstream);
		file.close( );
		return SUCCESS;
	}
}