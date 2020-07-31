#pragma once
#include "DxLib.h"
#include "Singleton.h"

class DxSystem : public Singleton<DxSystem> {
    DxSystem();
    friend Singleton< DxSystem >;
public:

};