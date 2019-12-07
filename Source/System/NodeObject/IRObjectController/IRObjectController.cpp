//
//  IRObjectController.cpp
//  IRiMaS
//
//  Created by Keitaro on 02/10/2019.
//

#include "IRObjectController.hpp"

IRObjectController::IRObjectController(IRStr* str) : IRStrComponent(str)
{
    addMouseListener(str->projectOwner, false);
}

IRObjectController::~IRObjectController()
{
    
}


