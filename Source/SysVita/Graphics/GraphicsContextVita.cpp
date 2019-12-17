
#include "stdafx.h"

#include <stdio.h>

#include "SysVita/GL.h"

#ifdef DAEDALUS_VITA
#include <vitaGL.h>
#include <vita2d.h>
#include <stdlib.h>
#endif

#include "Graphics/GraphicsContext.h"

#include "Graphics/ColourValue.h"


static u32 SCR_WIDTH = 640;
static u32 SCR_HEIGHT = 480;

class GraphicsContextGL : public CGraphicsContext
{
public:
	virtual ~GraphicsContextGL();


	virtual bool Initialise();
	virtual bool IsInitialised() const { return true; }

	virtual void ClearAllSurfaces();
	virtual void ClearZBuffer();
	virtual void ClearColBuffer(const c32 & colour);
	virtual void ClearToBlack();
	virtual void ClearColBufferAndDepth(const c32 & colour);
	virtual	void BeginFrame();
	virtual void EndFrame();
	virtual void UpdateFrame( bool wait_for_vbl );

	virtual void GetScreenSize(u32 * width, u32 * height) const;
	virtual void ViewportType(u32 * width, u32 * height) const;

	virtual void SetDebugScreenTarget( ETargetSurface buffer ) {}
	virtual void DumpNextScreen() {}
	virtual void DumpScreenShot() {}
};

template<> bool CSingleton< CGraphicsContext >::Create()
{
	DAEDALUS_ASSERT_Q(mpInstance == NULL);

	mpInstance = new GraphicsContextGL();
	return mpInstance->Initialise();
}


GraphicsContextGL::~GraphicsContextGL()
{
	vglEnd();
}

//extern bool initgl();
bool GraphicsContextGL::Initialise()
{

	vglInit(0x800000);
	vglWaitVblankStart(GL_TRUE);
	UpdateFrame(false);

	return true;

}


void GraphicsContextGL::GetScreenSize(u32 * width, u32 * height) const
{
	*width  = SCR_WIDTH;
	*height = SCR_HEIGHT;
}

void GraphicsContextGL::ViewportType(u32 * width, u32 * height) const
{
	GetScreenSize(width, height);
}

void GraphicsContextGL::ClearAllSurfaces()
{
	// FIXME: this should clear/flip a couple of times to ensure the front and backbuffers are cleared.
	// Not sure if it's necessary...
	ClearToBlack();
}


void GraphicsContextGL::ClearToBlack()
{
	glDepthMask(GL_TRUE);
	glClearDepth( 1.0f );
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
	glClear( GLbitfield(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
}

void GraphicsContextGL::ClearZBuffer()
{
	glDepthMask(GL_TRUE);
	glClearDepth( 1.0f );
	glClear( GL_DEPTH_BUFFER_BIT );
}

void GraphicsContextGL::ClearColBuffer(const c32 & colour)
{
	glClearColor( colour.GetRf(), colour.GetGf(), colour.GetBf(), colour.GetAf() );
	glClear( GL_COLOR_BUFFER_BIT );
}

void GraphicsContextGL::ClearColBufferAndDepth(const c32 & colour)
{
	glDepthMask(GL_TRUE);
	glClearDepth( 1.0f );
	glClearColor( colour.GetRf(), colour.GetGf(), colour.GetBf(), colour.GetAf() );
	glClear( GLbitfield(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void GraphicsContextGL::BeginFrame()
{
	vglStartRendering();

	// Get window size (may be different than the requested size)
	u32 width, height;
	GetScreenSize(&width, &height);

	// Special case: avoid division by zero below
	height = height > 0 ? height : 1;

	glViewport( 0, 0, width, height );
	glScissor( 0, 0, width, height );
}

void GraphicsContextGL::EndFrame()
{
	vglStopRendering();
}

void GraphicsContextGL::UpdateFrame( bool wait_for_vbl )
{

}
