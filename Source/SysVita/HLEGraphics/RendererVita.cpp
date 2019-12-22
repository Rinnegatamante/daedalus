
#include "stdafx.h"
#include "RendererVita.h"


BaseRenderer *gRenderer = nullptr;
RendererVita *gRendererVita = nullptr;


RendererVita::RendererVita() {}

//RendererVita::gRendererVita() {}
RendererVita::~RendererVita() {}

void RendererVita::RestoreRenderStates()
{
  
}

void RendererVita::RenderTriangles(DaedalusVtx *p_vertices, u32 num_vertices, bool disable_zbuffer)
{

}

 void RendererVita::TexRect(u32 tile_idx, const v2 & xy0, const v2 & xy1, TexCoord st0, TexCoord st1)
 {

 }
void RendererVita::TexRectFlip(u32 tile_idx, const v2 & xy0, const v2 & xy1, TexCoord st0, TexCoord st1)
{

}
void RendererVita::FillRect(const v2 & xy0, const v2 & xy1, TexCoord st0, TexCoord st1)
{

}

void RendererVita::Draw2DTexture(f32 x0, f32 y0, f32 x1, f32 y1,
                                 f32 u0, f32 v0, f32 u1, f32 v1, const CNativeTexture *texture)
{

}

void RendererVita::Draw2DTextureR(f32 x0, f32 y0, f32 x1, f32 y1,
                            f32 x2, f32 y2, f32 x3, f32 y3,
                            f32 s,  f32 t)
{

}
bool CreateRenderer()
{
//  gRendererVita = new RendererVita();
  gRenderer = gRendererVita;
  return true;
}

void DestroyRenderer()
{
  delete gRendererVita;
  gRendererVita = nullptr;
  gRenderer = nullptr;
}
