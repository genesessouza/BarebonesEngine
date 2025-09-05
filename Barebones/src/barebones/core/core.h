#pragma once

#ifdef VS
	inline constexpr const char* LIT_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/lit_shader.shader";
	inline constexpr const char* UNLIT_SHADER_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/unlit_shader.shader";
	inline constexpr const char* GIZMO_UNLIT_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/gizmo_unlit.shader";
	inline constexpr const char* WIRE_UNLIT_SOURCE = "../Barebones/src/barebones/rendering/shader/sources/wire_unlit.shader";
#else
	inline constexpr auto LIT_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/lit_shader.shader";
	inline constexpr auto UNLIT_SHADER_SOURCE = "../../../Barebones/src/barebones/rendering/shader/sources/unlit_shader.shader";
#endif