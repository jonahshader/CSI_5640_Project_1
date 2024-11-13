import json
import matplotlib.pyplot as plt
import numpy as np
from typing import Literal
from pathlib import Path
import argparse
from matplotlib.ticker import ScalarFormatter, FuncFormatter

def calculate_cells_updated(params):
    """Calculate total cells updated for a given parameter set."""
    return params['width_height'] * params['width_height'] * params['iterations']

def format_number(x, p):
    """Format numbers in full or scientific notation."""
    if x == 0:
        return '0'
    if abs(x) < 1e-3 or abs(x) > 1e5:
        return f'{x:.2e}'
    return f'{x:,.2f}'

class CustomScalarFormatter(ScalarFormatter):
    """Custom formatter that keeps scientific notation per-tick."""
    def _set_format(self):
        self.format = '%1.2e'

def create_plots(
    x_values: np.ndarray,
    duration_stats: list,
    params_list: list,
    benchmark_sets: list,
    sweep_type: str,
    output_dir: Path,
    base_filename: str,
    headless: bool = False,
    full_numbers: bool = False
) -> None:
    """
    Create both duration and efficiency plots, in both linear and log scales.
    """
    colors = ['blue', 'red', 'green', 'purple', 'orange']
    markers = ['o', 's', '^', 'D', 'v']
    
    # Create plots for both metrics (duration/efficiency) and both scales (linear/log)
    for plot_type in ['duration', 'efficiency']:
        for scale in ['linear', 'log']:
            plt.figure(figsize=(12, 8))
            
            for type_idx in range(len(duration_stats)):
                type_stats = duration_stats[type_idx]
                
                if plot_type == 'duration':
                    y_values = np.array([stat[0] for stat in type_stats])
                    yerr = np.array([stat[1] for stat in type_stats])
                    ylabel = 'Average Duration (seconds)'
                else:  # efficiency
                    cells_updated = np.array([calculate_cells_updated(params) 
                                            for params in params_list])
                    durations = np.array([stat[0] for stat in type_stats])
                    y_values = cells_updated / durations / 1e6
                    yerr = y_values * np.array([stat[1]/stat[0] for stat in type_stats])
                    ylabel = 'Efficiency (million cells/second)'
                
                benchmark_type = benchmark_sets[0]['benchmark_types'][type_idx]
                label = f"Type: {benchmark_type['description']}"
                
                color = colors[type_idx % len(colors)]
                marker = markers[type_idx % len(markers)]
                
                plt.errorbar(x_values, y_values, yerr=yerr,
                            fmt=f'{marker}-', capsize=5, capthick=1, elinewidth=1,
                            color=color, markersize=8, linewidth=2, label=label)
            
            plt.xlabel(sweep_type.replace('_', ' ').title())
            plt.ylabel(ylabel)
            plt.title(f'Benchmark {plot_type.title()} vs {sweep_type.title()} ({scale} scale)')
            
            plt.grid(True, linestyle='--', alpha=0.7)
            
            # Set scale for axes
            if scale == 'log':
                plt.xscale('log')
                plt.yscale('log')
            
            # Configure number formatting
            if full_numbers:
                formatter = FuncFormatter(format_number)
                plt.gca().xaxis.set_major_formatter(formatter)
                plt.gca().yaxis.set_major_formatter(formatter)
            else:
                # Use per-tick scientific notation
                plt.gca().xaxis.set_major_formatter(CustomScalarFormatter())
                plt.gca().yaxis.set_major_formatter(CustomScalarFormatter())
            
            if min(x_values) >= 0:
                plt.xlim(left=0)
            
            plt.legend()
            plt.tight_layout()
            
            # Save plot
            output_path = output_dir / f"{base_filename}_{plot_type}_{scale}.png"
            plt.savefig(output_path, dpi=300, bbox_inches='tight')
            
            if not headless:
                plt.show()
            plt.close()

def load_and_visualize(
    filepath: str | Path,
    headless: bool = False,
    output_dir: str | Path | None = None,
    full_numbers: bool = False
) -> None:
    """
    Load benchmark results from a JSON file and create visualizations.
    """
    with open(filepath, 'r') as f:
        data = json.load(f)
    
    sweep_type = data['sweep_type']
    benchmark_sets = data['benchmark_sets']
    
    if not benchmark_sets:
        raise ValueError("No benchmark sets found in the data")
    
    # Extract data
    x_values = []
    params_list = []
    num_benchmark_types = len(benchmark_sets[0]['benchmark_types'])
    duration_stats = [[] for _ in range(num_benchmark_types)]
    
    for benchmark_set in benchmark_sets:
        params = benchmark_set['parameters']
        params_list.append(params)
        x_values.append(params[sweep_type])
        
        for type_idx, benchmark_type in enumerate(benchmark_set['benchmark_types']):
            job_durations = [job['duration'] for job in benchmark_type['results']]
            avg_duration = np.mean(job_durations)
            std_duration = np.std(job_durations)
            duration_stats[type_idx].append((avg_duration, std_duration))
    
    # Create output directory if needed
    if output_dir is None:
        output_dir = Path(filepath).parent
    else:
        output_dir = Path(output_dir)
        output_dir.mkdir(parents=True, exist_ok=True)
    
    # Sort all data by x values
    sorted_indices = np.argsort(x_values)
    x_values = np.array(x_values)[sorted_indices]
    params_list = [params_list[i] for i in sorted_indices]
    duration_stats = [[stats[i] for i in sorted_indices] for stats in duration_stats]
    
    # Convert width_height to size (area)
    if sweep_type == 'width_height':
        sweep_type = 'size'
        x_values = x_values * x_values
    
    # Create plots
    base_filename = Path(filepath).stem
    create_plots(x_values, duration_stats, params_list, benchmark_sets,
                sweep_type, output_dir, base_filename, headless, full_numbers)

def main():
    parser = argparse.ArgumentParser(description='Visualize benchmark results')
    parser.add_argument('files', nargs='+', help='JSON files to process')
    parser.add_argument('--headless', action='store_true', help='Run without displaying plots')
    parser.add_argument('--output-dir', type=str, help='Directory to save plots to')
    parser.add_argument('--stats', action='store_true', help='Print summary statistics')
    parser.add_argument('--full-numbers', action='store_true', 
                       help='Display full numbers instead of scientific notation')
    
    args = parser.parse_args()
    
    for filepath in args.files:
        try:
            load_and_visualize(filepath, args.headless, args.output_dir, args.full_numbers)
            if args.stats:
                print_summary_statistics(filepath)
        except Exception as e:
            print(f"Error processing {filepath}: {e}")

if __name__ == "__main__":
    main()