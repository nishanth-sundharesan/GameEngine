#pragma once

// Windows libraries
#include <windows.h>

// Standard libraries
#include <vector>
#include <cstdint>

// OpenGL dependencies
#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#pragma warning ( push )
#pragma warning ( disable: 4201 )	//Suppressing the warning "message nonstandard extension used : nameless struct / union" in the file "\glm\detail\type_vec2.hpp"
#include "Glm/glm.hpp"
#pragma warning ( pop )