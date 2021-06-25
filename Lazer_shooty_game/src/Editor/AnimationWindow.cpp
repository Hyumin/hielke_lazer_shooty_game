#include "AnimationWindow.h"
#include "..\Engine\Texture.h"
#include "..\Engine\AnimationClip.h"
#include "..\Engine\TextFieldBuilder.h"
#include "..\Engine\ButtonBuilder.h"

AnimationWindow::AnimationWindow(Vector2 _pos, std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	Init(_IconsTexture);
}

AnimationWindow::AnimationWindow()
{
	m_Pos = Vector2{ 0,0 };
	m_Name = "empty window";
	Init(nullptr);
}

AnimationWindow::~AnimationWindow()
{
	delete m_IntervalInputField;
	m_IntervalInputField = nullptr;
	delete m_FrameX;
	m_FrameX = nullptr;
	delete m_FrameY;
	m_FrameY = nullptr;
	delete m_FrameW;
	m_FrameW = nullptr;
	delete m_FrameH;
	m_FrameH = nullptr;
	delete m_OffsetX;
	m_OffsetX = nullptr;
	delete m_OffsetY;
	m_OffsetY = nullptr;
	delete m_AnimationClipName;
	m_AnimationClipName = nullptr;
}

void AnimationWindow::Update(float _dt)
{
	EditorWindow::Update(_dt);


	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->Update(_dt);
	}
	//We can only update our animation clip if it has some frames in it
	if (m_CurrentClip != nullptr&& m_CurrentClip->m_SourceRects.size() > 0)
	{
		m_CurrentClip->Update(_dt * m_PlayBackSpeed);
		int currIndex = m_CurrentClip->m_CurrentIndex;
		m_FrameCounterTextField.SetText("Frame:" + std::to_string(m_CurrentClip->m_CurrentIndex));
		m_CurrentSpeedTextField.SetText("Speed:" + std::to_string(m_PlayBackSpeed));

		//update this the moment a new frame has been loaded else when paused can be used to edit certain things OwO
		if (currIndex != m_PrevFrameIndex)
		{
			try
			{
				const SDL_Rect& rect = m_CurrentClip->GetRect();
				m_FrameX->SetText(std::to_string(rect.x));
				m_FrameY->SetText(std::to_string(rect.y));
				m_FrameW->SetText(std::to_string(rect.w));
				m_FrameH->SetText(std::to_string(rect.h));

				Vector2 offset = m_CurrentClip->GetOffset();
				m_OffsetX->SetText(std::to_string((int)offset.x));
				m_OffsetY->SetText(std::to_string((int)offset.y));
			}
			catch (std::exception& e)
			{
				printf(e.what());
			}
		}
		//Only allow editing when the clip isn't playing
		if (!m_CurrentClip->m_IsPlaying)
		{
			if (m_FrameX->m_Changed)
			{
				m_CurrentClip->m_SourceRects[currIndex].x = std::stoi(m_FrameX->GetText());
				m_ChangeToAnimationClip = true;
			}
			if (m_FrameY->m_Changed)
			{
				m_CurrentClip->m_SourceRects[currIndex].y = std::stoi(m_FrameY->GetText());
				m_ChangeToAnimationClip = true;
			}
			if (m_FrameW->m_Changed)
			{
				m_CurrentClip->m_SourceRects[currIndex].w = std::stoi(m_FrameW->GetText());
				m_ChangeToAnimationClip = true;
			}
			if (m_FrameH->m_Changed)
			{
				m_CurrentClip->m_SourceRects[currIndex].h = std::stoi(m_FrameH->GetText());
				m_ChangeToAnimationClip = true;
			}
			if (m_OffsetX->m_Changed)
			{
				m_CurrentClip->m_Offsets[currIndex].x = std::stof(m_OffsetX->GetText());
			}
			if (m_OffsetY->m_Changed)
			{
				m_CurrentClip->m_Offsets[currIndex].y = std::stof(m_OffsetY->GetText());
			}
		}
		
		if (m_CurrentClip->m_IsFinished)
		{
			m_Playing = false;
		}
		//Vooodooo magic
		const std::string& interval = m_IntervalInputField->GetText();
		try
		{
			float i =std::stof(interval);
			m_CurrentClip->m_AnimInterval = i;
		}
		catch (std::invalid_argument& e)
		{
			printf(e.what());
		}
		
		m_Obj.m_RenderInterface.srcRect = m_CurrentClip->GetRect();
		m_Obj.m_Size = Vector2{ (float)m_CurrentClip->GetRect().w,(float)m_CurrentClip->GetRect().h };
		m_Obj.m_Size *= 2;

		m_InGame.m_RenderInterface.srcRect = m_CurrentClip->GetRect();
		m_InGame.m_Pos = m_InGamePos +m_CurrentClip->GetOffset();


		//Calculate aspect ratio
		Vector2 m_Aspectratio;
		m_Aspectratio.x = (float)m_InGame.m_RenderInterface.srcRect.w / 32.0f;
		m_Aspectratio.y = (float)m_InGame.m_RenderInterface.srcRect.h / 32.0f;

		m_InGame.m_Size = { 75.0f * m_Aspectratio.x ,75.0f * m_Aspectratio.y };


		m_PrevFrameIndex = m_CurrentClip->m_CurrentIndex;
	}

}

