import sys
print(f"Python version: {sys.version}")
print(f"Executable path: {sys.executable}")
print(f"Arguments to pow: trying pow(4, -1, 7)")
try:
    result = pow(4, -1, 7)
    print(f"Result: {result}")
except Exception as e:
    print(f"Error: {e}")
    print(f"Error type: {type(e)}")
