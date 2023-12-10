#ifndef TREE_GENERATOR_OPENGL_WINDOW_H_
#define TREE_GENERATOR_OPENGL_WINDOW_H_

#include <functional>
#include <tuple>

#include "../common/key_action.h"
#include "../common/key_token.h"
#include "../common/window.h"

struct GLFWwindow;

namespace tree_generator
{
	namespace opengl
	{
		class OpenGLWindow : public Window
		{
		public:
			OpenGLWindow(int width, int height, const std::string& title);
			~OpenGLWindow() override;

			int Width() const override { return width_; }
			int Height() const override { return height_; }

			void Display(RenderCallback renderCallback) override;
			void SetKeyboardCallback(KeyboardCallback keyboardCallback) override;

			void SendKeyboardEvent(KeyToken token, KeyAction action);

		private:
			int width_;
			int height_;

			GLFWwindow* internalWindow_;
			KeyboardCallback keyboardCallback_;
		};
	}
}

#endif  // !TREE_GENERATOR_OPENGL_WINDOW_H_