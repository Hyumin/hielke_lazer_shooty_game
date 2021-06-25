#include "Text.h"
#include <cmath>


Text::Text()
{
	m_Texture = NULL;
	m_Colour.a = 0xff;
	m_Colour.b = 0xaa;
	m_Colour.g = 0xaa;
	m_Colour.r = 0xaa;
	m_Text = "Not set text lol! ";
	m_FontPointer = NULL;

}

Text::~Text()
{
	SDL_DestroyTexture(m_Texture);
	m_Texture = NULL;
}


void Text::UpdateInterface(SDLRenderer* _renderer)
{
	//If we're dealing with text that has nothing in it  just return
	if (m_Text.length() == 0)
	{
		return;
	}
	//Delete old texture if any
	if (m_Texture != NULL)
	{
		SDL_DestroyTexture(m_Texture);
		m_Texture = NULL;
	}
	

	//Do stuff to update the surface
	SDL_Surface* mainSurface= SDL_CreateRGBSurface(0, (int)m_Size.x, (int)m_Size.y, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	

	if(WrapText(mainSurface,m_Text,0,0))
	{

		//Create texture from surface pixels
		m_Texture = SDL_CreateTextureFromSurface(_renderer->GetRenderer(), mainSurface);
		if (m_Texture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s \n", SDL_GetError());
		}
		else
		{
			m_SrcSize.x = (float)mainSurface->w;
			m_SrcSize.y = (float)mainSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(mainSurface);



	}
	else
	{
		SDL_DestroyTexture(m_Texture);
		m_Texture = nullptr;
	}

	m_Interface.texture = m_Texture;
	m_Interface.destRect.x =(int)m_pos.x;
	m_Interface.destRect.y =(int)m_pos.y;
	m_Interface.destRect.w =(int)m_SrcSize.x;
	m_Interface.destRect.h =(int)m_SrcSize.y;


	m_Interface.srcRect.x = 0;
	m_Interface.srcRect.y = 0;
	m_Interface.srcRect.w = (int)m_SrcSize.x;
	m_Interface.srcRect.h = (int)m_SrcSize.y;


	m_Interface.renderFlip = SDL_RendererFlip::SDL_FLIP_NONE;
	m_Interface.point = { 0,0 };
}

//this will edit the mainsurface and return true
bool  Text::WrapText(SDL_Surface* _Mainsurface, std::string _remainingText, float _sizePerCharacter, int _ypos)
{

	if (_Mainsurface->w <= 0 && _Mainsurface->h <= 0)
	{
		return false;
	}
	float sizePerChar = _sizePerCharacter;

	//Create a surface from the text
	SDL_Surface* textSurface = TTF_RenderText_Solid(m_FontPointer, _remainingText.data(), m_Colour);
	if (textSurface == NULL)
	{
		//printf("Unable to render text surface! SDL_ttf Error: %s \n", TTF_GetError());
		return false;
	}
	if (sizePerChar == 0)
	{
		//rough estimate of the size per char by dividing the text surface's width by the size
		// of the string
		sizePerChar = std::ceilf(textSurface->w / _remainingText.size());
	}


	int height = textSurface->h;
	SDL_Rect destRect= textSurface->clip_rect;
	destRect.y = height * _ypos;

	//does it fit in the main surface?
	if (textSurface->w > _Mainsurface->w&&sizePerChar<_Mainsurface->w)
	{
		
		//Get rid of old surface
		SDL_FreeSurface(textSurface);
		//intended cutoff index, divide the surface by sizer per char to figure out how many
		//chars can fit inside along the width of the surface
		int cutoffIndex = (int)(_Mainsurface->w / sizePerChar);

		std::string buffer;
		buffer.assign(_remainingText, 0, cutoffIndex);

		if (cutoffIndex > 1)
		{
			//Check whether it is not a white space if so we want to go back in the string untill we
			//find a white space, if none can be found we simply continue like normal
			if (_remainingText[cutoffIndex] != ' ')
			{
				for (int i = cutoffIndex; i >= 0; --i)
				{
					if (_remainingText[i] == ' ')
					{
						cutoffIndex = i;
						break;
					}
				}
			}
			buffer.assign(_remainingText, 0, cutoffIndex);

			textSurface = TTF_RenderText_Solid(m_FontPointer, buffer.data(), m_Colour);
			//In case the text surface is still bigger than the width of our surface
			//recalculate the size per char,
			if (textSurface->w > _Mainsurface->w)
			{
				sizePerChar = std::ceilf(textSurface->w / buffer.size());
				//Figure out where we went wrong
				int estimatedLostVikings = std::floor((textSurface->w - _Mainsurface->w) / sizePerChar);
				if (estimatedLostVikings == 0)
				{
					estimatedLostVikings = 1;
				}
				cutoffIndex -= estimatedLostVikings;
				if (_remainingText[cutoffIndex] != ' ')
				{
					for (int i = cutoffIndex; i >= 0; --i)
					{
						if (_remainingText[i] == ' ')
						{
							cutoffIndex = i;
							break;
						}
					}
				}
				buffer.assign(_remainingText, 0, cutoffIndex);
				textSurface = TTF_RenderText_Solid(m_FontPointer, buffer.data(), m_Colour);

			}
			std::string remaining;
			//std::copy(_remainingText.begin() + cutoffIndex, _remainingText.end(), &remaining);
			remaining.assign(_remainingText, cutoffIndex , _remainingText.size());

			
			destRect = textSurface->clip_rect;
			destRect.y = height * _ypos;
			SDL_BlitSurface(textSurface, &textSurface->clip_rect, _Mainsurface, &destRect);

			//Free the surface after we've applied it to the mainsurface
			SDL_FreeSurface(textSurface);

			//Recursive untill textsurface width will no longer exceed mainsurface width aka everything should fit
			//TODO add check for height as well, either if height exceeds don't add more or something else
			return WrapText(_Mainsurface, remaining, sizePerChar, _ypos + 1);
		}
		else
		{
			return false;
		}
	}
	else
	{
		SDL_BlitSurface(textSurface, &textSurface->clip_rect, _Mainsurface, &destRect);

		SDL_FreeSurface(textSurface);
	}
	return true;
}



