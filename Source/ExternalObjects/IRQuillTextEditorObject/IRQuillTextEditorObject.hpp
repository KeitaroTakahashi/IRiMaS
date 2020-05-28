//
//  IRQuillTextEditorObject.hpp
//  IRiMaS
//
//  Created by Keitaro on 21/05/2020.
//

#ifndef IRQuillTextEditorObject_hpp
#define IRQuillTextEditorObject_hpp

#include "IRNodeObject.hpp"
#include "IRQuillTextEditor.hpp"

class IRQuillTextEditorObject : public IRNodeObject
{
public:
    IRQuillTextEditorObject(Component* parent, IRStr* str);
    ~IRQuillTextEditorObject();
    // --------------------------------------------------

    void resized() override;
    // copy related methods
    
    IRNodeObject* copyThis() override;
    
    IRNodeObject* copyContents(IRNodeObject* object) override;
    
    IRNodeObject* copyDragDropContents(IRNodeObject* object) override;
    
    t_json saveThisToSaveData() override;

    void loadThisFromSaveData(t_json data) override;
    
    // called when this object position is changed
    void ObjectPositionChanged(int x, int y) override;
    
    // --------------------------------------------------


private:
    
    std::shared_ptr<IRQuillTextEditor> textEditor;
    void IRChangeListenerCallback (ChangeBroadcaster* source) override;
    
    void arrangeControllerChangedCallback(ArrangeController* source);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IRQuillTextEditorObject)

    
};
#endif /* IRQuillTextEditorObject_hpp */
