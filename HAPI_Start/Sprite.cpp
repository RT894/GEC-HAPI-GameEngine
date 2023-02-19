#include "Sprite.h"
#include "Vector3.h"
#include "Rectangle.h"

Sprite::Sprite(int aw, int ah, bool i, std::string s, int w, int h, bool t)
{
	//Constructs the basic elements of the sprite
	//Kills the program if the texture fails to load
	m_textureWidth = w;
	m_textureHeight = h;
	m_hasTransparency = t;
	m_isAnimation = i;

	if (m_isAnimation) {
		m_sheetWidth = aw;
		m_sheetHeight = ah;
	}
	else {
		m_sheetWidth = w;
		m_sheetHeight = h;
		m_currentFrame = 0;
	}

	m_frameCount = m_sheetWidth / m_textureWidth;
	
	if (!m_isAnimation) {
		if (!HAPI.LoadTexture(s, &m_texture, w, h)) {
			//No load
			HAPI.UserMessage("Texture Failed To Load", "ERROR");
			HAPI.Close();
		}
	}
	else {
		if (!HAPI.LoadTexture(s, &m_texture, aw, ah)) {
			//No load
			HAPI.UserMessage("Texture Failed To Load", "ERROR");
			HAPI.Close();
		}
	}
	
}

Sprite::~Sprite()
{
	delete[] m_texture;
}

void Sprite::StepAnimation()
{
	//Increments the animation one frame forward
	//Loops back to the first frame if m_currentFrame grows too big
	//Useful for simple looping animations
	if (m_isAnimation == false) {
		return;
	}

	if (m_currentFrame >= m_frameCount - 1) {
		m_currentFrame = 0;
	}
	else {
		m_currentFrame++;
	}
}

size_t Sprite::GetTextureStepOffset()
{
	if (m_isAnimation) {
		return 0;
	}

	return ((size_t)m_sheetWidth - m_textureWidth) * 4;
}

void Sprite::SetFrame(int i)
{
	//Sets current frame of the animation.
	//Defaults to either 0 or the max frame if the value is too big or small
	if (i >= m_frameCount || i < 0) {
		return;
	}
	else {
		m_currentFrame = i;
	}
}

void Sprite::Render(const Vector3& p, int f, BYTE*& s, int sw, int sh)
{
	SetFrame(f);

	//Uses rectangles to figure out how much of the sprite should be rendered on screen
	Rectangle screenRect(0, sw, 0, sh);
	Rectangle textureRect(0, m_textureWidth, 0, m_textureHeight);
	textureRect.Translate(p);
	Rectangle clippedRect = textureRect;
	clippedRect.Clip(screenRect);
	Vector3 invertedP = p;
	invertedP.Invert();
	clippedRect.Translate(invertedP);

	


	//Generic Sprite Rendering Here
	if (clippedRect.m_right - clippedRect.m_left <= 0 || clippedRect.m_bottom - clippedRect.m_top <= 0) {
		return;
	}

	//Creates pointers to the texture and the first pixel in the screen to render to
	int screenPositionToPointTo = (int)((std::roundf(p.GetX()) + std::roundf(p.GetY()) * sw) * 4.0f);
	BYTE* screenPointer = s + screenPositionToPointTo;
	BYTE* texturePointer = m_texture;

	//Uses memcpy to blit line by line if there is no transparency and blits pixel by pixel if there is
	if (m_hasTransparency == false) {
		if (clippedRect.m_right - clippedRect.m_left > 0) {
			//Need to get the difference between the clipped rect and the actual width of the sprite
			//Use that as offset for the memcpy
			int startOffset = (sw * clippedRect.m_top) * 4;
			int endOffset = (m_textureWidth - (textureRect.m_right - clippedRect.m_right)) * 4;
			screenPointer += (size_t)startOffset;
			texturePointer += (size_t)clippedRect.m_top * m_textureWidth * 4;
			screenPointer += (size_t)clippedRect.m_left * 4;
			texturePointer += (size_t)clippedRect.m_left * 4;
			for (int i = clippedRect.m_top; i < clippedRect.m_bottom; i++)
			{
				std::memcpy(screenPointer, texturePointer, ((size_t)clippedRect.m_right - clippedRect.m_left) * 4);

				if (m_isAnimation == true) {
					texturePointer += ((size_t)m_sheetWidth - m_textureWidth) * 4;
				}
				else {
					texturePointer += (size_t)m_textureWidth * 4;
				}



				screenPointer += (size_t)sw * 4;
			}
		}
	}
	else {
		//Calculates an offset to add to the pointer when the end of a row is reached
		int lineEndIncrement = (int)(sw - m_textureWidth) * 4;
		screenPointer += (size_t)clippedRect.m_top * sw * 4;
		texturePointer += (size_t)clippedRect.m_top * m_textureWidth * 4;
		screenPointer += (size_t)clippedRect.m_left * 4;
		texturePointer += (size_t)clippedRect.m_left * 4;
		texturePointer += ((size_t)m_currentFrame * m_textureWidth) * 4;

		for (int y = clippedRect.m_top; y < clippedRect.m_bottom; y++)
		{
			for (int x = clippedRect.m_left; x < clippedRect.m_right; x++)
			{
				//Fetches the alpha of the current pixel
				BYTE alpha = texturePointer[3];

				//If the alpha is 0, nothing is rendered and the pointers move to the next pixel
				if (alpha == 0) {
					texturePointer += 4;
					screenPointer += 4;
					continue;
				}

				//Fetches the rgb values
				BYTE red = texturePointer[0];
				BYTE green = texturePointer[1];
				BYTE blue = texturePointer[2];

				//Directly copies the rgb values if the pixel is fully opaque
				if (alpha == 255.0f) {
					screenPointer[0] = red;
					screenPointer[1] = green;
					screenPointer[2] = blue;
				}
				else {
					//Otherwise, a midpoint is found between the rgb of the background pixel and the sprite pixel
					//This midpoint is then rendered on the screen
					float mod = alpha / 255.0f;

					screenPointer[0] = (BYTE)(mod * red + (1.0f - mod) * screenPointer[0]);
					screenPointer[1] = (BYTE)(mod * green + (1.0f - mod) * screenPointer[1]);
					screenPointer[2] = (BYTE)(mod * blue + (1.0f - mod) * screenPointer[2]);
				}

				texturePointer += 4;
				screenPointer += 4;
			}

			//Special offset to account for if a texture is an animation
			if (m_isAnimation == true) {
				texturePointer += ((size_t)m_sheetWidth - m_textureWidth) * 4;
			}

			//Regular offsets based on dimensions of the screen and the clipping
			screenPointer += (size_t)lineEndIncrement;
			screenPointer += (size_t)clippedRect.m_left * 4;
			texturePointer += (size_t)clippedRect.m_left * 4;

			int widthDiff = m_textureWidth - clippedRect.m_right;
			screenPointer += (size_t)widthDiff * 4;
			texturePointer += (size_t)widthDiff * 4;
		}
	}

	return;
}
