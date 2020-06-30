//
//  IRQuillTextEditorObject.cpp
//  IRiMaS
//
//  Created by Keitaro on 21/05/2020.
//

#include "IRQuillTextEditorObject.hpp"

IRQuillTextEditorObject::IRQuillTextEditorObject(Component* parent, IRStr* str) :
IRNodeObject(parent, "IRQuillTextEditor", str, NodeObjectType(ordinaryIRComponent))
{
    this->textEditor.reset( new IRQuillTextEditor(this, str) );
    addAndMakeVisible(this->textEditor.get());
    
    setObjectSize(400, 300);

}

IRQuillTextEditorObject::~IRQuillTextEditorObject()
{
    this->textEditor.reset();
}

// --------------------------------------------------


void IRQuillTextEditorObject::resized()
{
    this->textEditor->setBounds(getLocalBounds().reduced(10));
}
// --------------------------------------------------

IRNodeObject* IRQuillTextEditorObject::copyThisObject()
{
    return new IRQuillTextEditorObject(this->parent, getStr());
}
// --------------------------------------------------
IRNodeObject* IRQuillTextEditorObject::copyContents(IRNodeObject* object)
{
    IRQuillTextEditorObject* obj = static_cast<IRQuillTextEditorObject*>(object);
    return obj;
}
// --------------------------------------------------
IRNodeObject* IRQuillTextEditorObject::copyDragDropContents(IRNodeObject* object)
{
    IRQuillTextEditorObject* obj = new IRQuillTextEditorObject(this->parent, getStr());
    return obj;
}
// --------------------------------------------------
// --------------------------------------------------
// SAVE LOAD
t_json IRQuillTextEditorObject::saveThisToSaveData()
{

    t_json saveData = t_json::object({
        });

    t_json save = t_json::object({
        {"quillTextEditor", saveData}
    });
    
    return save;
}
// --------------------------------------------------
void IRQuillTextEditorObject::loadThisFromSaveData(t_json data)
{
    t_json w = data["quillTextEditor"];
   
}
// --------------------------------------------------


// --------------------------------------------------

void IRQuillTextEditorObject::IRChangeListenerCallback (ChangeBroadcaster* source)
{
   
}
// --------------------------------------------------

void IRQuillTextEditorObject::ObjectPositionChanged(int x, int y)
{
    
}
