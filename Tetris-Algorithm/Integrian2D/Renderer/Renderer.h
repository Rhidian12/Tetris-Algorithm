#pragma once

#include "../Integrian2D_API.h"
#include "../Math/TypeDefines.h"
#include "OpenGLInformation/OpenGLInformation.h"

#include <vector>

struct SDL_Window;
namespace Integrian2D
{
	class Texture;

	/* This class renders Textures to the Window and can draw polygons to the Window */
	class Renderer final
	{
	public:
		/* Interal Usage
		   Do NOT call this function manually */
		static void CreateRenderer(SDL_Window* const pWindow) noexcept;

		/* Return an instance of the Renderer */
		INTEGRIAN2D_API static Renderer* GetInstance() noexcept;

		/* Internal Usage
		   Do NOT call this function manually */
		static void Cleanup() noexcept;

		/* Queue a Texture to be Rendered */
		INTEGRIAN2D_API void RenderTexture(Texture* const pTexture, const PRectf& destRect, const Rectf& sourceRect) noexcept;

		/* Render all queued Textures
		   Do NOT call this function manually, this gets called internally */
		void Render() noexcept;

		/* Internal Usage
		   Do NOT call this function manually, this gets called internally */
		void SetNewFrame() noexcept;

		/* Render an empty rectangle to the screen */
		INTEGRIAN2D_API void RenderRectangle(const Rectf& rect, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;

		/* Render an empty to the screen */
		INTEGRIAN2D_API void RenderRectangle(const PRectf& rect, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;

		/* Render a filled rectangle to the screen */
		INTEGRIAN2D_API void RenderFilledRectangle(const Rectf& rect, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;

		/* Render a filled rectangle to the screen */
		INTEGRIAN2D_API void RenderFilledRectangle(const PRectf& rect, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;

		/* Render an empty triangle to the screen */
		INTEGRIAN2D_API void RenderTriangle(const PTrianglef& triangle, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;

		/* Render an empty triangle to the screen */
		INTEGRIAN2D_API void RenderTriangle(const Trianglef& triangle, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;

		/* Render a filled triangle to the screen */
		INTEGRIAN2D_API void RenderFilledTriangle(const PTrianglef& triangle, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;

		/* Render a filled triangle to the screen */
		INTEGRIAN2D_API void RenderFilledTriangle(const Trianglef& triangle, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;
		
		/* Render a line to the screen */
		INTEGRIAN2D_API void RenderLine(const PLinef& edge, const float lineWidth, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;

		/* Render a line to the screen */
		INTEGRIAN2D_API void RenderLine(const Point2f& start, const Point2f& end, const float lineWidth, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;

		/* Render a circle to the screen */
		INTEGRIAN2D_API void RenderCircle(const Circlef& circle, const RGBColour& colour = RGBColour{ 255, 0, 0 }) noexcept;

		/* Render a filled circle to the screen */
		INTEGRIAN2D_API void RenderFilledCircle(const Circlef& circle, const RGBColour& colour = RGBColour{ 255, 0, 0 }) noexcept;

		/* Render a polygon to the screen */
		INTEGRIAN2D_API void RenderPolygon(const std::vector<Point2f>& points, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;

		/* Render a filled polygon to the screen */
		INTEGRIAN2D_API void RenderFilledPolygon(const std::vector<Point2f>& points, const RGBColour& colour = RGBColour{ 255,0,0 }) noexcept;

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) = delete;

	private:
		struct TextureInformation final
		{
			Texture* pTexture;
			PRectf destRect;
			Rectf sourceRect;
		};

		Renderer(SDL_Window* const pWindow);

		void StartRenderLoop() noexcept;
		void RenderAllTextures() noexcept;
		void EndRenderLoop() noexcept;

		void InternalRenderRectangle(const PRectf& rect, const RGBColour& colour, const bool isFilled) noexcept;
		void InternalRenderTriangle(const PTrianglef& triangle, const RGBColour& colour, const bool isFilled) noexcept;
		void InternalRenderLine(const PLinef& edge, const float lineWidth, const RGBColour& colour) noexcept;
		void InternalRenderCircle(const Circlef& circle, const RGBColour& colour, const bool isFilled) noexcept;
		void InternalRenderPolygon(const std::vector<Point2f>& points, const RGBColour& colour, const bool isFilled) noexcept;

		inline static Renderer* m_pInstance{};

		bool m_IsNewFrame;

		RGBColour m_ClearColour;
		std::vector<TextureInformation> m_TexturesToRender;
		std::vector<OpenGLRectangle> m_RectanglesToRender;
		std::vector<OpenGLTriangle> m_TrianglesToRender;
		std::vector<OpenGLLine> m_LinesToRender;
		std::vector<OpenGLCircle> m_CirclesToRender;
		std::vector<OpenGLPolygon> m_PolygonsToRender;

		SDL_Window* const m_pWindow;
	};
}

