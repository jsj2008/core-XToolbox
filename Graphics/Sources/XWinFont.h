/*
* This file is part of Wakanda software, licensed by 4D under
*  (i) the GNU General Public License version 3 (GNU GPL v3), or
*  (ii) the Affero General Public License version 3 (AGPL v3) or
*  (iii) a commercial license.
* This file remains the exclusive property of 4D and/or its licensors
* and is protected by national and international legislations.
* In any event, Licensee's compliance with the terms and conditions
* of the applicable license constitutes a prerequisite to any use of this file.
* Except as otherwise expressly stated in the applicable license,
* such license does not include any other license or rights on this file,
* 4D's and/or its licensors' trademarks and/or other proprietary rights.
* Consequently, no title, copyright or other proprietary rights
* other than those specified in the applicable license is granted.
*/
#ifndef __XWinFont__
#define __XWinFont__

#include <gdiplus.h> 

BEGIN_TOOLBOX_NAMESPACE

// Needed declarations
class VFontMetrics;

#define DWRITE_TEXT_LAYOUT_MAX_WIDTH 100000000.0f
#define DWRITE_TEXT_LAYOUT_MAX_HEIGHT 100000000.0f

class XTOOLBOX_API XWinFont
{
public:
			XWinFont (VFont *inFont, const VString& inFontFamilyName, const VFontFace& inFace, GReal inSize);
			~XWinFont ();
	
    
	void	ApplyToPort (PortRef inPort, GReal inScale);
	FontRef	GetFontRef () const { return fFontRef; }

	/** accessor on native font */
#if GRAPHIC_MIXED_GDIPLUS_D2D
	Gdiplus::Font *GetGDIPlusFont() const { return fGDIPlusFont; }
#endif
#if ENABLE_D2D
	/** accessor on DWrite font */
	IDWriteFont *GetDWriteFont() const { return fDWriteFont; }

	/** accessor on DWrite text format */
	IDWriteTextFormat *GetDWriteTextFormat() const { return fDWriteTextFormat; }

	/** create text layout from the specified text string & options 
	@remarks
		inWidth & inHeight are in DIP
	*/
	IDWriteTextLayout *CreateTextLayout(ID2D1RenderTarget *inRT, const VString& inText, const bool isAntialiased = true, const GReal inWidth = DWRITE_TEXT_LAYOUT_MAX_WIDTH, const GReal inHeight = DWRITE_TEXT_LAYOUT_MAX_HEIGHT, AlignStyle inHoriz = AL_LEFT, AlignStyle inVert = AL_TOP, TextLayoutMode inMode = TLM_NORMAL, VTreeTextStyle *inStyles = NULL, const GReal inRefDocDPI = 72.0f, const bool inUseCache = true);

	/** create text style for the specified text position 
	@remarks
		range of returned text style is set to the range of text which has same format as text at specified position
	*/
	VTextStyle *CreateTextStyle( IDWriteTextLayout *inLayout, const sLONG inTextPos = 0.0f);

	/**	create text styles tree for the specified text range */
	VTreeTextStyle *CreateTreeTextStyle( IDWriteTextLayout *inLayout, const sLONG inTextPos = 0.0f, const sLONG inTextLen = 1.0f);
#endif

	static DWORD	FontFaceToWinFontEffect (const VFontFace& inFace);
	static void		WinFontEffectToFontFace (DWORD inEffect, VFontFace& outFace);

			bool	MousePosToTextPos(const VString& inString, GReal inMousePos, sLONG& ioTextPos, GReal& ioPixPos) ;

			Boolean IsTrueTypeFont() { return fIsTrueType;}
protected:
	FontRef	_CreateFontRef (const VString& inFontFamilyName, const VFontFace& inFace, GReal inSize);
	int GetCharSetFromFontName(const VString& inFontFamilyName) const;
	bool _IsFontTrueType(PortRef inPort, FontRef inFontRef);
#if ENABLE_D2D
	void _GetStyleFromFontName(const VString &inFontFullName, VString &outFontFamilyName, int& outStyle, int& outWeight, int& outStretch) const;
	bool _GetFontWeightFromStyleName( const VString& inStyleName, int& outFontWeight) const;
	bool _GetFontStyleFromStyleName( const VString& inStyleName, int& outFontStyle) const;
	bool _GetFontStretchFromStyleName( const VString& inStyleName, int& outFontStretch) const;

	IDWriteTextLayout *_GetCachedTextLayout( const VString& inText, const GReal inWidth = DWRITE_TEXT_LAYOUT_MAX_WIDTH, const GReal inHeight = DWRITE_TEXT_LAYOUT_MAX_HEIGHT, AlignStyle inHoriz = AL_LEFT, AlignStyle inVert = AL_TOP, TextLayoutMode inMode = TLM_NORMAL, const bool isAntialiased = true, const GReal inRefDocDPI = 72.0f);
	void _ApplyMultiStyle( ID2D1RenderTarget *inRT, IDWriteTextLayout *inLayout, VTreeTextStyle *inStyles, VSize inTextLength, const GReal inRefDocDPI = 72.0f);
#endif
private:
    static int CALLBACK EnumFontFamiliesExProc(ENUMLOGFONTEX *lpelfe, NEWTEXTMETRICEX *lpntme,DWORD FontType,LPARAM lParam);
	VFont*	fFont;
	FontRef	fFontRef;
	bool	fIsTrueType;
#if GRAPHIC_MIXED_GDIPLUS_D2D
	Gdiplus::Font *fGDIPlusFont;
#endif
#if ENABLE_D2D
	IDWriteFont* fDWriteFont;
	IDWriteTextFormat *fDWriteTextFormat;
	IDWriteInlineObject *fDWriteEllipsisTrimmingSign;
	VString fCachedText;
	GReal fCachedWidth, fCachedHeight;
	AlignStyle fCachedHAlign, fCachedVAlign;
	TextLayoutMode fCachedLayoutMode;
	IDWriteTextLayout *fCachedDWriteTextLayout;
	GReal fCachedDPI;
	bool fCachedAntialiased;
#endif

};

typedef XWinFont XFontImpl;

class XTOOLBOX_API XWinFontMetrics
{
public:
			XWinFontMetrics ( VFontMetrics *inMetrics):fMetrics( inMetrics)	{;}
			~XWinFontMetrics ()	{;}

	GReal	GetTextWidth (const VString& inText) const;
	GReal	GetCharWidth (UniChar inChar) const;

	void	MeasureText( const VString& inString, std::vector<GReal>& outOffsets) const;
	void	MeasureText( const VString& inText, GReal* outOffsets) const;

	void	GetMetrics (GReal& outAscent, GReal& outDescent, GReal& outLeading) const;
	void	GetMetrics (GReal& outAscent, GReal& outDescent, GReal& outInternalLeading, GReal& outExternalLeading) const;

	bool	MousePosToTextPos(const VString& inString, GReal inMousePos, sLONG& ioTextPos, GReal& ioPixPos) ;
	


private:
	VFontMetrics*	fMetrics;
};

typedef XWinFontMetrics XFontMetricsImpl;

END_TOOLBOX_NAMESPACE

#endif