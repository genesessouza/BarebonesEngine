#pragma once

class timestep
{
public:
	timestep(float time = 0.0f)
		: m_time(time) {
	}

	inline operator float() const { return m_time; }
private:
	float m_time;
};