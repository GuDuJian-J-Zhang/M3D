/*
 * FTGL - OpenGL font library
 *
 * Copyright (c) 2001-2004 Henry Maddocks <ftgl@opengl.geek.nz>
 * Copyright (c) 2008 Éric Beets <ericbeets@free.fr>
 * Copyright (c) 2008 Sam Hocevar <sam@zoy.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "config.h"

#include "FTGL/ftgles.h"

#include "FTInternals.h"
#include "FTPolygonGlyphImpl.h"
#include "FTVectoriser.h"
//#include "Utility.h"

#define   NODEBLOCK 1000

//
//  FTGLPolyGlyph
//


FTPolygonGlyph::FTPolygonGlyph(FT_GlyphSlot glyph, float outset,
                               bool useDisplayList) :
    FTGlyph(new FTPolygonGlyphImpl(glyph, outset, useDisplayList))
{}


FTPolygonGlyph::~FTPolygonGlyph()
{}


const FTPoint& FTPolygonGlyph::Render(const FTPoint& pen, int renderMode)
{
    FTPolygonGlyphImpl *myimpl = dynamic_cast<FTPolygonGlyphImpl *>(impl);
    return myimpl->RenderImpl(pen, renderMode);
}
const FTPoint& FTPolygonGlyph::Render(const FTPoint& penPos, int renderMode,
									  vector<FTPoint> &pnt,vector<int> &pntnum,vector<int> &type)
{
	FTPolygonGlyphImpl *myimpl = dynamic_cast<FTPolygonGlyphImpl *>(impl);
	return myimpl->RenderImpl(penPos, renderMode, pnt, pntnum, type);
}

//
//  FTGLPolyGlyphImpl
//


FTPolygonGlyphImpl::FTPolygonGlyphImpl(FT_GlyphSlot glyph, float _outset,
                                       bool useDisplayList)
:   FTGlyphImpl(glyph),
    glList(0)
{
    if(ft_glyph_format_outline != glyph->format)
    {
        err = 0x14; // Invalid_Outline
        return;
    }

    vectoriser = new FTVectoriser(glyph);

    if((vectoriser->ContourCount() < 1) || (vectoriser->PointCount() < 3))
    {
        delete vectoriser;
        vectoriser = NULL;
        return;
    }

    /*
     * David Petrie Note:
     * 
     * The original FTGL code included support for gl display lists (via 
     * glGenList, etc). This has been removed because OpenGL ES 1.1 has
     * no support for them.
     */
    
    hscale = glyph->face->size->metrics.x_ppem * 64;
    vscale = glyph->face->size->metrics.y_ppem * 64;
    outset = _outset;
    
    /*
     * David Petrie Note:
     * 
     * vectoriser->MakeMesh was being called every DoRender() in the
     * original FTGL code. I've shifted it here so it is only called once,
     * as the iPhone is not efficient enough to handle a tesselation
     * on each frame.
     */
    if (vectoriser)
        vectoriser->MakeMesh(1.0, 1, outset);
}


FTPolygonGlyphImpl::~FTPolygonGlyphImpl()
{
    if (vectoriser)
    {
        delete vectoriser;
    }
}   


const FTPoint& FTPolygonGlyphImpl::RenderImpl(const FTPoint& pen,
                                              int renderMode)
{
    glTranslatef(pen.Xf(), pen.Yf(), pen.Zf());
    if (vectoriser)
    {
        DoRender(pen);
    }
    glTranslatef(-pen.Xf(), -pen.Yf(), -pen.Zf());
    return advance;
}

const FTPoint& FTPolygonGlyphImpl::RenderImpl(const FTPoint& penPos,int renderMode,
											  vector<FTPoint> &pnt,vector<int> &pntnum,vector<int> &type)
{
//	glTranslatef(pen.Xf(), pen.Yf(), pen.Zf());

	//LOGI("FTPolygonGlyphImpl::RenderImpl xf:%f,yf:%f,zf:%f",pen.Xf(), pen.Yf(), pen.Zf());

	if(vectoriser)
	{
		DoRender(penPos,pnt, pntnum, type);
	}

//	glTranslatef(-pen.Xf(), -pen.Yf(), -pen.Zf());

	return advance;
}

void FTPolygonGlyphImpl::DoRender(const FTPoint& pen)
{
    const FTMesh *mesh = vectoriser->GetMesh();

    for (unsigned int t = 0; t < mesh->TesselationCount(); ++t)
    {
        const FTTesselation* subMesh = mesh->Tesselation(t);
        unsigned int polygonType = subMesh->PolygonType();

        ftglBindTexture(0);
        
        ftglBegin(polygonType);
        for(unsigned int i = 0; i < subMesh->PointCount(); ++i)
        {
            FTPoint point = subMesh->Point(i);
            ftglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
            ftglTexCoord2f(point.Xf() / hscale, point.Yf() / vscale);
            ftglVertex3f(pen.Xf() + point.Xf() / 64.0f, pen.Yf() + point.Yf() / 64.0f, 0.0f);
        }
        ftglEnd();
    }
}
void FTPolygonGlyphImpl::DoRender(const FTPoint& pen,vector<FTPoint> &pnt,vector<int> &pntnum,vector<int> &type)
{
	vectoriser->MakeMesh(1.0, 1, outset);

	const FTMesh *mesh = vectoriser->GetMesh();

	for(unsigned int t = 0; t < mesh->TesselationCount(); ++t)
	{
		const FTTesselation* subMesh = mesh->Tesselation(t);
		unsigned int polygonType = subMesh->PolygonType();

		pntnum.push_back(subMesh->PointCount());
		type.push_back(polygonType);

		for(unsigned int i = 0; i < subMesh->PointCount(); ++i)
		{
			//移动到画笔位置，编程最终的显示数据，避免在显示的过程中在此做偏移
			FTPoint point  =subMesh->Point(i);
			FTPoint realPoint (point.X()/64.0f,point.Y()/64.0f,point.Z()/64.0f);
			realPoint =  realPoint + pen;
			pnt.push_back(realPoint);
			//FTGL_LOGI("FTPolygonGlyphImpl::DoRender realPnt:%f %f %f",realPoint.Xf(),realPoint.Yf(),realPoint.Zf());
		}
	}
}

