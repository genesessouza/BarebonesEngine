#pragma once

#ifdef VS
	inline constexpr const char* LIT_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/lit_shader.shader";
	inline constexpr const char* UNLIT_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/unlit_shader.shader";
	inline constexpr const char* CAMERA_DEPTH_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/camera_depth_shader.shader";
	inline constexpr const char* DEBUG_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/debug_shader.shader";
	inline constexpr const char* SDF_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/lit_shadow_shader.shader";
#else
	inline constexpr const char* LIT_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/lit_shader.shader";
	inline constexpr const char* UNLIT_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/unlit_shader.shader";
	inline constexpr const char* DEBUG_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/debug_shader.shader";
#endif