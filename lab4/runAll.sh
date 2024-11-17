#!/bin/bash

# Variables
DEFAULT_EXEC="./build/randtrack"  # Path to the default executable
EXECUTABLES=("randtrack_global_lock" "randtrack_list_lock" "randtrack_element_lock" "randtrack_reduction" "randtrack_tm")
OUTPUT_DIR="outputs"  # Directory to store output files
NUM_THREADS=4         # Number of threads to use
SAMPLES_TO_SKIP=50    # Samples to skip

# Create output directory if it doesn't exist
mkdir -p $OUTPUT_DIR

# Run the default executable
DEFAULT_OUTPUT="$OUTPUT_DIR/default_output.txt"
DEFAULT_SORTED="$OUTPUT_DIR/default_output_sorted.txt"

echo "Running default executable..."
$DEFAULT_EXEC $NUM_THREADS $SAMPLES_TO_SKIP > $DEFAULT_OUTPUT
sort -n $DEFAULT_OUTPUT > $DEFAULT_SORTED

# Run each executable and compare outputs
for EXEC in "${EXECUTABLES[@]}"; do
  EXEC_PATH="./build/$EXEC"  # Path to the executable
  OUTPUT_FILE="$OUTPUT_DIR/${EXEC}_output.txt"
  SORTED_FILE="$OUTPUT_DIR/${EXEC}_output_sorted.txt"
  DIFF_FILE="$OUTPUT_DIR/${EXEC}_diff.txt"

  if [[ -f $EXEC_PATH ]]; then
    echo "Running $EXEC..."
    $EXEC_PATH $NUM_THREADS $SAMPLES_TO_SKIP > $OUTPUT_FILE
    sort -n $OUTPUT_FILE > $SORTED_FILE

    diff $DEFAULT_SORTED $SORTED_FILE > $DIFF_FILE

    if [[ -s $DIFF_FILE ]]; then
      echo "Differences found for $EXEC. See $DIFF_FILE."
    else
      echo "No differences found for $EXEC."
      rm $DIFF_FILE  # Clean up empty diff files
    fi
  else
    echo "Executable $EXEC_PATH not found. Skipping."
  fi
done

echo "All comparisons complete. Outputs and diffs saved in $OUTPUT_DIR."
