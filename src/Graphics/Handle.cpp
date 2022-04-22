/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2022 Matthew McCall
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "york/Log.hpp"
#include "york/Graphics/Handle.hpp"

namespace york::graphics {

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

HandleBase::HandleBase(const HandleBase& other)
 : Identifiable(other) {
    for (HandleBase& dependency : other.m_dependencies) {
        addDependency(dependency);
    }
}

void HandleBase::create()
{
    this->destroy(); // m_created will be set false here

    for (HandleBase& dependency : m_dependencies) {
        if (!dependency.isCreated()) {
            dependency.create(); // however, this particular handle might be created as part of dependency resolution lower down in the stack
        }
    }

    if (!m_created) { // If this is the case, we want to prevent double creation once we move back up the stack
        m_created = this->createImpl();
    }

    for (auto i = m_dependents.begin(); i != m_dependents.end(); i++) { // Dependents may change as other dependents are created.
        if (!i->get().isCreated()) {
            i->get().create();
            i = m_dependents.begin();
        }
    }
}

void HandleBase::destroy()
{
    for (auto i = m_dependents.begin(); i != m_dependents.end(); i++) { // Dependents may change as other dependents are created.
        if (i->get().isCreated()) {
            i->get().destroy();
            i = m_dependents.begin();
        }
    }

    if (m_created) {
        this->destroyImpl();
    }

    m_created = false;
}

#pragma clang diagnostic pop

bool HandleBase::isCreated() const
{
    return m_created;
}

void HandleBase::addDependent(HandleBase& handle)
{
    m_dependents.emplace_back(handle);
}

void HandleBase::removeDependent(HandleBase& handle)
{
    for (auto i = m_dependents.begin(); i != m_dependents.end(); i++) {
        if (handle == *i) {
            m_dependents.erase(i);
            break;
        }
    }
}

void HandleBase::addDependency(HandleBase& handle)
{
    handle.addDependent(*this);
    m_dependencies.emplace_back(handle);
}

void HandleBase::removeDependency(HandleBase& handle)
{
    handle.removeDependent(*this);
    for (auto i = m_dependencies.begin(); i != m_dependencies.end(); i++) {
        if (handle == *i) {
            m_dependencies.erase(i);
            break;
        }
    }
}

HandleBase::~HandleBase()
{
    for (HandleBase& dependency : m_dependencies) {
        removeDependency(dependency);
    }
}

} // namespace york::graphics
