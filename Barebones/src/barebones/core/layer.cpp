#include "layer.h"

layer::layer(const std::string& name) : m_debug_name(name) {}

layer::~layer() {}

void layer::on_attach() {}

void layer::on_detach() {}

void layer::on_update(timestep timestep) {}

void layer::on_event(event& event) {}
