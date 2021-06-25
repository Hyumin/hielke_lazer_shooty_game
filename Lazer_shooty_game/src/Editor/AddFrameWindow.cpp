#include "AddFrameWindow.h"
#include "..\Engine\Texture.h"
#include "..\Engine\AnimationClip.h"
#include "..\Engine\TextFieldBuilder.h"
#include "..\Engine\ButtonBuilder.h"


void AddFrameWindow::Init(Texture* _IconsTexture)
{
	EditorWindow::Init(_IconsTexture);

	m_ContentBox.h = 200;

	//Init addframe button

	m_AddFrameButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, { 50,25 }, 1, std::bind(&AddFrameWindow::AddFrame, this),
		"Add", Button::DrawMode::WIREFRAME, { 0,0,0,255 }, { 100,100,100,255 }, { 255,255,255,255 }, { 0,0,0,255 });

	m_SelectButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, { 50,25 }, 1, std::bind(&AddFrameWindow::ToggleSelect, this),
		"Select", Button::DrawMode::WIREFRAME, { 0,0,0,255 }, { 100,100,100,255 }, { 255,255,255,255 }, { 0,0,0,255 });

	m_DragButton = ButtonBuilder::BuildButtonWireFrameOrFilledRect({ 0,0 }, { 50,25 }, 1, std::bind(&AddFrameWindow::ToggleDrag, this),
		"Drag", Button::DrawMode::WIREFRAME, { 0,0,0,255 }, { 100,100,100,255 }, { 255,255,255,255 }, { 0,0,0,255 });


	//Init the two arrow buttons 
	m_PrevFrameButton = Button{};
	m_NextFrameButton = Button{};

	m_PrevFrameButton.SetLayer(1);
	m_NextFrameButton.SetLayer(1);

	m_PrevFrameButton.SetSize({ 34,34 });
	m_NextFrameButton.SetSize({34,34});

	m_PrevFrameButton.SetCallbackFunction(std::bind(&AddFrameWindow::PrevFrame, this));
	m_NextFrameButton.SetCallbackFunction(std::bind(&AddFrameWindow::NextFrame, this));

	if (m_IconTexture != nullptr)
	{
		m_PrevFrameButton.SetTextureDrawModeWithSheet(m_IconTexture, {32,48,16,16}, {16,48,16,16}, {0,48,16,16});
		m_NextFrameButton.SetTextureDrawModeWithSheet(m_IconTexture, { 0,32,16,16 }, { 16,32,16,16 }, { 32,32,16,16 });
	}

	//Init input fields
	m_XInput = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "X:", nullptr, { 30,5 }, { 50,20 },5,
		{ 0,0,0,255 }, { 0,0,0,255 }, { 100,100,100,255 }, { 255,255 ,255 });
	m_YInput = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "Y:", nullptr, { 30,5 }, { 50,20 }, 5,
		{ 0,0,0,255 }, { 0,0,0,255 }, { 100,100,100,255 }, { 255,255 ,255 });
	m_WInput = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "W:", nullptr, { 30,5 }, { 50,20 }, 5,
		{ 0,0,0,255 }, { 0,0,0,255 }, { 100,100,100,255 }, { 255,255 ,255 });
	m_HInput = TextFieldBuilder::BuildInputTextField(InputTextField::InputTextMode::Numbers, "H:", nullptr, { 30,5 }, { 50,20 }, 5,
		{ 0,0,0,255 }, { 0,0,0,255 }, { 100,100,100,255 }, { 255,255 ,255 });

	//Init offsets for the input fields
	m_XInputOffset = Vector2{ 30,5 };
	m_YInputOffset = Vector2{ 125,5 };
	m_WInputOffset = Vector2{ 30,35 };
	m_HInputOffset = Vector2{ 125,35 };
	
	m_InputFields.push_back(m_XInput);
	m_InputFields.push_back(m_YInput);
	m_InputFields.push_back(m_WInput);
	m_InputFields.push_back(m_HInput);

	//init current frame field
	m_CurrentFrameField = TextField();
	m_CurrentFrameField.m_Size = Vector2{240, 30};
	m_CurrentFrameField.SetText("Current frame:");
	m_CurrentFrameField.SetColour({0,0,0,255});


	m_Buttons.push_back(&m_AddFrameButton);
	m_Buttons.push_back(&m_PrevFrameButton);
	m_Buttons.push_back(&m_NextFrameButton);
	m_Buttons.push_back(&m_SelectButton);
	m_Buttons.push_back(&m_DragButton);

	m_Preview = Object{ {0,0},{100,100} };
	
	m_Info = TextFieldBuilder::BuildTextField({ 0,0,0,255 }, " ", nullptr, { 0,0 }, { 250,70 });

	Reposition();

}

