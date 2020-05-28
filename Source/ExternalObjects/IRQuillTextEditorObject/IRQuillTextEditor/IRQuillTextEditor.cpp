//
//  IRQuillTextEditor.cpp
//  IRiMaS
//
//  Created by Keitaro on 21/05/2020.
//

#include "IRQuillTextEditor.hpp"


IRQuillTextEditor::IRQuillTextEditor(IRNodeObject* parent, IRStr* str) :
IRUIFoundation(parent, str)
{
    this->browser.reset( new WebBrowserComponent() );
    addAndMakeVisible(this->browser.get());
    
    String url = "/Users/keitaro/Desktop/test.html";
    String url2 = "https://quilljs.com";
    this->browser->goToURL(url2);
}

IRQuillTextEditor::~IRQuillTextEditor()
{
    this->browser.reset();
}


void IRQuillTextEditor::resized()
{
    this->browser->setBounds(getLocalBounds());
}
