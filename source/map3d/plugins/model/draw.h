#ifndef DRAW_H
#define DRAW_H

#include <osg/LineWidth>
#include <osg/Array>
#include <osg/Geode>
#include <osg/Geometry>
#include <osgText/Text>

osg::Node* drawLine(osg::Vec3Array* vertex, float lineWidth)
{

    osg::Geode* gnode = new osg::Geode;
    osg::Geometry* geom = new osg::Geometry;
    gnode->addDrawable(geom);

    // Set vertex
    geom->setVertexArray(vertex);

    // Set the color
    osg::Vec4Array* color = new osg::Vec4Array();
    color->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
    geom->setColorArray(color, osg::Array::BIND_OVERALL);

    // Set the line width
    osg::LineWidth* lw = new osg::LineWidth(lineWidth);
    geom->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
    // Turn off the light to make it brighter
    geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

    // Set add as line
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, vertex->size()));

    return gnode;
}

osg::Node* drawCordination(const osg::Vec3d &pos)
{
    osg::Geode* gnode = new osg::Geode;

    //X axis with red color----------------------------------------------------------------
    {
        osg::Geometry* geom = new osg::Geometry;
        // Set vertex
        osg::Vec3Array* keyPoint = new osg::Vec3Array;
        keyPoint->push_back(pos + osg::Vec3d(10,0,0));
        keyPoint->push_back(pos);
        geom->setVertexArray(keyPoint);
        // Set the color red x
        osg::Vec4Array* color = new osg::Vec4Array();
        color->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));
        geom->setColorArray(color, osg::Array::BIND_OVERALL);
        // Set the line width
        osg::LineWidth* lw = new osg::LineWidth(2);
        geom->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
        // Turn off the light to make it brighter
        geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
        // Set add as line
        geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 2));
        gnode->addDrawable(geom);

        osgText::Text* text = new osgText::Text;
        text->setFont();
        text->setColor(color->at(0));
        text->setCharacterSize(2);
        text->setPosition(pos + osg::Vec3d(10,0,0));

        // the default layout is left to right, typically used in languages
        // originating from europe such as English, French, German, Spanish etc..
        text->setLayout(osgText::Text::LEFT_TO_RIGHT);
        text->setText("X");
        gnode->addDrawable(text);
    }

    //Y axis with green color-----------------------------------------------------------------
    {
        osg::Geometry* geom = new osg::Geometry;
        // Set vertex
        osg::Vec3Array* keyPoint = new osg::Vec3Array;
        keyPoint->push_back(pos + osg::Vec3d(0,10,0));
        keyPoint->push_back(pos);
        geom->setVertexArray(keyPoint);
        // Set the color red x
        osg::Vec4Array* color = new osg::Vec4Array();
        color->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
        geom->setColorArray(color, osg::Array::BIND_OVERALL);
        // Set the line width
        osg::LineWidth* lw = new osg::LineWidth(2);
        geom->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
        // Turn off the light to make it brighter
        geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
        // Set add as line
        geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 2));
        gnode->addDrawable(geom);

        osgText::Text* text = new osgText::Text;
        text->setFont();
        text->setColor(color->at(0));
        text->setCharacterSize(2);
        text->setPosition(pos + osg::Vec3d(0,10,0));

        // the default layout is left to right, typically used in languages
        // originating from europe such as English, French, German, Spanish etc..
        text->setLayout(osgText::Text::LEFT_TO_RIGHT);
        text->setText("Y");
        gnode->addDrawable(text);
    }
    //Z axis with blue color-----------------------------------------------------------------
    {
        osg::Geometry* geom = new osg::Geometry;
        // Set vertex
        osg::Vec3Array* keyPoint = new osg::Vec3Array;
        keyPoint->push_back(pos + osg::Vec3d(0,0,10));
        keyPoint->push_back(pos);
        geom->setVertexArray(keyPoint);
        // Set the color red x
        osg::Vec4Array* color = new osg::Vec4Array();
        color->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
        geom->setColorArray(color, osg::Array::BIND_OVERALL);
        // Set the line width
        osg::LineWidth* lw = new osg::LineWidth(2);
        geom->getOrCreateStateSet()->setAttribute(lw, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
        // Turn off the light to make it brighter
        geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
        // Set add as line
        geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, 2));
        gnode->addDrawable(geom);

        osgText::Text* text = new osgText::Text;
        text->setFont();
        text->setColor(color->at(0));
        text->setCharacterSize(2);
        text->setPosition(pos + osg::Vec3d(0,0,10));

        // the default layout is left to right, typically used in languages
        // originating from europe such as English, French, German, Spanish etc..
        text->setLayout(osgText::Text::LEFT_TO_RIGHT);
        text->setText("Z");
        gnode->addDrawable(text);
    }

    return gnode;

}

#endif // DRAW_H
