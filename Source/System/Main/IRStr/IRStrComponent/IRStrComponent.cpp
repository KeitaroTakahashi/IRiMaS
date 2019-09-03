//
//  IRStrComponent.cpp
//  IRiMaS
//
//  Created by Keitaro on 29/08/2019.
//

#include "IRStrComponent.hpp"

IRStrComponent::IRStrComponent(IRStr* str)
{
    this->ir_str = str;
    
    this->USERSETTING = this->ir_str->USERSETTING;
    this->SYSTEMCOLOUR = this->ir_str->SYSTEMCOLOUR;
    this->ICONBANK = this->ir_str->ICONBANK;
    this->OBJECTFACTORY = this->ir_str->OBJECTFACTORY;

}

IRStrComponent::~IRStrComponent()
{
    
}


