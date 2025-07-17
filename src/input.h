#ifndef INPUT_MODULE_H
#define INPUT_MODULE_H

#include "vec2.h"

namespace input{

	static std::vector<bool> buttonDigit(10, false);
	static bool buttonBackspace = false;
	static bool buttonRightBracket = false;
	static bool buttonLeftBracket = false;
	static bool buttonMinus = false;
	static bool buttonRight = false;
	static bool buttonLeft = false;
	static bool buttonUp = false;
	static bool buttonDown = false;
	static int forward = 0;
	static int side = 0;
	static int vertical = 0;
	static int sideArrows = 0;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);

		if (key == GLFW_KEY_0)
			if (action == GLFW_PRESS)
				buttonDigit[0] = true;
			else if (action == GLFW_RELEASE)
				buttonDigit[0] = false;

		if (key == GLFW_KEY_1)
			if (action == GLFW_PRESS)
				buttonDigit[1] = true;
			else if (action == GLFW_RELEASE)
				buttonDigit[2] = false;

		if (key == GLFW_KEY_2)
			if (action == GLFW_PRESS)
				buttonDigit[2] = true;
			else if (action == GLFW_RELEASE)
				buttonDigit[2] = false;

		if (key == GLFW_KEY_3)
			if (action == GLFW_PRESS)
				buttonDigit[3] = true;
			else if (action == GLFW_RELEASE)
				buttonDigit[3] = false;

		if (key == GLFW_KEY_4)
			if (action == GLFW_PRESS)
				buttonDigit[4] = true;
			else if (action == GLFW_RELEASE)
				buttonDigit[4] = false;

		if (key == GLFW_KEY_5)
			if (action == GLFW_PRESS)
				buttonDigit[5] = true;
			else if (action == GLFW_RELEASE)
				buttonDigit[5] = false;

		if (key == GLFW_KEY_6)
			if (action == GLFW_PRESS)
				buttonDigit[6] = true;
			else if (action == GLFW_RELEASE)
				buttonDigit[6] = false;

		if (key == GLFW_KEY_7)
			if (action == GLFW_PRESS)
				buttonDigit[7] = true;
			else if (action == GLFW_RELEASE)
				buttonDigit[7] = false;

		if (key == GLFW_KEY_8)
			if (action == GLFW_PRESS)
				buttonDigit[8] = true;
			else if (action == GLFW_RELEASE)
				buttonDigit[8] = false;

		if (key == GLFW_KEY_9)
			if (action == GLFW_PRESS)
				buttonDigit[9] = true;
			else if (action == GLFW_RELEASE)
				buttonDigit[9] = false;

		if (key == GLFW_KEY_BACKSPACE)
			if (action == GLFW_PRESS)
				buttonBackspace = true;
			else if (action == GLFW_RELEASE)
				buttonBackspace = false;

		if (key == GLFW_KEY_RIGHT)
			if (action == GLFW_PRESS)
				buttonRight = true;
			else if (action == GLFW_RELEASE)
				buttonRight = false;

		if (key == GLFW_KEY_LEFT)
			if (action == GLFW_PRESS)
				buttonLeft = true;
			else if (action == GLFW_RELEASE)
				buttonLeft = false;

		if (key == GLFW_KEY_UP)
			if (action == GLFW_PRESS)
				buttonUp = true;
			else if (action == GLFW_RELEASE)
				buttonUp = false;

		if (key == GLFW_KEY_DOWN)
			if (action == GLFW_PRESS)
				buttonDown = true;
			else if (action == GLFW_RELEASE)
				buttonDown = false;

		if (key == GLFW_KEY_MINUS)
			if (action == GLFW_PRESS)
				buttonMinus = true;
			else if (action == GLFW_RELEASE)
				buttonMinus = false;

		if (key == GLFW_KEY_LEFT_BRACKET)
			if (action == GLFW_PRESS)
				buttonLeftBracket = true;
			else if (action == GLFW_RELEASE)
				buttonLeftBracket = false;

		if (key == GLFW_KEY_RIGHT_BRACKET)
			if (action == GLFW_PRESS)
				buttonRightBracket = true;
			else if (action == GLFW_RELEASE)
				buttonRightBracket = false;

		//forward
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
			forward += 1;
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
			forward -= 1;
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
			forward -= 1;
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
			forward += 1;

		//side
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
			side -= 1;
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
			side += 1;
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
			side += 1;
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
			side -= 1;

		//vertical
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
			vertical += 1;
		if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
			vertical -= 1;
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
			vertical -= 1;
		if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE)
			vertical += 1;
		
		//side
		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
			sideArrows += 1;
		if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
			sideArrows -= 1;
		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
			sideArrows -= 1;
		if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
			sideArrows += 1;
	}

	static float currentX = 0;
	static float currentY = 0;
	static float screen_scale = 1.0;

	static bool is_resolution_changed = false;

	static int window_width = 1000;
	static int window_height = 1000;

	static double mouse_cursor_x = 0;
	static double mouse_cursor_y = 0;
	static bool mouse_button_right = false;
	static bool mouse_button_left = false;

	static bool mouse_button_left_click = false;
	static bool mouse_button_right_click = false;
	static bool mouse_button_left_double_click = false;
	static bool mouse_button_right_double_click = false;
	static bool mouse_button_left_hold = false;
	static bool mouse_button_right_hold = false;

	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
	    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
			mouse_button_left = true;
	    }
	    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
			mouse_button_left = false;
	    }
	    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS){
			mouse_button_right = true;
	    }
	    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE){
			mouse_button_right = false;
	    }
	}

	static void mouse_cursor_callback(GLFWwindow* window, double xPos, double yPos)
	{
		mouse_cursor_x = xPos;
		mouse_cursor_y = yPos;
	}

	static double mouse_scroll_y = 0;

	static void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
	{
		mouse_scroll_y = yOffset;
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		window_width = width;
		window_height = height;
	    glViewport(0, 0, width, height);
	    is_resolution_changed = true;
	}

	bool is_mouse_in_cell(double _mouse_x, double _mouse_y, 
		const double cell_x0, const double cell_y0, const double cell_w, const double cell_h)
	{
		//_mouse_x = _mouse_x + currentX * 0.5f * window_width;
		//_mouse_y = _mouse_y - currentY * 0.5f * window_height;
		return (_mouse_x > cell_x0 && _mouse_x < cell_x0 + cell_w && _mouse_y > cell_y0 && _mouse_y < cell_y0 + cell_h);
	}

	bool is_mouse_in_button(int x, int y){
		return is_mouse_in_cell(mouse_cursor_x, mouse_cursor_y, window_width * 0.2 * (x - 1), window_height * 0.075 * (y - 1), 
			window_width * 0.2, window_height * 0.075);
	}

	gifi47::vec2 mouse_to_viewport(const double mouse_x, const double mouse_y){
		return gifi47::vec2((mouse_x / window_width) * 2.0f - 1.0f + currentX, (window_height - mouse_y) / window_height * 2.0f - 1.0f + currentY);
	}

	gifi47::vec2 mouse_to_viewport_const(const double mouse_x, const double mouse_y){
		return gifi47::vec2((mouse_x / window_width) * 2.0f - 1.0f, (window_height - mouse_y) / window_height * 2.0f - 1.0f);
	}

	template <typename T>
	bool is_point_in_vertex(gifi47::vec2 point, T vert){
		return gifi47::vec2::distance(point, vert.GetPosition()) < vert.GetScale() * 0.5f;
	}

	bool in_rect(gifi47::vec2 point, gifi47::vec2 p1, gifi47::vec2 p2, float height){
		gifi47::vec2 t1 = point - p1;
		gifi47::vec2 t2 = p2 - p1;
		gifi47::vec2 t3 = point - p2;
		float dotProduct = gifi47::vec2::dot(t1, t2);
		if (dotProduct < 0 || (gifi47::vec2::dot(t3, t2) > 0)){
			return false;
		}
		gifi47::vec2 n_n = t2 * dotProduct;
		gifi47::vec2 proj = p1 + n_n / std::pow(t2.length(), 2);
		return gifi47::vec2::distance(point, proj) < height;
	}

	bool in_rect(gifi47::vec2 point, gifi47::vec2 center, float width, float height){
		gifi47::vec2 p1 = center + gifi47::vec2(-width * 0.25f, -height * 0.5f);
		gifi47::vec2 p2 = center + gifi47::vec2(width * 0.75f, -height * 0.5f);
		gifi47::vec2 p3 = center + gifi47::vec2(-width * 0.25f, height * 0.5f);
		
		gifi47::vec2 t1 = point - p1;
		gifi47::vec2 t2 = p2 - p1;
		gifi47::vec2 t3 = point - p2;

		gifi47::vec2 t4 = p3 - p1;
		gifi47::vec2 t5 = point - p3;

		return !(gifi47::vec2::dot(t1, t2) < 0 
			|| gifi47::vec2::dot(t3, t2) > 0
			|| gifi47::vec2::dot(t1, t4) < 0
			|| gifi47::vec2::dot(t5, t4) > 0);
	}

}

#endif //INPUT_MODULE_H