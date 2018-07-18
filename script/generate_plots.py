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

def real_time_to_milliseconds(result):
	result["real_time"] = result["real_time"] / 1000000000.0
	return result

def filter_use_case_results(use_case, use_case_filter = None):
	regex = re.compile(re.escape(use_case) + "*")
	use_case_results = [result for result in results if re.match(regex, result["name"])]
	if use_case_filter != None:
		use_case_results = [result for result in use_case_results if use_case_filter(result)]

	return use_case_results

def generate_use_case_trace(use_case_results, x_axis_field, y_axis_field):

	if len(use_case_results) == 0:
		raise ValueError("use_case must have at least one result")

	x_axis = [result[x_axis_field] for result in use_case_results]
	y_axis = [result[y_axis_field] for result in use_case_results]
	trace = go.Scatter(
		x = x_axis,
		y = y_axis,
		marker = {"color":"#1baa27"}
	)

	return trace

def generate_use_case_plot(use_case, title, x_axis_field, y_axis_field, x_axis_title, y_axis_title, serial_use_case = None, use_case_filter = None, x_transform = None):

	use_case_results = filter_use_case_results(use_case, use_case_filter)
	if x_transform is not None:
		use_case_results = [x_transform(result) for result in use_case_results]

	parallel_trace = generate_use_case_trace(use_case_results, x_axis_field, y_axis_field)
	data = [parallel_trace]
	plot_filepath = plot_output_dir + "/" + use_case.replace("/","_") + ".html"

	y_values = [result[y_axis_field] for result in use_case_results]
	max_y = sorted(y_values)[-1]

	layout = {
		"title": title,
		"shapes":[],
		"xaxis":{
			"title": x_axis_title,
			"showgrid": False,
			"zeroline": False,
			"showline": False
		},
		"yaxis":{
			"title": y_axis_title,
			"range": [0, max_y * 1.05],
			"showgrid": False,
			"zeroline": False,
			"showline": False,
		}
	}

	pprint(serial_use_case)
	pprint(results)
	if serial_use_case != None:
		serial_result = [result for result in results if result["name"] == serial_use_case][0]

		if x_transform is not None:
			serial_result = x_transform(serial_result)

		# Figure out how long to make the serial offline
		x_values = [result[x_axis_field] for result in use_case_results]
		sorted_x_values = sorted(x_values)
		min_x = sorted_x_values[0]
		max_x = sorted_x_values[-1]
		x1 = max_x + min_x
		y = serial_result[y_axis_field]
		if y > max_y:
			max_y = y
			layout["yaxis"]["range"] = [0, max_y * 1.05]

		# Draw serial line
		serial_trace = {
			"mode": "lines",
			"name": "serial",
			"type": "scatter",
			"x": [0, x1],
			"y": [y, y],
			"marker": {"color":"#4286f4"}
		}

		# Distinguish parallel trace
		parallel_trace["name"] = "parallel"

		data.append(serial_trace)

	fig = {"data": data, "layout": layout}

	plot(
		figure_or_data=fig,
		filename=plot_filepath,
		output_type="file",
		image="svg",
		auto_open=False
	)

generate_use_case_plot(
	use_case ="AssetAllocationBenchmark/EvaluatePortfolioRestrictions_BankWide/",
	title = "Restriction Evaluation, 65536 Portfolios, Intel i7-7700HQ (4 cores, 8 Hyperthreads)",
	x_axis_field = "threadCount",
	y_axis_field = "real_time",
	x_axis_title = "Thread Count",
	y_axis_title = "Real Time (s)",
	use_case_filter = (lambda result: result["portfolioCount"] == 65536 and result["serial"] == 0.0),
	serial_use_case = "AssetAllocationBenchmark/EvaluatePortfolioRestrictions_serial_BankWide/65536",
	x_transform = real_time_to_milliseconds
)
