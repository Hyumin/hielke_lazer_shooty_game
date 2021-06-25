#include "RenderTarget.h"
#include "Texture.h"
#include "ResourceManager.h"

RenderTarget::RenderTarget(SDL_Renderer* _rendererRef)
{
	m_RendererRef = _rendererRef;
	Init();
}

RenderTarget::RenderTarget()
{
	m_RendererRef = nullptr;
}

RenderTarget::~RenderTarget()
{
}

void RenderTarget::ClearQueues()
{
	m_RenderQueue.clear();
	m_TextRenderQueue.clear();
	m_LineQueue.clear();
	m_FilledBoxes.clear();
	m_WireFrameBoxes.clear();
}

void RenderTarget::AddInterface(RenderInterface& _interface)
{
	m_RenderQueue.push_back(_interface);
}

void RenderTarget::AddText(TextRenderInterface& _textInterface)
{
	m_TextRenderQueue.push_back(_textInterface);
}

void RenderTarget::AddLine(Line& _line)
{
	m_LineQueue.push_back(_line);
}

void RenderTarget::AddFilledBox(FilledBox& _filledBox)
{
	m_FilledBoxes.push_back(_filledBox);
}

void RenderTarget::AddBox(WireFrameBox& _filledBox)
{
	m_WireFrameBoxes.push_back(_filledBox);
}

void RenderTarget::Render(ResourceManager* _resman)
{
	if (m_Tex != nullptr)
	{
		//SDL_SetRenderDrawBlendMode(m_RendererRef, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(m_RendererRef, 0x00, 0xAB, 0xAB, 0x00);
		SDL_RenderClear(m_RendererRef);
	


		for (unsigned int i = 0; i < m_LineQueue.size(); ++i)
		{
			Line l = m_LineQueue[i];

			SDL_SetRenderDrawColor(m_RendererRef, l.colour.r, l.colour.g, l.colour.b, l.colour.a);
			SDL_RenderDrawLine(m_RendererRef, (int)l.start.x, (int)l.start.y, (int)l.end.x, (int)l.end.y);
		}
		for (unsigned int i = 0; i < m_FilledBoxes.size(); ++i)
		{
			SDL_SetRenderDrawColor(m_RendererRef, m_FilledBoxes[i].col.r, m_FilledBoxes[i].col.g, m_FilledBoxes[i].col.b, m_FilledBoxes[i].col.a);
			SDL_RenderFillRect(m_RendererRef, &m_FilledBoxes[i].box);
		}
		for (unsigned int i = 0; i < m_WireFrameBoxes.size(); ++i)
		{
			SDL_SetRenderDrawColor(m_RendererRef, m_WireFrameBoxes[i].col.r, m_WireFrameBoxes[i].col.g, m_WireFrameBoxes[i].col.b, m_WireFrameBoxes[i].col.a);
			SDL_Rect rect;
			rect.x = m_WireFrameBoxes[i].box.pos.x;
			rect.y = m_WireFrameBoxes[i].box.pos.y;
			rect.w = m_WireFrameBoxes[i].box.w;
			rect.h = m_WireFrameBoxes[i].box.h;
			SDL_RenderDrawRect(m_RendererRef, &rect);
		}
		//Iterate through the render queue
		for (unsigned int i = 0; i < m_RenderQueue.size(); ++i)
		{
			RenderInterface inter = m_RenderQueue[i];
			//Get texture based on string identifier within the _interface
			//Then copy it to the renderer
			Texture* tex = inter.texture;
			if (tex != nullptr)
			{
				//SDL_point is only used for SDL_rotate
				SDL_RenderCopyEx(m_RendererRef, tex->GetTexture(), &inter.srcRect, &inter.destRect, 0, 0, inter.renderFlip);
			}
		}
		for (unsigned int i = 0; i < m_TextRenderQueue.size(); ++i)
		{
			TextRenderInterface inter = m_TextRenderQueue[i];
			//Get texture based on string identifier within the _interface
			//Then copy it to the renderer
			const SDL_RendererFlip flip = inter.renderFlip;
			if (inter.texture != nullptr)
			{
				SDL_RenderCopyEx(m_RendererRef, inter.texture, &inter.srcRect, &inter.destRect, 0, 0, flip);
			}
		}
		ClearQueues();
	}
}

SDL_Rect RenderTarget::GetDestRect()
{
	return SDL_Rect{ (int)m_TargetBox.pos.x, (int)m_TargetBox.pos.y,(int)m_TargetBox.w,(int)m_TargetBox.h };
}

SDL_Texture* RenderTarget::GetTexture()
{
	return m_Tex;
}

void RenderTarget::CreateTexture(Box _tProp)
{
	if (m_Tex != nullptr)
	{
		//Destroy texture if shit happens
		SDL_DestroyTexture(m_Tex);
	}

	m_TargetBox = _tProp;

	m_Tex = SDL_CreateTexture(m_RendererRef, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _tProp.w, _tProp.h);
	SDL_SetTextureBlendMode(m_Tex, SDL_BLENDMODE_BLEND);
	if (m_Tex == nullptr)
	{
		//unsuccesfull
	}

}

void RenderTarget::Init()
{
	m_Tex = nullptr;
}