void AddFrameWindow::ReScaleContent()
{
	EditorWindow::ReScaleContent();
}

void AddFrameWindow::Reposition()
{
	EditorWindow::Reposition();
	m_AfOffset.y = m_ContentBox.pos.y + m_ContentBox.h - m_PrevFrameButton.GetSize().y;

	m_PrevOffset.x = m_ContentBox.pos.x ;
	m_PrevOffset.y = m_ContentBox.pos.y + m_ContentBox.h - m_PrevFrameButton.GetSize().y;

	m_NextOffset.x = m_ContentBox.pos.x + m_ContentBox.w - m_NextFrameButton.GetSize().x * 2;
	m_NextOffset.y = m_ContentBox.pos.y + m_ContentBox.h - m_NextFrameButton.GetSize().y;

	m_AfOffset.x = m_PrevOffset.x + m_PrevFrameButton.GetSize().x;
	//Places the current counter roughly in the middle
	m_CurrentFrameField.m_pos = m_ContentBox.pos + Vector2{m_ContentBox.w/2,m_ContentBox.h/2}-(m_CurrentFrameField.m_Size/2);
	m_Info.m_pos = m_CurrentFrameField.m_pos + Vector2{0, m_CurrentFrameField.m_Size.y};

	m_AddFrameButton.SetPosition(m_AfOffset);
	m_SelectButton.SetPosition(m_AfOffset + Vector2{m_AddFrameButton.GetSize().x*1.5f, 0});
	m_DragButton.SetPosition(m_SelectButton.GetPosition() + Vector2{ m_SelectButton.GetSize().x * 1.5f, 0 });

	m_NextFrameButton.SetPosition(m_NextOffset);
	m_PrevFrameButton.SetPosition(m_PrevOffset);


	m_XInput->SetPosition(m_ContentBox.pos + m_XInputOffset);
	m_YInput->SetPosition(m_ContentBox.pos + m_YInputOffset);
	m_WInput->SetPosition(m_ContentBox.pos + m_WInputOffset);
	m_HInput->SetPosition(m_ContentBox.pos + m_HInputOffset);

	m_Preview.m_Pos = m_YInput->GetPosition() + Vector2{ m_YInput->GetSize().x,0 };
}

AddFrameWindow::AddFrameWindow(Vector2 _pos, const std::string& _name, Texture* _IconsTexture)
{
	m_Pos = _pos;
	m_Name = _name;
	Init(_IconsTexture);
}

AddFrameWindow::AddFrameWindow()
{
	m_Pos = Vector2{ 0,0 };
	m_Name = " Unnamed addFramwWindow";
	Init(nullptr);
}

AddFrameWindow::~AddFrameWindow()
{
	delete m_XInput;
	m_XInput = nullptr;
	delete m_YInput;
	m_YInput = nullptr;
	delete m_WInput;
	m_WInput= nullptr;
	delete m_HInput;
	m_HInput = nullptr;
}

void AddFrameWindow::UpdateDragSelectionBox()
{
	if (m_DragMode&& m_DragginSelection)
	{
		m_SelectionBox.pos = (m_MousePos+m_ViewPos) / m_ZoomVector.x - m_DragSelectionStart;
		m_XInput->SetText(std::to_string((int)m_SelectionBox.pos.x));
		m_YInput->SetText(std::to_string((int)m_SelectionBox.pos.y));
	}
}

