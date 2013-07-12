/*
 * Copyright (C) 2011, 2013 Canonical, Ltd.
 *
 * Authors:
 *  Florian Boucault <florian.boucault@canonical.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Self
#include "radiooptionfilter.h"

RadioOptionFilter::RadioOptionFilter(QObject *parent) :
    Filter(parent), m_unityRadioOptionFilter(nullptr), m_options(nullptr)
{
}

FilterOptions* RadioOptionFilter::options() const
{
    return m_options;
}

FilterOption* RadioOptionFilter::getOption(const QString& id) const
{
    Q_FOREACH (FilterOption* option, m_options->rawList()) {
        if (option->id() == id) {
            return option;
        }
    }
    return nullptr;
}

void RadioOptionFilter::setUnityFilter(unity::dash::Filter::Ptr filter)
{
    Filter::setUnityFilter(filter);
    m_unityRadioOptionFilter = std::dynamic_pointer_cast<unity::dash::RadioOptionFilter>(m_unityFilter);

    onOptionsChanged(m_unityRadioOptionFilter->options);
}

void RadioOptionFilter::onOptionsChanged(unity::dash::RadioOptionFilter::RadioOptions)
{
    if (m_options != nullptr) {
        m_signals.disconnectAll();
        delete m_options;
        m_options = nullptr;
    }
    m_options = new FilterOptions(m_unityRadioOptionFilter->options,
                                 m_unityRadioOptionFilter->option_added,
                                 m_unityRadioOptionFilter->option_removed);
    /* Property change signals */
    m_signals << m_unityRadioOptionFilter->options.changed.connect(sigc::mem_fun(this, &RadioOptionFilter::onOptionsChanged));

    Q_EMIT optionsChanged();
}
