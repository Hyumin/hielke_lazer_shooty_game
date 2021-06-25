#pragma once
#include "SDLRenderer.h"
#include "..\HielkMath.h"

//In essence this class wil be likes its own mini SDLRenderer,
// what differs is that instead of rendering to the renderer
//we render to a texture, and then we will get that texture
//and we will render that texture.
//and this is class is supposed to be used outside
 class RenderTarget
{
public:
	RenderTarget(SDL_Renderer* _rendererRef);
	RenderTarget();
	~RenderTarget();

	std::string& GetName() { return m_Name; }
	void SetName(std::string& _name) { m_Name = _name; }

	void ClearQueues();
	void AddInterface(RenderInterface& _interface);
	void AddText(TextRenderInterface& _textInterface);
	void AddLine(Line& _line);
	void AddFilledBox(FilledBox& _filledBox);
	void AddBox(WireFrameBox& _filledBox);
	void Render( ResourceManager* _resman);//Render the contents into the render target
	SDL_Rect GetDestRect();
	SDL_Texture* GetTexture();//Call this after rendering

	Box m_TargetBox;//We render to the proportions, and position of this box,
	
	void CreateTexture(Box _textureProportions);

private:
	void Init();
	SDL_Renderer* m_RendererRef;
	SDL_Texture* m_Tex;
	std::string m_Name;

	std::vector<RenderInterface> m_RenderQueue;
	std::vector<TextRenderInterface> m_TextRenderQueue;
	std::vector<Line> m_LineQueue;
	std::vector< FilledBox> m_FilledBoxes;
	std::vector<WireFrameBox> m_WireFrameBoxes;
};