void AddFrameWindow::UpdateSelectionBox()
{
	if (m_SelectingMode)
	{
		if (m_Selecting)
		{
			m_SelectEnd = (m_MousePos+m_ViewPos);
			m_SelectionBox.pos = m_SelectStart ;
			m_SelectionBox.w = m_SelectEnd.x - m_SelectStart.x;
			m_SelectionBox.h = m_SelectEnd.y - m_SelectStart.y;

			m_SelectionBox.pos /= m_ZoomVector.x;
			m_SelectionBox.w /= m_ZoomVector.x;
			m_SelectionBox.h /= m_ZoomVector.x;

			//When width or height is lower then 0 meaning negative
			//we want to substract that from the position and inverse the width
			if (m_SelectionBox.w < 0)
			{
				m_SelectionBox.pos.x += m_SelectionBox.w;
				m_SelectionBox.w *= -1;
			}
			if (m_SelectionBox.h < 0)
			{
				m_SelectionBox.pos.y += m_SelectionBox.h;
				m_SelectionBox.h *= -1;
			}

			m_XInput->SetText(std::to_string(m_SelectionBox.pos.x));
			m_YInput->SetText(std::to_string(m_SelectionBox.pos.y));
			m_WInput->SetText(std::to_string(m_SelectionBox.w));
			m_HInput->SetText(std::to_string(m_SelectionBox.h));
		}
	}
	else
	{
		m_SelectionBox.pos.x = std::stof(m_XInput->GetText());
		m_SelectionBox.pos.y = std::stof(m_YInput->GetText());
		m_SelectionBox.w = std::stof(m_WInput->GetText());
		m_SelectionBox.h = std::stof(m_HInput->GetText());
	}

	m_Preview.m_RenderInterface.srcRect = {(int)m_SelectionBox.pos.x,(int)m_SelectionBox.pos.y,(int)m_SelectionBox.w,(int)m_SelectionBox.h};
}

void AddFrameWindow::AddFrame()
{
	if (m_CurrentClip == nullptr)
	{
		printf("Attempt to add frame but no clip has been created addframewindow.cpp \n");
		return;
	}

	//Converst current selection box to rectangle and inserts it into the animation
	SDL_Rect r;
	
	r.x = std::stoi(m_XInput->GetText());
	r.y = std::stoi(m_YInput->GetText());
	r.w = std::stoi(m_WInput->GetText());
	r.h = std::stoi(m_HInput->GetText());

	if (m_CurrentClip->m_SourceRects.size() > 0)
	{
		//It might happen due to removal of a frame inanother class that the current index is larger than the size, in this case put it at the last index
		if (m_CurrentIndex >= m_CurrentClip->m_SourceRects.size())
		{
			m_CurrentIndex = (unsigned int)m_CurrentClip->m_SourceRects.size() - 1;
		}

		m_CurrentClip->AddFrameAtIndex(m_CurrentIndex + 1, r);
		//increment the index after adding, this is  to make it nice to use, having to press the right arrow every time is annoying
		m_CurrentIndex++;
	}
	else
	{
		m_CurrentClip->AddFrameAtIndex(0, r);
	}
	m_ChangeToAnimationClip = true;
}

void AddFrameWindow::PrevFrame()
{
	if (m_CurrentClip == nullptr)
	{
		return;
	}
	if (m_CurrentIndex > 0)
	{
		m_CurrentIndex--;
	}
	else if (m_CurrentIndex ==0)
	{
		m_CurrentIndex = (unsigned int)m_CurrentClip->m_SourceRects.size() - 1;
	}
}

void AddFrameWindow::NextFrame()
{

	if (m_CurrentClip == nullptr)
	{
		return;
	}
	//-1 since index minus one
	if (m_CurrentIndex < m_CurrentClip->m_SourceRects.size()-1)
	{
		m_CurrentIndex++;
	}
	else
	{
		m_CurrentIndex = 0;
	}
}

void AddFrameWindow::ToggleSelect()
{
	m_SelectingMode = m_SelectingMode ? false : true;
	m_Selecting = false;
	m_DragMode = false;

	if (m_SelectingMode)
	{
		m_Info.SetText("Click and hold left mouse button to create a box, press select again to disable ");
	}
	else
	{
		m_Info.SetText(" ");
	}
}

