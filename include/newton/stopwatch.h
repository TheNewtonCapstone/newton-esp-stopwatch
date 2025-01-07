#pragma once
#include <cstdint>
#include <vector>
#include "newton/result.h"

namespace newton
{
  enum class StopwatchState : uint8_t
  {
    DISABLED, // Stopwatch is not running and has never been started
    STOPPED,  // Stopwatch is not running but has been started before
    RUNNING,  // Stopwatch is running
  };

  enum class StopwatchUnits : uint32_t
  {
    SECONDS = 1'000'000,
    MILLISECONDS = 1'000,
    MICROSECONDS = 1,
  };

  /// @brief A stopwatch utility class for measuring time.
  class Stopwatch
  {
  public:
    /// @brief
    /// @param name Name given to the stopwatch (for user bookkeeping)
    Stopwatch(const char *name = "");
    ~Stopwatch();

    /// @brief Start the stopwatch.
    /// @note Calling this function while the stopwatch is running won't have any effect.
    void start();

    /// @brief Record a lap.
    /// @note Calling this function while the stopwatch is disabled or stopped won't have any effect.
    void lap();

    /// @brief Stop the stopwatch.
    /// @note Calling this function while the stopwatch is disabled or stopped won't have any effect.
    void stop();

    /// @brief Reset the stopwatch.
    /// @note Calling this function while the stopwatch is disabled won't have any effect. This function will reset the stopwatch to its initial state (`StopwatchState::DISABLED`). Can be called at any time and multiple times.
    void reset();

    const char *get_name() const;
    StopwatchState get_state() const;

    /// @brief Get the start time of the stopwatch.
    /// @param units The units in which the time is returned (in all cases, the value returned will be an unsigned integer).
    /// @return The start time of the stopwatch in the specified units.
    Result<uint64_t> get_start_time(const StopwatchUnits units = StopwatchUnits::MILLISECONDS) const;

    /// @brief Get the time the stopwatch was stopped.
    /// @param units The units in which the time is returned (in all cases, the value returned will be an unsigned integer).
    /// @return The time the stopwatch was stopped in the specified units.
    Result<uint64_t> get_end_time(const StopwatchUnits units = StopwatchUnits::MILLISECONDS) const;

    /// @brief Get the elapsed time since the stopwatch was started.
    /// @param units The units in which the time is returned (in all cases, the value returned will be an unsigned integer).
    /// @note If the stopwatch is running or is disabled, an errored result will be returned.
    /// @return The elapsed time since the stopwatch was started in the specified units.
    Result<uint64_t> get_elapsed_time(const StopwatchUnits units = StopwatchUnits::MILLISECONDS) const;

    /// @brief Get the duration of a lap.
    /// @param lap_idx The index of the lap to get the duration of.
    /// @param units The units in which the time is returned (in all cases, the value returned will be an unsigned integer).
    /// @note The stopwatch uses the last index of the laps to save the stop time; therefore, if querying at `get_lap_count() - 1` index, the duration between the last `lap()` call and the `stop()` call will be returned. If the duration of the last lap is desired, query at `get_lap_count() - 2`. If the stopwatch is running or is disabled, an errored result will be returned.
    /// @return The duration of the lap in the specified units.
    Result<uint64_t> get_lap_duration(const size_t lap_idx, const StopwatchUnits units = StopwatchUnits::MILLISECONDS) const;

    /// @brief Get the start time of a lap.
    /// @param lap_idx The index of the lap to get the start time of.
    /// @param units The units in which the time is returned (in all cases, the value returned will be an unsigned integer).
    /// @note If the stopwatch is running or is disabled, an errored result will be returned.
    Result<uint64_t> get_lap_start_time(const size_t lap_idx, const StopwatchUnits units = StopwatchUnits::MILLISECONDS) const;

    /// @brief Get the number of laps recorded.
    /// @return The number of laps recorded.
    size_t get_lap_count() const;

  private:
    const char *m_name;
    StopwatchState m_state;

    uint64_t m_start_time;
    std::vector<uint64_t> m_lap_start_times;
  };
}