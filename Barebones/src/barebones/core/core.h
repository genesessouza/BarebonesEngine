#pragma once

#ifdef VS
	inline constexpr const char* LIT_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/lit_shader.shader";
	inline constexpr const char* UNLIT_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/unlit_shader.shader";
	inline constexpr const char* DEBUG_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/debug_shader.shader";
	inline constexpr const char* EXPERIMENTAL_SDF_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/sdf_sample_experimental.shader";
	inline constexpr const char* SDF_INTEGRATION_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/sdf_integration_sample.shader";
#else
	inline constexpr const char* LIT_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/lit_shader.shader";
	inline constexpr const char* UNLIT_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/unlit_shader.shader";
	inline constexpr const char* DEBUG_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/debug_shader.shader";
#endif