#pragma once

enum class Scene {
    Setup,
    Title,
    Menu,

    None,
} ;

class ISceneChanger {
public:
    virtual ~ISceneChanger() = 0;
    virtual void ChangeScene(Scene NextScene) = 0;
};