void AnimationWindow::MouseDown(unsigned int _key)
{
	EditorWindow::MouseDown(_key);

	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->MouseDown(_key);
	}
	if (_key == (SDL_BUTTON_LEFT))
	{
		if (m_EnableLooping.BoxCollision(m_EnableLooping, m_MousePos))
		{
			LoopClip();
		}
	}
}

void AnimationWindow::MouseUp(unsigned int _key)
{
	EditorWindow::MouseUp(_key);
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->MouseUp(_key);
	}
}

void AnimationWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	EditorWindow::MouseMove(_x, _y);
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x, _y);
	}
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->MouseMove(_x, _y);
	}
	if (m_ScalingSize)
	{
		ReScaleContent();
	}
}

void AnimationWindow::KeyDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->KeyDown(_key);
	}
}

void AnimationWindow::KeyUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->KeyUp(_key);
	}
}


void AnimationWindow::Render(SDLRenderer* _renderer)
{
	EditorWindow::Render(_renderer);

	m_Obj.Render(_renderer, Vector2{ 0, 0 },1);
	m_InGame.Render(_renderer, Vector2{ 0,0 },1);

	for (int i = 0; i < m_TextFields.size(); ++i)
	{
		m_TextFields[i]->Render(_renderer, { 0,0 }, 1);
	}
	for (unsigned int i =0; i <m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Render(_renderer);
	}
	_renderer->DrawFilledBox(m_BottomContentBox, m_Color, { 0,0 }, 0);
	_renderer->DrawFilledBox(m_RawPreviewBox, { 0x0, 0x8D, 0xAD, 0xff }, { 0,0 }, 0);
	_renderer->DrawFilledBox(m_InGamePreviewBox, { 0x0, 0x8D, 0xAD, 0xff }, { 0,0 }, 0);
	_renderer->DrawFilledBox(m_TopContentBox, m_Color, { 0,0 }, 0);
	_renderer->DrawFilledBox(m_EditFrameBox, m_Color);
	
	for (unsigned int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->Render(_renderer);
	}
	if (m_CurrentClip != nullptr)
	{
		if (m_CurrentClip->m_Looping)
		{
			_renderer->DrawFilledBox((int)m_EnableLooping.pos.x, (int)m_EnableLooping.pos.y,(int) m_EnableLooping.w,(int) m_EnableLooping.h, { 0,0,0,255 });
		}
		else
		{
			_renderer->DrawBox(m_EnableLooping, { 0,0,0,255 }, { 0,0 }, 1);
		}
		if (m_CurrentClip->m_UseOffsets)
		{
			_renderer->DrawFilledBox(m_ToggleOffset.GetCollider(), { 0,0,0,255 });
		}

	}
	if (m_Pausing)
	{
		_renderer->DrawBox(m_PauseButton.GetCollider(), m_LightColor, { 0,0 }, 1);
	}
	if (m_Looping)
	{
		_renderer->DrawBox(m_LoopButton.GetCollider(), m_LightColor, { 0,0 },1);
	}
	if (m_Playing)
	{
		_renderer->DrawBox(m_PlayButton.GetCollider(), m_LightColor, { 0,0 }, 1);
	}
}

