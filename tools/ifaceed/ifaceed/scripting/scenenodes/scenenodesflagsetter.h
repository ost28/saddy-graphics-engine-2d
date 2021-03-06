/*! \file scenenodesflagsetter.h
    

    Defines a setter for  visibility property of scene node
 */
#pragma once
#include "scenenodesabstractsetter.h"

namespace core
{
class Editor;    
}

namespace scripting
{
    
namespace scenenodes
{
     
/*! Defines a visibility setter for scene node
 */
class FlagSetter: public scripting::scenenodes::AbstractSetter<bool>
{
public: 
    /*! A callback for building command 
     */
    typedef history::Command* (*Callback)(core::Editor*, sad::SceneNode*, bool, bool);
    /*! Constructs new visibility setter 
        \param[in] ed editor
        \param[in] e engine
        \param[in] prop a property to be set
        \param[in] f flag
     */
    FlagSetter(
        core::Editor* ed,
        QScriptEngine* e, 
        const QString& prop, 
        scripting::scenenodes::FlagSetter::Callback f
    );
    /*! Can be inherited
     */
    virtual  ~FlagSetter();
    /*! Returns command for editing a property
        \param[in] obj an object to be set
        \param[in] propertyname a property for object
        \param[in] oldvalue old value 
        \param[in] newvalue new value
        \return a command to be used
     */
    virtual history::Command* command(sad::SceneNode* obj, const sad::String& propertyname, bool oldvalue,  bool newvalue);
protected:
    /*! A callback
     */
    Callback m_f;
    /*! An editor
     */
    core::Editor* m_editor;
};


}

}
