/*
Copyright (C) 2007 StrmnNrmn

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


#ifndef SYSPSP_UI_UISETTING_H_
#define SYSPSP_UI_UISETTING_H_

#include "UIElement.h"


class CUISetting : public CUIElement
{
public:
	CUISetting( const char * name, const char * description );
	~CUISetting() override {}

	virtual const char *	GetSettingName() const = 0;

		void			OnSelected() override			{ OnNext(); }

	virtual const char *	GetName() const			{ return mName; }
	const char *	GetDescription() const override	{ return mDescription; }

	virtual bool			IsReadOnly() const		{ return false; }
	//virtual bool			IsSelectable() const	{ return !IsReadOnly(); }		// We actually want to be able to read the descriptions of various settings, so don't do this.

	u32				GetHeight( CUIContext * context ) const override;
	void			Draw( CUIContext * context, s32 min_x, s32 max_x, EAlignType halign, s32 y, bool selected ) const override;

private:
	const char *			mName;
	const char *			mDescription;
};


class CBoolSetting : public CUISetting
{
public:

	CBoolSetting( bool * p_bool, const char * name, const char * description, const char * true_text, const char * false_text, bool read_only = false )
		:	CUISetting( name, description )
		,	mBool( p_bool )
		,	mTrueText( true_text )
		,	mFalseText( false_text )
		,	mReadOnly( read_only )
	{
	}

		void			OnNext() override				{ *mBool = !*mBool; }
		void			OnPrevious() override 			{ *mBool = !*mBool; }

	bool			IsReadOnly() const override		{ return mReadOnly; }

	const char *	GetSettingName() const override	{ return *mBool ? mTrueText : mFalseText; }

private:
	bool *					mBool;
	const char *			mTrueText;
	const char *			mFalseText;
	bool					mReadOnly;
};

#endif // SYSPSP_UI_UISETTING_H_