void AnimationWindow::SetClip(AnimationClip* _clip)
{
	m_CurrentClip = _clip;

	m_AnimationClipName->SetText(_clip->m_ClipName);
	m_Looping = m_CurrentClip->m_Looping;
	

	//Set textures for the objects
	m_Obj.m_RenderInterface.texture = m_CurrentClip->m_SourceTexture;
	m_InGame.m_RenderInterface.texture = m_Obj.m_RenderInterface.texture;
	
	//Update edit textfields
	m_IntervalInputField->SetText(std::to_string(m_CurrentClip->m_AnimInterval));

	if (m_CurrentClip->m_SourceRects.size() > 0)
	{
		const SDL_Rect& rect = m_CurrentClip->GetRect();
		m_FrameX->SetText(std::to_string(rect.x));
		m_FrameY->SetText(std::to_string(rect.y));
		m_FrameW->SetText(std::to_string(rect.w));
		m_FrameH->SetText(std::to_string(rect.h));

		Vector2 offset = m_CurrentClip->m_Offsets[m_CurrentClip->m_CurrentIndex];
		m_OffsetX->SetText(std::to_string((int)offset.x));
		m_OffsetY->SetText(std::to_string((int)offset.y));
	}

}

void AnimationWindow::SetFont(TTF_Font* _font)
{
	EditorWindow::SetFont(_font);
	for (int i = 0; i < m_TextFields.size(); ++i)
	{
		m_TextFields[i]->SetFont(_font);
	}
	for (int i = 0; i < m_InputTextFields.size(); ++i)
	{
		m_InputTextFields[i]->SetFont(_font);
	}
	m_DeleteYes.SetFont(_font);
	m_DeleteNo.SetFont(_font);
}

