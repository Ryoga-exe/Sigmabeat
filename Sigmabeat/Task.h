#pragma once

class Task {
public:
    virtual ~Task() {}
    virtual void Initialize() {}
    virtual void Finalize() {}
    virtual void Update() = 0;
    virtual void Draw() = 0;
};