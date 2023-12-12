#include "highlightOutline.h"

#include <qdebug.h>


HighlightOutline::HighlightOutline() : Effect(), _width(2), _color(1,1,1,1), _technique(0)
{
}

HighlightOutline::HighlightOutline(const HighlightOutline &copy, const osg::CopyOp &op): osgFX::Effect(copy, op)
{
    _width = copy._width;
    _color = copy._color;
    _technique = copy._technique;
}

void HighlightOutline::setWidth(float w)
{
    _width = w;
    if (_technique) {
        _technique->setWidth(w);
    }
}

float HighlightOutline::getWidth() const
{
    return _width;
}

void HighlightOutline::setColor(const osg::Vec4& color)
{
    _color = color;
    if (_technique) {
        _technique->setColor(color);
    }
}

const osg::Vec4 &HighlightOutline::getColor() const
{
    return _color;
}

bool HighlightOutline::define_techniques()
{

    _technique = new CustomOutlineTechnique;
    addTechnique(_technique);

    setWidth(_width);
    setColor(_color);
    return true;
}

CustomOutlineTechnique::CustomOutlineTechnique() : osgFX::Technique(),
    _lineWidth(), _width(2),
    _material(), _color(1,1,1,1)
{

}

void CustomOutlineTechnique::setWidth(float w)
{
    _width = w;
    if (_lineWidth.valid()) {
        _lineWidth->setWidth(w);
    }
}

void CustomOutlineTechnique::setColor(const osg::Vec4 &color)
{
    _color = color;
    if (_material.valid()) {
        const osg::Material::Face face = osg::Material::FRONT_AND_BACK;
        _material->setAmbient(face, color);
        _material->setDiffuse(face, color);
        _material->setSpecular(face, color);
        _material->setEmission(face, color);
    }
}

void CustomOutlineTechnique::define_passes()
{
    qDebug()<<"define_passes";
    /*
             * draw
             * - set stencil buffer to ref=1 where draw occurs
             * - clear stencil buffer to 0 where test fails
             */
    {
        osg::StateSet* state = new osg::StateSet;

        // stencil op
        osg::Stencil* stencil  = new osg::Stencil;
        stencil->setFunction(osg::Stencil::ALWAYS, 1, ~0u);
        stencil->setOperation(osg::Stencil::KEEP,
                              osg::Stencil::KEEP,
                              osg::Stencil::REPLACE);
        state->setAttributeAndModes(stencil, Override_On);

        addPass(state);
    }

    /*
             * post-draw
             * - only draw where draw didn't set the stencil buffer
             * - draw only back-facing polygons
             * - draw back-facing polys as lines
             * - disable depth-test, lighting & texture
             */
    {
        osg::StateSet* state = new osg::StateSet;

        // stencil op
        osg::Stencil* stencil  = new osg::Stencil;
        stencil->setFunction(osg::Stencil::NOTEQUAL, 1, ~0u);
        stencil->setOperation(osg::Stencil::KEEP,
                              osg::Stencil::KEEP,
                              osg::Stencil::REPLACE);
        state->setAttributeAndModes(stencil, Override_On);

        // cull front-facing polys
        osg::CullFace* cullFace = new osg::CullFace;
        cullFace->setMode(osg::CullFace::FRONT);
        state->setAttributeAndModes(cullFace, Override_On);

        // draw back-facing polygon lines
        osg::PolygonMode* polyMode = new osg::PolygonMode;
        polyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
        state->setAttributeAndModes(polyMode, Override_On);

        // outline width
        _lineWidth = new osg::LineWidth;
        setWidth(_width);
        state->setAttributeAndModes(_lineWidth.get(), Override_On);

        // outline color/material
        _material = new osg::Material;
        _material->setColorMode(osg::Material::OFF);
        setColor(_color);
        state->setAttributeAndModes(_material.get(), Override_On);

        // disable modes
        state->setMode(GL_BLEND, Override_Off);
        //state->setMode(GL_DEPTH_TEST, Override_Off);
        state->setTextureMode(0, GL_TEXTURE_1D, Override_Off);
        state->setTextureMode(0, GL_TEXTURE_2D, Override_Off);
        state->setTextureMode(0, GL_TEXTURE_3D, Override_Off);

        addPass(state);
    }
}
