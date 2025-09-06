#pragma once

#ifdef VS
	inline constexpr const char* LIT_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/lit_shader.shader";
	inline constexpr const char* UNLIT_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/unlit_shader.shader";
	inline constexpr const char* DEBUG_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/debug_shader.shader";
#else
	inline constexpr auto LIT_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/lit_shader.shader";
	inline constexpr auto UNLIT_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/unlit_shader.shader";
	inline constexpr const char* DEBUG_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/debug_shader.shader";
#endif