void AnimationWindow::Init(Texture* _IconsTexture)
{
	EditorWindow::Init(_IconsTexture);
	
	m_PlayBackSpeed = 1.0f;
	m_Obj = Object();
	m_InGame = Object();

	m_InGame.m_Size = { 75,75 };

	//Initialize textfields
	m_IsLoopingTextField = TextFieldBuilder::BuildTextField({ 0,0,0,255 }, "Looping:", nullptr, { 0,0 }, { 70,20 });
	m_FrameCounterTextField = TextFieldBuilder::BuildTextField({ 255,255,255,255 }, "Frame:", nullptr, { 0,0 }, { 80,20 });
	m_CurrentSpeedTextField = TextFieldBuilder::BuildTextField({ 255,255,255,255 }, "Speed:", nullptr, { 0,0 }, { 80,20 });
	m_RawText = TextFieldBuilder::BuildTextField({ 255,255,255,255 }, "Raw:", nullptr, { 0,0 }, { 80,20 });
	m_InGameText = TextFieldBuilder::BuildTextField({ 255,255,255,255 }, "In-Game:", nullptr, { 0,0 }, { 80,20 });
	m_EnableOffsetTextField = TextFieldBuilder::BuildTextField({ 0,0,0,255 }, "Enable Offsets:", nullptr, { 0,0 }, { 120,20 });
	m_OffsetText = TextFieldBuilder::BuildTextField({ 0,0,0,255 }, "Offset:", nullptr, { 0,0 }, { 60,20 });
	m_FrameRectText = TextFieldBuilder::BuildTextField({ 0,0,0,255 }, "Frame Rect:", nullptr, { 0,0 }, { 100,20 });
	m_DeletePrompt = TextFieldBuilder::BuildTextField({ 0,0,0,255 }, " ", nullptr, { 0,0 }, { 200,40 });

	m_TextFields.push_back(&m_OffsetText);
	m_TextFields.push_back(&m_FrameRectText);
	m_TextFields.push_back(&m_EnableOffsetTextField);
	m_TextFields.push_back(&m_IsLoopingTextField);
	m_TextFields.push_back(&m_FrameCounterTextField);
	m_TextFields.push_back(&m_CurrentSpeedTextField);
	m_TextFields.push_back(&m_RawText);
	m_TextFields.push_back(&m_InGameText);
	m_TextFields.push_back(&m_DeletePrompt);

	//Define  sizes of content boxes( the green boxes )
	m_BottomContentBox = Box{ 0,m_ContentBox.pos.y+m_ContentScaleObject.m_Size.y- m_Bar.h,m_ContentBox.w,m_ContentBox.h/5 };
	m_TopContentBox = Box{m_ContentBox.pos.x,m_ContentBox.pos.y,m_ContentBox.w,m_ContentBox.h/3};
	m_InGamePreviewBox = Box{};
	m_RawPreviewBox = Box{};
	
	//Initialize buttons,
	m_EnableLooping = Box(0, 0, 20, 20);

	m_PlayButton = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 },1,std::bind(&AnimationWindow::PlayClip,this));
	m_PauseButton = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::PauseClip, this));
	m_LoopButton = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::LoopClip, this));
	m_FastForward = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::SpeedUp, this));
	m_SlowDown = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::SlowDown, this));
	m_NextFrame = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::NextFrame, this));
	m_PrevFrame = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::PrevFrame, this));
	m_ToggleOffset = ButtonBuilder::BuildButton({ 0,0 }, { 20,20 }, 1, std::bind(&AnimationWindow::ToggleOffset, this));
	m_DeleteButton = ButtonBuilder::BuildButton({ 0,0 }, { 35,35 }, 1, std::bind(&AnimationWindow::DeleteCallback, this));
	m_DeleteYes = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, {40,20 }, 1, std::bind(&AnimationWindow::DeleteYes, this),
		"Yes", Button::DrawMode::WIREFRAME, { 0,0,0,255 }, { 255,255,255,255 }, { 100,100,100,255 }, {0,0,0,255});
	m_DeleteNo = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, { 40,20 }, 1, std::bind(&AnimationWindow::DeleteYes, this),
		"no", Button::DrawMode::WIREFRAME, { 0,0,0,255 }, { 255,255,255,255 }, { 100,100,100,255 }, { 0,0,0,255 });

	m_ToggleOffset.SetWireFrameMode({0,0,0,255}, {100,100,100,255}, {255,255,255});

	m_ButtonsOffset = Vector2{ m_TopContentBox.w / 2 -(m_PlayButton.GetSize().x*1.5f),20 };

	if (m_IconTexture != nullptr)
	{
		SDL_Rect norm= { 0,0,16,16 };
		SDL_Rect hovered = { 64,0,16,16 };
		SDL_Rect clicked = {112,0,16,16};
		m_PlayButton.SetTextureDrawModeWithSheet(_IconsTexture, norm, clicked, hovered);
		
		norm= { 32,16,16,16 };
		hovered = { 96,0,16,16 };
		clicked= { 144,0,16,16 };
		m_LoopButton.SetTextureDrawModeWithSheet(_IconsTexture, norm, clicked, hovered);

		norm = { 16,0,16,16 };
		hovered = { 80,0,16,16 };
		clicked= { 128,0,16,16 };
		m_PauseButton.SetTextureDrawModeWithSheet(_IconsTexture, norm, clicked, hovered);

		norm = { 0,32,16,16 };
		hovered = { 16,32,16,16 };
		clicked = { 32,32,16,16 };
		m_NextFrame.SetTextureDrawModeWithSheet(_IconsTexture, norm, clicked, hovered);
		m_PrevFrame.SetTextureDrawModeWithSheet(_IconsTexture, norm, clicked, hovered,SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

		norm = { 48,32,16,16 };
		hovered = { 64,32,16,16 };
		clicked = { 80,32,16,16 };
		m_FastForward.SetTextureDrawModeWithSheet(_IconsTexture, norm, clicked, hovered);
		m_SlowDown.SetTextureDrawModeWithSheet(_IconsTexture, norm, clicked, hovered, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);

		norm = { 160,0,16,16 };
		hovered = { 176,0,16,16 };
		clicked = { 192,0,16,16 };
		m_DeleteButton.SetTextureDrawModeWithSheet(_IconsTexture, norm, clicked, hovered);

	}
	m_Playing = false;
	m_Looping = false;
	m_Pausing = false;

	m_Buttons.push_back(&m_PlayButton);
	m_Buttons.push_back(&m_PauseButton);
	m_Buttons.push_back(&m_LoopButton);	
	m_Buttons.push_back(&m_FastForward);
	m_Buttons.push_back(&m_SlowDown);
	m_Buttons.push_back(&m_NextFrame);
	m_Buttons.push_back(&m_PrevFrame);
	m_Buttons.push_back(&m_ToggleOffset);
	m_Buttons.push_back(&m_DeleteButton);

	//Initialize input text field
	m_IntervalInputField = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "Interval", nullptr,
			{ 0,0 }, { 70,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });
	m_AnimationClipName = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Text, "Name:", nullptr,
		{ 0,0 }, { 200,20 }, 50, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });


	m_IntervalInputField->m_NameOffset = Vector2{ -60,0 };
	m_AnimationClipName->m_NameOffset = Vector2{ -60,0 };

	//What a fricking mess it is to initilaize this much
	m_FrameX = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "X", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });
	m_FrameY = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "Y", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });
	m_FrameW = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "W", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });
	m_FrameH = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "H", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });
	m_OffsetX = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "X", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });
	m_OffsetY = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "Y", nullptr,
		{ 0,0 }, { 50,20 }, 9, { 0,0,0,255 }, { 0,0,0,255 }, { 75,75,75,255 }, { 255,255,255,255 });


	m_InputTextFields.push_back(m_FrameX);
	m_InputTextFields.push_back(m_FrameY);
	m_InputTextFields.push_back(m_FrameW);
	m_InputTextFields.push_back(m_FrameH);
	m_InputTextFields.push_back(m_OffsetX);
	m_InputTextFields.push_back(m_OffsetY);
	m_InputTextFields.push_back(m_AnimationClipName);

	m_InputTextFields.push_back(m_IntervalInputField);

	m_RawPreviewBox.w = m_ContentBox.w / 2;
	m_RawPreviewBox.h = m_ContentBox.h / 4;
	m_InGamePreviewBox.w = m_ContentBox.w / 2;
	m_InGamePreviewBox.h = m_ContentBox.h / 4;

	m_TopContentBox.w = m_ContentBox.w;
	m_TopContentBox.h = m_ContentBox.h / 4;
	m_EditFrameBox.w = m_ContentBox.w;
	m_EditFrameBox.h = m_ContentBox.h / 4;

	m_BottomContentBox.w = m_ContentBox.w;
	m_BottomContentBox.h = m_ContentBox.h - m_TopContentBox.h - m_InGamePreviewBox.h - m_EditFrameBox.h;


	m_ScaleOnStart = Vector2{ m_ContentBox.w,m_ContentBox.h };
	m_ScaleStart = Vector2{ m_ContentBox.w,m_ContentBox.h };
	m_MousePos = Vector2{400,500};
	ReScaleContent();
	Reposition();
	m_PrevFrameIndex = -1;
	
}

