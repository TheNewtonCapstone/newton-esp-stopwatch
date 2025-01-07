#include "newton/stopwatch.h"
#include "newton/result.h"
#include <esp_timer.h>

using namespace newton;

Stopwatch::Stopwatch(const char *name) : m_name(name), m_state(StopwatchState::DISABLED), m_start_time(0), m_lap_start_times()
{
}

Stopwatch::~Stopwatch()
{
}

void Stopwatch::start()
{
    if (m_state == StopwatchState::RUNNING)
        return;

    m_lap_start_times.clear();
    m_start_time = esp_timer_get_time();
    m_state = StopwatchState::RUNNING;
}

void Stopwatch::lap()
{
    if (m_state == StopwatchState::DISABLED)
        return;

    if (m_state == StopwatchState::STOPPED)
        return;

    uint64_t current_time = esp_timer_get_time();
    m_lap_start_times.push_back(current_time);
}

void Stopwatch::stop()
{
    if (m_state == StopwatchState::DISABLED)
        return;

    if (m_state == StopwatchState::STOPPED)
        return;

    uint64_t current_time = esp_timer_get_time();
    m_lap_start_times.push_back(current_time);
    m_state = StopwatchState::STOPPED;
}

void Stopwatch::reset()
{
    m_state = StopwatchState::DISABLED;
    m_start_time = 0;
    m_lap_start_times.clear();
}

const char *Stopwatch::get_name() const
{
    return m_name;
}

StopwatchState Stopwatch::get_state() const
{
    return m_state;
}

Result<uint64_t> Stopwatch::get_start_time(const StopwatchUnits units) const
{
    if (m_state == StopwatchState::DISABLED)
        return Result<uint64_t>::error("Stopwatch is disabled, please start it first!");

    return m_start_time / static_cast<uint64_t>(units);
}

Result<uint64_t> Stopwatch::get_end_time(const StopwatchUnits units) const
{
    if (m_state == StopwatchState::DISABLED)
        return Result<uint64_t>::error("Stopwatch is disabled, please start it first!");

    if (m_state == StopwatchState::RUNNING)
        return Result<uint64_t>::error("Stopwatch is running, please stop it first!");

    return m_lap_start_times.back() / static_cast<uint64_t>(units);
}

Result<uint64_t> Stopwatch::get_elapsed_time(const StopwatchUnits units) const
{
    if (m_state == StopwatchState::DISABLED)
        return Result<uint64_t>::error("Stopwatch is disabled, please start it first!");

    if (m_state == StopwatchState::RUNNING)
        return Result<uint64_t>::error("Stopwatch is running, please stop it first!");

    return (m_lap_start_times.back() - m_start_time) / static_cast<uint64_t>(units);
}

Result<uint64_t> Stopwatch::get_lap_duration(const size_t lap_idx, const StopwatchUnits units) const
{
    if (m_state == StopwatchState::DISABLED)
        return Result<uint64_t>::error("Stopwatch is disabled, please start it first!");

    if (m_state == StopwatchState::RUNNING)
        return Result<uint64_t>::error("Stopwatch is running, please stop it first!");

    if (lap_idx >= m_lap_start_times.size())
        return Result<uint64_t>::error("Lap index is out of bounds!");

    if (lap_idx == 0)
        return (m_lap_start_times[0] - m_start_time) / static_cast<uint64_t>(units);

    return (m_lap_start_times[lap_idx] - m_lap_start_times[lap_idx - 1]) / static_cast<uint64_t>(units);
}

Result<uint64_t> newton::Stopwatch::get_lap_start_time(const size_t lap_idx, const StopwatchUnits units) const
{
    if (m_state == StopwatchState::DISABLED)
        return Result<uint64_t>::error("Stopwatch is disabled, please start it first!");

    if (m_state == StopwatchState::RUNNING)
        return Result<uint64_t>::error("Stopwatch is running, please stop it first!");

    // We save the overall end time as the last lap start time, but we don't want to count it as a lap
    if (lap_idx >= m_lap_start_times.size() - 1)
        return Result<uint64_t>::error("Lap index is out of bounds!");

    if (lap_idx == 0)
        return m_start_time / static_cast<uint64_t>(units);

    return m_lap_start_times[lap_idx] / static_cast<uint64_t>(units);
}

size_t newton::Stopwatch::get_lap_count() const
{
    // We save the overall end time as the last lap start time, but we don't want to count it as a lap
    return std::max(m_lap_start_times.size() - 1, size_t(0));
}
