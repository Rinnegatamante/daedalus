/*
Copyright (C) 2001 CyRUS64 (http://www.boob.co.uk)
Copyright (C) 2006 StrmnNrmn

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#include "stdafx.h"
#include "IniFile.h"

#include <stdio.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "Utility/StringUtil.h"

//*****************************************************************************
//
//*****************************************************************************
class IIniFileProperty : public CIniFileProperty
{
	public:
		IIniFileProperty( const char * p_name, const char * p_value )
			:	mName( p_name )
			,	mValue( p_value )
		{
		}

		const char *	GetName() const override			{ return mName.c_str(); }
		const char *	GetValue() const override		{ return mValue.c_str(); }

		bool	GetBooleanValue( bool default_value ) const override
		{
			const char * str( mValue.c_str() );

			if( strcmp( str, "yes" ) == 0 ||
				strcmp( str, "true" ) == 0 ||
				strcmp( str, "1" ) == 0 ||
				strcmp( str, "on" ) == 0 )
			{
				return true;
			}
			if( strcmp( str, "no" ) == 0 ||
				strcmp( str, "false" ) == 0 ||
				strcmp( str, "0" ) == 0 ||
				strcmp( str, "off" ) == 0 )
			{
				return false;
			}

			return default_value;
		}

		int	GetIntValue( int default_value ) const override
		{
			int	value;

			if( sscanf( mValue.c_str(), "%d", &value ) != 1 )
			{
				value = default_value;
			}
			return value;
		}

		float	GetFloatValue( float default_value ) const override
		{
			float	value;

			if( sscanf( mValue.c_str(), "%f", &value ) != 1 )
			{
				value = default_value;
			}
			return value;
		}

	private:
		friend class IIniFileSection;
		std::string				mName;
		std::string				mValue;
};

//*****************************************************************************
//
//*****************************************************************************
CIniFileProperty::~CIniFileProperty()
{
}

//*****************************************************************************
//
//*****************************************************************************
class IIniFileSection : public CIniFileSection
{
	public:
		IIniFileSection( const char * p_name )
			:	mName( p_name )
		{
		}

		~IIniFileSection() override;


		const char *	GetName() const override			{ return mName.c_str(); }
		bool			FindProperty( const char * p_name, const CIniFileProperty ** p_property ) const override;

				void			AddProperty( const IIniFileProperty * p_property );

	private:

		typedef std::vector< const IIniFileProperty * >	PropertyVec;

		struct SCompareProperties
		{
			bool operator()( const IIniFileProperty * a, const IIniFileProperty * b ) const
			{
				return strcmp( a->mName.c_str(), b->mName.c_str() ) < 0;
			}
			bool operator()( const char * a, const IIniFileProperty * b ) const
			{
				return strcmp( a, b->mName.c_str() ) < 0;
			}
			bool operator()( const IIniFileProperty * a, const char * b ) const
			{
				return strcmp( a->mName.c_str(), b ) < 0;
			}
		};

		std::string				mName;
		PropertyVec				mProperties;
};

//*****************************************************************************
//
//*****************************************************************************
CIniFileSection::~CIniFileSection()
{
}

//*****************************************************************************
//
//*****************************************************************************
IIniFileSection::~IIniFileSection()
{
	for(auto & mPropertie : mProperties)
	{
		delete mPropertie;
	}

	mProperties.clear();
}

//*****************************************************************************
//
//*****************************************************************************
bool	IIniFileSection::FindProperty( const char * p_name, const CIniFileProperty ** p_property ) const
{
	auto it( std::lower_bound( mProperties.begin(), mProperties.end(), p_name, SCompareProperties() ) );
	if( it != mProperties.end() && strcmp( (*it)->mName.c_str(), p_name ) == 0 )
	{
		*p_property = *it;
		return true;
	}
	else
	{
		*p_property = NULL;
		return false;
	}
}

//*****************************************************************************
// Once added, this class takes responsibility for deleting the property
//*****************************************************************************
void	IIniFileSection::AddProperty( const IIniFileProperty * p_property )
{
	auto it( std::lower_bound( mProperties.begin(), mProperties.end(), p_property->GetName(), SCompareProperties() ) );
	#ifdef DAEDALUS_ENABLE_ASSERTS
	DAEDALUS_ASSERT( it == mProperties.end() || strcmp( (*it)->mName.c_str(), p_property->GetName() ) != 0, "This property already exists" );
	#endif
	mProperties.insert( it, p_property );
}

//*****************************************************************************
//
//*****************************************************************************
class IIniFile : public CIniFile
{
	public:
		IIniFile();
		~IIniFile() override;

		//
		// CIniFile implementation
		//
		virtual bool					Open( const char * filename );

		const CIniFileSection *	GetDefaultSection() const override;

		u32						GetNumSections() const override;
		const CIniFileSection *	GetSection( u32 section_idx ) const override;

		const CIniFileSection *	GetSectionByName( const char * section_name ) const override;

	private:
		IIniFileSection *				mpDefaultSection;

		std::vector<IIniFileSection *>	mSections;
};

//*****************************************************************************
//
//*****************************************************************************
CIniFile::~CIniFile()
{
}

//*****************************************************************************
// Constructor
//*****************************************************************************
IIniFile::IIniFile()
:	mpDefaultSection( NULL )
{
}

//*****************************************************************************
//
//*****************************************************************************
IIniFile::~IIniFile()
{
	delete mpDefaultSection;

	for(auto & mSection : mSections)
	{
		delete mSection;
		mSection = NULL;
	}
}

//*****************************************************************************
//	Remove the specified characters from p_string
//*****************************************************************************
static bool	trim( char * p_string, const char * p_trim_chars )
{
	u32 num_trims = strlen( p_trim_chars );
	char * pin = p_string;
	char * pout = p_string;
	bool found;
	while ( *pin )
	{
		char c = *pin;

		found = false;
		for ( u32 i = 0; i < num_trims; i++ )
		{
			if ( p_trim_chars[ i ] == c )
			{
				// Skip
				found = true;
				break;
			}
		}

		if ( found )
		{
			pin++;
		}
		else
		{
			// Copy
			*pout++ = *pin++;
		}
	}
	*pout = '\0';
	return true;
}

//*****************************************************************************
//
//*****************************************************************************
CIniFile *	CIniFile::Create( const char * filename )
{
	auto * p_file( new IIniFile );
	if( p_file != NULL )
	{
		if( p_file->Open( filename ) )
		{
			return p_file;
		}

		delete p_file;
	}

	return NULL;
}

//*****************************************************************************
//
//*****************************************************************************
bool IIniFile::Open( const char * filename )
{
	const u32	BUFFER_LEN = 1024;
	char		readinfo[BUFFER_LEN+1];
	const char	trim_chars[]="{}[]"; //remove first and last character

	FILE * fh( fopen( filename, "r" ) );
	if (fh == NULL)
	{
		return false;
	}

	//
	//	By default start with the default section
	//
	mpDefaultSection = new IIniFileSection( "" );
	IIniFileSection * p_current_section( mpDefaultSection );
	readinfo[BUFFER_LEN] = '\0';

	// XXXX Using fgets needs reworking...
	while (fgets( readinfo, BUFFER_LEN, fh ) != NULL)
	{
		Tidy(readinfo);			// Strip spaces from end of lines

		// Handle comments
		if (readinfo[0] == '/')
			continue;

		// Check that the line isn't empty
		if (*readinfo != 0)
		{
			// Check for a section heading
			if (readinfo[0] == '{' || readinfo[0] == '[')
			{
				trim(readinfo,trim_chars);

				p_current_section = new IIniFileSection( readinfo );

				mSections.push_back( p_current_section );
			}
			else
			{
				char *key, *value;

				char *	equals_idx = strchr(readinfo, '=');
				if( equals_idx != NULL)
				{
					*equals_idx = '\0';
					key = &readinfo[0];
					value = equals_idx+1;
				}
				else
				{
					key = &readinfo[0];
					value = NULL;
				}

				Tidy( key );
				Tidy( value );
				#ifdef DAEDALUS_ENABLE_ASSERTS
				DAEDALUS_ASSERT( p_current_section != NULL, "There is no current section" );
				#endif
				auto * p_property( new IIniFileProperty( key, value ) );

				p_current_section->AddProperty( p_property );
			}
		}
	}
	fclose(fh);
	return true;
}

//*****************************************************************************
//
//*****************************************************************************
const CIniFileSection *	IIniFile::GetDefaultSection() const
{
	return mpDefaultSection;
}

//*****************************************************************************
//
//*****************************************************************************
u32		IIniFile::GetNumSections() const
{
	return mSections.size();
}

//*****************************************************************************
//
//*****************************************************************************
const CIniFileSection *	IIniFile::GetSection( u32 section_idx ) const
{
	if( section_idx < mSections.size() )
	{
		return mSections[ section_idx ];
	}
	#ifdef DAEDALUS_DEBUG_CONSOLE
	DAEDALUS_ERROR( "Invalid section index" );
	#endif
	return NULL;
}

//*****************************************************************************
//
//*****************************************************************************
const CIniFileSection *		IIniFile::GetSectionByName( const char * section_name ) const
{
	// TODO: Could use a map for this if it starts to prove expensive
	for(auto mSection : mSections)
	{
		if( strcmp( mSection->GetName(), section_name ) == 0 )
		{
			return mSection;
		}
	}

	return NULL;
}