void AnimationWindow::ReScaleContent()
{
	EditorWindow::ReScaleContent();

	float newWidth = m_ScaleOnStart.x - (m_ScaleStart.x - m_MousePos.x);
	float newHeight = m_ScaleOnStart.y - (m_ScaleStart.y - m_MousePos.y);

	if (newWidth < 100)
	{
		newWidth = 100;
	}
	if (newHeight < 100)
	{
		newHeight = 100;
	}
	m_ContentBox.w = newWidth;
	m_ContentBox.h = newHeight;

	m_Bar.w = m_ContentBox.w;
	m_CrossRelativePos.x = m_Bar.w - m_ExitButton.GetSize().x;


	// top part
	m_TopContentBox.w = m_ContentBox.w;
	m_TopContentBox.h = m_AnimationClipName->GetSize().y + m_IsLoopingTextField.m_Size.y + m_IntervalInputField->GetSize().y;

	//bottom part
	m_BottomContentBox.w = m_ContentBox.w;
	m_BottomContentBox.h = m_CurrentSpeedTextField.m_Size.y+ m_LoopButton.GetSize().y+ m_PrevFrame.GetSize().y;

	//edit fram part or upple middle part
	m_EditFrameBox.w = m_ContentBox.w;
	m_EditFrameBox.h =  m_FrameRectText.m_Size.y +( m_OffsetX->GetSize().y*2)+ m_DeleteButton.GetSize().y + m_DeletePrompt.m_Size.y+ m_DeleteYes.GetSize().y;

	//Preview boxes scaling
	m_RawPreviewBox.w = m_ContentBox.w / 2;
	m_InGamePreviewBox.w = m_ContentBox.w / 2;

	m_InGamePreviewBox.h = m_ContentBox.h-  m_EditFrameBox.h- m_BottomContentBox.h - m_TopContentBox.h;
	m_RawPreviewBox.h = m_InGamePreviewBox.h;


	m_ButtonsOffset = Vector2{ m_TopContentBox.w / 2 - (m_PlayButton.GetSize().x * 1.5f),20 };
}

void AnimationWindow::Reposition()
{
	EditorWindow::Reposition();
	RepositionTop();
	RepositionEditFrame();
	RepositionPreview();
	RepositionBottom();
}