void AddFrameWindow::ToggleDrag()
{
	m_DragMode = m_DragMode ? false : true;
	m_DragginSelection = false;
	m_SelectingMode = false;
	if (m_DragMode)
	{
		m_Info.SetText("Click on the selection box to drag it, press drag again to disable ");
	}
	else
	{
		m_Info.SetText(" ");
	}
}

void AddFrameWindow::Update(float _dt)
{
	EditorWindow::Update(_dt);
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->Update(_dt);
	}
	if (m_CurrentClip != nullptr)
	{
		m_CurrentFrameField.SetText((m_ConstFrameText + std::to_string(m_CurrentIndex).data()));
	}
	UpdateSelectionBox();
}

void AddFrameWindow::MouseDown(unsigned int _key)
{
	EditorWindow::MouseDown(_key);
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseDown(_key);
	}

	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->MouseDown(_key);
	}
	if (_key == 1&&!m_InContentBox)
	{
		if (m_SelectingMode)
		{
			m_Selecting = true;
			m_SelectStart = (m_MousePos+m_ViewPos);
		}
		if (m_DragMode)
		{
			//Check collision with the selection box
			if (Box::BoxCollision(m_SelectionBox, (m_MousePos+m_ViewPos)/m_ZoomVector.x))
			{
				m_DragginSelection = true;
				m_DragSelectionStart = (m_MousePos+m_ViewPos) / m_ZoomVector.x - m_SelectionBox.pos;
			}
		}
	}
}

void AddFrameWindow::MouseUp(unsigned int _key)
{
	EditorWindow::MouseUp(_key);

	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseUp(_key);
	}

	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->MouseUp(_key);
	}
	if (_key == 1 )
	{
		if (m_Selecting)
		{
			UpdateSelectionBox();
			m_Selecting = false;
		}
		if (m_DragginSelection)
		{
			m_DragginSelection = false;
		}
	}
}

void AddFrameWindow::MouseMove(unsigned int _x, unsigned int _y)
{
	EditorWindow::MouseMove(_x, _y);
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->MouseMove(_x, _y);
	}
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->MouseMove(_x, _y);
	}
	m_MousePos = {(float)_x,(float)_y};
	m_InContentBox = m_ContentBox.BoxCollision(m_ContentBox, m_MousePos);

	UpdateDragSelectionBox();
}

void AddFrameWindow::SetFont(TTF_Font* _font)
{
	EditorWindow::SetFont(_font);
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->SetFont(_font);
	}
	m_Info.SetFont(_font);

	m_CurrentFrameField.SetFont(_font);
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->SetFont(_font);
	}
}

void AddFrameWindow::KeyDown(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->KeyDown(_key);
	}
}

void AddFrameWindow::KeyUp(unsigned int _key)
{
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->KeyUp(_key);
	}
}

void AddFrameWindow::Render(SDLRenderer* _renderer)
{
	EditorWindow::Render(_renderer);

	Box zoomedBox=  m_SelectionBox;
	zoomedBox.pos.x *= m_ZoomVector.x;
	zoomedBox.pos.y *= m_ZoomVector.y;
	zoomedBox.w *= m_ZoomVector.x;
	zoomedBox.h*= m_ZoomVector.y;

	_renderer->DrawBox(zoomedBox, { 255,255,255,255 },m_ViewPos);
	_renderer->DrawFilledBox(m_ContentBox, m_Color, { 0,0 }, 0);

	if (m_CurrentClip != nullptr)
	{
		m_Preview.Render(_renderer, { 0,0 }, 1);
	}

	m_Info.Render(_renderer, { 0,0 }, 1);
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i]->Render(_renderer);
	}

	m_CurrentFrameField.Render(_renderer, { 0,0 }, 1);
	for (unsigned int i = 0; i < m_InputFields.size(); ++i)
	{
		m_InputFields[i]->Render(_renderer);
	}

}

void AddFrameWindow::SetClip(AnimationClip* _clip)
{
	m_CurrentClip = _clip;

	m_CurrentIndex = 0;

	m_Preview.m_RenderInterface.texture = _clip->m_SourceTexture;
}
