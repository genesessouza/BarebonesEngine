#pragma once

#ifdef VS
	inline constexpr const char* LIT_SHADER_SOURCE			= "../Barebones/src/barebones/rendering/shader/sources/lit_shader.shader";
	inline constexpr const char* UNLIT_SHADER_SOURCE		= "../Barebones/src/barebones/rendering/shader/sources/unlit_shader.shader";
	inline constexpr const char* DEPTH_SHADER_SOURCE		= "../Barebones/src/barebones/rendering/shader/sources/depth_shader.shader";
	inline constexpr const char* DEPTH_DEBUG_SHADER_SOURCE	= "../Barebones/src/barebones/rendering/shader/sources/depth_debug_shader.shader";
	inline constexpr const char* DEBUG_SHADER_SOURCE		= "../Barebones/src/barebones/rendering/shader/sources/debug_shader.shader";
#else
	inline constexpr const char* LIT_SHADER_SOURCE			= "shader/lit_shader.shader";
	inline constexpr const char* UNLIT_SHADER_SOURCE		= "shader/unlit_shader.shader";
	inline constexpr const char* DEPTH_SHADER_SOURCE		= "shader/depth_shader.shader";
	inline constexpr const char* DEPTH_DEBUG_SHADER_SOURCE	= "shader/depth_debug_shader.shader";
	inline constexpr const char* DEBUG_SHADER_SOURCE		= "shader/debug_shader.shader";
#endif