void AnimationWindow::RepositionTop()
{
	//Top part
	m_TopContentBox.pos = m_ContentBox.pos;
	m_AnimationClipName->SetPosition(m_TopContentBox.pos-m_AnimationClipName->m_NameOffset);
	m_IsLoopingTextField.m_pos = m_TopContentBox.pos;
	m_IsLoopingTextField.m_pos.y += m_AnimationClipName->GetSize().y;

	m_EnableLooping.pos = m_IsLoopingTextField.m_pos;
	m_EnableLooping.pos.x += m_IsLoopingTextField.m_Size.x;
	m_IntervalInputField->SetPosition(m_IsLoopingTextField.m_pos + Vector2{ m_IntervalInputField->m_NameOffset.x * -1,m_IsLoopingTextField.m_Size.y });

	m_EnableOffsetTextField.m_pos = m_EnableLooping.pos + Vector2{ m_EnableLooping.w ,0 };
	m_ToggleOffset.SetPosition(m_EnableOffsetTextField.m_pos + Vector2{ m_EnableOffsetTextField.m_Size.x,0 });

}

void AnimationWindow::RepositionEditFrame()
{
	//The edit framebox position
	m_EditFrameBox.pos = m_TopContentBox.pos + Vector2{ 0,m_TopContentBox.h };

	m_FrameRectText.m_pos = m_EditFrameBox.pos;
	m_OffsetText.m_pos = m_FrameRectText.m_pos + Vector2{ m_FrameW->GetSize().x * 4,0 };

	m_FrameX->SetPosition(m_FrameRectText.m_pos + m_FrameX->m_NameOffset * -1 + Vector2{ 0,m_FrameX->GetSize().y });
	m_FrameY->SetPosition(m_FrameX->GetPosition() + Vector2{ m_FrameX->GetSize().x * 2,0 });

	m_FrameW->SetPosition(m_FrameX->GetPosition() + Vector2{ 0,m_FrameX->GetSize().y });
	m_FrameH->SetPosition(m_FrameW->GetPosition() + Vector2{ m_FrameW->GetSize().x * 2,0 });

	m_OffsetX->SetPosition(m_OffsetText.m_pos + Vector2{ 0,m_OffsetText.m_Size.y });
	m_OffsetY->SetPosition(m_OffsetX->GetPosition() + Vector2{ m_OffsetY->GetSize().x * 2,0 });

	m_DeleteButton.SetPosition(m_FrameW->GetPosition() + Vector2{ 0,m_FrameW->GetSize().y });
	m_DeletePrompt.m_pos = m_DeleteButton.GetPosition() + Vector2{m_DeleteButton.GetSize().x,0};
	m_DeleteYes.SetPosition(m_DeletePrompt.m_pos + Vector2{ 0,m_DeletePrompt.m_Size.y });
	m_DeleteNo.SetPosition(m_DeleteYes.GetPosition() + Vector2{ m_DeleteYes.GetSize().x,0 });

}

void AnimationWindow::RepositionPreview()
{
	//The two preview panels part
	m_InGamePreviewBox.pos = m_EditFrameBox.pos + Vector2{ 0,m_EditFrameBox.h };
	m_RawPreviewBox.pos = m_EditFrameBox.pos + Vector2{ m_InGamePreviewBox.w,m_EditFrameBox.h };

	m_InGameText.m_pos = m_InGamePreviewBox.pos;
	m_RawText.m_pos = m_RawPreviewBox.pos;

	m_Obj.m_Pos = m_RawPreviewBox.pos + Vector2{ m_RawPreviewBox.w / 4,m_RawPreviewBox.h / 4 };
	m_InGamePos = m_InGamePreviewBox.pos + Vector2{ m_InGamePreviewBox.w / 4,m_InGamePreviewBox.h / 4 };
	m_InGame.m_Pos = m_InGamePos;
}

