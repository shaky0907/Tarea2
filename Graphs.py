
import matplotlib.pyplot as plt


# Initialize empty lists to store data
message_sizes = []
execution_times = []

# Read the file and extract data
with open("data.txt", 'r') as file:
    for line in file:
        # Split each line into two parts based on the semicolon
        parts = line.strip().split(';')
        
        # Extract data and convert to appropriate data types
        message_size = int(parts[0])
        execution_time = float(parts[1])
        
        # Append the data to the lists
        message_sizes.append(message_size)
        execution_times.append(execution_time)

# Plot the data
plt.stem(message_sizes, execution_times)

# Add labels and title
plt.xlabel('Message Size')
plt.ylabel('Execution Time')
plt.title('Execution Time vs. Message Size')

# Show the plot
plt.show()
