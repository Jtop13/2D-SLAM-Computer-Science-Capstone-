def calculate_mean(data_file):    
	with open(data_file, 'r') as f:
  		data = [float(line.strip()) for line in f]
	mean = sum(data) / len(data)
	return mean


