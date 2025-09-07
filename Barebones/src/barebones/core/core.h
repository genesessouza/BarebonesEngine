#pragma once

#ifdef VS
	inline constexpr const char* LIT_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/lit_shader.shader";
	inline constexpr const char* UNLIT_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/unlit_shader.shader";
	inline constexpr const char* DEBUG_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/debug_shader.shader";
	inline constexpr const char* LIT_SDF_SAMPLE_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/lit_sdf_sample.shader";
#else
	inline constexpr const char* LIT_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/lit_shader.shader";
	inline constexpr const char* UNLIT_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/unlit_shader.shader";
	inline constexpr const char* DEBUG_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/debug_shader.shader";
#endif