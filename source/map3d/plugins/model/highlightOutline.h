#ifndef HIGHLIGHTLINE_H
#define HIGHLIGHTLINE_H

#include <osgFX/Outline>
#include <osgFX/Scribe>

#include <osg/CullFace>
#include <osg/Stencil>
#include <osg/PolygonMode>

const unsigned int Override_On = osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE;
const unsigned int Override_Off = osg::StateAttribute::OFF|osg::StateAttribute::OVERRIDE;

class CustomOutlineTechnique : public osgFX::Technique
{
public:
    /// Constructor.
    CustomOutlineTechnique();

    /// Validate.
    bool validate(osg::State&) const {
        return true;
    }

    /// Set outline width.
    void setWidth(float w);

    void setColor(const osg::Vec4& color);

protected:
    /// Define render passes.
    void define_passes();

private:
    /// Outline width.
    osg::ref_ptr<osg::LineWidth> _lineWidth;
    float _width;

    /// Outline Material.
    osg::ref_ptr<osg::Material> _material;
    osg::Vec4 _color;
};

class HighlightOutline : public osgFX::Effect
{
public:
    /// Constructor.
    HighlightOutline();

    /// Copy constructor.
    HighlightOutline(const HighlightOutline& copy, const osg::CopyOp& op = osg::CopyOp::SHALLOW_COPY);
    // Effect class info
    META_Effect(osgFX, HighlightOutline, "Outline",
                "Stencil buffer based object outline effect.\n"
                "This effect needs a properly setup stencil buffer.",
                "Ulrich Hertlein");

    /// Set outline width.
    void setWidth(float w);

    /// Get outline width.
    float getWidth() const;

    /// Set outline color.
    void setColor(const osg::Vec4& color);

    /// Get outline color.
    const osg::Vec4& getColor() const;
    /// Define available techniques.
    bool define_techniques() override;

protected:
    /// Destructor.
    virtual ~HighlightOutline() {
    }


private:
    /// Outline width.
    float _width;

    /// Outline color.
    osg::Vec4 _color;

    /// Technique.
    CustomOutlineTechnique* _technique;
};

#endif // HIGHLIGHTLINE_H
