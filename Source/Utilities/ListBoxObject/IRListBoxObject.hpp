//
//  ListTypeScrollableMenu.h
//  Cello
//
//  Created by Keitaro on 11/12/2018.
//

#ifndef ListTypeScrollableMenu_h
#define ListTypeScrollableMenu_h

#include "JuceHeader.h"

enum IRTableListBoxStatus
{
    selectedRowChanged,
    selectedColumnChanged,
    selectedRowDoubleClicked,
    scrolled
};

class IRTableListBox : public Component,
public TableListBoxModel,
public ChangeBroadcaster
{
public:
    IRTableListBox(String title, int rowNumber)
    {
        this->title = title;
        this->numRows = rowNumber;
        
        this->table.setColour(ListBox::outlineColourId, Colours::transparentWhite);
        this->table.setColour(ListBox::backgroundColourId, Colours::transparentWhite);
        this->table.setColour(ListBox::textColourId, Colours::black);
        
        this->table.setOutlineThickness(0);
        this->table.setHeaderHeight(0);
        this->table.setRowHeight(110);
        
        this->table.getViewport()->setScrollBarsShown(true,false);
        
        addAndMakeVisible(this->table);
    }
    
    ~IRTableListBox()
    {
        
    }
    // =============================================
    void makeTableContents()
    {
        
        int width = getWidth() - 20;
        this->table.getHeader().addColumn(this->title,
                                          1,
                                          width,
                                          width,
                                          width,
                                          TableHeaderComponent::defaultFlags);
    }
    // =============================================
    
    void resized() override
    {
        this->table.setBounds(0,0,getWidth(), getHeight());
    }
    
    void selectedRowsChanged(int lastRowSelected) override
    {
        this->status = IRTableListBoxStatus::selectedRowChanged;
        this->selectedIndex = lastRowSelected;
        sendChangeMessage();
    }
    
    void selectedRowDoubleClicked(int lastRowSelected)
    {
        this->status = IRTableListBoxStatus::selectedRowDoubleClicked;
        this->selectedIndex = lastRowSelected;
        sendChangeMessage();
    }
    // =============================================
    //TableList
    
    int getNumRows() override
    {
        return this->numRows;
    }
    
    void setNumRows(int n){
        this->numRows = n;
        this->table.updateContent();
    }
    
    void paintRowBackground(Graphics& g,
                            int rowNumber,
                            int width, int height,
                            bool rowIsSelected) override
    {
        if(rowIsSelected) g.fillAll(Colour::fromFloatRGBA(1,1,1,0.25));
    }
    
    void paintCell (Graphics& g, int rowNumber, int columnId,
                    int width, int height, bool rowIsSelected) override
    {
        
    }
    
    int getColumnAutoSizeWidth(int columnId) override
    {
        int widest = 32;
        return widest;
    }
    
    void sortOrderChanged(int newSortColumnId, bool isForwards) override
    {
        
    }
    // =============================================
    virtual Component* refreshComponentForCell (int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override
    {
        jassert(existingComponentToUpdate == nullptr);
        return nullptr;
    }
    // =============================================
    
    int getSelection(const int rowNumber) const
    {
        return this->dataList->getChildElement(rowNumber)->getIntAttribute("Select");
    }
    
    void setSelection(const int rowNumber, const int newSelection)
    {
        this->dataList->getChildElement(rowNumber)->setAttribute("Select", newSelection);
    }
    
    // =============================================
    int getSelectedIndex() const { return this->selectedIndex; }
    IRTableListBoxStatus getStatus() const { return this->status; }
    
    // =============================================
    
    
public:
    
    TableListBox table {{}, this};
    
private:
    
    IRTableListBoxStatus status;
    
    int selectedIndex;
    
    String title;
    XmlElement* dataList = nullptr;
    
    int numRows = 0;
};




#endif /* ListTypeScrollableMenu_h */