void AnimationWindow::RepositionBottom()
{
	//Bottom part
	m_BottomContentBox.pos.x = m_ContentBox.pos.x;
	m_BottomContentBox.pos.y = m_InGamePreviewBox.pos.y + m_InGamePreviewBox.h;

	//Reposition bottom buttons
	m_PauseButton.SetPosition(m_BottomContentBox.pos + m_ButtonsOffset + Vector2{ m_LoopButton.GetSize().x,0 });
	m_PlayButton.SetPosition(m_BottomContentBox.pos + m_ButtonsOffset + Vector2{ m_PauseButton.GetSize().x * 2,0 });
	m_LoopButton.SetPosition(m_BottomContentBox.pos + m_ButtonsOffset);
	m_SlowDown.SetPosition(m_LoopButton.GetPosition() + Vector2{ -m_SlowDown.GetSize().x,0 });
	m_FastForward.SetPosition(m_PlayButton.GetPosition() + Vector2{ m_FastForward.GetSize().x,0 });
	m_PrevFrame.SetPosition(m_LoopButton.GetPosition() + Vector2{ -m_LoopButton.GetSize().x / 2,m_LoopButton.GetSize().y });
	//Speed, frame counter and next frame arrow
	m_FrameCounterTextField.m_pos = m_PrevFrame.GetPosition() + Vector2{ m_PrevFrame.GetSize().x,0 };
	m_CurrentSpeedTextField.m_pos = m_LoopButton.GetPosition() + Vector2{ 0,-m_LoopButton.GetSize().y / 2 };
	m_NextFrame.SetPosition(m_FrameCounterTextField.m_pos + Vector2{ m_FrameCounterTextField.m_Size.x,0 });

}

void AnimationWindow::PlayClip()
{
	if (m_CurrentClip != nullptr)
	{
		m_Playing = m_Playing ? false : true;
		if (m_Playing)
		{
			m_CurrentClip->Play();
			m_Pausing = false;
		}
	}
}

void AnimationWindow::LoopClip()
{
	if (m_CurrentClip != nullptr)
	{
		m_Looping = m_Looping ? false : true;
		m_CurrentClip->m_Looping = m_Looping;
	}
}

void AnimationWindow::PauseClip()
{
	if (m_CurrentClip != nullptr)
	{
		m_Pausing = m_Pausing ? false : true;
		m_CurrentClip->m_IsPlaying = false;
		m_Playing = false;
	}
}

void AnimationWindow::NextFrame()
{
	if (m_CurrentClip != nullptr)
	{
		m_CurrentClip->NextFrame();
	}
}

void AnimationWindow::PrevFrame()
{
	if (m_CurrentClip != nullptr)
	{
		m_CurrentClip->PrevFrame();
	}
}

void AnimationWindow::SpeedUp()
{
	m_PlayBackSpeed += 0.1f;
}

void AnimationWindow::SlowDown()
{
	if (m_PlayBackSpeed > 0.1f)
	{
		m_PlayBackSpeed -= 0.1f;
	}
}

void AnimationWindow::ToggleOffset()
{
	if (m_CurrentClip != nullptr)
	{
		m_CurrentClip->m_UseOffsets = m_CurrentClip->m_UseOffsets ? false : true;
	}
}

void AnimationWindow::DeleteCallback()
{
	if (m_CurrentClip != nullptr)
	{
		m_DeleteMode = m_DeleteMode ? false : true;

		if (m_DeleteMode)
		{
			m_Buttons.push_back(&m_DeleteNo);
			m_Buttons.push_back(&m_DeleteYes);
			m_DeletePrompt.SetText("Are you sure you want to delete frame:" + std::to_string(m_CurrentClip->m_CurrentIndex) + "?");
		}
		else
		{
			RemoveYesNoButtons();
		}
	}
}

void AnimationWindow::DeleteYes()
{
	//Delete the actual frame, and put current animation to the previous one
	m_DeleteMode = false;
	m_CurrentClip->RemoveFrameAtIndex(m_CurrentClip->m_CurrentIndex);
	m_ChangeToAnimationClip = true;
	PrevFrame();
	RemoveYesNoButtons();
	
}

void AnimationWindow::DeleteNo()
{
	m_DeleteMode = false;
	RemoveYesNoButtons();
}

void AnimationWindow::RemoveYesNoButtons()
{
	int leIndex = -1;
	for (int i = 0; i < m_Buttons.size(); ++i)
	{
		if (m_Buttons[i] == &m_DeleteNo)
		{
			leIndex = i;
		}
		if (m_Buttons[i] == &m_DeleteYes)
		{
			leIndex = i;
		}
	}
	if (leIndex >= 0)
	{
		m_Buttons.erase(m_Buttons.begin() + leIndex);
		leIndex = -1;
		for (int i = 0; i < m_Buttons.size(); ++i)
		{
			if (m_Buttons[i] == &m_DeleteNo)
			{
				leIndex = i;
			}
			if (m_Buttons[i] == &m_DeleteYes)
			{
				leIndex = i;
			}
		}
		if (leIndex >= 0)
		{
			m_Buttons.erase(m_Buttons.begin() + leIndex);
		}
	}


	m_DeletePrompt.SetText(" ");
}
