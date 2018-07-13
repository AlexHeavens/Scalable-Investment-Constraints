import plotly.graph_objs as go
from plotly.offline import download_plotlyjs, init_notebook_mode, plot, iplot
import json
from pprint import pprint
import os
import re

script_dir = os.path.dirname(os.path.realpath(__file__))
project_root_dir = os.path.dirname(os.path.realpath(script_dir))

results_path = project_root_dir + "/perf_res/benchmark_results.json"
plot_output_dir = project_root_dir + "/analysis/plots"

with open(results_path) as results_file:
	results = json.load(results_file)["benchmarks"]

def generate_use_case_trace(use_case, x_axis_field, y_axis_field):
	regex = re.compile(re.escape(use_case) + "*")
	use_case_results = [result for result in results if re.match(regex, result["name"])]
	x_axis = [result[x_axis_field] for result in use_case_results]
	y_axis = [result[y_axis_field] for result in use_case_results]
	trace = go.Scatter(
		x = x_axis,
		y = y_axis
	)
	return trace

def generate_use_case_plot(use_case, x_axis_field, y_axis_field):
	trace = generate_use_case_trace(use_case, x_axis_field, y_axis_field)
	data = [trace]
	plot_filepath = plot_output_dir + "/" + use_case.replace("/","_") + ".html"
	plot(
		figure_or_data=data,
		filename=plot_filepath,
		output_type="file",
		image="svg",
		auto_open=False
	)

generate_use_case_plot("AssetAllocationBenchmark/EvaluatePortfolioRestrictions_BankWide/", "portfolioCount", "real_time")
