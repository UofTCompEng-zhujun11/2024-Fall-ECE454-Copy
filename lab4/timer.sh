#!/bin/bash

# Output directory for timing results
OUTPUT_DIR="timing_results"
mkdir -p $OUTPUT_DIR

# Reset timings.log at the start of the script
TIMINGS_LOG="$OUTPUT_DIR/timings.log"
echo "Executable,Threads,SamplesToSkip,Time(s)" > $TIMINGS_LOG

# Executables
EXECUTABLES=("randtrack" "randtrack_global_lock" "randtrack_list_lock" "randtrack_element_lock" "randtrack_tm" "randtrack_reduction")

# Samples to skip
SAMPLES_TO_SKIP=100

# Thread counts for parallel versions
THREAD_COUNTS=(1 2 4)

# Timing function
run_experiment() {
  local executable=$1
  local num_threads=$2
  local samples_to_skip=$3
  local output_file="$OUTPUT_DIR/${executable}_threads${num_threads}_skip${samples_to_skip}.txt"

  # Measure runtime using /usr/bin/time
  local runtime
  if [ "$executable" == "randtrack" ]; then
    echo "Running $executable (single-threaded)..."
    runtime=$(/usr/bin/time -f "%e" ./build/$executable $num_threads $samples_to_skip > $output_file 2>> "$OUTPUT_DIR/timings.log")
    echo "$executable,1,$samples_to_skip,$runtime" >> $TIMINGS_LOG
  else
    echo "Running $executable with $num_threads threads..."
    runtime=$(/usr/bin/time -f "%e" ./build/$executable $num_threads $samples_to_skip > $output_file 2>> "$OUTPUT_DIR/timings.log")
    echo "$executable,$num_threads,$samples_to_skip,$runtime" >> $TIMINGS_LOG
  fi
}

# Run experiments
echo "Starting timing experiments..."
for executable in "${EXECUTABLES[@]}"; do
  if [ "$executable" == "randtrack" ]; then
    # Original randtrack doesn't use num_threads
    run_experiment $executable 1 $SAMPLES_TO_SKIP
  else
    # Parallel versions with different thread counts
    for num_threads in "${THREAD_COUNTS[@]}"; do
      run_experiment $executable $num_threads $SAMPLES_TO_SKIP
    done
  fi
done

echo "Timing experiments completed. Consolidated timings saved in $TIMINGS_LOG."
