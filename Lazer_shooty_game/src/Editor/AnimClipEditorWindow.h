#pragma once
#include "EditorWindow.h"

class AnimationClip;

class AnimClipEditorWindow :
    public virtual EditorWindow
{
public:
    AnimClipEditorWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture);
    AnimClipEditorWindow();
    virtual ~AnimClipEditorWindow();
    virtual void SetClip(AnimationClip* _clip);
    virtual void Update(float _dt);
    virtual void MouseDown(unsigned int _key);
    virtual void MouseUp(unsigned int _key);
    virtual void MouseMove(unsigned int _x, unsigned int _y);
    virtual void KeyDown(unsigned int _key) = 0;
    virtual void KeyUp(unsigned int _key) = 0;

    virtual void SetFont(TTF_Font* _font);

    virtual void Render(SDLRenderer* _renderer);
    bool m_ChangeToAnimationClip;// If anything changes that requires the animation clip editor to change something set this to true

protected:

    virtual void Init(Texture* _IconsTexture);
    virtual void ReScaleContent();
    virtual void Reposition();//Makes it so the positions of the objects only change when dragging 

    AnimationClip* m_CurrentClip;

};

