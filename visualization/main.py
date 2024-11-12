import json
import matplotlib.pyplot as plt
import numpy as np
from typing import Literal
from pathlib import Path

def load_and_visualize(
    filepath: str | Path,
    x_axis: Literal["width_height", "iterations"],
    output_file: str | None = None
) -> None:
    """
    Load benchmark results from a JSON file and create a visualization.
    
    Args:
        filepath: Path to the JSON file
        x_axis: Which parameter to plot on x-axis ('width_height' or 'iterations')
        output_file: If provided, save plot to this file instead of displaying
    """
    # Load and parse JSON
    with open(filepath, 'r') as f:
        data = json.load(f)
    
    # Extract x values and durations
    x_values = []
    durations = []
    
    for benchmark in data['benchmarks']:
        params = benchmark['parameters']
        x_value = params[x_axis]
        
        # Calculate average duration across all jobs in this benchmark
        job_durations = [job['duration'] for job in benchmark['results']['results']]
        avg_duration = np.mean(job_durations)
        std_duration = np.std(job_durations)
        
        x_values.append(x_value)
        durations.append((avg_duration, std_duration))
    
    # Sort data points by x value
    sorted_data = sorted(zip(x_values, durations))
    x_values = [x for x, _ in sorted_data]
    avg_durations = [d[0] for _, d in sorted_data]
    std_durations = [d[1] for _, d in sorted_data]
    
    # Create the visualization
    plt.figure(figsize=(10, 6))
    
    # Plot average duration with error bars
    plt.errorbar(x_values, avg_durations, yerr=std_durations, 
                fmt='o-', capsize=5, capthick=1, elinewidth=1,
                color='blue', markersize=8, linewidth=2)
    
    # Add labels and title
    plt.xlabel(x_axis.replace('_', ' ').title())
    plt.ylabel('Average Duration (seconds)')
    plt.title(f'Benchmark Performance: Duration vs {x_axis.replace("_", " ").title()}')
    
    # Add grid
    plt.grid(True, linestyle='--', alpha=0.7)
    
    # Format y-axis with appropriate scale
    plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
    
    # Ensure x-axis starts at 0 if all values are positive
    if min(x_values) >= 0:
        plt.xlim(left=0)
    
    # Add error bar explanation to legend
    plt.plot([], [], 'b-', label='Average Duration')
    plt.plot([], [], 'b|', label='Standard Deviation')
    plt.legend()
    
    # Tight layout to prevent label clipping
    plt.tight_layout()
    
    # Save or display the plot
    if output_file:
        plt.savefig(output_file, dpi=300, bbox_inches='tight')
        plt.close()
    else:
        plt.show()

def print_summary_statistics(filepath: str | Path) -> None:
    """
    Print summary statistics for the benchmark data.
    
    Args:
        filepath: Path to the JSON file
    """
    with open(filepath, 'r') as f:
        data = json.load(f)
    
    print("\nBenchmark Summary Statistics:")
    print("-" * 50)
    
    for i, benchmark in enumerate(data['benchmarks'], 1):
        params = benchmark['parameters']
        job_durations = [job['duration'] for job in benchmark['results']['results']]
        
        print(f"\nBenchmark #{i}")
        print(f"Parameters:")
        for key, value in params.items():
            print(f"  {key}: {value}")
        print(f"Results:")
        print(f"  Number of jobs: {len(job_durations)}")
        print(f"  Average duration: {np.mean(job_durations):.6f} seconds")
        print(f"  Std deviation: {np.std(job_durations):.6f} seconds")
        print(f"  Min duration: {min(job_durations):.6f} seconds")
        print(f"  Max duration: {max(job_durations):.6f} seconds")

# Example usage
if __name__ == "__main__":
    # Example usage of the visualization function
    # Load and visualize width_height vs duration
    load_and_visualize("benchmark_results.json", "width_height", "width_height_plot.png")
    
    # Load and visualize iterations vs duration
    load_and_visualize("benchmark_results.json", "iterations", "iterations_plot.png")
    
    # Print summary statistics
    print_summary_statistics("benchmark_results.json")