#include "..\Engine\TextField.h"

class SDLRenderer;

namespace Hielke
{
	class PlayerDebugInfo
	{
	public:
		PlayerDebugInfo();
		~PlayerDebugInfo();

		void InitTextFields(TTF_Font* _font);
		void CleanUp();//Cleansup any pointers this class initalizes
		void Update(float _dt);
		void Toggle();//toggles info on/off
		void Render(SDLRenderer* _renderer);

		SDL_Colour m_BackgroundColour = {0,0,0,0xff};
		SDL_Colour m_TextColour = {0xff,0xff,0xff,0xff};
		Box m_Background;

	private:

		bool m_Active = false;
		
		TextField m_pos, m_state, m_vel, m_anim;
		std::vector<TextField*> m_TextFields;

	};
}