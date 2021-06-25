#pragma once
#include "AnimClipEditorWindow.h"
#include "..\Engine\InputTextField.h"

class AnimationWindow :
    public virtual  AnimClipEditorWindow
{
public:
      
      AnimationWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture);
      AnimationWindow();
      virtual ~AnimationWindow();
      virtual void Update(float _dt);
      virtual void MouseDown(unsigned int _key);
      virtual void MouseUp(unsigned int _key);
      virtual void MouseMove(unsigned int _x, unsigned int _y);
      virtual void KeyDown(unsigned int _key);
      virtual void KeyUp(unsigned int _key);

      virtual void Render(SDLRenderer* _renderer);
      virtual void SetClip(AnimationClip* _clip);
      virtual void SetFont(TTF_Font* _font);

protected:
       virtual void Init(Texture* _IconsTexture);
       virtual void ReScaleContent();
       virtual void Reposition();

       //We seperated our 4 sections into seperate functions to make it more clear
       void RepositionTop();
       void RepositionEditFrame();
       void RepositionPreview();
       void RepositionBottom();

       void PlayClip();
       void LoopClip();
       void PauseClip();
       void NextFrame();
       void PrevFrame();
       void SpeedUp();
       void SlowDown();
       void ToggleOffset();
       void DeleteCallback();
       void DeleteYes();
       void DeleteNo();
       void RemoveYesNoButtons();

       Box m_EnableLooping; //Could be a button, 
       Box m_BottomContentBox;
       Box m_TopContentBox;
       Box m_InGamePreviewBox;
       Box m_RawPreviewBox;
       Box m_EditFrameBox;//This box area will be used to edit the frame

       //Buttons
       Button m_PlayButton;
       Button m_PauseButton;
       Button m_LoopButton;
       Button m_NextFrame;
       Button m_PrevFrame;
       Button m_ToggleOffset;
       Button m_DeleteButton;
       Button m_DeleteYes;
       Button m_DeleteNo;

    
       //Fast forward button
       Button m_FastForward;
       Button m_SlowDown;

       float m_PlayBackSpeed;

       bool m_Playing, m_Looping, m_Pausing, m_DeleteMode;
    

       //Textfields
       TextField m_IsLoopingTextField;
       TextField m_EnableOffsetTextField;
       TextField m_FrameCounterTextField;
       TextField m_CurrentSpeedTextField;
       TextField m_InGameText;
       TextField m_RawText;
       TextField m_OffsetText;
       TextField m_FrameRectText;
       TextField m_DeletePrompt;

       //Input TextFields
       InputTextField* m_IntervalInputField;
       InputTextField* m_FrameX;
       InputTextField* m_FrameY;
       InputTextField* m_FrameW;
       InputTextField* m_FrameH;
       InputTextField* m_OffsetX;
       InputTextField* m_OffsetY;
       InputTextField* m_AnimationClipName;

       //Use this to call generic functions like Render
       std::vector<TextField*> m_TextFields;
       std::vector<Button*> m_Buttons;
       std::vector<InputTextField*> m_InputTextFields;

       Object m_Obj;
       Object m_InGame;


       Vector2 m_ButtonsOffset;
       Vector2 m_InGamePos;

       int m_PrevFrameIndex;
};

