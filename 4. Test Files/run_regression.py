import subprocess
import random
import re
import sys

# ==============================================================================
# Configuration
# ==============================================================================
NUM_RUNS = 100

# Base command to run the Makefile. We assume 'make build' is run before this script.
CMD_BASE = ["make", "run", "--no-print-directory"]

# ==============================================================================
# Coverage Target Dictionary
# ==============================================================================
# Keys strictly match the text printed by functional_coverage.cpp before the colon.
# Assuming DEPTH = 16, DATA_WIDTH = 4. 
# Cross bins = 2 (we) * 16 (addr) * 16 (data) = 512 total combinations.
coverage_targets = {
    "CS=1 Hits": 1,                       
    "CS=0 Hits": 1,                       
    "Write Hits": 1,                      
    "Read Hits": 1,                       
    "Unique Addresses Tested": 16,        
    "Unique Data Values Tested": 16,      
    "Valid Cross Combinations (CS=1)": 512 
}

# Track the MAXIMUM hits seen in any single test run to approximate regression merging
coverage_achieved = {key: 0 for key in coverage_targets.keys()}

print("======================================================")
print(f"  STARTING C++ VERILATOR REGRESSION SUITE ({NUM_RUNS} LOOPS)")
print("======================================================")

# Ensure the executable is built and up to date before running the loops
print("Building simulation executable...")
subprocess.run(["make", "build"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
print("Build complete.\n")

passed_tests = 0

# 1. Regression Loop: Run the executable NUM_RUNS times with different seeds
for i in range(1, NUM_RUNS + 1):
    current_seed = random.randint(1, 999999999)
    run_cmd = CMD_BASE + [f"SEED={current_seed}"]
    
    # Execute silently and capture output
    result = subprocess.run(run_cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True)
    output = result.stdout

    # 2. Check for Testbench Fails (Custom C++ logic)
    # Looking for our Scoreboard mismatch string, Verilator %Error, or memory faults
    test_failed = False
    if re.search(r"DATA MISMATCH", output):
        print(f"  -> Run {i:02d} (Seed: {current_seed:<10}) \033[91m[FAIL - DATA MISMATCH]\033[0m")
        test_failed = True
    elif re.search(r"%Error|Segmentation fault", output):
        print(f"  -> Run {i:02d} (Seed: {current_seed:<10}) \033[91m[FAIL - FATAL ERROR]\033[0m")
        test_failed = True
    else:
        print(f"  -> Run {i:02d} (Seed: {current_seed:<10}) \033[92m[PASS]\033[0m")
        passed_tests += 1

    # 3. Parse Coverage Data via Regex (Only if test passed to avoid garbage data)
    if not test_failed:
        for key in coverage_targets.keys():
            # Escape parentheses in keys like "Valid Cross Combinations (CS=1)"
            safe_key = re.escape(key)
            # Look for the key, followed by a colon, optional spaces, and capture the number
            match = re.search(fr"{safe_key}\s*:\s*(\d+)", output)
            
            if match:
                current_val = int(match.group(1))
                # For unique items (like Map sizes), take the max achieved in any test run
                if current_val > coverage_achieved[key]:
                    coverage_achieved[key] = current_val

# ==============================================================================
# Calculate Final Percentage
# ==============================================================================
total_percentage = 0.0
num_metrics = len(coverage_targets)

print("\n======================================================")
print("             CUMULATIVE COVERAGE SUMMARY              ")
print("======================================================")
print(f" Total Runs Executed : {NUM_RUNS}")
print(f" Passing Runs        : {passed_tests}/{NUM_RUNS}")
print("------------------------------------------------------")

# Print the aggregated metrics against their targets
for key, target in coverage_targets.items():
    achieved = coverage_achieved[key]
    
    # Cap the metric at 100% so over-hitting doesn't warp the final average
    metric_pct = min((achieved / target) * 100, 100.0)
    total_percentage += metric_pct
    
    if achieved >= target:
        print(f" \033[92m[100%]\033[0m {key:<35} : {achieved}/{target}")
    elif achieved > 0:
        print(f" \033[93m[{metric_pct:>3.0f}%]\033[0m {key:<35} : {achieved}/{target}")
    else:
        print(f" \033[91m[  0%]\033[0m {key:<35} : 0/{target}")

final_coverage = total_percentage / num_metrics

print("======================================================")
# Color code the final percentage
if final_coverage >= 100.0:
    print(f" FINAL FUNCTIONAL COVERAGE: \033[92m{final_coverage:.1f}%\033[0m")
else:
    print(f" FINAL FUNCTIONAL COVERAGE: \033[93m{final_coverage:.1f}%\033[0m")
print("======================================================\n")